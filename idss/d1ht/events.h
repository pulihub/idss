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
 * @file events.h
 * @brief 
 */

#ifndef EVENTMSG_H_
#define EVENTMSG_H_

class eventlist_t {
public:
    IP *ips;
    KEYID *ids;
    PORTNO *ports;
};

class eventmsg_t {
    bool initiated;
public:
    D1HT_MAINTENANCE_body body;

    struct msg_hd msg_header;
    struct sockaddr_in sender_address;
    int time_received;
    IPaddr whosent;
    int tot_events;
    NUM_8 SeqNo;
    NUM_8 TTL;

    eventmsg_t *next;
    eventlist_t eventlist[2][2];

#define D1HT_EVENT_MAX_IOVCNT 8
    int iovcnt;
    struct iovec iov[D1HT_EVENT_MAX_IOVCNT];

    bool eventmsg_receive(const int socket);
    void* eventmsg_init();
    void set_ids();
    void* fill(const IPaddr ip, const int ttl, const int itype);
    void destroy();

#if defined(DEBUG)
    void eventmsg_check(char* filename, const uint linenumber);

    void eventmsg_print() {
        PORTNO port;
        for (int iport = DEFAULT_PORT; iport <= OTHER_PORT; iport++) {

            port = G_parms.default_udp_port;

            for (int itype = JOINS; itype <= LEAVES; itype++) {
                if (body.nevents[iport][itype] == 0) continue;
                VERBOSE(stderr, "Events received from %s:%u SeqNo=%u ttl=%u default_port=%u type=%u qty=%u: ", IPtoP(whosent), SeqNo, TTL, iport, itype, body.nevents[iport][itype]);
                for (int ievent = 0; ievent < (int) body.nevents[iport][itype]; ievent++) {
                    if (iport != DEFAULT_PORT) port = eventlist[iport][itype].ports[ievent];
                    ASSERT(eventlist[iport][itype].ips[ievent].s_addr != 0);
                    fprintf(stderr, "(%u, %s:%u) ", ip2id(eventlist[iport][itype].ips[ievent], port),
                            NTOP(eventlist[iport][itype].ips[ievent]), ntohs(port));
                }
                fprintf(stderr, "\n");
            }
        }
    }
#endif
};

#endif /*EVENTMSG_H_*/
