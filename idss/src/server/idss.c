/*
 * ! \file idss.c
 * threaded server
 *
 * Copyright 2022-2023, University of Salento, Italy.
 * All rights reserved.
 *
 *
 */

#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>
//#include "config.h"
#include "idssH.h"
#include "utils.h"
#include "idssStub.h"
#include "idss_data.h"
#include "idss_server.h"
#include "idss_client.h"
#include "idss_unix.h"
#include "data_access_structures.h"
#include "data_access_func.h"
#include "idss_p2p.h"
#include "p2p_comm.h"
#include "p2p_utils.h"
#include "idss_parser.h"
#include "idss_parser_dq.h"
#include "dq_funcs.h"

// Wrapper for D1HT overlay
#include "wrapper/d1ht_wrapper.h"

/******************
* Macro Definition
*******************/

#define  xmlAssociateSchema(root_node) {xmlNewProp(root_node, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance"); \
         xmlNewProp(root_node, BAD_CAST "xsi:noNamespaceSchemaLocation", BAD_CAST "http://www.gridlab.org/WorkPackages/wp-10/Software/idss_schema.xsd");}

#define  xmlAssociateRecordsetSchema(root_node) {xmlNewProp(root_node, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance"); \
         xmlNewProp(root_node, BAD_CAST "xsi:noNamespaceSchemaLocation", BAD_CAST "http://www.gridlab.org/WorkPackages/wp-10/Software/idss_recordset.xsd");}
#define on_error(ptr_h) { \
				        char errbuf[BUF_SIZE]; \
	                                ptr_destroy(ptr_h); \
	                                if (get_debug_flg() >= 1) \
					{ \
				           sprintf(errbuf, "Exiting %s line %d function %s\n", __FILE__, __LINE__, __func__); \
                                           idss_errmsg_r(errbuf); \
					} \
				       }

/************
* Variables
*************/


pthread_mutex_t service_mutex, firewall_mutex, webservice_mutex, parser_mutex, merge_mutex;

pthread_mutex_t db_handle_mutex;
sqlite_handle db_handle;

char ip_address[25];


/****************\
| D1HT variables |
\****************/

/* overlay D1HT : global variable, handle of the id1ht overlay */
d1ht_h* d1ht_overlay = NULL;

/* D1HT bootstrap node: derived from -j option */
char *d1ht_bootstrap_ip = NULL;

/* port for join on bootstrap node */
int d1ht_bootstrap_port = 0;

/* IP of local node */
char *d1ht_node_ip = NULL;

/* listening port for local node */
int d1ht_node_port = 0;


double redfact = 0;

typedef void    Sigfunc(int);

/*
 * static char *idss_preserve_environ[] = { "HOSTNAME",
 * "IDSS_CONF_FILENAME", "IDSS_DEBUG",
 * "TZ", NULL };
 */



static int      verbose = 0;
static char    *service_description = NULL;

struct dime_write_handle {
	char           *name;
	FILE           *fd;
};

extern char    *optarg;

void            read_service_description(struct soap * soap);

/*****************************
* Static F(unction Declaration
*****************************/


void           *process_request(void *arg);


/*******
* Code
********/

int verify_callback(int ok, X509_STORE_CTX *store)
{
        char data[256];

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering verify_callback");

        // The ok parameter of this callback indicates whether the verification of the certificate in question passed
        // (ok == 1) or failed (ok == 0)
        // If the callback returns 1 then the handshake is continued and the connection maybe established
        // To return 1 when ok == 0 requires resetting the error state with X509_STORE_CTX_set_error(store, X509_V_OK)
        // If the callback returns 0 then the handshake is immediately terminated with "verification failed"
        //  and a verification failure alert is sent to the peer.


        if (!ok)
        {

		if(X509_STORE_CTX_get_error(store) == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN){

			if (get_debug_flg() >= 1){
				idss_errmsg_r("This certificate is self-signed...");
				idss_errmsg_r("Allowing self-signed certificate for testing purposes");
				idss_errmsg_r("Self-signed certificates must not be used in production");
			}

                	// allow self-signed certificates for testing;
                	// self-signed certificates must not be used for production
                	X509_STORE_CTX_set_error(store, X509_V_OK);
                	ok = 1;
		}
        	else{

                	X509 *cert = X509_STORE_CTX_get_current_cert(store);
                	int depth = X509_STORE_CTX_get_error_depth(store);
                	int err = X509_STORE_CTX_get_error(store);

                	fprintf(stderr, "Error with certificate at depth: %i\n", depth);
                	X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
                	fprintf(stderr, " issuer = %s\n", data);
                	X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
                	fprintf(stderr, " subject = %s\n", data);
                	fprintf(stderr, " err %i:%s\n", err, X509_verify_cert_error_string(err));

        	}

	}
	else {

		if (get_debug_flg() >= 1){
			idss_errmsg_r("The following certificate has been successfully verified:");
                        X509 *cert = X509_STORE_CTX_get_current_cert(store);
                        X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
			idss_errmsg_r(" issuer = ");
			idss_errmsg_r(data);
                        X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
			idss_errmsg_r(" subject = ");
			idss_errmsg_r(data);
			idss_errmsg_r("exiting verify_callback");
		}

	}

        return ok;
}





void
read_service_description(struct soap * soap)
{
	char            buf[1024] = {'\0'};
	FILE           *fd;

	struct stat     st;

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering read_service_description");

	if (stat(SERVICE_DESCRIPTION_FILE, &st) < 0) {
		service_description = (char *) soap_strdup(soap,
				   "Can not find service description file");
		if (get_debug_flg() >= 1)
			idss_errmsg_r("exiting read_service_description");
		return;
	}
	fd = fopen(SERVICE_DESCRIPTION_FILE, "r");
	if (!fd) {
		service_description = (char *) soap_strdup(soap,
				   "Can not open service description file");
		if (get_debug_flg() >= 1)
			idss_errmsg_r("exiting read_service_description");
		return;
	}
	service_description =
		(char *) soap_malloc(soap, (st.st_size + 1) * sizeof(char));

	while (fgets(buf, 1024, fd)) {
		memcpy(service_description, buf, strlen(buf));
		service_description += strlen(buf);
	}
	fclose(fd);
	service_description[st.st_size] = '\0';
	service_description -= st.st_size;
	if (get_debug_flg() >= 1)
		idss_errmsg_r("exiting read_service_description");
	return;
}



void           *
process_request(void *arg)
{
	struct soap    *soap;

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering process_request");

	pthread_detach(pthread_self());

	soap = (struct soap *) arg;


	if (soap_ssl_accept(soap)){
       		soap_print_fault(soap, stderr);
	}
	else {
		soap_serve(soap);
		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
		free(soap);

		if (get_debug_flg() >= 1)
			idss_errmsg_r("exiting process_request");

	}

	pthread_exit(NULL);

}

/*
 * @brief performs the setup of a D1HT overlay based on the input parameters
 * @param process_name, name of the process (string)
 * @param bootstrap_ip, IP of bootstrap node (string)
 * @param bootstrap_port, listening port of bootstrap server D1HT (integer)
 * @param local_ip, IP of local node (string)
 * @param local_port, listening port of local node (integer)
 * @param card, string containing the cardinality of the D1HT overlay (a numerical string)
 *
 * @return ptr pointer to the d1ht_h data structure, which is the handle to the D1HT overlay jsut created (d1ht_h*)
 * oppure la lasciamo void ed avvalora l'oggetto globale
 */
void d1ht_setup(char *process_name, char *bootstrap_ip, int bootstrap_port, char *local_ip, int local_port, char *card, void (*query_received), void (*response_received)) {

    int d1ht_argc = 5;
    int d1ht_arglen = 128;

    //int d1ht_port = 19501;

    char **d1ht_args = malloc(d1ht_argc * sizeof(char *));

    int idx = 0;
    for (idx = 0; idx < d1ht_argc; idx ++)
    {
        d1ht_args[idx] = malloc(d1ht_arglen * sizeof(char));
        memset(d1ht_args[idx], '0', d1ht_arglen);
    }

    snprintf(d1ht_args[0], d1ht_arglen-1, "%s", process_name);

    //-A bootstrapIP:port -a ip:port -N numbers
    if (bootstrap_ip == NULL){
        // I am the bootstrap node
        snprintf(d1ht_args[1], d1ht_arglen-1, "%s", "-a");
        snprintf(d1ht_args[2], d1ht_arglen-1, "%s:%d", local_ip, local_port);
	snprintf(d1ht_args[3], d1ht_arglen-1, "%s", "-N");
        if (card == NULL) {
            snprintf(d1ht_args[4], d1ht_arglen-1, "%s", "1024");
        } else {
            snprintf(d1ht_args[4], d1ht_arglen-1, "%s", card);
        }

    } else {
        // we know the bootstrap node
        snprintf(d1ht_args[1], d1ht_arglen-1, "%s", "-a");
	snprintf(d1ht_args[2], d1ht_arglen-1, "%s:%d", local_ip, local_port);
	snprintf(d1ht_args[3], d1ht_arglen-1, "%s", "-A");
	snprintf(d1ht_args[4], d1ht_arglen-1, "%s:%d", bootstrap_ip, bootstrap_port);
    }

    // set global variable
    for (idx = 0; idx < d1ht_argc; idx++) {
        fprintf(stderr, "%s\n", d1ht_args[idx]);
    }

    d1ht_overlay = d1ht_new(d1ht_argc, d1ht_args, response_received, query_received);

    for (idx = 0; idx < d1ht_argc; idx++) {
        free(d1ht_args[idx]);
    }
    free(d1ht_args);
}




int
main(int argc, char **argv)
{
	/* main soap runtime environment */
	struct soap     soap;


	int             m;	/* master socket, listening for incoming
				 * connections */

	/* return value of getopt */
	int             c;

	/* result codes */
	int             rc = 0;
	int             rcs;

	/* port server is listening on */
	unsigned short int port;

	/* id of the generated thread */
	pthread_t       tid;
	static char    *USAGE;


	char           *conf_filename;

	char           *errfile;/* filename for stdout */
	char           *logfile;/* filename for stderr */

	char            errbuf[BUF_SIZE];

	int 		p2p_port, join_port;
	char		stringforkey[512];
	char 	       *hn = NULL;
	int		i;
	char	       *ipaddress = NULL;

	USAGE = "\nUSAGE:\n%s \n \
           \t -h this help message\n \
           \t -p <num> sets service port number\n \
           \t -f <file> sets iDSS configuration file pathname\n \
           \t -d <level> sets debug level (0-2)\n \
           \t -v prints iDSS version\n \
           \t -e <file> sets pathname of file where standard error will be redirected\n \
           \t -l <log file> sets pathname of file where iDSS run-time information will be logged\n \
	   \t -o <num> sets P2P overlay node port number\n \
	   \t -a <ipaddress> specify node IP address \n \
	   \t -r <reduction factor> specify reduction factor for TTL \n \
	   \t -j <host:port> sets the bootstrap node \n";

	port = (unsigned short int) IDSS_SERVICE_LISTENING_PORT;
	p2p_port = port + 1;

	/* get the configuration file path from the environment variable */
	if (getenv("IDSS_CONF_FILENAME")) {
		conf_filename = strdup(getenv("IDSS_CONF_FILENAME"));
		if (!conf_filename) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory; malloc failed");

			exit(EXIT_FAILURE);
		}
	} else {
		conf_filename = strdup(IDSS_CONF_FILENAME);
		if (!conf_filename) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory; malloc failed");

			exit(EXIT_FAILURE);
		}
	}

	/* parameter initialization */
	errfile = NULL;
	logfile = NULL;

	/* read command line options */
	while ((c = getopt(argc, argv, "f:p:d:e:l:o:j:a:r:hv")) != -1) {
		switch (c) {
		case 'h':
			sprintf(errbuf, USAGE, argv[0]);
			idss_errmsg_r(errbuf);
			if (conf_filename)
				free(conf_filename);
			if (logfile)
				free(logfile);
			if (errfile)
				free(errfile);
			if (hn)
				free(hn);
			if (ipaddress)
				free(ipaddress);

			exit(EXIT_SUCCESS);

		case 'p':
			port = (unsigned short int) atoi(optarg);
			sprintf(errbuf, "port:%d", port);
			idss_errmsg_r(errbuf);
			break;

		case 'd':
			verbose = atoi(optarg);
			break;
		case 'v':
			sprintf(errbuf, "IDSS - The InnoCYPES Data Storage Service, version: %s", IDSS_VERSION);
			idss_errmsg_r(errbuf);
			if (conf_filename)
				free(conf_filename);
			if (logfile)
				free(logfile);
			if (errfile)
				free(errfile);
			if (hn)
				free(hn);
			if (ipaddress)
				free(ipaddress);

			exit(EXIT_SUCCESS);

		case 'f':
			free(conf_filename);
			conf_filename = strdup(optarg);
			if (!conf_filename) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory, strdup() failed");
				if (logfile)
					free(logfile);
				if (errfile)
					free(errfile);
				if (hn)
					free(hn);
				if (ipaddress)
					free(ipaddress);

				exit(EXIT_FAILURE);
			}
			break;

		case 'l':
			logfile = strdup(optarg);
			if (!logfile) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory, strdup() failed");
				if (conf_filename)
				free(conf_filename);
				if (errfile)
					free(errfile);
				if (hn)
					free(hn);
				if (ipaddress)
					free(ipaddress);

				exit(EXIT_FAILURE);
			}
			break;

		case 'e':
			errfile = strdup(optarg);
			if (!errfile) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory, strdup() failed");
				if (conf_filename)
					free(conf_filename);
				if (logfile)
					free(logfile);
				if (hn)
					free(hn);
				if (ipaddress)
					free(ipaddress);

				exit(EXIT_FAILURE);
			}
			break;

		case 'o':
			p2p_port = (int)strtol(optarg, (char **)NULL, 10);
			d1ht_node_port = p2p_port;//+555
			break;

		case 'j':
			for(i = 0; optarg[i] != ':' && i < strlen(optarg); i++);
			optarg[i] = 0;
			hn = strdup(optarg);

			if (!hn){
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory, strdup() failed");
				if (conf_filename)
					free(conf_filename);
				if (errfile)
					free(errfile);
				if (logfile)
					free(logfile);
				if (ipaddress)
					free(ipaddress);

				exit(EXIT_FAILURE);
			}
			sscanf(optarg + (i + 1), "%d", &join_port);

            /** D1HT Bootstrap IP e Port:
	      * if the check on hn did not fail, we have the D1HT bootstrap node
              */
            d1ht_bootstrap_ip = strdup(optarg);
	    d1ht_bootstrap_port = join_port;//+555;
            break;

		case 'a':
			ipaddress = strdup(optarg);
			snprintf(ip_address, 25, "%s", ipaddress);
			if (!ipaddress) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory, strdup() failed");
				if (conf_filename)
					free(conf_filename);
				if (errfile)
					free(errfile);
				if (logfile)
					free(logfile);
				if (ipaddress)
					free(ipaddress);

                        exit(EXIT_FAILURE);
			}

                        /** D1HT local IP del nodo:
                         * se il check su ipaddress è ok, allora ho il mio IP
                         */
                        d1ht_node_ip = strdup(ipaddress);
                    break;

		case 'r':
			redfact = strtod(optarg, (char **)NULL);
			break;
		default:
			sprintf(errbuf, USAGE, argv[0]);
			idss_errmsg_r(errbuf);
			if (conf_filename)
				free(conf_filename);
			if (logfile)
				free(logfile);
			if (errfile)
				free(errfile);
			if (hn)
				free(hn);
			if (ipaddress)
				free(ipaddress);

			exit(EXIT_FAILURE);
		}
	}

	if (!ipaddress)
	{
            idss_errmsg_r("You have to specify node IP address in this test version of iDSS!");

		if (conf_filename)
			free(conf_filename);
		if (logfile)
			free(logfile);
		if (errfile)
			free(errfile);
		if (hn)
			free(hn);
		if (ipaddress)
			free(ipaddress);

            exit(EXIT_FAILURE);
	}

	if (redfact == 0)
	{

        idss_errmsg_r("You have to specify a reduction factor for TTL in this test version of iDSS!");

		if (conf_filename)
			free(conf_filename);
		if (logfile)
			free(logfile);
		if (errfile)
			free(errfile);
		if (hn)
			free(hn);
		if (ipaddress)
			free(ipaddress);

        exit(EXIT_FAILURE);
	}



	/* daemonize iDSS server */
	rc = idss_daemonize();
	if (rc) {
		if (get_debug_flg() >= 1)
			idss_errmsg_r("A problem occurred while daemonizing the iDSS server");
		if (conf_filename)
			free(conf_filename);
		if (logfile)
			free(logfile);
		if (errfile)
			free(errfile);
		if (hn)
			free(hn);
		if (ipaddress)
			free(ipaddress);
		idss_clean_up();
		xmlCleanupParser();
		exit(EXIT_FAILURE);
	}



	/* initialize registered hosts, provider list and authorized nodes */

	data_allocation();

	/* set debug level */
	set_debug_flg(verbose);

	/* sanitize file descriptors */
	idss_sanitize_files();

	/* setup of log files */
	idss_set_err_descr(errfile);

	idss_set_logfile(logfile);

	if (errfile)
		free(errfile);
	if (logfile)
		free(logfile);

	/* prevents the iDSS server form core dumping */
	idss_limit_core();

	/* setup signal dispositions */
	rcs = idss_handle_signals();
	if (rcs != 0) {
		if (get_debug_flg() >= 1)
			idss_errmsg_r("A problem occurred while setting up signal dispositions");
		if (conf_filename)
			free(conf_filename);
		if (ipaddress)
			free(ipaddress);
		idss_clean_up();
		xmlCleanupParser();

                if (hn)
			free(hn);

		exit(EXIT_FAILURE);
	}


	/* set configuration filename */
	set_conf_filename(conf_filename);
	free(conf_filename);


	pthread_mutex_init(&service_mutex, NULL);
	pthread_mutex_init(&firewall_mutex, NULL);
	pthread_mutex_init(&webservice_mutex, NULL);
	pthread_mutex_init(&db_handle_mutex, NULL);
	pthread_mutex_init(&parser_mutex, NULL);
	pthread_mutex_init(&merge_mutex, NULL);

	soap_init2(&soap, SOAP_IO_KEEPALIVE | SOAP_IO_CHUNK, SOAP_IO_KEEPALIVE | SOAP_IO_CHUNK);
	soap_ssl_init(); /* init OpenSSL */

	soap.fdimereadopen = dime_read_open;
	soap.fdimereadclose = dime_read_close;
	soap.fdimeread = dime_read;
	soap.fdimewriteopen = dime_write_open;
	soap.fdimewriteclose = dime_write_close;
	soap.fdimewrite = dime_write;

	//soap.send_timeout = 60; // 60 seconds
	//soap.recv_timeout = 60; // 60 seconds
	//soap.accept_timeout = 3600; // server stops after 1 hour of inactivity
        //soap->transfer_timeout = 30;                // 30 seconds max message transfer time (unlimited by default)
	//soap->recv_maxlength = 1048576;             // limit messages received to 1MB (2GB by default)
	soap.max_keep_alive = 100; // max keep-alive sequence
	soap.bind_flags |= SO_REUSEADDR;

	read_service_description(&soap);

	/* initialize libxml2 */
	xmlInitParser();

	if (read_conf_file(&soap)) {
                        if (get_debug_flg() >= 1) {
                                sprintf(errbuf, "No valid configuration file found: %s", get_conf_filename());
                                idss_errmsg_r(errbuf);
                        }
                        idss_clean_up();
                        xmlCleanupParser();
		 	if (hn)
				free(hn);
			if (ipaddress)
				free(ipaddress);
            exit(1);
	}

	pthread_mutex_lock(&db_handle_mutex);
	init_connection(&db_handle);
	rc = open_connection(get_db_name(), &db_handle);
	pthread_mutex_unlock(&db_handle_mutex);

	if (rc != SQLITE_OK)
 			{
			 if (rc == SQLITE_OPEN_DB_ERROR)
                         	idss_errmsg_r
					("Error while opening db connection");
			 if (rc == SQLITE_MEMORY_ERROR)
				idss_errmsg_r
					("Error while opening db connection: not enough memory!");

			 idss_clean_up();
			 xmlCleanupParser();
			 pthread_mutex_lock(&db_handle_mutex);
			 close_connection(&db_handle);
			 pthread_mutex_unlock(&db_handle_mutex);
		 	if (hn)
				free(hn);
			 if (ipaddress)
				free(ipaddress);
			 exit(EXIT_FAILURE);
                	}

	/* execute the providers manager */
	//provider_manager();

	/* execute the DBMS manager in a separate thread */
	pthread_create(&tid, NULL, &idss_dbms_manager, NULL);

	/* execute the collector in a separate thread */
	//pthread_create(&tid, NULL, &collect_main, (void *) soap_copy(&soap));


    /* setting up the D1HT P2P overlay */
    d1ht_setup(argv[0], d1ht_bootstrap_ip, d1ht_bootstrap_port, d1ht_node_ip, d1ht_node_port, "32768", on_query_received, on_response_received);

    //pthread_create(&tid, NULL, &query_check, NULL);


	soap.fsslverify = verify_callback;

	if (soap_ssl_server_context(&soap, SOAP_SSL_REQUIRE_CLIENT_AUTHENTICATION | SOAP_SSL_SKIP_HOST_CHECK | SOAP_TLSv1, "server.pem", "InnoCyPES2024", "cacert.pem", "/etc/ssl/certs", NULL, NULL, "IDSS server"))
  	{
    		soap_print_fault(&soap, stderr);
    		exit(EXIT_FAILURE);
  	}


	/* listen for incoming connections */
	m = soap_bind(&soap, NULL, port, 128);
	if (!soap_valid_socket(m)){
		if (get_debug_flg() >= 1)
			idss_errmsg_r
				("Failing to listen for incoming connections, now exiting");
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);
		idss_clean_up();
		xmlCleanupParser();
		pthread_mutex_lock(&db_handle_mutex);
		close_connection(&db_handle);
		pthread_mutex_unlock(&db_handle_mutex);
		exit(1);
	}


	/* server's main loop */
	{

		struct soap    *tsoap = NULL;
		pthread_t       tid;
		int             s;
		char            errbuf[BUF_SIZE];

		if (get_debug_flg() >= 1)
			idss_errmsg_r("entering worker");

		for (;;) {


			/* accepting an incoming connection */
			s = soap_accept(&soap);
			if (!soap_valid_socket(s)){
				break;
			}


			/* retrieving information about the peer */
			if (get_debug_flg() >= 1) {
				sprintf(errbuf,
					"Accepted  connection on socket %d from IP %u.%u.%u.%u\n",
					s, (soap.ip >> 24) & 0xFF, (soap.ip >> 16) & 0xFF,
				     (soap.ip >> 8) & 0xFF, soap.ip & 0xFF);
				idss_errmsg_r(errbuf);
			}
			tsoap = soap_copy(&soap);
			if (!tsoap)
				break;

			/* now we serve the request */
			pthread_create(&tid, NULL, &process_request, (void *) tsoap);

		}


		/* cleanup of gsoap runtime enrvironments */
		soap_destroy(&soap);
		soap_end(&soap);
		soap_done(&soap);

                /* setting down the D1HT overlay */
                d1ht_delete(d1ht_overlay);


		if (get_debug_flg() >= 1)
			idss_errmsg_r("exiting worker");
		idss_clean_up();
		xmlCleanupParser();
		pthread_mutex_lock(&db_handle_mutex);
		close_connection(&db_handle);
		pthread_mutex_unlock(&db_handle_mutex);
		exit(EXIT_FAILURE);
	}

}




/*
 * ! \fn idss__getServiceDescription(struct soap * soap, void *_,
 * xsd__string * description)
 *
 * \brief returns description and status of the service
 *
 * \param void *_: dummy input parameter to allow compatibility across different
 * unix platform \param xsd__string description: the string of description
 */
int
idss__getServiceDescription(struct soap * soap, void *_,
			     xsd__string * description)
{
	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering idss__getServiceDescription");
	*description = soap_strdup(soap, service_description);

	if (get_debug_flg() >= 1)
		idss_errmsg_r("exiting idss__getServiceDescription");
	return SOAP_OK;
}





/*
 * ! \fn idss__search_sql(struct soap * soap, xsd__string filter, xsd__string *result)
 *
 * \brief query the iStore server
 *
 * \param struct soap *soap The current gSOAP runtime environment
 * \param xsd__string  filter: SQL string to search
 * \param xsd__string *result:  query result
 *
 */

int
idss__search_sql(struct soap * soap, xsd__string filter,
		  xsd__string * result)
{
	int             rc = 0;
	int             i, k;
	int             rows;
	int             result_query;
	char            buf[20];
	int		res = 0;

	query_answer    answerh;

	/* document pointer */
	xmlDocPtr       doc = NULL;

	/* node pointers */
	xmlNodePtr      root_node = NULL, node_records = NULL, node_schema = NULL;

	init_answer(&answerh);

	rows = 0;

	/* create a new xml document */
	doc = xmlNewDoc(BAD_CAST "1.0");

	LIBXML_TEST_VERSION;


	/* Parse the SQL filter */
	if (filter) {

		rc = idss_parse(filter);
	} else {
		xmlFreeDoc(doc);
		soap_fault(soap);
		soap->fault->faultstring = "No SQL SELECT statement has been provided";
		*result = NULL;
		return SOAP_FAULT;
	}

	if (rc == -1) {
		xmlFreeDoc(doc);
		soap_fault(soap);
		soap->fault->faultstring = "Cannot parse this SQL SELECT statement";
		*result = NULL;
		return SOAP_FAULT;
	}

	pthread_mutex_lock(&db_handle_mutex);
	/* execute the query */
	result_query = exec_query(&db_handle, filter);
	if (result_query == SQLITE_OK)
		{
		 rows = n_tuples(&db_handle);
		 if (rows > 0)
			rc = single_query_retrieve_information(&db_handle, &answerh);
		}
	pthread_mutex_unlock(&db_handle_mutex);


	if (result_query != SQLITE_OK) {
		if (result_query == SQLITE_MEMORY_ERROR) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);
			exit(EXIT_FAILURE);
		} else {
			xmlFreeDoc(doc);
			soap_fault(soap);
			soap->fault->faultstring = "Query uncorrectly executes";
			*result = NULL;
			return SOAP_FAULT;
		}
	}

	if (rows > 0) {
		if (rc == -1) {
			xmlFreeDoc(doc);
			soap_fault(soap);
			soap->fault->faultstring = "Error retrieving information";
			*result = NULL;
			free_answer(&answerh);
			return SOAP_FAULT;
		}
	} else {
		xmlFreeDoc(doc);
		soap_fault(soap);
		soap->fault->faultstring = "Database is empty!";
		*result = NULL;
		free_answer(&answerh);
		return SOAP_OK;
	}

	root_node = xmlNewNode(NULL, BAD_CAST "Recordset");
	xmlAssociateRecordsetSchema(root_node);
	xmlDocSetRootElement(doc, root_node);

	node_schema = xmlNewTextChild(root_node, NULL, BAD_CAST "Schema", NULL);
	for (i = 0; i < answerh.num_field; i++) {
		xmlNodePtr      node_field;
		char           *type_char;

		node_field =
			xmlNewTextChild(node_schema, NULL, BAD_CAST "field", NULL);
		xmlNewProp(node_field, BAD_CAST "name", BAD_CAST answerh.schema[i]);
		sprintf(buf, "%d", i);
		xmlNewProp(node_field, BAD_CAST "id", BAD_CAST buf);
		type_char = return_type(answerh.type[i]);
		xmlNewProp(node_field, BAD_CAST "type", BAD_CAST type_char);
		if (type_char)
			free(type_char);
	}

	node_records = xmlNewTextChild(root_node, NULL, BAD_CAST "Records", NULL);
	sprintf(buf, "%d", answerh.num_record);
	xmlNewProp(node_records, BAD_CAST "numrecord", BAD_CAST buf);

	for (k = 0; answerh.records[k] != NULL; k++) {
		xmlNodePtr      node_record;

		node_record =
			xmlNewTextChild(node_records, NULL, BAD_CAST "record", NULL);
		sprintf(buf, "%d", k % answerh.num_field);
		xmlNewProp(node_record, BAD_CAST "field", BAD_CAST buf);
		xmlNewProp(node_record, BAD_CAST "value", BAD_CAST answerh.records[k]);

	}

	if (root_node) {
		xmlChar        *xmldoc;
		int             buffersize;

		xmlDocDumpFormatMemory(doc, &xmldoc, &buffersize, 1);
		*result = (xsd__string) soap_strdup(soap, (char *) xmldoc);
		free(xmldoc);
	}
	xmlFreeDoc(doc);
	free_answer(&answerh);
	return SOAP_OK;
}



/*
 * ! \fn idss__istore(struct soap * soap, struct dimeData * data, xsd__int * result)
 *
 * \brief Stores information about an iServe registered to the localhost iStore
 *
 * \param struct soap *soap  The current gSOAP runtime environment
 * \param struct dimeData * data  Contains the xml file to be stored
 * \param xsd__int * result  Result code
 *
 */

int
idss__istore(struct soap * soap, struct dimeData * data, xsd__int * result)
{

	int             rc;

	struct stat     sst;
	char            ffile[256] = {'\0'};
	char            sfile[256] = {'\0'};
	char            dfile[256] = {'\0'};
	char           *buf = NULL;
	char           *name = NULL;
	char            errbuf[BUF_SIZE] = {'\0'};
	FILE           *dxml = NULL;
	FILE           *sxml = NULL;
	ptr_mng         ptr_handle = NULL;


	if ((soap->omode & SOAP_IO) == SOAP_IO_STORE) {
		soap->omode = (soap->omode & ~SOAP_IO) | SOAP_IO_BUFFER;
	}

	ptr_register(&ptr_handle, (void **) &name, 0);
	ptr_register(&ptr_handle, (void **) &buf, 0);

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering idss__istore");


	//if (data->id) {
	//fprintf(stderr, "%s\n", data->id);
		if (!data->__ptr) {
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}
		if(((struct dime_write_handle *) (data->__ptr))->name)
		  name = strdup(((struct dime_write_handle *) (data->__ptr))->name);
		else
		{
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}

		if (!name) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory; strdup failed");
			on_error(ptr_handle);
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);

			exit(EXIT_FAILURE);
		}

		sprintf(sfile, "%s/%s", get_inode_info_sd(), name);
		sprintf(dfile, "%s/%s_tmp", get_inode_info_sd(), name);
		sprintf(ffile, "%s/%s", get_inode_info_sd(), name + 1);

		dxml = fopen(dfile, "w");
		if (!dxml) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error opening file %s", dfile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}
		if (!stat(sfile, &sst)) {
			int             sxmlfd;

			sxml = fopen(sfile, "r");
			if (!sxml) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error opening file %s", sfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			sxmlfd = fileno(sxml);

			buf = (char *) calloc(sst.st_size + 1, sizeof(char));
			if (!buf) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory; malloc failed");
				on_error(ptr_handle);
				idss_clean_up();
				xmlCleanupParser();
				pthread_mutex_lock(&db_handle_mutex);
				close_connection(&db_handle);
				pthread_mutex_unlock(&db_handle_mutex);

				exit(EXIT_FAILURE);
			}
			ptr_register(&ptr_handle, (void **) &buf, 0);

			if (idss_readn(sxmlfd, buf, sst.st_size) == -1) {
                                if (get_debug_flg() >= 1) {
                                        sprintf(errbuf, "Error reading %ld bytes of file %s",
                                                (long) sst.st_size, sfile);
                                        idss_errmsg_r(errbuf);
                                }
                                soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
                                                    NULL);
                                soap_send_fault((struct soap *) soap);
                                on_error(ptr_handle);
                                return SOAP_FAULT;
                        }


			fclose(sxml);

			sprintf(errbuf,
				"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<TABLES grelc_dbname=\"%s\">\n",
				get_db_name());
			if (fwrite(errbuf, sizeof(char), strlen(errbuf), dxml) !=
			    strlen(errbuf)) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error writing into file %s", dfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			if (fwrite(buf, sizeof(char), strlen(buf), dxml) != strlen(buf)) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error writing into file %s", dfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			fclose(dxml);

			pthread_mutex_lock(&service_mutex);
			ptr_register(&ptr_handle, (void **) &service_mutex, 6);
			pthread_mutex_lock(&webservice_mutex);
			ptr_register(&ptr_handle, (void **) &webservice_mutex, 6);
			pthread_mutex_lock(&firewall_mutex);
			ptr_register(&ptr_handle, (void **) &firewall_mutex, 6);

			/*
		         * This function translates the GRelC MQ file specified in
		         * the (char *) xmlresult input parameter
		         */
			pthread_mutex_lock(&db_handle_mutex);
			rc = get_sql(dfile, &db_handle, 1);
			pthread_mutex_unlock(&db_handle_mutex);

			if (rc == -1) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r
						("Error during the translation from MQ file to SQL string");
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
		}
		else
		{
		  if (get_debug_flg() >= 1)
					idss_errmsg_r
						("stat failed");
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
		}

		if (rename(dfile, ffile) == -1) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error renaming file %s to %s", dfile, ffile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);

			on_error(ptr_handle);
			return SOAP_FAULT;
		}
	        /* calls for iServe info collection */
	        //collect(soap);
#if 0
		if (unlink(sfile) == -1) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error unlinking file %s", sfile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}
#endif
//	}
//else
//fprintf(stderr, "No data_id \n");
	on_error(ptr_handle);
	*result = 0;
	return SOAP_OK;
}

int
idss__istore2(struct soap * soap, xsd__string file, xsd__string host, xsd__int * result)
{

	int             rc;

	struct stat     sst;
	char            ffile[256] = {'\0'};
	char            sfile[256] = {'\0'};
	char            dfile[256] = {'\0'};
	char           *buf = NULL;
	char           *name = NULL;
	char            errbuf[BUF_SIZE] = {'\0'};
	FILE           *dxml = NULL;
	FILE           *sxml = NULL;
	ptr_mng         ptr_handle = NULL;
	size_t len;

	FILE *fd = NULL;
	char           *spool = NULL;
	char timen[12]= {'\0'};
	time_t now = time (0);


	if(!soap || !file || !host)
	{
	  idss_errmsg_r("There are null parameters");
	  return SOAP_FAULT;
	}

	sprintf(timen, "_%ld", (long)mktime(gmtime(&now)));

	if ((soap->omode & SOAP_IO) == SOAP_IO_STORE) {
		soap->omode = (soap->omode & ~SOAP_IO) | SOAP_IO_BUFFER;
	}

	ptr_register(&ptr_handle, (void **) &name, 0);
	ptr_register(&ptr_handle, (void **) &buf, 0);
	ptr_register(&ptr_handle, (void **) &spool, 0);
	len = strlen(file);

	spool =	(char *) malloc((strlen(get_inode_info_sd()) + strlen(host) +strlen(timen) + 3) * sizeof(char));
	if (!spool) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
			return SOAP_FAULT;
	}
	strcpy(spool, get_inode_info_sd());
	strcat(spool, "/.");
	strcat(spool, host);
	strcat(spool, timen);
	printf("%s\n", spool);
	fd = fopen(spool, "wb");

	name = (char *) malloc(strlen(host) + strlen(timen) +2);
    	strcpy(name, ".");
    	strcat(name, host);
    	strcat(name, timen);

	while (len) {
		size_t          nwritten = fwrite(file, sizeof(char), len, fd);

		if (!nwritten) {
		  free(spool);
			soap->errnum = errno;
			return SOAP_EOF;
		}
		len -= nwritten;
		buf += nwritten;
	}
	fclose(fd);

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering idss__istore");


		if (!file) {
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}

		if (!name) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory; strdup failed");
			on_error(ptr_handle);
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);

			exit(EXIT_FAILURE);
		}

		sprintf(sfile, "%s/%s", get_inode_info_sd(), name);
		sprintf(dfile, "%s/%s_tmp", get_inode_info_sd(), name);
		sprintf(ffile, "%s/%s", get_inode_info_sd(), name + 1);

		dxml = fopen(dfile, "w");
		if (!dxml) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error opening file %s", dfile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}
		if (!stat(sfile, &sst)) {
			int             sxmlfd;

			sxml = fopen(sfile, "r");
			if (!sxml) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error opening file %s", sfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			sxmlfd = fileno(sxml);

			buf = (char *) calloc(sst.st_size + 1, sizeof(char));
			if (!buf) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r("Not enough memory; malloc failed");
				on_error(ptr_handle);
				idss_clean_up();
				pthread_mutex_lock(&db_handle_mutex);
				close_connection(&db_handle);
				pthread_mutex_unlock(&db_handle_mutex);

				exit(EXIT_FAILURE);
			}
			ptr_register(&ptr_handle, (void **) &buf, 0);

			if (idss_readn(sxmlfd, buf, sst.st_size) == -1) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error reading %ld bytes of file %s",
						(long) sst.st_size, sfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}

			fclose(sxml);

			sprintf(errbuf,
				"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<TABLES grelc_dbname=\"%s\">\n",
				get_db_name());
			if (fwrite(errbuf, sizeof(char), strlen(errbuf), dxml) !=
			    strlen(errbuf)) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error writing into file %s", dfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			if (fwrite(buf, sizeof(char), strlen(buf), dxml) != strlen(buf)) {
				if (get_debug_flg() >= 1) {
					sprintf(errbuf, "Error writing into file %s", dfile);
					idss_errmsg_r(errbuf);
				}
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
			fclose(dxml);

			pthread_mutex_lock(&service_mutex);
			ptr_register(&ptr_handle, (void **) &service_mutex, 6);
			pthread_mutex_lock(&webservice_mutex);
			ptr_register(&ptr_handle, (void **) &webservice_mutex, 6);
			pthread_mutex_lock(&firewall_mutex);
			ptr_register(&ptr_handle, (void **) &firewall_mutex, 6);

			/*
		         * This function translates the GRelC MQ file specified in
		         * the (char *) xmlresult input parameter
		         */
			pthread_mutex_lock(&db_handle_mutex);
			rc = get_sql(dfile, &db_handle, 1);
			pthread_mutex_unlock(&db_handle_mutex);
			if (rc != SQLITE_OK) {
				if (get_debug_flg() >= 1)
					idss_errmsg_r
						("Error during the translation from MQ file to SQL string");
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
			}
		}
		else
		{
		  if (get_debug_flg() >= 1)
					idss_errmsg_r
						("stat failed");
				soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
						    NULL);
				soap_send_fault((struct soap *) soap);
				on_error(ptr_handle);
				return SOAP_FAULT;
		}

		if (rename(dfile, ffile) == -1) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error renaming file %s to %s", dfile, ffile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}
	        /* calls for iServe info collection */
	        //collect(soap);

		if (unlink(sfile) == -1) {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error unlinking file %s", sfile);
				idss_errmsg_r(errbuf);
			}
			soap_receiver_fault((struct soap *) soap, "iStore info ingestion failed\n",
					    NULL);
			soap_send_fault((struct soap *) soap);
			on_error(ptr_handle);
			return SOAP_FAULT;
		}


	on_error(ptr_handle);
	*result = 0;

	return SOAP_OK;
}







int idss__query_test(struct soap * soap, xsd__string query, xsd__string *result)
{


	int             result_query;
	int		          rows = 0;
	char            errbuf[BUF_SIZE];


	 if (get_debug_flg() >= 1){
		idss_errmsg_r("entering query_test");
    sprintf(errbuf, "query_test: executing query %s\n", query);
		idss_errmsg_r(errbuf);
   }

	/* submit the query */
	pthread_mutex_lock(&db_handle_mutex);
	result_query = exec_query(&db_handle, query);
	if(get_debug_flg() >= 1){
    sprintf(errbuf, "query_test: exec_query SQLITE result code is %d\n", result_query);
		idss_errmsg_r(errbuf);
  }
	pthread_mutex_unlock(&db_handle_mutex);


  if(result_query != SQLITE_OK){
			soap_fault(soap);
			soap->fault->faultstring = "Error submitting query to dbms";

			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error submitting query: %s", query);
				idss_errmsg_r(errbuf);
			}

			*result = NULL;
			return SOAP_FAULT;
  }

	*result = (xsd__string) soap_strdup(soap, "Operation done successfully\n");

	 if (get_debug_flg() >= 1)
		idss_errmsg_r("exiting query_test");

	return SOAP_OK;

}






void           *
dime_read_open(struct soap * soap, void *handle, const char *id,
	       const char *type, const char *options)
{
	FILE           *fd;

	fd = fopen((char *) handle, "rb");
	return (void *) fd;
}

void
dime_read_close(struct soap * soap, void *handle)
{
	fclose((FILE *) handle);
}

size_t
dime_read(struct soap * soap, void *handle, char *buf, size_t len)
{
	return fread(buf, 1, len, (FILE *) handle);
}

void           *
dime_write_open(struct soap * soap, const char *id, const char *type,
		const char *options)
{

	struct dime_write_handle *handle;
	char           *spool = NULL;

fprintf(stderr, "write_open: %s\n", options+4);
	handle =
		(struct dime_write_handle *) soap_malloc(soap,
							 sizeof(struct
							dime_write_handle));

	if (!handle) {
		soap->error = SOAP_EOM;
		return NULL;
	}
	if (options) {
		size_t          len = ((size_t) options[2] << 8) | ((size_t) options[3]);

		handle->name = (char *) soap_malloc(soap, len + 1);
		strncpy(handle->name, options + 4, len);
		handle->name[len] = '\0';

		spool =
			(char *)
			malloc((strlen(get_inode_info_sd()) + strlen(handle->name) +
				2) * sizeof(char));
		if (!spool) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
			return NULL;
		}
		strcpy(spool, get_inode_info_sd());
		strcat(spool, "/");
		strcat(spool, handle->name);
		handle->fd = fopen(spool, "wb");
		free(spool);
		if (!handle->fd) {
			soap->error = SOAP_EOF;
			soap->errnum = errno;
			return NULL;
		}
	} else
		soap->error = soap_receiver_fault(soap,
						  "Cannot save to file, because no file name was present in attachment",
						  NULL);

	return (void *) handle;
}

void
dime_write_close(struct soap * soap, void *handle)
{
	fclose(((struct dime_write_handle *) handle)->fd);
}

int
dime_write(struct soap * soap, void *handle, const char *buf, size_t len)
{
fprintf(stderr, "Writing file..\n");
	while (len) {
		size_t          nwritten =
		fwrite(buf, 1, len, ((struct dime_write_handle *) handle)->fd);

		if (!nwritten) {
			soap->errnum = errno;
			return SOAP_EOF;
		}
		len -= nwritten;
		buf += nwritten;
	}
	return SOAP_OK;
}



int
idss__query_submit (struct soap *soap, xsd__string value, xsd__int ttl, struct idss__query_submit_response *res)
{
	char    sqlh[10000] = {0};
	char		*_uqi = NULL;
	long long	arrive_time;
	char            errbuf[BUF_SIZE] = {0};
 	int		result_query = 0;
	char		data[8192] = {0};


 	query_s		*qs = NULL;
	/* id of the generated thread */
	pthread_t       tid;
	int 		rc = 0, result = 0;
	long long 	start = 0;

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering idss__query_submit");

	res->uqi = NULL;
	res->result = 1;

	//Initializing
	qs = (query_s *) calloc(1, sizeof(query_s));
	if(!qs){
		if (get_debug_flg() >= 1)
			idss_errmsg_r("Not enough memory");

		idss_clean_up();
		xmlCleanupParser();
		pthread_mutex_lock(&db_handle_mutex);
		close_connection(&db_handle);
		pthread_mutex_unlock(&db_handle_mutex);

		exit(EXIT_FAILURE);
	}

	if (value == NULL)
	{
		soap_fault(soap);
		soap->fault->faultstring = "No input query specified!";
		return SOAP_OK;
	}

	start = get_current_time();

	//First parse of input query
	rc = buildAST(value, &(qs->e));

  	if (rc != 0)
   	{
		freeResult(qs->e);
  		soap_fault(soap);
		soap->fault->faultstring = "Cannot parse this SQL SELECT statement";
		return SOAP_FAULT;
   	}

  	if ((qs->e.parentIsAgg > 0) && (qs->e.numOfAggSubs > 0))
   	{
		//Our parent query contains agg functions and also subqueries with agg functions
		freeResult(qs->e);
		soap_fault(soap);
		soap->fault->faultstring = "This version of iDSS cannot execute this SQL SELECT statement: aggregate function in parent query and subqueries!";
		return SOAP_FAULT;
   	}

   	if ((qs->e.numOfAggSubs > 0) && (qs->e.numOfSubs > qs->e.numOfAggSubs))
   	{
		//We have subqueries with agg functions and subqueries without
		freeResult(qs->e);
		soap_fault(soap);
		soap->fault->faultstring = "This version of iDSS cannot execute this SQL SELECT statement: subqueries with aggregate functions and subqueries without aggregate functions inside the same nested query!";
		return SOAP_FAULT;
   	}

	get_random_value (&_uqi);
	if (_uqi == NULL)
		{
		 if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
		 freeResult(qs->e);
		 idss_clean_up();
		 xmlCleanupParser();
		 pthread_mutex_lock(&db_handle_mutex);
		 close_connection(&db_handle);
		 pthread_mutex_unlock(&db_handle_mutex);

		 exit(EXIT_FAILURE);
		}

	snprintf(errbuf, BUF_SIZE, "Query %s submitted at time %lld", _uqi, start);
  	idss_errmsg_r(errbuf);

	//snprintf(errbuf, BUF_SIZE, "%s%s%s%s%s%s%s%lld", MYKEY, SEP, p2p_nodekey, UQI, _uqi, SEP, RCVAT, start);
	snprintf(errbuf, BUF_SIZE, "%s%s%s%s%s%s%s%lld", MYKEY, SEP, "NULL", UQI, _uqi, SEP, RCVAT, start);
	print_result(_uqi, ip_address, errbuf);

	arrive_time = get_current_time();

	snprintf(sqlh, 10000,
		"INSERT INTO tb_query (uqi, value, ttl, arrive_time, sender_key) VALUES ('%s', '%s', %d, %lld, NULL)",
		_uqi, value, ttl, arrive_time);

	pthread_mutex_lock(&db_handle_mutex);
	result_query = exec_query(&db_handle, sqlh);
	pthread_mutex_unlock(&db_handle_mutex);

	if (result_query != SQLITE_OK) {
		if (result_query == SQLITE_MEMORY_ERROR) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
			freeResult(qs->e);
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);

			if (_uqi != NULL)
				{
				 free (_uqi);
				 _uqi = NULL;
				}
			exit(EXIT_FAILURE);
		} else {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error submitting query: %s", sqlh);
				idss_errmsg_r(errbuf);
			}
			if (_uqi != NULL)
				{
				 free (_uqi);
				 _uqi = NULL;
				}
			freeResult(qs->e);
			soap_fault(soap);
			soap->fault->faultstring = "Error submitting query to dbms";
			return SOAP_FAULT;
		}
	}

	qs->uqi = strdup(_uqi);
	qs->value = strdup(value);
	qs->key = strdup("(NULL)");
	qs->ttl = (double) ttl;
	if ((!qs->uqi) || (!qs->value) || (!qs->key))
	{
		if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
		freeResult(qs->e);
		idss_clean_up();
		xmlCleanupParser();
		pthread_mutex_lock(&db_handle_mutex);
		close_connection(&db_handle);
		pthread_mutex_unlock(&db_handle_mutex);

		if (qs->uqi)
			{
			 free(qs->uqi);
			 qs->uqi = NULL;
			}
		if (qs->value)
			{
			 free(qs->value);
			 qs->value = NULL;
			}
		if (qs->key)
			{
			 free(qs->key);
			 qs->key = NULL;
			}

		if(qs){
			free(qs);
			qs = NULL;
		}
		if (_uqi != NULL)
			{
			 free (_uqi);
			 _uqi = NULL;
			}
		exit(EXIT_FAILURE);
	}

	pthread_create(&tid, NULL, &query_execution, (void *) qs);


	// TTL reduction is handled within D1HT

	/** D1HT query broadcast via EDRA
         * I am the iDSS+D1HT node who is the query originator in the D1HT overlay
         * Regarding the query, I know: uqi, ttl, sql
 	*/
        char temp[256] = "";
        snprintf(temp, 255, "D1HT broadcast query '%s', with uqi '%s' and ttl=%d- redfact =%f", value, _uqi, ttl, redfact);
        idss_errmsg_r(temp);

        int dissemination = d1ht_broadcast_query(d1ht_overlay, value, _uqi, ttl, redfact);



	res->uqi =  soap_strdup (soap, _uqi);
        res->result = 0;
	if (_uqi)
		{
		 free(_uqi);
		 _uqi = NULL;
		}

	if (get_debug_flg() >= 1)
		idss_errmsg_r("exiting idss__query_submit");

	return SOAP_OK;
}








int
idss__get_file(struct soap * soap, xsd__string uqi, struct idss__get_file_response *res)
{
	char sqlh[256] = {0};
	int result_query = 0;
	int rows_n = 0;
	query_answer answer;
	int temp = 0;
	char filename[1024] = {0};
	char *buffer = NULL;
	char *start = NULL;
	FILE *file_a = NULL;
   	struct stat sst;
	size_t nbyte = 0;
	char errbuf[BUF_SIZE] = {0};

	if (get_debug_flg() >= 1)
		idss_errmsg_r("entering idss__get_file");

	res->result = NULL;
	res->status = 1;
	init_answer(&answer);

	snprintf(sqlh, 256, "SELECT uqi, local_exec, completed, failed FROM tb_query WHERE uqi = '%s' AND sender_key IS NULL", uqi);
	pthread_mutex_lock(&db_handle_mutex);
	result_query = exec_query(&db_handle, sqlh);
	if (result_query == SQLITE_OK)
		{
		 rows_n = n_tuples(&db_handle);
		 if (rows_n > 0)
			single_query_retrieve_information(&db_handle, &answer);
		}
	pthread_mutex_unlock(&db_handle_mutex);

	if (result_query != SQLITE_OK){
		if (result_query == SQLITE_MEMORY_ERROR) {
			if (get_debug_flg() >= 1)
				idss_errmsg_r("Not enough memory");
			idss_clean_up();
			xmlCleanupParser();
			pthread_mutex_lock(&db_handle_mutex);
			close_connection(&db_handle);
			pthread_mutex_unlock(&db_handle_mutex);

			free_answer(&answer);
			exit(EXIT_FAILURE);
		} else {
			if (get_debug_flg() >= 1) {
				sprintf(errbuf, "Error submitting query: %s", sqlh);
				idss_errmsg_r(errbuf);
			}

			soap_fault(soap);
			soap->fault->faultstring = "Error submitting query to dbms";
			free_answer(&answer);
			return SOAP_FAULT;
		}
	}

	if (rows_n == 0)
		{
		 //Unable to find a query with the specified uqi
		 res->result = NULL;
		 res->status = 1;
		}
	else 	{
		 temp = (int)strtol(answer.records[3], (char **)NULL, 10);
		 if (temp != 0)
			{
			 //Query failed
			 res->result = NULL;
		 	 res->status = 4;
			}
		 else
			{
			 temp = (int)strtol(answer.records[2], (char **)NULL, 10);
		 	 if (temp == 0)
				{
			 	 //Query not completed yet
			 	 res->result = NULL;
		 	 	 res->status = 2;
				}
		 	 else
				{
			 	 //Query completed
			 	 snprintf(filename, 1024, "%s/%s_f", get_query_results_sd(), uqi);
			 	 if (!stat(filename, &sst) && sst.st_size > 0)
          				{
				 	 file_a = fopen(filename, "r");
				 	 buffer = (char *) calloc (sst.st_size + 1, sizeof (char));
    				 	 if (!buffer)
    						{
  					 	 if (get_debug_flg() >= 1)
          						idss_errmsg_r("Memory allocation error");
					 	 idss_clean_up();
					 	 xmlCleanupParser();
						 free_answer(&answer);
					 	 fclose(file_a);
					 	 file_a = NULL;
					 	 pthread_mutex_lock(&db_handle_mutex);
					 	 close_connection(&db_handle);
					 	 pthread_mutex_unlock(&db_handle_mutex);

					 	 exit(EXIT_FAILURE);
    						}
				 	 start = buffer;
				 	 while(!feof(file_a))
						{
					 	 nbyte = fread(buffer, 1, 1024, file_a);
					 	 buffer = buffer + nbyte;
						}
				 	 fclose(file_a);
				 	 file_a = NULL;
				 	 *buffer = 0;
				 	 buffer = start;
				 	 res->result =  soap_strdup (soap, buffer);
        			 	 res->status = 0;
				 	 free(buffer);
				 	 buffer = NULL;
				 	 start = NULL;
					}
			 	 else	{
				 	 //Unable to find the query file
				 	 res->result = NULL;
		 	 	 	 res->status = 3;
					}
				}
			}
		}

	free_answer(&answer);
	return SOAP_OK;
}


struct Namespace namespaces[] = {
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/"},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/"},
	{"xsi", "http://www.w3.org/1999/XMLSchema-instance"},
	{"xsd", "http://www.w3.org/1999/XMLSchema"},
	{"idss", "urn:idss"},
	{"ns", "urn:security.idss.org"},
	{NULL, NULL}
};
