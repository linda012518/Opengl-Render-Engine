#ifndef _File_System_
#define _File_System_

#include <Windows.h>

class FileSystem
{
private:
	static void getExePath(char pPath[1024])
	{
		char    szPathName[1024];
		char    szDriver[64];
		char    szPath[1024];
		GetModuleFileNameA(NULL, szPathName, sizeof(szPathName));
		_splitpath(szPathName, szDriver, szPath, 0, 0);
		sprintf(pPath, "%s%s", szDriver, szPath);
	}

public:
	static void getResourcePath(const char* fileName, char outPath[1024])
	{
		char    binPath[1024];
		getExePath(binPath);
		sprintf(outPath, "%s%s", binPath, fileName);
	}

};

#endif // !_File_System_

