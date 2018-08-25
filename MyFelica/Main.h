#ifndef MAIN_H_
#define MAIN_H_

#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>
#include <assert.h>
#include <iostream>

#define PCSC_TRANS_BUFF_LEN				(262)
#define PCSC_RECV_BUFF_LEN				(262)
#define SONY_CR_DEVICE_NAME				"Sony FeliCa Port/PaSoRi 3.0"

#define ESC_CMD_GET_INFO				0xC0
#define ESC_CMD_SET_OPTION				0xC1
#define ESC_CMD_TARGET_COMM				0xC4
#define ESC_CMD_SNEP					0xC6
#define ESC_CMD_APDU_WRAP				0xFF

//ESC_CMD_GET_INFO
#define INFO_TYPE_GET_DRV_VER			0x01
#define INFO_TYPE_GET_FW_VER			0x02
#define INFO_TYPE_GET_VID				0x04
#define INFO_TYPE_GET_VNAME             0x05
#define INFO_TYPE_GET_PID               0x06
#define INFO_TYPE_GET_PNAME				0x07
#define INFO_TYPE_GET_SERIAL_NUM		0x08
#define INFO_TYPE_GET_CAP_CARD_ID		0x10
//ESC_CMD_SET_OPTION
//ESC_CMD_TARGET_COMM
//ESC_CMD_SNEP
//ESC_CMD_APDU_WRAP
#define INFO_TYPE_GET_DATA			    0xCA
//INFO_TYPE_GET_DATA 
#define DATA_TYPE_UID					0x00
#define DATA_TYPE_PMM					0x01
#define DATA_TYPE_CARD_ID				0xF0
#define DATA_TYPE_CARD_TYPE				0xF3
#define DATA_TYPE_CARD_TYPE_NAME		0xF4
#define	GET_DATA_LENGTH_MAX				0x00
//DATA_TYPE_CARD_TYPE
#define CARD_TYPE_UNKNOWN               0x00
#define CARD_TYPE_TYPE_A                0x01
#define CARD_TYPE_FELICA                0x04
#define CARD_TYPE_TYPE_4A               0x07

SCARD_IO_REQUEST* CardProtocol2PCI(DWORD dwProtocol);
void vGetCmd(int*, BYTE*, int*);

#endif
