#include <windows.h>
#include <iostream>
#include <atlstr.h> 

#define FILEPATH_MAX_SIZE (255)
#define INIFILE_PATH "C://Users//hogehoge//Desktop//TEST_"
#define SETTING_FILE_NAME_A "//AAA//Setting.ini"
#define SETTING_FILE_NAME_B "//BBB//Setting.ini"

int nGetType(CString&, CString&);

using namespace std;

int nGetType(CString& c, CString& t)
{
	char FilePathTmp[FILEPATH_MAX_SIZE] = { '\0' };
	char FilePath[FILEPATH_MAX_SIZE] = { '\0' };
	::GetCurrentDirectory((DWORD)FILEPATH_MAX_SIZE, (LPTSTR)FilePathTmp);

	c = FilePathTmp;
	t = c + "//TypeSet.ini";

	int  nType = ::GetPrivateProfileInt("TYPE", "Type", 0, t);

	return nType;
}

void vSelectSettingFilePath(int nType, CString& s, CString& c)
{
	switch (nType) {
	case 0:
		s = c + SETTING_FILE_NAME_A;
		break;
	case 1:
		s = c + SETTING_FILE_NAME_B;
		break;
	default:
		cout << "Error. Invalid type." << endl;
		break;
	}
}

void vReplaceValue(const CString& s, CString& ifp)
{
	int nCount = ::GetPrivateProfileInt("COMMON", "Count", 0, s);
	CString cAppName = _T("");

	for (int nCnt = 0; nCnt < nCount; nCnt++) {
		cAppName.Format("SETTING%d", nCnt + 1);
		int nFileType = ::GetPrivateProfileInt(cAppName, "FileType", 0, s);
		cout << "FileType=" << nFileType << endl;
		char Tmp[20] = { '\0' };
		::GetPrivateProfileString(cAppName, "FileName", "", Tmp, sizeof(Tmp), s);
		if (nFileType == 0) {
			ifp.Format("%s%s%s", INIFILE_PATH, "//_TEST", Tmp);
		}
		else {
			ifp.Format("%s%s", INIFILE_PATH, Tmp);
		}
		char cSetAppName[20] = { '\0' };
		::GetPrivateProfileString(cAppName, "AppName", (LPCTSTR)"", cSetAppName, sizeof(cSetAppName), s);
		char cSetKeyName[20] = { '\0' };
		::GetPrivateProfileString(cAppName, "KeyName", "", cSetKeyName, sizeof(cSetKeyName), s);
		int nValueType = ::GetPrivateProfileInt(cAppName, "ValueType", 0, s);
		char cSetValue[20] = { '\0' };
		::GetPrivateProfileString(cAppName, "Value", "", cSetValue, sizeof(cSetValue), s);
		::WritePrivateProfileString(cSetAppName, cSetKeyName, cSetValue, ifp);
	}

}
int main()
{
	CString CurrentPath = _T("");
	CString TypeSetPath = _T("");
	CString SettingFilePath = _T("");
	CString IniFilePath = _T("");

	int nType = nGetType(CurrentPath, TypeSetPath);
	cout << TypeSetPath << endl;
	cout << nType << endl;
	
	vSelectSettingFilePath(nType, SettingFilePath, CurrentPath);

	vReplaceValue(SettingFilePath, IniFilePath);

	return 0;
}
