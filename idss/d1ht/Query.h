/**
 * @file:   Query.h
 * Author: HPC Lab
 *
 * Created on October 18, 2016
 */

#ifndef QUERY_H
#define	QUERY_H



/**
 * @brief risposta ad una query, mapping 1:1 col messaggio D1HT_QUERY_REPLY_body
 */
struct response_t {
    int idQuery; /**< identificativo univoco della query cui pertiene questa reply */
    char response[MAXREPLYLEN]; /**< i dati risultanti dalla query */
    d1ht::IPaddr responderNode; /**< chi ha inviato la risposta */
    pthread_mutex_t reply_mutex; /**< in caso di accesso concorrente alla reply */
};

/**
 *  @brief Query da disseminare sull'overlay D1HT, così come ricevuta da IDSS
 */
struct request_t {
    char *uqi; /**< identificativo univoco assegnato alla query da iGrid */
    char msg[MAXQUERYLEN]; /**< query SQL */
    int job_time; /**< tempo massimo di elaborazione assegnato alla query */

    char hostname[INET6_ADDRSTRLEN+6]; /**< ip:port del nodo che deve immettere la query nell'overlay */
};


/**
 * utility function per la stampa/visualizzazione di una reply
 * @param res, la struct di tipo response_t da visualizzare
 */
void printResponse(struct response_t res);


/**
 * utility function per la stampa di una query
 * @param q, la struct query_t da stampare
 */
void printRequest(struct request_t q);


/**
 * @brief class Query
 * Tiene traccia di tutte le query che giungono ad un nodo
 * e che devono essere elaborate e rilanciate sull'overlay
 *
 * Una query è creata su un Origin Node (da cui parte la disseminazione sull'overlay)
 * ed è ricevuta e rilanciata da tutti i suoi successori.
 * Tutti i nodi che ricevono una query risponderanno al mittente con la loro risposta.
 *
 * Ogni nodo mittente concatenerà la sua risposta alle risposte ricevute
 * (in funzione dei rilanci effettuati)
 * e invierà il risultato al suo mittente.
 *
 * Il procedimento termina allo scadere del tempo assegnato alla query.
 *
 */
class Query {
private:
    static int _counter;

public:
    pthread_mutex_t     query_mutex;    /**< per accesso escluviso all'oggetto */
    pthread_rwlock_t    query_lock;     /**< per accesso concorrente all'oggetto */

    long id_query;          /**< identificativo univoco assegnato alla query dal costruttore */

    char uqi[MAXQUERYLEN];  /**< identificativo univoco assegnato alla query da IDSS */
    char msg[MAXQUERYLEN];  /**< la query SQL */
    int  job_time;          /**< tempo massimo di elaborazione assegnato alla query (TTL di IDSS) */
    double reduction_factor;
    char original_hostname[INET6_ADDRSTRLEN+6]; /**< IP:porta del nodo IDSS da cui parte la query */

    bool is_origin_node;        /**< true se sono il nodo che ha disseminato la query, false altrimenti */

    IPaddr origin_node;   /**< il nodo che ha disseminato la query (ha ricevuto la richiesta dal Client GSOAP) */
    KEYID origin_key;     /**< la D1HT key associata all'origin node */

    IPaddr sender_node;   /**< chi ha inoltrato la query, NULL se is_origin_node è true */

    int edra_ttl;               /**< D1HT-TTL (EDRA) con cui si è ricevuta la query */

    int max_relays;             /**< a quanti inoltro la query */
    IPaddr *successors;  /**< a chi inoltro la query */

    /* I nodi successori a cui inoltro la query è un subset
     * dell'insieme di tutti successori del nodo nell'overlay D1HT:
     * ad un certo punto il rilancio si ferma quando si torna sull'originNode
     * -- STRETCH condition --
     * per evitare di inviare la query all'infinito nell'overlay.
     * Inoltre il rilancio si ferma per raggiunto TTL=0.
     */
    int expectedReplies; /**< numero massimo di reply che devo collezionare=maxRelays+1 */
    //std::list<struct response_t> replies; /**< le risposte che ricevo */

    ///metodi
    Query();

    ~Query(){
        //printf("Invocato distruttore di query %ld, ", this->id_query);
        pthread_mutex_destroy(&query_mutex);
        pthread_rwlock_destroy(&query_lock);
        //printf("... distrutti i mutex\n\n");
    }

    /**
     * Costruttore di un oggetto della classe Request
     * @param id            identificativo univoco della query,
     *                      come calcolato dal componente esterno
     * @param query         stringa da distribuire su overlay
     * @param deadline      tempo disponibile per l'esecuzione della query
     *                      (tempo residuo a disposizione)
     * @param ttl           quanti rilanci della query (su D1HT overlay)
     *                      occorre fare (TTL associato alla query ricevuta)
     * @param origin        nodo che ha immesso la query nell'overlay
     * @param key           key del nodo origine
     * @param startNode     true se sono l'origin node
     * @param sender        IP del nodo che ha inoltrato la query
     *                      (a cui deve essere ritornata la risposta)
     *
     * @return              l'oggetto Query istanziato
     */
    Query(char *e_uqi, char *e_query, int deadline, double red_factor, int ttl, d1ht::IPaddr origin, d1ht::KEYID key, bool isStartNode, d1ht::IPaddr sender);


    /**
    @brief costruttore di una query a partire da iGrid
    in iGrid quando la query viene ricevuta dal server TCP GSOAP sulla porta 19000,
    le uniche informazioni note sono:
    @param e_uqi - uqi esterno calcolato da IDSS
    @param e_query - query SQL sottomessa dal client TCP GSOAP
    @param deadline - tempo di esecuzione per la query indicato dal client TCP GSOAP
    @param start_node - sono il nodo IDSS che ha ricevuto la query (true)
    */
    Query(char *e_uqi, char *e_query, int deadline, bool start_node, double reductor);



    /**
     * @brief Calcola i destinatari del rilancio (secondo EDRA)
     * @param dest  a chi inoltro la query,
     *              value-result argument,
     *              aggiornato con l'elenco dei successori
     * @param count a quanti inoltro la query,
     *              il numero di successori
     *              (dipende da quanti siamo nell'overlay)
     *
     * @return intero, stato dell'esecuzione
     *
     * Il metodo determina i rho successori del nodo corrente a cui può
     * essere inoltrata la query:
     * dato il churning, il set è variabile nel tempo.
     * Inoltre, dato il TTL con cui si riceve la query, i successori effettivi
     * saranno 'less-equal' del valore ritornato dal metodo.
     * Poiché il rilancio della query non può proseguire all'infinito, il numero
     * effettivo di destinazioni contattate sarà minore del massimo riportato
     * dal metodo quando sia soddisfatta la condizione di IN_STRETCH.
     */
    int get_successors(d1ht::IPaddr **dest, int *count);


    /**
     * @brief Dissemina la query sull'overlay, a partire dall'Origin Node
     * @return intero, stato dell'esecuzione.
     *
     * Viene usata dall'origin node per iniziare la disseminazione
     * di una query sull'overlay D1HT.
     */
    int disseminate_query();


    /**
     * @brief Rilancia la query ricevuta con TTL decrescente
     * e si ferma quando ritorna al punto di partenza.
     * @return intero, stato dell'esecuzione.
     *
     * @note La differenza tra *disseminateQuery()* e *relayQuery()*
     * è nella gestione del TTL e dei successori del nodo.
     */
    int relay_query();

    /**
     * debug print della struttura dati
     */
     void print_query();

};

#endif	/* QUERY_H */
