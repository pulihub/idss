/**
 * @file:   Query.c
 * Author: HPC Lab
 *
 * Created on October 18, 2016
 */

#include "D1HT.h"
#include "Query.h"

int d1ht::Query::_counter = 0;

namespace d1ht {

    void printResponse(struct response_t res) {
        printf("Reply to queryId = %d received from %s:%u\n'%s'\n", res.idQuery, IPtoP(res.responderNode), res.response);
    }

    void printRequest(struct request_t q) {
        printf("Query with UQI = '%s', originated from '%s', jobTime=%d\n'%s'\n", q.uqi, q.hostname, q.job_time, q.msg);
    }



    //*****************************************************
    //***** Class Request: methods  ***********************
    //*****************************************************

    Query::Query() {
        id_query=++_counter;

        memset(uqi, '0', sizeof(char)*MAXQUERYLEN);
        memset(msg, '0', sizeof(char)*MAXQUERYLEN);
        memset(original_hostname, '0', strlen(original_hostname));
        job_time = 0;
        reduction_factor = 1.0;
        is_origin_node = false;

        edra_ttl = 0;
        max_relays = 0;
        successors = NULL;

        //rough initialization dei mutex...
        pthread_mutex_init(&query_mutex, NULL);
        pthread_rwlock_init(&query_lock, NULL);
        expectedReplies = 0;
    }


    Query::Query(char *e_uqi, char *e_query, int deadline, double red_factor, int ttl, d1ht::IPaddr origin, d1ht::KEYID key, bool isStartNode, d1ht::IPaddr sender){

        id_query=++_counter;

        memcpy(uqi, e_uqi, strlen(e_uqi));
        memcpy(msg, e_query, strlen(e_query));
        job_time = deadline;
        reduction_factor = red_factor;
        memcpy(&origin_node, &origin, sizeof (IPaddr));

        origin_key = key;
        is_origin_node = isStartNode;

        edra_ttl = ttl;

        max_relays = 0;
        successors = NULL;

        //il nodo origine non ha un mittente per la query
        if (is_origin_node == false ) {
            sender_node = sender;
        }


        //rough initialization dei mutex...
        pthread_mutex_init(&query_mutex, NULL);
        pthread_rwlock_init(&query_lock, NULL);
        expectedReplies = 0;
   }

//request from IDSS
Query::Query(char *e_uqi, char *e_query, int deadline, bool start_node, double reductor) {
    if (start_node == true) {
        //sono il peer IDSS che ha ricevuto una query e che desidera disseminarla sull'overlay D1HT tramite EDRA
        is_origin_node = true;

        id_query = ++_counter;

        memset(uqi, '\0', sizeof(uqi));
        memcpy(uqi, e_uqi, strlen(e_uqi));

        memset(msg, '\0', sizeof(msg));
        memcpy(msg, e_query, strlen(e_query));

        job_time = deadline; //millisecondi

        reduction_factor = reductor;

        origin_node = G_parms.myIP;
        origin_key = G_myID;

        //per inizializzazione...
        sender_node = G_parms.myIP;

        memset(original_hostname, '0', sizeof(char)* (INET6_ADDRSTRLEN +6 ) );
        snprintf(original_hostname, INET6_ADDRSTRLEN+5, "%s:%d", IPtoP(G_parms.myIP));

        edra_ttl = EDRA_TTL_TO_ALL;
        max_relays = 0;
        successors = NULL;

        //rough initialization dei mutex...
        pthread_mutex_init(&query_mutex, NULL);
        pthread_rwlock_init(&query_lock, NULL);
        expectedReplies = 0;
    }//fi

   }

    int Query::get_successors(IPaddr **dest, int *count) {

        //trova i rho successori del nodo in questo momento
        int rho = G_RT.pub_rho();

        if (rho == 0) {
            //fprintf(stderr, "at the moment, I'm alone, no successors\n");

            dest = NULL;
            (*count) = 0;

            return NO_SUCCESSORS;
        } //fi

        IPaddr *p = (IPaddr *) malloc(sizeof (IPaddr) * rho);

        IPaddr iterNode = G_RT.succ_IP();
        KEYID iterKey = G_RT.succ_ID();

        int offset = 1;

        p[0] = iterNode;

        for (int i = 1; i < rho; i++) {

            p[i] = G_RT.pub_succn(iterKey, offset);

            iterKey = ip2id(p[i]);
            offset = 2 * offset;
        } //for

        *count = rho;
        *dest = p;
        return 0;
    }


    int Query::disseminate_query() {

        pthread_rwlock_wrlock(&query_lock);
            //sicuramente genero almeno la mia risposta alla query
            expectedReplies = 1;
        pthread_rwlock_unlock(&query_lock);


        //ho dei successori?
        if (get_successors(&successors, &max_relays) != 0) {
            return -1;
        }//fi


        struct D1HT_QUERY_REQUEST_body body;
        BZERO(body);

        body.idQuery = id_query;

        memcpy(body.msg, msg, strlen(msg));
        memcpy(body.uqi, uqi, strlen(uqi));

        //questo sono io
        body.originNode = origin_node; //G_parms.myIP;
        body.originKey = origin_key; //G_myID;

        //decurto il jobTime
        body.deadline = (int) (job_time*reduction_factor);
        body.reductor = reduction_factor;

        //invio i messaggi
        int connection;

        for (int i = 0; i < max_relays; i++) {

                body.TTL = i;

                //invia messaggio
                int iret = TCP_CONNECTION(connection, 0, successors[i], XEQ_SILENT);
                if (iret == 0 && connection >= 0) {
                    SEND_MSG(connection, D1HT_QUERY_REQUEST, body, XEQ_WARNING);
                } //fi connection
                D1HT_CLOSE(connection);


                pthread_rwlock_wrlock(&query_lock);
                    //aggiorno il numero di reply attese
                    this->expectedReplies++;
                pthread_rwlock_unlock(&query_lock);

        } //for

        return SUCCESS;
    }


    int Query::relay_query() {

        pthread_rwlock_wrlock(&query_lock);
            expectedReplies = 1;
        pthread_rwlock_unlock(&query_lock);

        if (edra_ttl == 0) {
            // questo significa che non appena ho la mia risposta
            // la posso rilanciare al mio relayer

            pthread_rwlock_wrlock(&query_lock);

                successors = NULL;
                max_relays = 0;
            pthread_rwlock_unlock(&query_lock);
            return NULL_TTL;
        }


        if (get_successors(&successors, &max_relays) != 0) {
            //non ci sono altri nodi nel sistema
            return -1;
        }//fi


        struct D1HT_QUERY_REQUEST_body body;
        BZERO(body);

        body.idQuery = id_query;
        memcpy(body.msg, msg, strlen(msg));
        memcpy(body.uqi, uqi, strlen(uqi));

        body.originNode = origin_node;
        body.originKey = origin_key;

        //decurto il tempo
        body.deadline = (int) (job_time*reduction_factor);
        body.reductor = reduction_factor;

        //invio i messaggi
        int connection;

        //quanti messaggi al max devo inviare
        int max_relays = edra_ttl - 1;
        int i = 0;

        do {

                /* The statement below is to enforce rule 8 from the
                 * IPDPS paper, i.e., we will remove from the message
                 *  any event that is in strech(myself,target)
                 */
                if (INSTRETCH(body.originKey, G_myID, ip2id(successors[i])))
                {
                    //printf("%s:%d is in stretch between me and the origin node\n", IPtoP(successors[i]));

                    /* se mi fermo, devo aggiornare il numero di risposte attese
                     * con le query effettivamente rilanciate;
                     * il numero di successori è maggiore del numero di rilanci,
                     * perché alcuni nodi saranno
                     * in stretch con l'origine della query!
                     */
                    pthread_rwlock_wrlock(&query_lock);
                        max_relays = i;
                    pthread_rwlock_unlock(&query_lock);
                    break;
                }
                else
                {
                    body.TTL = i;

                    //invia messaggio
                    int iret = TCP_CONNECTION(connection, 0, successors[i], XEQ_SILENT);
                    if (iret == 0 && connection >= 0) {
                        SEND_MSG(connection, D1HT_QUERY_REQUEST, body, XEQ_WARNING);
                    } //fi connection
                    D1HT_CLOSE(connection);

                    pthread_rwlock_wrlock(&query_lock);
                        //aggiungo 1 risposta, attesa da questa destinazione
                        expectedReplies++;
                    pthread_rwlock_unlock(&query_lock);
                }//fi IN_STRETCH
            i++;

        } while (i <= max_relays);

    return SUCCESS;
    }



    /**
     * @brief debug print dell'oggetto, protetto da lock per letture concorrenti.
     */
    void Query::print_query() {
        pthread_rwlock_rdlock(&query_lock);
        fprintf(stderr, "[%s]\n\tQuery: idQuery=%d\n\treceivedTTL=%d\n\tjobTime=%d\n\tmessage='%s'\n\toriginNode=%s:%u\n", __FILE__, id_query, edra_ttl, job_time, msg, IPtoP(origin_node));
        pthread_rwlock_unlock(&query_lock);
    }

}//namespace
