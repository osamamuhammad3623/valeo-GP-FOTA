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
err_t tcp_SendMessage (char *Message , int messageLength);
static void tcp_ReseveMessage (void );



#endif /* INC_TCP_CLIENT_H_ */


//
///*
// * tcp_client.h
// *
// *  Created on: May 17, 2023
// *      Author: Kyrillos Phelopos Sawiris
// */
//
//#ifndef INC_TCP_CLIENT_H_
//#define INC_TCP_CLIENT_H_
//
////=========================================================
////========== includes =====================================
//#include "lwip/opt.h"
//#include "lwip/api.h"
//#include "lwip/sys.h"
//#include "string.h"
////=====================================================
//
////================ definitions============================
//#define MAX_BACKOFF_MS 100000 // backoff max delay
////========================================================
//
//
//typedef enum {
//	PS_TARGET = 1, WIPERS_TARGET
//}TargetECU;
//
//struct target_confg {
//	char* ip_add;
//	unsigned short portNum;
//	TargetECU targetECU;
//};
//
//void tcpclient_init ();
//
//// Function to send the data to the server
//static void tcp_ReceiveMessage (TargetECU targetECU, struct netconn *conn ,struct netbuf *buf );
//err_t tcp_SendMessage (TargetECU targetECU, uint8_t *Message , int messageLength);
//void init_uds_request_callback(void (*p) (TargetECU targetECU));
//void init_uds_recv_resp_clbk(void (*p)(TargetECU targetECU, uint8_t *responseFrame));

//#endif /* INC_TCP_CLIENT_H_ */

