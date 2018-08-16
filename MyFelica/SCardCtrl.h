#ifndef SCARDCTRL_H_
#define SCARDCTRL_H_
#include <winscard.h>
#pragma comment (lib, "winscard.lib")

class MySmartCard {
public:
    MySmartCard();
	~MySmartCard();
	BOOL bOpenService();
	void vCloseService();
	BOOL bConnectToSCResourceMng();
	BOOL bSearchAndGetCRDeviceName();
    BOOL bConnectToSCard();
	BOOL bGetRelatedData(BYTE);
    BOOL bDisplayRelatedData_Type4A(BYTE, BYTE*, DWORD);
    BOOL bDisplayRelatedData_Felica(BYTE, BYTE*, DWORD);
    BYTE byCheckCardType();
private:
	SCARDCONTEXT hContext;
    DWORD		 dwActiveProtocol;
    SCARDHANDLE  hCard;
    BYTE         byCardType;
	LPTSTR	     lpszReaderName;
};

#endif SCARDCTRL_H_