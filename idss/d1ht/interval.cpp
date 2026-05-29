/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             		 *
 *  No title to or ownership of D1HT is hereby transferred.	                 *
 *									     									 *
 *  D1HT is free software; you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License version 3 as        * 
 *  as published by the Free Software Foundation. Besides, the               *
 *  recipient of D1HT acknowledges and agrees to                             *
 *  strictly respect the following guidelines: 	                             *
 *                                                                           *
 *  (1) This whole copyright notice shall not be removed or modified,        *
 *      although it can and should be appended with informations about       *
 *      modifications included in the software                               *
 *  (2) No results or statistics obtained with the software compiled         *
 *      with the DEBUG flag turned on should be considered as valid          *
 *  (3) Any modified version of this software should be made publicy         *
 *      available BEFORE the results or statistics obtained with this        *
 *      respective version are published.                                    *
 *  (4) Errors and problems with the software (including its results and     *
 *      statistics) should be immediately reported to support.D1HT@gmail.com *
 *  (5) After modifying the source code, it is necessary to compile D1HT     *
 *      with the DEBUG flag on and run it with the flags “–c –V –T” with at  *
 *      least a few dozen peers before considering the modifications as valid*
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with D1HT. If not, see <http://www.gnu.org/licenses/>.              *
 *                                                                           *
 * D1HT is distributed in the hope that it will be usefull, but              *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU GPL      *
 * for more details.                                                         *
 *                                                                           *
 * The information in this software is subject to change without notice      *
 * and should not be construed as a commitment by its authors.               *
 *                                                                           *
 * The authors assume no responsibility or liability for                     *
 * the use, reliability, performance, and results of D1HT.                   *
 *                                                                           *
 *     ################  ==> Please see ChangeLog <==  ################      *
 *                                                                           *
 *****************************************************************************/

/*************************************************************************
 *                    INITIAL VERSION REMARKS                            *
 *                                                                       *
 * Author:        Luiz Rodolpho Monnerat, Petrobras                      *
 * Advisor:       Prof. Claudio L Amorim, COPPE Systems Engineering,     *
 *                Federal University of Rio de Janeiro.                  *
 * Documentation: L. Monnerat and C. Amorim, Peer-to-Peer Single Hop     *
 *                Distributed Hash Tables, in Proceedings of the IEEE    *
 *                Global Communications Conference (GLOBECOM), Nov/2009. *
 *                Available from http://www.cos.ufrj.br/~monnerat        *
 *                                                                       *
 *************************************************************************
 *                                                                       *
 * The msg headers used in this implementation are slightly different(and*
 * bigger) than the ones used in the experiments presented in the        *
 * GLOBECOM 2009 paper. This is because we included a 4 bytes “systemid” *
 * field in the message headers presented in Figures 1.a and 1.b of      *
 * the GLOBECOM paper, and so all messages will have 4 more bytes (these *
 * 4 bytes should be added to all D1HT, CALOT and OneHop msgs, including *
 * acks). This was done because in some tests that we made after         *
 * uploading the final paper version, we observed that when              *
 * running 2 distinct D1HT and/or CALOT  peers in the same node, a peer  *
 * from one system could (by mistake or software bug) send a msg to the  *
 * peer of the other system, which would pollute both peers Routing      *
 * Tables.                                                               *
 *                                                                       *
 *************************************************************************/
/*********************** D1HT COPYRIGHT NOTICE END **********************/
/**
 * @file interval.c
 * @brief gestione degli eventi allo scadere dell'intervallo theta
 */

#include "D1HT.h"

namespace d1ht {
    extern join_t G_joining_peers;
    extern ttls_received_t G_ttls_received;

    interval_t *G_inter_current;
    theta_t G_theta;

    events_t g_last_pred_event; // protected by rt_mutex

    extern void *close_interval(const int npeers, const bool mutexed, const bool exiting, const bool timeout, const bool forced);

    void *routing_table::ack_event_pred(const IPaddr ip, const int itype) {

        events_t temp_event;
        BZERO(temp_event);

        temp_event.peer = ip;
        temp_event.type = itype;

        LOCK(rt_mutex);
        temp_event.TTL = rho(npeers() + 1);

        if (temp_event == g_last_pred_event) {
            UNLOCK(rt_mutex);
            VERBOSE(stderr, "Discharging duplicated event with predecessor: IP = %s:%d, TTL=%d, TYPE=%d\n", IPtoP(ip), temp_event.TTL, itype);
            return NULL;
        }

        g_last_pred_event = temp_event;

        ASSERT(itype <= LEAVES);

        G_stats.total.stats[stat_events_detected].count++;
        if (itype == JOINS) {
            G_stats.total.stats[stat_joins].count++;
        } else {
            G_stats.total.stats[stat_leaves].count++;
        }

        UNLOCK(rt_mutex);

        VERBOSE(stderr, "Acknowledging event with predecessor: IP = %s:%d, TTL=%d, TYPE=%d\n", IPtoP(ip), temp_event.TTL, itype);
        ack_event(ip, temp_event.TTL, itype);

        return NULL;
    }

    void *routing_table::ack_event(const IPaddr ip, const int TTL, const int itype) {

        VERBOSE(stderr, "ack IP = %s:%d, TTL=%d, TYPE=%d\n", IPtoP(ip), TTL, itype);
        ASSERT(itype <= LEAVES);

        int ittl = TTL;

        eventmsg_t* tempmsg = CALLOC(1, eventmsg_t);

        tempmsg->fill(ip, ittl, itype);

        ack_msg(tempmsg);

        FREE_DESTROY(tempmsg);

        return NULL;
    }

    /**
     * dopo aver ricevuto un eventmsg contenente gli eventi di join e leaves ed aver aggiornato consenguentemente la RT, creo gli eventi da forwardare che allo scadere dell'intervallo theta verranno  inoltrati ai miei rho targets 
     * @param msg
     * @return 
     */
    void *routing_table::ack_msg(eventmsg_t* msg) {

        //se ho inviato io il messaggio, mi fermo
        ASSERT(msg->whosent != G_parms.myIP);
#if defined(DEBUG)
        char whosent_char[20];
        sprintf(whosent_char, "%s:%u", IPtoP(msg->whosent));
        if (G_parms.check) msg->eventmsg_check(__FILE__, __LINE__);
#endif

        ASSERT(msg->TTL != D1HT_TTL_0_NOEVENTS);
        ASSERT((msg->body.nevents[0][0] + msg->body.nevents[0][1] + msg->body.nevents[1][0] + msg->body.nevents[1][1]) > 0);

        //inoltro il messaggio ai nodi che ho nella lista joining (presumo quelli che non hanno completato il join)
        G_joining_peers.forward_msg(msg);


        //  We will first calculate the IDs for all events brought the message. Those IDs will make the critical section locked by rt_mutex
        //  faster, and will be used to enforce rule 8 when forwarding the events
        msg->set_ids();
        ASSERT(msg->tot_events != 0);

#if defined(DEBUG)
        if (G_parms.VERBOSE) {
            if (msg->whosent.port == 0 && msg->whosent.ip.s_addr != G_parms.myIP.ip.s_addr) {
                VERBO(stderr, "Forwarded msg with TTL=%u received SeqNo=%u nevents=%u\n", msg->TTL, msg->SeqNo, msg->tot_events);
                ASSERT(msg->whosent.ip.s_addr == 0);
            } else if (msg->whosent.port == 0 && msg->whosent.ip.s_addr == G_parms.myIP.ip.s_addr) {
                VERBO(stderr, "Event locally detected SeqNo=%u nevents=%u\n", msg->SeqNo, msg->tot_events);
                ASSERT(msg->tot_events == 1);
                ASSERT(msg->SeqNo == 1);
            } else {
                VERBO(stderr, "Maintenance msg with TTL=%u received from=%s:%u SeqNo=%u nevents=%u\n", msg->TTL, IPtoP(msg->whosent), msg->SeqNo, msg->tot_events);
            }
            for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
                for (int itype = JOINS; itype <= LEAVES; itype++) {
                    if (msg->body.nevents[iport][itype] == 0) continue;
                    VERBOSE(stderr, "Events received, port=%u, type=%u, qty=%u: ", iport, itype, msg->body.nevents[iport][itype]);
                    PORTNO portno;
                    for (int ievent = 0; ievent < msg->body.nevents[iport][itype]; ievent++) {
                        if (iport == DEFAULT_PORT) {
                            portno = G_parms.default_udp_port;
                        } else {
                            portno = msg->eventlist[iport][itype].ports[ievent];
                        }
                        fprintf(stderr, "%s:%u, ", inet_ntoa(msg->eventlist[iport][itype].ips[ievent]), ntohs(portno));
                    }
                    fprintf(stderr, "\n");
                }
            }
        }
#endif
        LOCK(rt_mutex);

        //questo è il mio predecessore
        IPaddr predecessor = pred_IP();

        if (G_exiting || !G_started) {
            UNLOCK(rt_mutex);
            return NULL;
        }

        int temp_npeers;
        IPaddr old_pred = pred_IP();

        //DEFAULT_PORT e OTHER_PORT (0 e 1 rispettivamente), hanno lo stesso valore di JOINS e LEAVES
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {


            //mi prendo tutti i leaves, identificando i nodi per le KEYs 
            for (int ievent = 0; ievent < msg->body.nevents[iport][LEAVES]; ievent++)
                del(msg->eventlist[iport][LEAVES].ids[ievent]);
            //elimino il nodo cui corrisponde il leave event

            //mi prendo tutti i joins, prendendo IP, porta e Key dei peers
            for (int ievent = 0; ievent < msg->body.nevents[iport][JOINS]; ievent++)
                add(msg->eventlist[iport][JOINS].ips[ievent], msg->eventlist[iport][JOINS].ports[ievent], msg->eventlist[iport][JOINS].ids[ievent]);
            //aggiungo il nodo cui corrisponde un evento di join
        }

        //chi ha inviato il messaggio?
        if (msg->whosent.port != 0 && msg->whosent.ip.s_addr != 0) {
            if (msg->SeqNo == (NUM_8) 0) { //chi ha inviato il messaggio a sua volta sta facendo il LEAVE: lo elimino
                // SeqNo 0 is used when the peer is leaving
                del(msg->whosent);
            } else {
                //lo aggiungo alla RT
                add(msg->whosent);
                //se chi ha inviato non è il mio prec e non è il vecchio prec e il TTL è <4
                if (msg->whosent != pred_IP() && msg->whosent != old_pred && msg->TTL < 4) {

                    int offset = (int) pow(2, (double) msg->TTL) - 1;
                    
                    //calcolo il precedente di ordine offset
                    IPaddr finger = predn(G_RT.pred_ID(), offset);

                    //se questo prec non è chi ha inviato il messaggio, vuol dire che ho ricevuto un msg dal nodo sbagliato 
                    if (finger != msg->whosent) {
                        VERBOSE(stderr, "WARNING Cause: msg received from wrong peer. Detail: TTL==%u from %s (id=%u), while it should be %s:%u (id=%u)\n", (uint) msg->TTL,
                                whosent_char, (uint) ip2id(msg->whosent), IPtoP(finger), (uint) ip2id(finger));

                        //print_table(stderr);

                        if (msg->TTL == 0 && INSTRETCH(ip2id(pred_IP()), G_myID, ip2id(msg->whosent))) G_probe.probe(pred_IP(), __FILE__, __LINE__);
                        
                        //invoco stabilize: ricalcolo dei succ e prec
                        stabilize(msg->whosent, __FILE__, __LINE__);
                    }
                }
            }
        }

        temp_npeers = npeers();

        UNLOCK(rt_mutex);

        if (msg->whosent.port == 0 && msg->whosent.ip.s_addr == 0) {
            // forwarded msg
            return NULL;
        }

        G_theta.update_erate(temp_npeers, msg->tot_events);

        if (msg->TTL == 0) {

            return NULL;
        }

        G_ttls_received.received_ttl(msg->TTL);

        events_t *prev_event = CALLOC(1, events_t);
        events_t *new_events = prev_event;
        int valid_events = 0;

        
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {
            for (int itype = JOINS; itype <= LEAVES; itype++) {
                for (int ievent = 0; ievent < (int) msg->body.nevents[iport][itype]; ievent++) {
                    events_t *next_event = CALLOC(1, events_t);
                    next_event->peer.ip = msg->eventlist[iport][itype].ips[ievent];
                    if (iport == DEFAULT_PORT) {
                        next_event->peer.port = G_parms.default_udp_port;
                    } else {
                        next_event->peer.port = msg->eventlist[iport][itype].ports[ievent];
                    }
                    next_event->id = msg->eventlist[iport][itype].ids[ievent];
                    ASSERT(next_event->id == ip2id(next_event->peer));

                    next_event->TTL = msg->TTL;
                    next_event->type = itype;

                    valid_events++;

                    VERBOSE(stderr, "event=(id=%u,ip=%s:%u,ttl=%d,type=%d)\n", next_event->id, IPtoP(next_event->peer), next_event->TTL, next_event->type);
                    
                    prev_event->next = next_event;
                    prev_event = next_event;
                }
            }
        }
        new_events = new_events->next;

        if (valid_events == 0) return NULL;

        LOCK(G_interval_mutex);
        if (G_inter_current == NULL) {
            UNLOCK(G_interval_mutex);
            ASSERT(G_exiting);
            return NULL;
        }

        if (G_inter_current->time_first_event == 0) G_inter_current->time_first_event = msg->time_received;

        events_t head_chain;
        events_t *old_event;

        head_chain.next = G_inter_current->events;

        events_t *temp_event = NULL;

        for (events_t *next_event = new_events; next_event != NULL; next_event = temp_event) {
            prev_event = &head_chain;
            for (old_event = head_chain.next; old_event != NULL && old_event->id != next_event->id; old_event = old_event->next) {
                prev_event = old_event;
            }
            Assert(prev_event->next == old_event, XEQ_ERROR);
            if (old_event != NULL) { // duplicated event
                valid_events--;
                ASSERT(old_event->id == next_event->id);

                old_event->TTL = MAX(old_event->TTL, next_event->TTL);
                if (old_event->type != next_event->type || old_event->peer != next_event->peer) {
                    VERBOSE(stderr, "mismatched event ip=%s:%u, id=%u\n", IPtoP(old_event->peer), old_event->id);
                    // if we have a join and a leave for the same IP, we remove both events and spawn a thread to check it and propagate the right event
                    prev_event->next = old_event->next;
                    G_inter_current->E--;
                    PTHREAD_CREATE(probe_and_propagate, old_event, &G_detached);
                }
                temp_event = next_event->next;
                FREE(next_event);
            } else {
                temp_event = next_event->next;
                next_event->next = head_chain.next;
                head_chain.next = next_event;
            }

        }
        ASSERT(valid_events >= 0);

        if (G_inter_current->E == 0) G_inter_current->bigger_ttl = 0;

        VERBOSEX(stderr, "E=%u, valid_events=%d, bigger_ttl=%d, msg->TTL=%d, G_exiting=%d\n",
                (uint) G_inter_current->E, (int) valid_events, (int) G_inter_current->bigger_ttl, (int) msg->TTL, (int) G_exiting);

        ASSERT(!(G_inter_current->bigger_ttl > 0 && G_inter_current->E == 0));
        ASSERT(valid_events >= 0);

        if (valid_events == 0) {
            UNLOCK(G_interval_mutex);
            return NULL;
        } else {
            if (msg->TTL > G_inter_current->bigger_ttl) G_inter_current->bigger_ttl = msg->TTL;
        }

        G_inter_current->E += valid_events;
        G_inter_current->T_bytes += (int) (valid_events * sizeof (IPaddr));
        G_inter_current->events = head_chain.next;

        //G_inter_current->print_check(__FILE__, __LINE__);
        //WARNING: close_interval will UNLOCK G_interval_mutex!!!!!
        close_interval(temp_npeers, true, false, false, false);

        return NULL;
    }

} // namespace d1ht
