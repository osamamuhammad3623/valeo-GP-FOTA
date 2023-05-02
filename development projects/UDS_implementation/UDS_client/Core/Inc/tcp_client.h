/*
 * tcp_client.h
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

#ifndef INC_TCP_CLIENT_H_
#define INC_TCP_CLIENT_H_

#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

typedef enum {
	PS_TARGET = 1, WIPERS_TARGET
}TargetECU;

struct target_confg {
	char* ip_add;
	unsigned short portNum;
	TargetECU targetECU;
};

void tcpclient_init (uint8_t* targetToConnectWith);
//void UDS_receive_response(struct netconn *conn ,char* message);
// Function to send the data to the server
static void tcp_ReceiveMessage (TargetECU targetECU, struct netconn *conn ,struct netbuf *buf );
void tcp_SendMessage (TargetECU targetECU, uint8_t *Message , int messageLength);
void init_uds_request_callback(void (*p) (TargetECU targetECU));
void init_uds_recv_resp_clbk(void (*p)(TargetECU targetECU, uint8_t *responseFrame));


#endif /* INC_TCP_CLIENT_H_ */
