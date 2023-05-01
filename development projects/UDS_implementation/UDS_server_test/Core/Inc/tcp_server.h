/*
 * tcp_server.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

#ifndef INC_TCP_SERVER_H_
#define INC_TCP_SERVER_H_
//uint32_t image_size;
#define CHUNK_SIZE						20000U

//uint32_t downloadSize;

void tcpserver_init (void);
void tcp_RecievePrograme(programLength);
void tcp_SendResponse(uint8_t *data , int len);
void init_execute_request_callback(void (*p)(void *argument));
void tcp_receiveChunk(uint16_t chunkSize);

#endif /* INC_TCP_SERVER_H_ */
