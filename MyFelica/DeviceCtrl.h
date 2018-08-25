#ifndef DEVICECTRL_H_
#define DEVICECTRL_H_

#include <winscard.h>
#include "SCardCtrl.h"
#pragma comment (lib, "winscard.lib")

class MyFelicaDevice {
public:
    MyFelicaDevice();
    ~MyFelicaDevice();
    BOOL bOpenService();
    void vCloseService();
    BOOL bConnectToSCResourceMng();
    BOOL bSearchAndGetCRDeviceName();
    BOOL bConnectToSCard();
    void vDisConnectSCard();
    SCARDCONTEXT hGetContext();
    LPTSTR lpszGetReaderName();
private:
    SCARDCONTEXT hContext;
    LPTSTR	     lpszReaderName;
public:
    MySmartCard* mysc;
};

#endif DEVICECTRL_H_

