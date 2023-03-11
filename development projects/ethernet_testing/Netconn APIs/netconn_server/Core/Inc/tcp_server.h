/*
 * tcp_server.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

#ifndef INC_TCP_SERVER_H_
#define INC_TCP_SERVER_H_

void tcpserver_init (void);
void tcp_RecievePrograme(programLength);
void tcp_SendResponse(char *data , int len);


#endif /* INC_TCP_SERVER_H_ */
