#include "DeviceCtrl.h"
#include "Main.h"

MyFelicaDevice::MyFelicaDevice()
{
    this->hContext = 0;
    this->lpszReaderName = nullptr;
    this->mysc = nullptr;
}
//------------------------------------------
MyFelicaDevice::~MyFelicaDevice()
{

}
//------------------------------------------
BOOL MyFelicaDevice::bOpenService()
{
	BOOL bRtn = TRUE;
	bRtn = bConnectToSCResourceMng();
	if (bRtn != TRUE) return FALSE;

	bRtn = bSearchAndGetCRDeviceName();
	if (bRtn != TRUE) return FALSE;

	return TRUE;
}
//------------------------------------------
void MyFelicaDevice::vCloseService()
{
    if (mysc != NULL) {
        this->vDisConnectSCard();
        delete mysc;
        mysc = NULL;
    }
	if (this->lpszReaderName != NULL) {
		::SCardFreeMemory(this->hContext, this->lpszReaderName);
	}
	if (this->hContext != 0) {
		::SCardReleaseContext(this->hContext);
	}
	system("pause");
}
//------------------------------------------
BOOL MyFelicaDevice::bConnectToSCResourceMng()
{
	SCARDCONTEXT	hContext = 0;
	LONG lResult = ::SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hContext);
	if (lResult != SCARD_S_SUCCESS) {
		if (lResult == SCARD_E_NO_SERVICE) {
			fprintf_s(stdout, "Smart Card Servise is not Started.\n\n");
		}
		else {
			fprintf_s(stdout, "SCardEstablishContext Error.\nErrorCode %08X\n\n", lResult);
		}
		return FALSE;
	}
	fprintf_s(stdout, "Smart Card Servise is Started.\n");
	//set
	this->hContext = hContext;
	return TRUE;
}
//------------------------------------------
BOOL MyFelicaDevice::bSearchAndGetCRDeviceName()
{
	LPTSTR	lpszReaderNameTmp = NULL;
	DWORD	dwAutoAllocate = SCARD_AUTOALLOCATE;

	LONG lResult = ::SCardListReaders(this->hContext, NULL, (LPTSTR)&this->lpszReaderName, &dwAutoAllocate);
	if (lResult != SCARD_S_SUCCESS) {
		if (lResult == SCARD_E_NO_READERS_AVAILABLE) {
			fprintf_s(stdout, "Reader/Writer is not Found.\n\n");
		}
		else {
			fprintf_s(stdout, "SCardListReaders Error.\nErrorCode %08X\n\n", lResult);
		}
		return FALSE;
	}
	fprintf_s(stdout, "Reader/Writer is %s.\n\n", this->lpszReaderName);
	if (int iRtn = memcmp(this->lpszReaderName, SONY_CR_DEVICE_NAME, strlen(SONY_CR_DEVICE_NAME)) != 0) {
		return FALSE;
	}
	return TRUE;
}
//------------------------------------------
BOOL MyFelicaDevice::bConnectToSCard()
{
    mysc = new MySmartCard();
    if (mysc == NULL) return FALSE;

	SCARDHANDLE hCard = 0;
	DWORD dwActiveProtocol = 0;
	LONG lResult = ::SCardConnect(this->hContext, this->lpszReaderName,
									SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
									&hCard, &dwActiveProtocol);
	if (lResult != SCARD_S_SUCCESS) {
		fprintf_s(stdout, "SCardConnect Error.\nErrorCode %08X\n\n", lResult);
		return false;
	}
	//set
	this->mysc->vSetActiveProtocol(dwActiveProtocol);
	this->mysc->vSetCardContxt(hCard);
	return true;
}
//------------------------------------------
void MyFelicaDevice::vDisConnectSCard()
{
	this->mysc->vDisConnectSCard();
}
