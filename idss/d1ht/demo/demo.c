/**
 * @file demo.c
 *
 * @brief test di D1HT con le estensioni HPC Lab
 *
 * @TODO implementare il get delle risposte ad un messaggio con id noto.
 */

#include "d1ht_wrapper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * global vars: ogni volta che ricevo una query salvo uqi e mittente in queste var globali
 * fake simulation di una reply da IDSS
*/
char G_uqi[256] = {0};
char G_dest[256] = {0};





void usage(){
    printf("Premere:\n");
    printf("\t - d per stampare la routing table del nodo\n");
    printf("\t - m per distribuire un messaggio sull'overlay\n");
    printf("\t - r per inserire una risposta ad un messaggio ricevuto dall'overlay\n");
    printf("\t - q per terminare il processo\n\n");
}



/**
 * callbacks
 *
 */

void onSendResponse(void *ctx) {
	printf("\n\tCallback function 'onSendResponse': '%s'\n",  (char *)ctx);
}


void onQueryRecvd(char *sql, char *uqi, int ttl, long origin_key, char *sender) {
    printf("\n\tHello from callback function 'onQueryRecvd'\n");

    printf("Received message with: ");
    printf("\n\tquery='%s'", sql);
    printf("\n\tuqi='%s'", uqi);
    printf("\n\tdeadline=%d", ttl);
    printf("\n\toriginKey: %ld", origin_key);
    printf("\n\tsender node='%s'\n\n",sender);

    memset(G_uqi, '\0', 256);
    memset(G_dest, '\0', 256);

    memcpy(G_uqi,  uqi, strlen(uqi));
    memcpy(G_dest, sender, strlen(sender));
}





int main(int argc, char** argv) {

    d1ht_h* overlay = d1ht_new(argc, argv, onSendResponse, onQueryRecvd);


    char c;         /**< scanf, per decidere cosa fare */
    int quit = 0;   /**< per terminare l'esecuzione */


    //valori immessi dall'utente per il test
    int id = 0;
    char uqi[256] = "";
    int jobTime = 0;
    char msg[256] = "";

    usage();

    while (!quit)
    {
        scanf(" %c", &c);
        fflush(stdin);

        if (c == 'q') {
            quit = 1;
        }
        else if (c == 'r') {
            //rispondo ad una query precedentemente ricevuta, di cui ho G_uqi e G_dest

//            char uqi[256] = {0};
//            char destination[80] = {0};

//            printf("\n\tUQI query (string int) cui rispondere: ");
//            scanf(" %s", uqi);

//            printf("\n\tdestinazione ip:porta: ");
//            scanf(" %s", destination);

            char res[256] = {0};
            printf("\n\trisposta: ");
            scanf(" %s", res);

            fflush(stdin);

            d1ht_send_response(overlay, G_uqi, G_dest, res);

        }
        else if (c == 'd') {

            d1ht_dumpRT(overlay);

        }
        else if (c == 'm') {

            printf("\n\tPer inviare un messaggio nell'overlay D1HT inserire un uqi(string int): ");
            scanf(" %[^\n]s ", uqi);
            printf("\n\tun tempo di attesa(int) e una stringa(\"\")\n\n");
            scanf(" %d %[^\n]s", &jobTime, msg);

            fflush(stdin);
            d1ht_broadcast_query(overlay, msg, uqi, jobTime, 0.75);

            printf("\nMessage with uqi='%s' broadcasted over D1HT overlay\n\n", uqi);

        }//fi
    }//wend

    d1ht_delete(overlay);
    return 0;
}
