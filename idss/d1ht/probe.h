/*********************** D1HT COPYRIGHT NOTICE BEGIN **********************/

/*****************************************************************************
 *                                                                           *
 *  Copyright (c) 2009 Luiz Monnerat, Rio de Janeiro, Brazil                 *
 *                                                                           *
 *  This file is part of D1HT.                                               *
 *                                                                           *
 *  D1HT is furnished under a GNU GPL license and may be used and            *
 *  copied only in accordance with the terms of such license and with the    *
 *  inclusion of this whole copyright notice.     		             *
 *  No title to or ownership of D1HT is hereby transferred.	             *
 *									     *
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
 * @file probe.h
 * @brief 
 */


#ifndef PROBE_STAB_H_
#define PROBE_STAB_H_

/**
 * definizione del thread probe
 */
void* probe_pthread(void* host);

/**
 * definizione del thread probe_and_propagate
 */
void* probe_and_propagate(void* event);

/**< classe dei probed peer */
class probed_peers_t {
public:
    IPaddr ip;
    int last_probe;

    /**
     * costruttore della classe, che istanzia un oggetto con un IP e il tempo di probe (adesso)
     * @param host
     */
    probed_peers_t(const IPaddr host) {
        ip = host;
        last_probe = (int) elapsed_msecs(); /**< il tempo di probe è quello attuale */
    }

    /**
     * da quanto non invio un probe al peer
     * @param delta
     * @param reset
     * @return true se l'ho probato di recente, false al contrario
     */
    bool peer_xeq_probed(int *delta, const bool reset) {
        bool retcode;

        int now_time = (int) elapsed_msecs();

        if ((now_time - last_probe) < *delta) {
            retcode = true;
        } else {
            retcode = false;
        }

        //		VERBO(stderr, "peer_xeq_probed %s:%u retcode=%u, now=%u, last_probe=%u, delta=%u, deltamax=%u\n", 
        //				IPtoP(ip), retcode, now_time, last_probe, (now_time - last_probe), *delta);
        ASSERT(*delta >= D1HT_MIN_THETA);

        if (!retcode || reset) {
            *delta = 0;
            last_probe = now_time;
        } else {
            *delta = (now_time - last_probe);
        }

        return retcode;
    }
};

/**< class probe_t per gestire l'elenco dei probed peers */
class probe_t {
    lock_t probe_mutex;
    int theta;
    std::list<probed_peers_t> peers;
    bool no_probe;

    /**
     * 
     * @param host
     * @param file
     * @param line
     * @param detached
     * @param forced
     * @return 
     */
    void *probe(const IPaddr host, const char* file, const uint line, const bool detached, const bool forced) {

        if (G_parms.myIP == host || no_probe || G_exiting || host.port == 0) return NULL;
        Assert(G_started, XEQ_ERROR);

        if (forced) {
            probed(host);
        } else if (xeq_probed(host)) {
            VERBOSEFL(stderr, file, line, "Probe avoided ip=%s:%u\n", IPtoP(host));
            return NULL;
        }
        VERBOSEFL(stderr, file, line, "Probe not avoided ip=%s:%u\n", IPtoP(host));

        if (detached) {
            IPaddr *temp_host = CALLOC(1, IPaddr);
            *temp_host = host;
            PTHREAD_CREATE(probe_pthread, temp_host, &G_detached);
        } else {
            probe_detached(host);
        }
        return NULL;
    }

    void probe(const IPaddr host, const char* file, const uint line, const bool detached) {
        probe(host, file, line, detached, false);
    }

    bool xeq_probed(const IPaddr host) {
        return xeq_probed(host, false);
    }

    bool xeq_probed(const IPaddr host, const bool reset) {
        return xeq_probed(host, reset, 0);
    }

    bool xeq_probed(const IPaddr host, const bool reset, const int new_theta) {
        return xeq_probed(host, reset, new_theta, NULL);
    }

    /**
     * mi dice se host è stato probed o no, aggiornando i dati del probe
     * @param host
     * @param reset
     * @param new_theta
     * @param delta
     * @return 
     */
    bool xeq_probed(const IPaddr host, const bool reset, const int new_theta, int *delta) {

        bool already_probed = false;

        if (host.port == 0) return true;

        LOCK(probe_mutex);

        if (no_probe) {
            UNLOCK(probe_mutex);
            return true;
        }

        //aggiorno theta
        if (new_theta != 0) theta = new_theta;

        //calcolo il nuovo delta
        int probe_delta = theta + D1HT_MIN_THETA;

        if (delta != NULL) probe_delta = *delta;

        //prendo la lista dei probed peers
        std::list<probed_peers_t>::iterator p = peers.begin();

        while (p != peers.end()) {

            int temp_delta = probe_delta;

            if (p->ip == host) {
                already_probed = p->peer_xeq_probed(&probe_delta, reset);
                break;
            } else if (!p->peer_xeq_probed(&temp_delta, false)) {
                // I know it ist not the peer I am looking for, bu I am taking this opportunity to clean the prebed peer list
                p = peers.erase(p);
            } else {
                p++;
            }
        }

        if (p == peers.end()) {
            ASSERT(!already_probed);
            probed_peers_t peer(host);
            peers.push_back(peer);
        }

        if (already_probed) {
            if (delta != NULL) *delta = probe_delta;
        } else {
            Assert(probe_delta <= (theta + D1HT_MIN_THETA), XEQ_ERROR);
        }

        UNLOCK(probe_mutex);
        return already_probed;
    }

public:

    probe_t() {

        LOCK_INIT(probe_mutex, maychain = true, 25);
        LOCK(probe_mutex);
        no_probe = false;
        theta = D1HT_INIT_THETA;
        peers.clear();
        UNLOCK(probe_mutex);
    }

    bool xeq_succ_probed(const int new_theta, int *delta) {
        ASSERT(delta != NULL);
        IPaddr succ = G_RT.pub_succ_IP();
        *delta = new_theta;
        return xeq_probed(succ, false, new_theta, delta);
    }

    void probe_close_interval(const int new_theta) {
        ASSERT(new_theta >= D1HT_MIN_THETA);
        IPaddr pred = G_RT.pub_pred_IP();
        if (!xeq_probed(pred, false, new_theta)) {
            probe_detached(pred);
            LOCK(G_stats.stat_mutex);
            UNLOCK(G_stats.stat_mutex);
        }
    }

    /**
     * apre una connessione TCP verso host, e gli invia un D1HT_PROBE: se l'host risponde con un D1HT_PROBE, ritorna true. Se risponde con un LEAVE, elimina il nodo dalla RT. 
     * @param host
     * @return 
     */
    bool probe_host(const IPaddr host) {

        if (host.port == 0) return true;

        bool retcode = false;
        int connection, iret;

        if (G_exiting) return false;
        Assert(G_started, XEQ_ERROR);

        probed(host);

        iret = TCP_CONNECTION(connection, 0, host, XEQ_SILENT);

        if (iret == 0 && connection >= 0 && !G_exiting) {
            iret = SEND_HEADER(connection, D1HT_PROBE, XEQ_SILENT);
            if (iret == D1HT_MSG_HEADER_size) {
                msg_hd header = READ_HEADER_TYPE(connection, D1HT_PROBE, XEQ_SILENT);
                if (header.systemid == G_parms.systemid) {
                    if (header.type == D1HT_PROBE) {
                        retcode = true;
                    } else if (header.type == D1HT_LEAVE && header.SeqNo == D1HT_LEAVE) {
                        G_RT.pub_del(host, false);
                    }
                }
            }
        }

        D1HT_CLOSE(connection);
        connection = -1;

        // when it is our pred, we probe twice before concluding it has failed, in order to avoid the possibility of injecting a false leave in the system

        if (retcode == false && G_RT.pub_pred_IP() == host) {

            iret = TCP_CONNECTION(connection, 0, host, XEQ_SILENT);

            if (iret == 0 && connection >= 0 && !G_exiting) {
                iret = SEND_HEADER(connection, D1HT_PROBE, XEQ_SILENT);
                if (iret == D1HT_MSG_HEADER_size) {
                    msg_hd header = READ_HEADER_TYPE(connection, D1HT_PROBE, XEQ_SILENT);
                    if (header.systemid == G_parms.systemid) {
                        if (header.type == D1HT_PROBE) {
                            retcode = true;
                        } else if (header.type == D1HT_LEAVE && header.SeqNo == D1HT_LEAVE) {
                            G_RT.pub_del(host, false);
                        }
                    }
                }
            }
        }

        D1HT_CLOSE(connection);
        connection = -1;

        return retcode;
    }

    void stop_probing() {
        no_probe = true;
    }

    void probe(const IPaddr host, const char* file, const uint line) {
        probe(host, file, line, true);
    }

    void probe_not_detached(const IPaddr host, const char* file, const uint line) {
        ASSERTFL((!G_RT.rt_mutex.xeq_locked_byme()), "rt_mutex must not be hold when calling probe_not_detached", file, line);
        probe(host, file, line, false);
    }

    void force_probe(const IPaddr host, const char* file, const uint line) {
        ASSERTFL((!G_RT.rt_mutex.xeq_locked_byme()), "rt_mutex must not be hold when calling force_probe", file, line);
        probe(host, file, line, false, true);
    }

    bool probed(const IPaddr host) {
        return xeq_probed(host, true);
    }

    /**
     * effettuo un probe all'host 2 volte: se mi risponde lo aggiungo alla RT, altrimenti lo elimino
     * @param host
     * @return 
     */
    void *probe_detached(const IPaddr host) {

        if (G_parms.myIP == host || no_probe || G_exiting || host.port == 0 || !G_started) return NULL;

        bool probeOK = probe_host(host);
        if (!probeOK) probeOK = probe_host(host); // try twice

        if (probeOK) {
            VERBOSEX(stderr, "Probe OK %s:%u\n", IPtoP(host));
            G_RT.pub_add(host);
        } else {
            VERBOSEX(stderr, "WARNING Cause: Probe failed %s:%u\n", IPtoP(host));
            G_RT.pub_del(host);
        }
        return NULL;
    }
};

extern probe_t G_probe;

#endif /*PROBE_STAB_H_*/
