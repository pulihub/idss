/**
 * @file d1ht_h.h
 *
 *  Created on: Nov 23, 2010
 *      Author: anderson
 *
 *  Modified on: Aug 17, 2012 by Luiz Monnerat
 *
 */

#ifndef P2P_DHT_H_
#define P2P_DHT_H_

#include "D1HT.h"
#include <arpa/inet.h>
#include "wrapper/query_defs.h"

namespace d1ht {

    class d1ht_t {
        pthread_t d1ht_ptid;

    public:

        d1ht_t() {
        };
        
        d1ht_t(int argc, char** argv);
        d1ht_t(int argc, char** argv, const bool _wait_join);
        
        //d1ht_t(int argc, char** argv, void (*response_received)(void *p), void (*query_received)(char *sql, char *uqi, int ttl, long origin_key, char *sender) );
        d1ht_t(int argc, char** argv, void (*response_received)(void *args), void (*query_received)(void *args) );
        
        
        
        ~d1ht_t();

        bool wait_join();
        void start_d1ht(int argc, char** argv, const bool _wait_join);
        void stop_d1ht(const bool err, const char* cause, const bool should_exit);

        bool route(const char key[D1HT_MAX_KEY_SIZE], IPaddr& addr, void* payload, const int payload_len, void* reply_payload, int reply_payload_len);
        bool route(const KEYID key, IPaddr& addr, void* payload, const int payload_len, void* reply_payload, int reply_payload_len);
        bool lookup(const char key[D1HT_MAX_KEY_SIZE], IPaddr& addr);
        bool lookup(const KEYID key, IPaddr& addr);
        bool succ(const IPaddr& peer, const int count, IPaddr& succ);
       
    };

}
#endif /* d1ht_H_ */
