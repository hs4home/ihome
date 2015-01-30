#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#define MAX_TCP_MSG_SIZE	128

/*********
 *
 * ������ ���������:
 * 					�����
 * CommandID		1
 * CommandPAR		1
 * DataLEN			2		(MSB ������ LSB ������)
 * Data				n
 * CRC8						algorithm?
 *
 * ��������� ����� ���� ������������ ��� ������, ��� ������ ��.
 * ��� ��������� ������������ ����� � ������ ��������� TCP ����������.
 *
 * ���������, ��������� � ������� ����:
 * ���� -> ��
 *
 * 1. GET RELAY STATE
 * 1.1 ���� ���������� ���������:
 *
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= GET_RSTATE_REQ 			���� ������ ���������
 * 		LEN 	= n 			n - ���������� ����, ��������� ������� ���� �������
 * 		DAT 	= BoardIndex_i, BoardIndex_i+1 ...  - ������ ����
 * 		CRC8
 *
 * ��, ������ ��� ���������, ������ �������� ��������� ����������, ������ ������ � ���������
 * ����� ����� �� ����������� �������
 * 		int current_relay_state[NUM_RELAY_BOARDS];  (���� �� ��������� ��� ������ relay_thread)
 *
 * 1.2 �� ��������

 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= GET_RSTATE_ANS 			�� �������� �� ������ ������ ���������
 * 		LEN 	= 2*n 			n - ���������� ����, ��������� ������� ���� �������
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2. SET RELAY STATE
 * 2.1 ���� ���������� ���������:
 *
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RSTATE_REQ 			���� ������������� ���������
 * 		LEN 	= 2*n 			n - ���������� ����, ��������� ������� ���� ����������
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2.2 �� ��������
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RSTATE_ACK 			�� ��������, ��� ������� ��������� ��������� �������
 * 		LEN 	= 0x00 			0 ����� ������
 * 		CRC8
 *
 *
 * 	�� -> ����
 *
 * 	�� - ������
 *
 * 	3. REPORT RELAY STATE
 * 	3.1 �� ���������� ���������
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= REPORT_RSTATE_REQ 			�� ��������, ��� ���������� ��������� ����� ��� ���������� ���� ����
 * 		LEN 	= 2*n 			n - ���������� ���� ���� � ����� ����������
 * 		DAT 	= {BoardIndex, RelayState} * n
 * 		CRC8
 *
 * 	3.2 ���� ��������
 *
	 * 	CID 	= CID_RELAY 			���������, ��������� � ������ ����
	 * 	CPAR 	= REPORT_RSTATE_ACK 			���� ��������, ��� ������� �������
	 * 	LEN 	= 0x00 			0 ����� ������
	 * 	CRC8
 *
 */

// Command IDs
#define CID_RELAY			0x01	// commands for working with relay boards
#define CID_CONFIG			0x02	//
#define CID_ERROR			0xFF


// Command Parameter

// Command Parameters for CID_RELAY
#define GET_RSTATE_REQ		0x01
#define GET_RSTATE_ANS		0x02
#define SET_RSTATE_REQ		0x03
#define SET_RSTATE_ACK		0x04

#define REPORT_RSTATE_REQ		0x05
#define REPORT_RSTATE_ACK		0x06


// Command Parameters for CID_CONFIG
#define GET_CONFIG_T_REQ		0x01	// host is reading mcu_config_t
#define GET_CONFIG_T_ANS		0x02
#define SET_CONFIG_T_REQ		0x03
#define SET_CONFIG_T_ACK		0x04


// Common Command Parameters
#define MSG_ERROR		0xFF


// Error codes
#define ERROR_CID_NACK		1		// cmd id not recognized
#define ERROR_CPAR_NACK		2		// cmd parameter not recognized
#define ERROR_DATA			3







#endif // __TCP_PROTOCOL_H
