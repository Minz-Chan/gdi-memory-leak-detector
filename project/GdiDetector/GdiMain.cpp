//#include "StdAfx.h"
#pragma warning(disable:4786)
#include <windows.h>
//#include <windef.h>
#include <tchar.h>
#include <vector>
//#include "StackWalker.h"

HWND g_hMain;
HANDLE g_hMapFile;
LPVOID g_pBuf;
char szMainProPath[256];


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
    switch ( fdwReason )
    {
    case DLL_PROCESS_ATTACH:
        {
			try{
				char szSectionObjectName[] = "HandleOfMain";
				g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szSectionObjectName);
				//if (g_hMapFile == NULL) 
				//	return FALSE;
				
				g_pBuf = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 128);                   
				if (g_pBuf == NULL)
				{
					CloseHandle(g_hMapFile);
				//	return FALSE;
				}
				
				g_hMain = (HWND)(*(DWORD*)g_pBuf);

				char *pCur = (char *)g_pBuf;
				pCur += sizeof(DWORD);
				memcpy(szMainProPath, pCur, 256);
			}
			catch(exception e){}
			//MessageBox( NULL, szMainProPath, _T("信息"), MB_ICONINFORMATION );

            MessageBox( NULL, _T("DLL已进入目标进程..."), _T("信息"), MB_ICONINFORMATION );

        }
        break;
    case DLL_PROCESS_DETACH:
        {
            MessageBox( NULL, _T("DLL已从目标进程卸载..."), _T("信息"), MB_ICONINFORMATION );
        }
        break;
    }
    return TRUE;
}
