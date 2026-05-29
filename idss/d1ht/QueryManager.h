/**
 * @file:   QueryManager.h
 * Author: HPC Lab
 *
 * Created on February 16, 2016, 4:57 PM
 */

#ifndef QUERYMANAGER_H
#define	QUERYMANAGER_H

#include "Query.h"


#define EDRA_TTL_TO_ALL  -9     /**< disseminazione su overlay (usato sugli origin node) */
#define NULL_TTL -5             /**< se l'EDRA_TTL è 0, il rilancio si ferma */

#define NO_SUCCESSORS -7        /**< quando il nodo non ha successori (alone) */

#define SUCCESS 9               /**< corretta disseminazione e rilancio di query */


/**
 * @brief risposta ad una query, mapping 1:1 col messaggio D1HT_QUERY_REPLY_body
 */
struct reply_t {
    int subquery_number;    /**< ??? */
    char uqi[MAXQUERYLEN];  /**< identificativo univoco della query cui pertiene questa reply */
    char response[MAXREPLYLEN]; /**< i dati risultanti dalla query */
    d1ht::IPaddr sender; /**< chi ha inviato la risposta */

};




/**
 *  @brief Query da disseminare sull'overlay D1HT
 */
struct query_t {
    int idQuery; /**< identificativo univoco assegnato alla query dal sistema esterno */
    char msg[MAXQUERYLEN]; /**< la query */
    int jobTime; /**< tempo massimo assegnato alla query per la generazione di una risposta */

    int receivedTTL; /**< TTL con cui si è ricevuta con la query */

    d1ht::IPaddr originNode; /**< il nodo che ha disseminato la query */
    d1ht::KEYID originKey; /**< la D1HT key associata all'origin node */
};


/**
 * utility function per la stampa/visualizzazione di una reply
 * @param res, la struct di tipo reply_t da visualizzare
 */
void printReply(struct reply_t res);


/**
 * utility function per la stampa di una query
 * @param q, la struct query_t da stampare
 */
void printRequest(struct query_t q);


/**
 * @brief Query Manager: contiene tutte le query ricevute ed i loro dati...
 * @TODO: magari far funzionare map invece di list...
 *
 */
class QueryManager {


    //callbacks
    //void (*query_received_callback)(char *sql, char *uqi, int ttl, double red_fact, char *origin_ip, int origin_port, int origin_key, char *sender_ip, int sender_port, int edra_ttl); //void *p);
    void (*query_received_callback)(void *args);


    void (*response_received_callback)(void *args);
    //void (*response_received_callback)(void *p);

public:
    std::map<int, Query*> querymap;


    pthread_mutex_t qm_mutex; /**< per consentire il contemporaneo inserimento di query .. */
    pthread_rwlock_t qm_lock; /**< per consentire il contemporaneo inserimento di query .. */


    ///methods
    QueryManager();

    ~QueryManager();

    /**
     * @brief broadcast_query
     * viene eseguita quando ad un nodo IDSS viene sottomessa una query SQL: il nodo elabora la query e la dissemina nell'overlay D1HT
     * @param query - SQL da disseminare
     * @param uqi - unique id della query generato da iGrid
     * @param timeout - tempo residuo per la risoluzione della query
     * @param reduction - fattore di riduzione del tempo residuo ( TTL IDSS )
     * @return
     */
    int broadcast_query(char *query, char *uqi, int timeout, double reduction);

    /**
     * @brief send_query_reply invocata da IDSS quando una reply deve essere inviata al mittente della query cui pertiene.
     * @param uqi - unique id della query
     * @param sender - chi ha inoltrato la query SQL
     * @param response - risposta alla query
     * @return
     */
    int send_query_reply(char *uqi, char *sender, char *response);


    /**
     * @brief debug_print_query: stampa tutti i dati noti di un messaggio di QUERY_BROADCAST
     * @param sql
     * @param uqi
     * @param ttl
     * @param red_fact
     * @param origin_ip
     * @param origin_port
     * @param origin_key
     * @param sender_ip
     * @param sender_port
     * @param edra_ttl
     */
    void debug_print_query(char *sql, char *uqi, int ttl, double red_fact, char *origin_ip, int origin_port, int origin_key, char *sender_ip, int sender_port, int edra_ttl);



    /**
     * @brief set_query_received_callback: assegna la funzione func alla callback definita per il Query Manager
     * @param func - funzione assegnata alla callback di QM

     * func è una funzione che deve avere i seguenti parametri:
     *  * @param sql
     *  * @param uqi
     *  * @param ttl
     *  * @param origin_key
     *  * @param sender - al momento è un hostname "ip:porta" del mittente del messaggio;
     */
    void set_query_received_callback(void (*func)(void *args));

    /**
     *
     * @param func
     *      * @param p
     */
    //void set_response_received_callback(void (*func)(void *p));
    void set_response_received_callback(void (*func)(void *args));

    /**
     *
     * @param sql
     * @param uqi
     * @param ttl
     * @param origin_key
     * @param sender
     * @return
     */
    int exec_query_received_callback(void *args);

    /**
     *
     * @param msg
     * @return
     */
    int exec_response_received_callback(void* args);

};

/**< global variable, il QueueManager che contiene tutte le query da condividere con l'esterno (istanziata in extern.c) */
extern QueryManager G_QM;


#endif	/* QUERYMANAGER_H */
