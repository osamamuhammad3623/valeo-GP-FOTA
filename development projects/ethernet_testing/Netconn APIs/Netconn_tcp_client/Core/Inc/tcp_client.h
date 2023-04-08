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

struct target_confg {
	char* ip_add;
	unsigned short portNum;
};

void tcpclient_init (void);
void UDS_receive_response(struct netconn *conn ,char* message);
// Function to send the data to the server
static void tcp_ReseveMessage (struct netconn *conn ,struct netbuf *buf );
void tcp_SendMessage (struct netconn *conn, char *Message , int messageLength);



#endif /* INC_TCP_CLIENT_H_ */
