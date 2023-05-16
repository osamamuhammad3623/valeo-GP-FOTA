/*
 * tcp_client.h
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

#ifndef INC_TCP_CLIENT_H_
#define INC_TCP_CLIENT_H_

void tcpclient_init (void);
void UDS_receive_response(char* message);
void tcp_SendMessage (char *Message , int messageLength);
static void tcp_ReseveMessage (void );



#endif /* INC_TCP_CLIENT_H_ */
