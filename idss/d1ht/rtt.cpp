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
 * @file rtt.c
 * @brief 
 */

#include "D1HT.h"

namespace d1ht
{
timeval rtt_info::rto(bool lookup) {
	
	float	frto;  // RTO in usecs
	
	LOCK(rtt_mutex);
	
	// for the lookups we use a more agressive RTO
	
	if (lookup) {
		if (!inited) rtt_init((NUM_32) G_rtt_msg.rtt(), (NUM_32) G_rtt_msg.rttv());
		
		frto = srtt + (((float) D1HT_RTT_VARMULT) * rttvar);
		
	} else {
		frto = srtt + (4.0 * ((float) D1HT_RTT_VARMULT) * rttvar);
	}
	
	if (frto < G_parms.RTOMIN)
		frto = (float) G_parms.RTOMIN;
	else if (frto > (float) D1HT_RTT_RXTMAX)
		frto = (float) D1HT_RTT_RXTMAX;
	
	if (lookup) {
		G_stats.total.stats[stat_rto].value += (double) (frto/1000.0);
		G_stats.total.stats[stat_rto].count++;
	}
	
	UNLOCK(rtt_mutex);

	timeval retcode = USEC2TV(frto);
	
	VERBOSEX(stderr, "rto=%f (%lu secs, %lu usecs), srtt=%f, rttvar=%f\n", frto, (ulong) retcode.tv_sec, (ulong) retcode.tv_usec, srtt, rttvar);

	return retcode;
}

void rtt_info::update_rtt(const float usecs, const float usecs_stat) {
	/*
	 * Update our estimators of RTT and mean deviation of RTT.
	 * See Jacobson's SIGCOMM '88 paper, Appendix A, for the details.
	 * We use floating point here for simplicity.
	 */
	
	if (usecs < (float) (D1HT_RTT_RXTMIN / 2.0)) return;
	
	LOCK(rtt_mutex);

	float delta = usecs - srtt;
	
	srtt += delta / 8.0;								/* g = 1/8 */

	if (delta < 0.0) delta = -delta;					/* |delta| */

	rttvar += (delta - rttvar) / 4.0;					/* h = 1/4 */
	
	if (usecs_stat != 0.0) {
		G_stats.total.stats[stat_rtt].value += (double) (usecs_stat/1000.0);
		G_stats.total.stats[stat_rtt].count++;
	}

	VERBOSEX(stderr,"usecs=%f, srtt=%f, rttvar=%f, delta=%f\n", usecs, srtt, rttvar, delta);
	
	UNLOCK(rtt_mutex);
}

void rtt_info::update_rtt(const timeval tv_begin, const timeval tv_end) {
	
	float usecs = (float) TVDIFF2USEC(tv_begin, tv_end);
	
	// if (usecs < 0.0) EXIT("usecs < 0, begin=(%u,%u), end=(%u,%u), usecs=%f\n", (uint) tv_begin.tv_sec, (uint) tv_begin.tv_usec, (uint) tv_end.tv_sec, (uint) tv_end.tv_usec, usecs );
	
	update_rtt(usecs, usecs); 
}

void rtt_info::update_rtt(const timeval rtt) {
	
	float usecs =  (((float) rtt.tv_sec) * 1000000.0) + ((float) rtt.tv_usec) ;
	
	VERBOSEX(stderr, "rtt=(%u,%u), rto update msecs=%f\n", (uint) rtt.tv_sec, (uint) rtt.tv_usec, usecs );
	
	if (usecs < 1.0) usecs = 1.0;
	
	update_rtt(usecs, 0.0); 
}

} // namespace d1ht
