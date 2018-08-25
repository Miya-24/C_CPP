#include "SCardCtrl.h"
#include "Main.h"
#include "DeviceCtrl.h"

MySmartCard::MySmartCard()
{
	this->hCard = 0;
	this->dwActiveProtocol = 0;
}
//------------------------------------------
MySmartCard::~MySmartCard()
{

}
//------------------------------------------
BOOL MySmartCard::bGetRelatedData(BYTE cmd)
{
	BYTE bySndCmd[] = { ESC_CMD_APDU_WRAP, INFO_TYPE_GET_DATA, cmd, 0x00, GET_DATA_LENGTH_MAX };
	BYTE byRcvBuf[PCSC_RECV_BUFF_LEN] = {0x00};
	DWORD dwResSize = sizeof(byRcvBuf);

	LONG lResult = ::SCardTransmit(this->hCard, CardProtocol2PCI(this->dwActiveProtocol),
									bySndCmd, sizeof(bySndCmd), NULL, byRcvBuf, &dwResSize);
	if (lResult != SCARD_S_SUCCESS) {
		fprintf_s(stdout, "SCardTransmit Error.\nErrorCode %08X\n\n", lResult);
		return FALSE;
	}

	BOOL bRtn = TRUE;
	if (this->byCardType == CARD_TYPE_FELICA) {
		bRtn = this->bDisplayRelatedData_Felica(cmd, byRcvBuf, dwResSize);
	} else if (this->byCardType == CARD_TYPE_TYPE_4A) {
		bRtn = this->bDisplayRelatedData_Type4A(cmd, byRcvBuf, dwResSize);
	} else {
		return FALSE;
	}
	if (bRtn != TRUE) return FALSE;
	return TRUE;
}
//------------------------------------------
BOOL MySmartCard::bDisplayRelatedData_Felica(BYTE cmd, BYTE* byRcvBuf, DWORD dwResSize)
{
	if (cmd == DATA_TYPE_UID) {
		fprintf_s(stdout, "UID : ");
	} else if (cmd == DATA_TYPE_PMM) {
		fprintf_s(stdout, "PMm : ");
	} else if (cmd == DATA_TYPE_CARD_ID) {
		fprintf_s(stdout, "Card ID : ");
	} else if (cmd == DATA_TYPE_CARD_TYPE_NAME) {
		fprintf_s(stdout, "Card type name : %s\n", byRcvBuf);
		return true;
	}

	switch (cmd) {
	case DATA_TYPE_UID:
	case DATA_TYPE_PMM:
	case DATA_TYPE_CARD_ID:
		for (UINT i = 0; i < dwResSize - 2; i++) {
			fprintf_s(stdout, "%02X", byRcvBuf[i]);
			if (i < dwResSize - 3) {
				fprintf_s(stdout, " ");
			} else {
				fprintf_s(stdout, "\n");
			}
		}
		return true;
	default:
		fprintf_s(stdout, "\n");
		return false;
	}
}
//------------------------------------------
BOOL MySmartCard::bDisplayRelatedData_Type4A(BYTE cmd, BYTE* byRcvBuf, DWORD dwResSize)
{

	if (cmd == DATA_TYPE_UID) {
		fprintf_s(stdout, "UID : ");
	} else if (cmd == DATA_TYPE_PMM) {
		fprintf_s(stdout, "ATS : ");
	} else if (cmd == DATA_TYPE_CARD_ID) {
		fprintf_s(stdout, "Card ID : ");
	} else if (cmd == DATA_TYPE_CARD_TYPE_NAME) {
		fprintf_s(stdout, "Card type name : %s\n", byRcvBuf);
		return true;
	}

	if (dwResSize == 2) {
		fprintf(stdout, "No Data...\n");
		return TRUE;
	}	
	switch (cmd) {
	case DATA_TYPE_UID:
	case DATA_TYPE_PMM:
	case DATA_TYPE_CARD_ID:
		for (UINT i = 0; i < dwResSize - 2; i++) {
			fprintf_s(stdout, "%02X", byRcvBuf[i]);
			if (i < dwResSize - 3) {
				fprintf_s(stdout, " ");
			} else {
				fprintf_s(stdout, "\n");
			}
		}
		return true;
	default:
		fprintf_s(stdout, "\n");
		return false;
	}
}
//------------------------------------------
BYTE MySmartCard::byCheckCardType()
{
	BYTE bySndCmd[] = { ESC_CMD_APDU_WRAP, INFO_TYPE_GET_DATA, 
						DATA_TYPE_CARD_TYPE, 0x00, GET_DATA_LENGTH_MAX };
	BYTE byRcvBuf[PCSC_RECV_BUFF_LEN] = {0x00};
	DWORD dwResSize = sizeof(byRcvBuf);
	LONG lResult = ::SCardTransmit(this->hCard, CardProtocol2PCI(this->dwActiveProtocol),
									bySndCmd, sizeof(bySndCmd), NULL, byRcvBuf, &dwResSize);
	if (lResult != SCARD_S_SUCCESS) {
		fprintf_s(stdout, "SCardTransmit Error.\nErrorCode %08X\n\n", lResult);
		return CARD_TYPE_UNKNOWN;
	}
	switch (byRcvBuf[0]) {
	case CARD_TYPE_TYPE_A:
	case CARD_TYPE_FELICA:
	case CARD_TYPE_TYPE_4A:
		this->byCardType = byRcvBuf[0];
		break;
	case CARD_TYPE_UNKNOWN:
	default:
		fprintf(stdout, "Card type is Unknown\n\n");	
		break;
	}
	return byRcvBuf[0];
}
//------------------------------------------
void MySmartCard::vDisConnectSCard()
{
	if (this->hCard != 0) {
		::SCardDisconnect(this->hCard, SCARD_LEAVE_CARD);
	}
}
//------------------------------------------
void MySmartCard::vSetActiveProtocol(DWORD tmp)
{
	this->dwActiveProtocol = tmp;
}
//------------------------------------------
void MySmartCard::vSetCardContxt(SCARDHANDLE tmp)
{
	this->hCard = tmp;
}
