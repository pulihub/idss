/**
 * @file:   QueryManager.c
 * Author: HPC Lab
 *
 * Created on February 16, 2016, 4:57 PM
 */

#include "D1HT.h"

namespace d1ht {


    void printReply(struct reply_t res) {
        printf("Reply to queryId = %d received from %s:%u\n'%s'- UQI:'%s'\n", res.subquery_number, IPtoP(res.sender), res.response, res.uqi);
    }

    void printRequest(struct query_t q) {
        //printf("Query with Id = %d, originated from %s:%u, ttl=%d, jobTime=%d\n'%s'\n", q.idQuery, IPtoP(q.originNode), q.receivedTTL, q.jobTime, q.msg);
    }


    //*****************************************************
    //***** Query Manager methods     *********************
    //*****************************************************
    QueryManager::QueryManager(){
        pthread_rwlock_init(&qm_lock, NULL);
    }

    QueryManager::~QueryManager(){

        pthread_rwlock_wrlock(&qm_lock);

        for (map<int, Query *>::iterator it = querymap.begin(); it != querymap.end(); ++it) {
            delete it->second;
        }
        querymap.clear();

        pthread_rwlock_unlock(&qm_lock);

        pthread_rwlock_destroy(&qm_lock);
    }



    /**
     * @brief Estensione HPC Lab per la disseminazione broadcast di una query distribuita sull'overlay D1HT
     * @param query - la query SQL da distribuire
     * @param uqi - id univoco della query
     * @param timeout - tempo a disposizione per generare una risposta alla query
     * @param reduction - reduction factor
     *
     * @return stato dell'esecuzione
     *
     * Viene richiamata da IDSS, quando una query deve essere disseminata nell'overlay.
     * Fa partire la disseminazione EDRA-like della query sull'overlay D1HT.
     */
    int QueryManager::broadcast_query(char *query, char *uqi, int timeout, double reduction){
        if (query == NULL || uqi == NULL) {
            return -1;
        }

        //fprintf(stderr, "%s: broadcast della query con uqi %s, ttl=%d, redfactor %f, query: %s\n", __FILE__, uqi, timeout, reduction, query);

        Query q = Query(uqi, query, timeout, true, reduction);
        //q.print_query();

        q.disseminate_query();

        return 0;
    }



    /**
     *
     * @param uqi - uqi della query
     * @param sender - hostname ip:port a cui inviare la risposta
     * @param response - msg da inviare
     * @return
     */
    int QueryManager::send_query_reply(char *uqi, char *sender, char *response){

        struct D1HT_QUERY_REPLY_body body;
        BZERO(body);

        memcpy(body.response, response, strlen(response));
        memcpy(body.uqi, uqi, strlen(uqi));

        d1ht::IPaddr sender_destination;
        d1ht::get_IPaddr(sender, &sender_destination);


        int connection;
        //fprintf(stderr, "D1HT Send Response msg to %s:%u\n", IPtoP(sender_destination));

        int iret = TCP_CONNECTION(connection, 0, sender_destination, XEQ_SILENT);
        if (iret == 0 && connection >= 0) {
            SEND_MSG(connection, D1HT_QUERY_REPLY, body, XEQ_WARNING);
        }
        D1HT_CLOSE(connection);

        //fprintf(stderr, "D1HT Response msg sent to %s:%u\n", IPtoP(sender_destination));
        return 0;
    }



    void QueryManager::debug_print_query(char *sql, char *uqi, int ttl, double red_fact, char *origin_ip, int origin_port, int origin_key, char *sender_ip, int sender_port, int edra_ttl) {
/*
        fprintf(stderr, "Received Query message with: ");
        fprintf(stderr, "\n\tquery='%s'", sql);
        fprintf(stderr, "\n\tuqi='%s'", uqi);
        fprintf(stderr, "\n\tdeadline=%d - reduction factor : %f", ttl, red_fact);
        fprintf(stderr, "\n\tEDRA-TTL=%d", edra_ttl);
        fprintf(stderr, "\n\torigin node = '%s:%d' - originKey: %ld", origin_ip, origin_port, origin_key); //long int è la key
        fprintf(stderr, "\n\tsender node='%s:%d'\n\n",sender_ip, sender_port);
        fflush(stderr);
*/
        VERBO(stderr, "Received Query message with: \n\tquery='%s'\n\tuqi='%s'\n\tdeadline=%d - reduction factor : %f\n\tEDRA-TTL=%d\n\tsender node='%s:%d'\n", sql, uqi, ttl, red_fact, edra_ttl, sender_ip, sender_port);
    }



    void QueryManager::set_query_received_callback(void (*func)(void *args)) {
        query_received_callback = func;
    }

    void QueryManager::set_response_received_callback(void (*func)(void *args)){
        response_received_callback = func;
    }




    int QueryManager::exec_query_received_callback(void *args) {
        if (NULL == query_received_callback) {
            return 1;
        }
        query_received_callback(args);
        return 0;
    }



    int QueryManager::exec_response_received_callback(void *args) {
        if (NULL == response_received_callback) {
            return 1;
        }

        response_received_callback(args);
        return 0;
    }



}//namespace
