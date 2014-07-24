#include "stdafx.h"
#include "Inject.h"


BOOL WINAPI LoadLib(DWORD dwProcessId, LPTSTR lpszLibName)
{
	BOOL bResult		= FALSE;
	HANDLE hProcess		= NULL;
	HANDLE hThread		= NULL;
	PSTR pszLibFileRemote	= NULL;

	_try
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,
			FALSE,
			dwProcessId
			);
		if(hProcess == NULL)
			_leave;

		int cch = 1 + strlen(lpszLibName);

		pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess,
			NULL,
			cch,
			MEM_COMMIT,
			PAGE_READWRITE
			);

		if (pszLibFileRemote == NULL)
			_leave;

		if (!WriteProcessMemory(
			hProcess,
			(PVOID)pszLibFileRemote,
			(PVOID)lpszLibName,
			cch,
			NULL))
			_leave;
        
		PTHREAD_START_ROUTINE pfnThreadRtn = 
			(PTHREAD_START_ROUTINE)GetProcAddress(
			GetModuleHandle("Kernel32"), "LoadLibraryA");

		if (pfnThreadRtn == NULL)
			_leave;

		DWORD dwThreadID =0;
		hThread = CreateRemoteThread(
			hProcess,
			NULL,
			0,
			pfnThreadRtn,
			(PVOID)pszLibFileRemote,
			0,
			&dwThreadID
			);

		if (hThread == NULL)
			_leave;

		WaitForSingleObject(hThread, INFINITE);

		bResult = TRUE;
	}

	_finally
	{
		if (pszLibFileRemote != NULL)
			VirtualFreeEx(hProcess, 
			(PVOID)pszLibFileRemote,
			0,
			MEM_RELEASE);

		if(hThread != NULL)
			CloseHandle(hThread);

		if (hProcess != NULL)
			CloseHandle(hProcess);
	}

	return bResult;
}


BOOL WINAPI FreeLib(DWORD dwProcessId, LPTSTR lpszLibName)
{
	BOOL bResult		= FALSE;
	HANDLE hProcess		= NULL;
	HANDLE hThread		= NULL;
	HANDLE hthSnapshot	= NULL;
	MODULEENTRY32 hMod	= {sizeof(hMod)};

	_try
	{
		hProcess = OpenProcess(
			PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION,
			FALSE,
			dwProcessId
			);

		if (hProcess == NULL)
			_leave;

		PTHREAD_START_ROUTINE pfnThreadRtn = 
			(PTHREAD_START_ROUTINE) GetProcAddress(
			GetModuleHandle("Kernel32.dll"), "FreeLibrary");

		if (pfnThreadRtn == NULL)
			_leave;

		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
			dwProcessId);

		if (hthSnapshot == NULL)
			_leave;

		BOOL bMoreMods = Module32First(hthSnapshot, & hMod);
		if (bMoreMods == FALSE)
			_leave;

		for (;bMoreMods; bMoreMods = Module32Next(hthSnapshot, &hMod))
		{
			if ((strcmp(hMod.szExePath, lpszLibName) == 0 ) ||
				(strcmp(hMod.szModule, lpszLibName) == 0))
				break;
		}

		hThread = CreateRemoteThread(hProcess, NULL,
			0,
			pfnThreadRtn,
			hMod.modBaseAddr,
			0,
			NULL);

		if (hThread == NULL)
			_leave;

		WaitForSingleObject(hThread, INFINITE);

		bResult = TRUE;
	}

	_finally
	{

		if(hThread != NULL)
			CloseHandle(hThread);

		if (hthSnapshot != NULL)
			CloseHandle(hthSnapshot);

		if (hProcess != NULL)
			CloseHandle(hProcess);
	}

	return bResult;

}


#define _countof(array) (sizeof(array)/sizeof(array[0]))

BOOL InjectDll(HANDLE hProcess, LPSTR szDllPath){
	BOOL bSuccess = FALSE;
	TCHAR szFilename[_MAX_PATH];
	//::GetModuleFileName(NULL, szFilename, _countof(szFilename));
	//TCHAR* pPos = _tcsrchr(szFilename, _T('\\'));
	//_tcscpy(pPos, _T("\\GdiDetector.dll"));
	//memcpy(szFilename, szDllPath, sizeof(szDllPath));
	_tcscpy(szFilename, szDllPath);
	PTSTR pRemoteDllPath = (PTSTR)VirtualAllocEx(hProcess, NULL, sizeof(szFilename), MEM_COMMIT,
		PAGE_READWRITE);
	if(pRemoteDllPath != NULL)
	{
		WriteProcessMemory(hProcess, pRemoteDllPath, szFilename, sizeof(szFilename), NULL);
		
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
			GetModuleHandle(TEXT("Kernel32")),
#ifdef UNICODE
			"LoadLibraryW"
#else
			"LoadLibraryA"
#endif
			);
		// 此方式加载的DLL不会向调试器发送LOAD_DLL消息
		HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, 
			pfnThreadRtn, pRemoteDllPath, 0, NULL);
		if (hThread != NULL)
		{
			bSuccess = TRUE;
			SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			CloseHandle(hThread);
		}
	}
	
	return bSuccess;
}

TCHAR * szCurrExePath;

void SetCurrExePath(LPCTSTR lpszExePath){

	TCHAR *path = new TCHAR[256];
	_tcscpy(path, lpszExePath);
	szCurrExePath = path;
}

// 取得当前EXE的所在文件夹路径
TCHAR * GetCurrExePath(){
	return szCurrExePath;
}