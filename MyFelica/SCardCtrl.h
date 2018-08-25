#ifndef SCARDCTRL_H_
#define SCARDCTRL_H_
#include <winscard.h>
#pragma comment (lib, "winscard.lib")

class MySmartCard {
public:
    MySmartCard();
    ~MySmartCard();
    BOOL bConnectToSCard();
    BOOL bGetRelatedData(BYTE);
    BOOL bDisplayRelatedData_Type4A(BYTE, BYTE*, DWORD);
    BOOL bDisplayRelatedData_Felica(BYTE, BYTE*, DWORD);
    BYTE byCheckCardType();
    void vDisConnectSCard();
    void vSetActiveProtocol(DWORD);
    void vSetCardContxt(SCARDHANDLE);
private:
    DWORD		 dwActiveProtocol;
    SCARDHANDLE  hCard;
    BYTE         byCardType;
};

#endif SCARDCTRL_H_
