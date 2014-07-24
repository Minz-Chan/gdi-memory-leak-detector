#pragma warning(disable:4786)
#include "RedirectedApiFunc.h"
#include "GdiDetector.h"
#include "StackWalker.h"
#include "XMLOperator.h"
#include "Struct_Define.h"


extern std::map<DWORD, HANDLE_INFORAMTION > g_handleInfo;	
extern HANDLE_TYPE handleType;

//StackWalker g_stackWalker;

extern HWND g_hMain;
extern char szMainProPath[];

#define WM_GDIOBJACTION WM_USER + 9


// 保存句柄相关的一些信息
// #define SAVE_HANDLE_INFORMATION(handleInfo) \
// 	do{\
// 	g_handleInfo.insert(map<DWORD, HANDLE_INFORAMTION>\
// 	::value_type(handleInfo.handle, handleInfo));\
// 	char szPath[256];\
// 	sprintf(szPath, "%s\\ObjAction.xml", szMainProPath);\
// 	xmloperator xml;\
// 	xml.SetFileName(szPath);\
// 	xml.Createxml();\
// 	xml.Start(szPath);\
// 	xml.Add(&handleInformation);\
// 	::PostMessage(g_hMain, WM_GDIOBJACTION, NULL, NULL);\
// 	xml.End(szPath);\
// 	}while(0);
#define SAVE_HANDLE_INFORMATION(handleInfo) \
	do{\
	g_handleInfo.insert(map<DWORD, HANDLE_INFORAMTION>\
	::value_type(handleInfo.handle, handleInfo));\
	WriteToMemory(&handleInfo);\
	::PostMessage(g_hMain, WM_GDIOBJACTION, NULL, NULL);\
	}while(0);


/* 将结构体HANDLE_INFORMATION的信息写入内存映射区域 */
BOOL WriteToMemory(PHANDLE_INFORMATION lpHandleInfo){
	
	HANDLE g_hMapFile;
	LPVOID g_pBuf;
	
	char szSectionObjectName[] = "HandleOfGdiObj";
	
	g_hMapFile = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // 
		1024 * 64,                // buffer size  
		szSectionObjectName);  // name of mapping object
	
	if (g_hMapFile == NULL)
	{ 
		return FALSE;
	}
	
	g_pBuf = (LPTSTR)::MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);           
	if (g_pBuf == NULL)
	{
		CloseHandle(g_hMapFile);
		return FALSE;
	}
	
	
	char *pCur = (char *)g_pBuf;
	
	// 等待状态转换为可写状态
	while(*(int *)pCur){};  
	
	// 状态值
	*(int *)pCur = 1;	//正在写入...
	pCur += sizeof(int);
	
	// 句柄值
	*(unsigned long *)pCur = lpHandleInfo->handle;
	pCur += sizeof(unsigned long);
	
	// 类型值
	*(unsigned *)pCur = lpHandleInfo->uType;
	pCur += sizeof(unsigned);
	
	// 参数值
	memcpy(pCur, lpHandleInfo->szParaInfo, 256);
	pCur += (sizeof(char) * 256);
	
	// Registers
	memcpy(pCur, &lpHandleInfo->registers, 48);
	pCur += sizeof(REGISTERS_INFORMATION);
	
	// Module Information 
	int nModule = lpHandleInfo->moduleInfo.size();
	*(int *)pCur = nModule;
	pCur += sizeof(int);
	for(int i=0; i < nModule; i++){
		memcpy(pCur, &lpHandleInfo->moduleInfo[i], sizeof(MODULE_INFORMATION));
		pCur += sizeof(MODULE_INFORMATION);
	}
	
	// CallStacks
	int nCallStack = (lpHandleInfo->callStack).callStack.size();
	*(int *)pCur = nCallStack;
	pCur += sizeof(int);
	for(int j=0; j < nCallStack; j++){
		int nStr = lpHandleInfo->callStack.callStack[j].size();
		char szCallStack[256];
		sprintf(szCallStack, "%s", lpHandleInfo->callStack.callStack[j].c_str());
		*(int *)pCur = nStr;
		pCur += sizeof(int);
		memcpy(pCur, szCallStack, nStr);
		pCur += nStr;
	}
	
	pCur = (char *)g_pBuf;
	
	// 状态值
	*(int *)pCur = 0;	//写入完成
	
	
	return TRUE;
}


/* 获取参数信息 */
void GetParaInfo(string s, PHANDLE_INFORMATION pHandleInfo){
	
	pHandleInfo->handle = (DWORD)atoi((s.substr(s.find(":", s.find("#")) + 1, \
		s.find("|") - s.find(":", s.find("#")) - 1)).c_str());
	
	pHandleInfo->uType = atoi((s.substr(5, s.find("#") - s.find(":") - 1)).c_str());
	
	string szPara = s.substr(s.find("#") +1 );
	sprintf(pHandleInfo->szParaInfo, "%s", szPara.c_str());
}



HBITMAP WINAPI _CreateBitmap(
					 int nWidth,         // bitmap width, in pixels
					 int nHeight,        // bitmap height, in pixels
					 UINT cPlanes,       // number of color planes used by device
					 UINT cBitsPerPel,   // number of bits required to identify a color
					 CONST VOID *lpvBits // pointer to array containing color data
					 ){

	PARA_DECLARE_REGION
	
	sprintf(szPara, "int:%d | int:%d | UINT:%u | UINT:%u | CONST VOID *:%p\0", 
		nWidth, nHeight, cPlanes, cBitsPerPel, (LPVOID)lpvBits);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateBitmap(nWidth, nHeight, cPlanes, cBitsPerPel, lpvBits);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
	
	return hVarHBITMAP;
}




HBITMAP WINAPI _CreateBitmapIndirect(
									 CONST BITMAP *lpbm    // pointer to the bitmap data
									 ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST BITMAP *:%p\0",(LPVOID)lpbm);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateBitmapIndirect(lpbm);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);;
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;


}

HBITMAP WINAPI _CreateCompatibleBitmap(
									   HDC hdc,        // handle to device context
									   int nWidth,     // width of bitmap, in pixels
									   int nHeight     // height of bitmap, in pixels
									   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld | int:%d | int:%d\0", hdc, nWidth, nHeight);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateCompatibleBitmap(hdc, nWidth, nHeight);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;

}

HBITMAP WINAPI _CreateDIBitmap(
							   HDC hdc,                  // handle to device context
							   CONST BITMAPINFOHEADER *lpbmih,  // pointer to bitmap size and
							   // format data
							   DWORD fdwInit,            // initialization flag
							   CONST VOID *lpbInit,      // pointer to initialization data
							   CONST BITMAPINFO *lpbmi,  // pointer to bitmap color-format data
							   UINT fuUsage              // color-data usage
							   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld | CONST BITMAPINFOHEADER *:%p | DWORD:%ld | CONST VOID *:%p"
		" | CONST BITMAPINFO *:%p | UINT:%u\0", hdc, (LPVOID)lpbmih, fdwInit, (LPVOID)lpbInit,
		(LPVOID) lpbmi, fuUsage);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateDIBitmap(hdc, lpbmih, fdwInit, lpbInit, lpbmi, fuUsage);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;
}

HBITMAP WINAPI _CreateDIBSection(
								 HDC hdc,          // handle to device context
								 CONST BITMAPINFO *pbmi,
								 // pointer to structure containing bitmap size, 
								 // format, and color data
								 UINT iUsage,      // color data type indicator: RGB values or 
								 // palette indexes
								 VOID **ppvBits,    // pointer to variable to receive a pointer to 
								 // the bitmap's bit values
								 HANDLE hSection,  // optional handle to a file mapping object
								 DWORD dwOffset    // offset to the bitmap bit values within the 
								 // file mapping object
								 ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld | CONST BITMAPINFO *:%p | UINT:%u | VOID *:%p"
		" | HANDLE:%ld | DWORD:%ld\0", hdc, (LPVOID)pbmi, iUsage, (LPVOID)ppvBits,
		(LPVOID) hSection, dwOffset);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateDIBSection(hdc, pbmi, iUsage, ppvBits, hSection, dwOffset);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;

}

HBITMAP WINAPI _CreateDiscardableBitmap(
										HDC hdc,     // handle to device context
										int nWidth,  // bitmap width
										int nHeight  // bitmap height
										){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld | int:%d | int:%d\0", hdc, nWidth, nHeight);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__CreateDiscardableBitmap(hdc, nWidth, nHeight);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;

}

HBITMAP WINAPI _LoadBitmapA(
							HINSTANCE hInstance,  // handle to application instance
							LPSTR lpBitmapName  // address of bitmap resource name
							){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HINSTANCE:%ld | LPSTR:%s\0", hInstance, lpBitmapName);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__LoadBitmapA(hInstance, lpBitmapName);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;

}

HBITMAP WINAPI _LoadBitmapW(
							HINSTANCE hInstance,  // handle to application instance
							LPWSTR lpBitmapName  // address of bitmap resource name
							){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HINSTANCE:%ld | LPWSTR:%s\0", hInstance, lpBitmapName);
	
	HBITMAP hVarHBITMAP = CGdiDetector::__LoadBitmapW(hInstance, lpBitmapName);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBITMAP:%ld | %s\0", _BITMAP, (DWORD)hVarHBITMAP, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
		SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBITMAP;


}


HBRUSH WINAPI _CreateBrushIndirect(
								   CONST LOGBRUSH *lplb   // pointer to structure describing brush
								   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST LOGBRUSH *:%p \0", (LPVOID)lplb);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreateBrushIndirect(lplb);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;

}


HBRUSH WINAPI _CreateDIBPatternBrush(
									 HGLOBAL hglbDIBPacked,  // handle to device-independent bitmap
									 UINT fuColorSpec        // color table data
									 ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HGLOBAL:%ld | UINT:%u\0", hglbDIBPacked, fuColorSpec);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;

}

HBRUSH WINAPI _CreateDIBPatternBrushPt(
									   CONST VOID *lpPackedDIB,  // pointer to structure and bitmap bits
									   UINT iUsage               // usage flags
									   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST VOID *:%p | UINT:%u\0", (LPVOID)lpPackedDIB, iUsage);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;
}
									   

HBRUSH WINAPI _CreateHatchBrush(
								int fnStyle,      // hatch style
								COLORREF clrref   // color value
								){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | COLORREF:%#X\0", fnStyle, clrref);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreateHatchBrush(fnStyle, clrref);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;
}


HBRUSH WINAPI _CreatePatternBrush(
								  HBITMAP hbmp   // handle to bitmap
								  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HBITMAP:%ld\0", hbmp);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreatePatternBrush(hbmp);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;
}

HBRUSH WINAPI _CreateSolidBrush(
								COLORREF crColor   // brush color value
								){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "COLORREF:%#X\0", crColor);
	
	HBRUSH hVarHBRUSH = CGdiDetector::__CreateSolidBrush(crColor);
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HBRUSH:%ld | %s\0", _BRUSH, (DWORD)hVarHBRUSH, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
	return hVarHBRUSH;
}






HDC WINAPI _CreateDCA(
					  LPCSTR lpszDriver,  // pointer to string specifying driver name
					  LPCSTR lpszDevice,  // pointer to string specifying device name
					  LPCSTR lpszOutput,  // do not use; set to NULL
					  CONST DEVMODE *lpInitData 
					  // pointer to optional printer data
					  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "LPCSTR:%s | LPCSTR:%s | LPCSTR:%s |CONST DEVMODE *:%p\0",  lpszDriver, lpszDevice,
	 lpszOutput,(CONST DEVMODE *)lpInitData);
	
	HDC hVarHDC = CGdiDetector::__CreateDCA(lpszDriver,lpszDevice,lpszOutput,lpInitData);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
		
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}
HDC WINAPI _CreateDCW(
					  LPCWSTR lpszDriver,  // pointer to string specifying driver name
					  LPCWSTR lpszDevice,  // pointer to string specifying device name
					  LPCWSTR lpszOutput,  // do not use; set to NULL
					  CONST DEVMODE *lpInitData 
					  // pointer to optional printer data
					  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "LPCWSTR:%s | LPCWSTR:%s | LPCWSTR:%s | CONST DEVMODE *:%p\0",  lpszDriver, lpszDevice,
	 lpszOutput,(CONST DEVMODE *)lpInitData);
	
	HDC hVarHDC = CGdiDetector::__CreateDCW(lpszDriver,lpszDevice,lpszOutput,lpInitData);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}

HDC WINAPI _CreateCompatibleDC(
							   HDC hdc   // handle to the device context
							   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HWND:%ld\0",  hdc);
	
	HDC hVarHDC = CGdiDetector::__CreateCompatibleDC(hdc);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
		
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}

/* GetDC */
HDC WINAPI _GetDC(HWND hwnd){
	
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HWND:%ld\0", hwnd);
	
	HDC hVarHDC = CGdiDetector::__GetDC(hwnd);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	//TRACE_INFO_VIEW
// 	string szCallStack;
// 		std::vector<std::string>::iterator it;
// 		for(it=handleInformation.callStack.callStack.begin()
// 			; it != handleInformation.callStack.callStack.end()
// 			; it++){
// 			szCallStack+=*it;
// 			szCallStack+="\n";
// 		}
// 	::MessageBox(NULL,szCallStack.c_str(),"CallStack Message", MB_ICONINFORMATION);

	
	
// 	string szModulesInfo = "";
// 	MODULE_INFORMATION mInfo;
// 	std::vector<MODULE_INFORMATION>::iterator it;
// 	for(it=handleInformation.moduleInfo.begin()
// 		; it != handleInformation.moduleInfo.end()
// 		; it++){
// 
// 		char szTmp[256];
// 		mInfo = *it;
// 
// 		DWORD v4 = (DWORD)	mInfo.FileVersion & 0xFFFF;
// 		DWORD v3 = (DWORD) (mInfo.FileVersion>>16) & 0xFFFF;
// 		DWORD v2 = (DWORD) (mInfo.FileVersion>>32) & 0xFFFF;
// 		DWORD v1 = (DWORD) (mInfo.FileVersion>>48) & 0xFFFF;
// 
// 		
// 		sprintf(szTmp, "%s.DLL [%#x] %s (size:%d) [TimeDateStamp:%ld] [FileVersion:%d.%d.%d.%d]\n",
// 			mInfo.ModuleName, mInfo.BaseOfImage, mInfo.ImageName, mInfo.ImageSize,
// 			mInfo.TimeDateStamp, v1, v2, v3,v4);
// 		szModulesInfo += szTmp;
// 	}

	//::MessageBox(NULL,szModulesInfo.c_str(),"Modules Inforamtion Message", MB_ICONINFORMATION);
	
	SAVE_HANDLE_INFORMATION(handleInformation)

	//写入单个 HADNLE_INFORMATION 信息到 ObjAction.tmp

// 	char szPath[256];
// 	
// 	sprintf(szPath, "%s\\ObjAction.xml", szMainProPath);
// 	
// 
// 	xmloperator xml;
// 	xml.SetFileName(szPath);
// 	xml.Createxml();
// 	
// 	xml.Start(szPath);
// 	xml.Add(&handleInformation);	
// 	xml.End(szPath);
// 
// 	::PostMessage(g_hMain, WM_GDIOBJACTION, NULL, NULL);
		
		
	return hVarHDC;
}

HDC WINAPI _GetDCEx(
					HWND hWnd,      // handle of window
					HRGN hrgnClip,  // handle of clip region
					DWORD flags     // device-context creation flags
					){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HWND:%ld | HRGN:%ld | DWORD:%ld\0", hWnd, hrgnClip, flags);
	
	HDC hVarHDC = CGdiDetector::__GetDCEx(hWnd,hrgnClip,flags);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}



HDC WINAPI _GetWindowDC(
						HWND hWnd   // handle of window
						){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HWND:%ld\0", hWnd);
	
	HDC hVarHDC = CGdiDetector::__GetWindowDC(hWnd);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
		
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}

HDC WINAPI _CreateICA(
					  LPCSTR lpszDriver,  // pointer to string specifying driver name
					  LPCSTR lpszDevice,  // pointer to string specifying device name
					  LPCSTR lpszOutput,  // pointer to string specifying port or file name
					  CONST DEVMODE *lpdvmInit 
					  // pointer to optional initialization data
					  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "LPCSTR:%s | LPCSTR:%s | LPCSTR:%s | CONST DEVMODE *:%p\0",  lpszDriver, lpszDevice,
	 lpszOutput,(CONST DEVMODE *)lpdvmInit);
	
	HDC hVarHDC = CGdiDetector::__CreateICA(lpszDriver,lpszDevice,lpszOutput,lpdvmInit);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}

HDC WINAPI _CreateICW(
					  LPCWSTR lpszDriver,  // pointer to string specifying driver name
					  LPCWSTR lpszDevice,  // pointer to string specifying device name
					  LPCWSTR lpszOutput,  // pointer to string specifying port or file name
					  CONST DEVMODE *lpdvmInit 
					  // pointer to optional initialization data
					  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "LPCWSTR:%s | LPCWSTR:%s | LPCWSTR:%s | CONST DEVMODE *:%p\0", lpszDriver, lpszDevice,
	 lpszOutput,(CONST DEVMODE *)lpdvmInit);
	
	HDC hVarHDC = CGdiDetector::__CreateICW(lpszDriver,lpszDevice,lpszOutput,lpdvmInit);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}

HDC WINAPI _BeginPaint(
					   HWND hwnd,  // handle to window
					   LPPAINTSTRUCT lpPaint
					   // pointer to structure for paint information
					   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HWND:%ld | LPPAINTSTRUCT:%p\0",  hwnd, (LPVOID)lpPaint);
	
	HDC hVarHDC = CGdiDetector::__BeginPaint(hwnd,lpPaint);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HDC:%ld | %s\0", _DC, (DWORD)hVarHDC, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHDC;
}



HFONT WINAPI _CreateFontA(
						  int nHeight,             // logical height of font
						  int nWidth,              // logical average character width
						  int nEscapement,         // angle of escapement
						  int nOrientation,        // base-line orientation angle
						  int fnWeight,            // font weight
						  DWORD fdwItalic,         // italic attribute flag
						  DWORD fdwUnderline,      // underline attribute flag
						  DWORD fdwStrikeOut,      // strikeout attribute flag
						  DWORD fdwCharSet,        // character set identifier
						  DWORD fdwOutputPrecision,  // output precision
						  DWORD fdwClipPrecision,  // clipping precision
						  DWORD fdwQuality,        // output quality
						  DWORD fdwPitchAndFamily,  // pitch and family
						  LPCSTR lpszFace         // pointer to typeface name string
						  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | int:%d | int:%d | int:%d | DWORD:%ld | DWORD:%ld | DWORD:%ld | DWORD:%ld |"
	 "DWORD:%ld | DWORD:%ld | DWORD:%ld | DWORD:%ld | LPCSTR:%s\0", nHeight, nWidth, nEscapement, nOrientation, fnWeight,
		   fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision,
		    fdwQuality, fdwPitchAndFamily, lpszFace);
	
	HFONT hVarHFONT = CGdiDetector::__CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
		fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision,
		    fdwQuality, fdwPitchAndFamily, lpszFace);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HFONT:%ld | %s\0", _FONT, (HFONT)hVarHFONT, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHFONT;
}


HFONT WINAPI _CreateFontW(
						  int nHeight,             // logical height of font
						  int nWidth,              // logical average character width
						  int nEscapement,         // angle of escapement
						  int nOrientation,        // base-line orientation angle
						  int fnWeight,            // font weight
						  DWORD fdwItalic,         // italic attribute flag
						  DWORD fdwUnderline,      // underline attribute flag
						  DWORD fdwStrikeOut,      // strikeout attribute flag
						  DWORD fdwCharSet,        // character set identifier
						  DWORD fdwOutputPrecision,  // output precision
						  DWORD fdwClipPrecision,  // clipping precision
						  DWORD fdwQuality,        // output quality
						  DWORD fdwPitchAndFamily,  // pitch and family
						  LPCWSTR lpszFace         // pointer to typeface name string
						  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | int:%d | int:%d | int:%d | DWORD:%ld | DWORD:%ld | DWORD:%ld | DWORD:%ld | " 
	   "DWORD:%ld | DWORD:%ld | DWORD:%ld | DWORD:%ld | LPCWSTR:%s\0", nHeight, nWidth, nEscapement, nOrientation, fnWeight,
		fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision,fdwQuality, fdwPitchAndFamily, lpszFace);
	
	HFONT hVarHFONT = CGdiDetector::__CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight,fdwItalic, 
		fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision,fdwQuality, fdwPitchAndFamily, lpszFace);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
		sprintf(sInfo,"Type:%d#RTN_HFONT:%ld | %s\0", _FONT, (HFONT)hVarHFONT, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHFONT;
}

HFONT WINAPI _CreateFontIndirectA(
								  CONST LOGFONTA *lplf   // pointer to logical font structure
								  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST LOGFONTA *:%p\0",(CONST LOGFONTA *)lplf);
	
	HFONT hVarHFONT = CGdiDetector::__CreateFontIndirectA(lplf);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
		sprintf(sInfo,"Type:%d#RTN_HFONT:%ld | %s\0", _FONT, (HFONT)hVarHFONT, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHFONT;
}

HFONT WINAPI _CreateFontIndirectW(
								  CONST LOGFONTW *lplf   // pointer to logical font structure
								  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST LOGFONTW *:%p\0",(CONST LOGFONTW *)lplf);
	
	HFONT hVarHFONT = CGdiDetector::__CreateFontIndirectW(lplf);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HFONT:%ld | %s\0", _FONT, (HFONT)hVarHFONT, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHFONT;
}


HPALETTE WINAPI _CreatePalette(
							   CONST LOGPALETTE *lplgpl   // pointer to logical color palette
							   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST LOGPALETTE *:%p\0",(CONST LOGPALETTE *)lplgpl);
	
	HPALETTE hVarHPALETTE = CGdiDetector::__CreatePalette(lplgpl);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HPALETTE:%ld | %s\0", _PAL, (HPALETTE)hVarHPALETTE, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHPALETTE;
}


HPALETTE WINAPI _CreateHalftonePalette(
									   HDC hdc   // handle to device context
									   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld\0",hdc);
	
	HPALETTE hVarHPALETTE = CGdiDetector::__CreateHalftonePalette(hdc);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HPALETTE:%ld | %s\0", _PAL, (HPALETTE)hVarHPALETTE, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHPALETTE;
}


HPEN WINAPI _CreatePen(
					   int fnPenStyle,    // pen style
					   int nWidth,        // pen width
					   COLORREF crColor   // pen color
					   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | COLORREF:%#s\0",fnPenStyle, nWidth, crColor);
	
	HPEN hVarHPEN = CGdiDetector::__CreatePen(fnPenStyle, nWidth, crColor);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HPEN:%ld | %s\0", _PEN, (HPEN)hVarHPEN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHPEN;
}

HPEN WINAPI _CreatePenIndirect(
							   CONST LOGPEN *lplgpn   // pointer to LOGPEN structure
							   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST LOGPEN *:%p\0",(CONST LOGPEN *)lplgpn);
	
	HPEN hVarHPEN = CGdiDetector::__CreatePenIndirect(lplgpn);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HPEN:%ld | %s\0", _PEN, (HPEN)hVarHPEN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHPEN;
}


HPEN WINAPI _ExtCreatePen(
						  DWORD dwPenStyle,      // pen style
						  DWORD dwWidth,         // pen width
						  CONST LOGBRUSH *lplb,  // pointer to structure for brush attributes
						  DWORD dwStyleCount,    // length of array containing custom style bits
						  CONST DWORD *lpStyle   // optional array of custom style bits
						  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "DWORD:%ld |  DWORD:%ld | CONST LOGBRUSH *:%p | DWORD:%ld | CONST DWORD *:%p\0",dwPenStyle, 
	dwWidth, (CONST LOGBRUSH *)lplb, dwStyleCount, (CONST DWORD *)lpStyle);
	
	HPEN hVarHPEN = CGdiDetector::__ExtCreatePen(dwPenStyle, dwWidth, lplb, dwStyleCount, lpStyle);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HPEN:%ld | %s\0", _PEN, (HPEN)hVarHPEN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	
	SAVE_HANDLE_INFORMATION(handleInformation)
		
		
	return hVarHPEN;
}








HRGN WINAPI _CreateEllipticRgn(
							   int nLeftRect,   // x-coord of the upper-left corner of the bounding rectangle
							   int nTopRect,    // y-coord of the upper-left corner of the bounding rectangle
							   int nRightRect,  // x-coord of the lower-right corner of the bounding rectangle
							   int nBottomRect  // y-coord of the lower-right corner of the bounding rectangle
							   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | int:%d | int:%d\0", nLeftRect, nTopRect, 
		nRightRect, nBottomRect);
	
	HRGN hVarHRGN = CGdiDetector::__CreateEllipticRgn(nLeftRect, nTopRect, 
		nRightRect, nBottomRect);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
			
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;

}

HRGN WINAPI _CreateEllipticRgnIndirect(
									   CONST RECT *lprc   // pointer to bounding rectangle
									   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST RECT *:%p\0", (LPVOID)lprc);
	
	HRGN hVarHRGN = CGdiDetector::__CreateEllipticRgnIndirect(lprc);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}

HRGN WINAPI _CreatePolygonRgn(
							  CONST POINT *lppt,  // pointer to array of points
							  int cPoints,        // number of points in array
							  int fnPolyFillMode  // polygon-filling mode
							  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST RECT *:%p | int:%d | int:%d\0", (LPVOID)lppt, cPoints, fnPolyFillMode);
	
	HRGN hVarHRGN = CGdiDetector::__CreatePolygonRgn(lppt, cPoints, fnPolyFillMode);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}

HRGN WINAPI _CreatePolyPolygonRgn(
								  CONST POINT *lppt,        // pointer to array of points
								  CONST INT *lpPolyCounts,  // pointer to count of vertices
								  int nCount,               // number of integers in array
								  int fnPolyFillMode        // polygon fill mode
								  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST POINT *:%p | CONST INT *:%p | int:%d | int:%d\0", (LPVOID)lppt, 
		lpPolyCounts, nCount ,fnPolyFillMode);
	
	HRGN hVarHRGN = CGdiDetector::__CreatePolyPolygonRgn(lppt, lpPolyCounts, nCount ,fnPolyFillMode);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}

HRGN WINAPI _CreateRectRgn(
						   int nLeftRect,   // x-coordinate of region's upper-left corner
						   int nTopRect,    // y-coordinate of region's upper-left corner
						   int nRightRect,  // x-coordinate of region's lower-right corner
						   int nBottomRect  // y-coordinate of region's lower-right corner
						   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | int:%d | int:%d\0", nLeftRect, 
		nTopRect, nRightRect ,nBottomRect);
	
	HRGN hVarHRGN = CGdiDetector::__CreateRectRgn(nLeftRect, nTopRect, nRightRect ,nBottomRect);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}


HRGN WINAPI _CreateRectRgnIndirect(
								   CONST RECT *lprc   // pointer to the rectangle
								   ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST RECT *:%p\0", (LPVOID)lprc);
	
	HRGN hVarHRGN = CGdiDetector::__CreateRectRgnIndirect(lprc);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}


HRGN WINAPI _CreateRoundRectRgn(
								int nLeftRect,      // x-coordinate of the region's upper-left corner
								int nTopRect,       // y-coordinate of the region's upper-left corner
								int nRightRect,     // x-coordinate of the region's lower-right corner
								int nBottomRect,    // y-coordinate of the region's lower-right corner
								int nWidthEllipse,  // height of ellipse for rounded corners
								int nHeightEllipse  // width of ellipse for rounded corners
								){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "int:%d | int:%d | int:%d | int:%d | int:%d | int:%d\0", nLeftRect, 
		nTopRect, nRightRect ,nBottomRect, nWidthEllipse, nHeightEllipse);
	
	HRGN hVarHRGN = CGdiDetector::__CreateRoundRectRgn(nLeftRect, 
		nTopRect, nRightRect ,nBottomRect, nWidthEllipse, nHeightEllipse);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
	
}

HRGN WINAPI _PathToRegion(
						  HDC hdc   // handle to device context
						  ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "HDC:%ld\0", hdc);
	
	HRGN hVarHRGN = CGdiDetector::__PathToRegion(hdc);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}

HRGN WINAPI _ExtCreateRegion(
							 CONST XFORM *lpXform,     // pointer to transformation data
							 DWORD nCount,             // size of structure containing region data
							 CONST RGNDATA *lpRgnData  // pointer to region data
							 ){
	PARA_DECLARE_REGION
		
	sprintf(szPara, "CONST XFORM *:%p | DWORD:%ld | CONST RGNDATA *:%p\0", (LPVOID)lpXform, 
		nCount, (LPVOID)lpRgnData);
	
	HRGN hVarHRGN = CGdiDetector::__ExtCreateRegion(lpXform, nCount, lpRgnData);
	
	GET_CALLSTACK_TRACE(handleInformation)
		
	sprintf(sInfo,"Type:%d#RTN_HRGN:%ld | %s\0", _REGION, (DWORD)hVarHRGN, szPara);
	
	GetParaInfo(sInfo, &handleInformation);
	
	SAVE_HANDLE_INFORMATION(handleInformation)		
		
	return hVarHRGN;
}















int WINAPI _ReleaseDC(
					  HWND hWnd,  // handle to window
					  HDC hDC     // handle to device context
					  ){
	
	int hVarINT = CGdiDetector::__ReleaseDC(hWnd, hDC);
	g_handleInfo.erase((DWORD)hDC);
	
	return hVarINT;
}
 





BOOL WINAPI _DeleteObject(
						  HGDIOBJ hObject   // handle to graphic object
						  ){	
	BOOL hVarBOOL = CGdiDetector::__DeleteObject(hObject);
	g_handleInfo.erase((DWORD)hObject);
		
	return hVarBOOL;
}

BOOL WINAPI _DeleteDC(
					  HDC hdc   // handle to device context
					  ){
	
	BOOL hVarBOOL = CGdiDetector::__DeleteDC(hdc);
	g_handleInfo.erase((DWORD)hdc);
		
	return hVarBOOL;
}

BOOL WINAPI _DeleteMetaFile(
							HMETAFILE hmf   // handle to Windows-format metafile
							){

	
	BOOL hVarBOOL = CGdiDetector::__DeleteMetaFile(hmf);
	g_handleInfo.erase((DWORD)hmf);
		
	return hVarBOOL;
}


BOOL WINAPI _DeleteEnhMetaFile(
							   HENHMETAFILE hemf   // handle to an enhanced metafile
							   ){
	BOOL hVarBOOL = CGdiDetector::__DeleteEnhMetaFile(hemf);
	g_handleInfo.erase((DWORD)hemf);
		
	return hVarBOOL;
}







BOOL WINAPI _EndPaint(
					  HWND hWnd,  // handle to window
					  CONST PAINTSTRUCT *lpPaint 
					  // pointer to structure for paint data
					  ){
	
	BOOL hVarBOOL = CGdiDetector::__EndPaint(hWnd, lpPaint);
	g_handleInfo.erase((DWORD)hWnd);
		
	return hVarBOOL;

}






HMETAFILE WINAPI _CloseMetaFile(
								HDC hdc   // handle to metafile device context
								){
	HMETAFILE hVarHMETAFILE = CGdiDetector::__CloseMetaFile(hdc);
	g_handleInfo.erase((DWORD)hdc);
		
	return hVarHMETAFILE;

}


HENHMETAFILE WINAPI _CloseEnhMetaFile(
									  HDC hdc   // handle to an enhanced-metafile device context
									  ){
	HENHMETAFILE hVarHMETAFILE = CGdiDetector::__CloseEnhMetaFile(hdc);
	g_handleInfo.erase((DWORD)hdc);
		
	return hVarHMETAFILE;
}