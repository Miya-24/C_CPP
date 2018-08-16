#include "Main.h"
#include "SCardCtrl.h"

int main()
{
	MySmartCard* mysc = new MySmartCard();
	if (mysc == NULL) return EXIT_FAILURE;
	BOOL bRtn = true;

	//Open smart card reader / writer service
	bRtn = mysc->bOpenService();
	if (!bRtn) {
		mysc->vCloseService();
		delete mysc;
		mysc = NULL;
		return EXIT_FAILURE;
	}
	//Connect to smart card
	bRtn = mysc->bConnectToSCard();
	if (!bRtn) {
		mysc->vCloseService();
		delete mysc;
		mysc = NULL;
		return EXIT_FAILURE;
	}
	//Check card type
	BYTE byRtn = mysc->byCheckCardType();
	if (byRtn == CARD_TYPE_UNKNOWN) {
		mysc->vCloseService();
		delete mysc;
		mysc = NULL;
		return EXIT_FAILURE;
	}
	//Get command
	int Num;
	int CmdSize = 0;
	BYTE cmd[4] = {'\0'};
	vGetCmd(&Num, cmd, &CmdSize);
	if (Num < 1 || Num > 5) {
		mysc->vCloseService();
		delete mysc;
		mysc = NULL;
		return EXIT_FAILURE;
	}
	//Get related data(UID, PMm, Card Name, ...)
	for (int i = 0; i < CmdSize; i++) {
		bRtn = mysc->bGetRelatedData(cmd[i]);
		if (!bRtn) {
			mysc->vCloseService();
			delete mysc;
			mysc = NULL;
			return EXIT_FAILURE;
		}
	}
	fprintf_s(stdout, "\n");
	//Close service
	mysc->vCloseService();
	delete mysc;
	mysc = NULL;
	return EXIT_SUCCESS;
}
//------------------------------------------
SCARD_IO_REQUEST* CardProtocol2PCI(DWORD dwProtocol)
{
	if (dwProtocol == SCARD_PROTOCOL_T0) {
		return (SCARD_IO_REQUEST *)SCARD_PCI_T0;
	}
	else if (dwProtocol == SCARD_PROTOCOL_T1) {
		return (SCARD_IO_REQUEST *)SCARD_PCI_T1;
	}
	else if (dwProtocol == SCARD_PROTOCOL_RAW) {
		return (SCARD_IO_REQUEST *)SCARD_PCI_RAW;
	}
	else if (dwProtocol == SCARD_PROTOCOL_UNDEFINED) {
		assert(false);
		return NULL;
	}
	return (SCARD_IO_REQUEST *)SCARD_PCI_T1;
}
//------------------------------------------
void vGetCmd(int* NumTmp, BYTE* cmdTmp, int* CmdTmpSize)
{
	fprintf_s(stdout, "Please select number...\n");
	fprintf_s(stdout, " 1 : UID\n 2 : ATS-HB/INF/PMm\n 3 : Card ID\n 4 : Card type name\n 5 : All data\n >>");
	std::cin >> *NumTmp;
	fprintf_s(stdout, "\n");
	
	switch (*NumTmp) {
	case 1:
		cmdTmp[0] = DATA_TYPE_UID;
		*CmdTmpSize = 1;
		break;
	case 2://ATS-HB/INF/PMm
		cmdTmp[0] = DATA_TYPE_PMM;
		*CmdTmpSize = 1;
		break;
	case 3:
		cmdTmp[0] = DATA_TYPE_CARD_ID;
		*CmdTmpSize = 1;
		break;
	case 4:
		cmdTmp[0] = DATA_TYPE_CARD_TYPE_NAME;
		*CmdTmpSize = 1;
		break;
	case 5:
		cmdTmp[0] = DATA_TYPE_UID;
		cmdTmp[1] = DATA_TYPE_PMM;
		cmdTmp[2] = DATA_TYPE_CARD_ID;
		cmdTmp[3] = DATA_TYPE_CARD_TYPE_NAME;
		*CmdTmpSize = 4;
		break;
	default:
		fprintf_s(stdout, "Invalid Number\n\n");
	}
}