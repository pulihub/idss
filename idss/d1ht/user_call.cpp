/**
 * @file user_call.c
 * @brief 
 */
#include "D1HT.h"

namespace d1ht
{
void user_call_target(const D1HT_LOOKUP_body *lookup_req_body, const IPaddr *peer, const void* payload_buff, void* reply_payload, size_t *reply_len) {
	
	// the D1HT_LOOKUP_body structure is defined in messages.h
	// the IPaddr structure is defined in base.h
	// peer contain the address of the peer who sent the msg 
	// payload points to the payload sent with the msg
	// reply_payload points to a buffer where any reply should be put
	// when returning reply_len must contain the size of the actual reply_payload (0 if thre won't be any reply_payload)
	
	if (reply_len == NULL) EXIT("Problem in user_call_target: reply_len == NULL");
	
	*reply_len = 0;

	if (lookup_req_body->payload_len 			== 0) return;
	if (payload_buff == NULL)  EXIT("Problem in user_call_target: payload_buff == NULL");
	
	if (lookup_req_body->max_reply_payload_len 	== 0) return;
	if (reply_payload == NULL) EXIT("Problem in user_call_target: reply_payload == NULL");

	ASSERT(lookup_req_body->payload_len 			== sizeof(int));
	ASSERT(lookup_req_body->max_reply_payload_len	== sizeof(int));
	
	int *i = (int *) payload_buff;
	int *r = (int *) reply_payload;
	
	*r = *i + *i;
	
	*reply_len = sizeof(int);
}

} // namespace d1ht
