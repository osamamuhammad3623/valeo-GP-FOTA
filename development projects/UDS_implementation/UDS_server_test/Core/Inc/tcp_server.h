/*
 * tcp_server.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

#ifndef INC_TCP_SERVER_H_
#define INC_TCP_SERVER_H_

#define CHUNK_SIZE						20000U

void tcpserver_init (void);
void tcp_SendResponse(uint8_t *data , int len);
void init_uds_callback(void (*p)(void));
void init_execute_request_callback(void (*p)(void *argument));
void tcp_receiveChunk(uint16_t chunkSize);

#endif /* INC_TCP_SERVER_H_ */
