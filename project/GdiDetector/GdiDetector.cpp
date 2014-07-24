#pragma warning(disable: 4786)
//#include "StdAfx.h"
//#include "string_split.h"
#include "Struct_Define.h"
#include "Inject.h"
#include "GdiDetector.h"
#include "StackWalker.h"
#include "RedirectedApiFunc.h"
#include "XMLOperator.h"

#define WM_DLLUNLOAD WM_USER + 8

// 存储句柄对应的信息
std::map<DWORD, HANDLE_INFORAMTION > g_handleInfo;	
HANDLE_TYPE handleType;


CGdiDetector CGdiDetector::g_CGdiDetector;

extern HWND g_hMain;
extern HANDLE g_hMapFile;
extern LPVOID g_pBuf;
extern char szMainProPath[];
	


// 重定向指定模块中的某个函数
#define REDIRECTAPI(dllName, FuncName)	\
	RedirectApi(#dllName, #FuncName, (DWORD)_##FuncName, &hItem_##FuncName);



// BITMAP
STATIC_VAR_INIT(CreateBitmap)
STATIC_VAR_INIT(CreateBitmapIndirect)
STATIC_VAR_INIT(CreateCompatibleBitmap)
STATIC_VAR_INIT(CreateDIBitmap)
STATIC_VAR_INIT(CreateDIBSection)
STATIC_VAR_INIT(CreateDiscardableBitmap)
STATIC_VAR_INIT(LoadBitmapA)	// USER32.DLL
STATIC_VAR_INIT(LoadBitmapW)	// USER32.DLL

// BRUSH
STATIC_VAR_INIT(CreateBrushIndirect)
STATIC_VAR_INIT(CreateDIBPatternBrush)
STATIC_VAR_INIT(CreateDIBPatternBrushPt)
STATIC_VAR_INIT(CreateHatchBrush)
STATIC_VAR_INIT(CreatePatternBrush)
STATIC_VAR_INIT(CreateSolidBrush)

// DEVICE CONTEXT
STATIC_VAR_INIT(CreateDCA)
STATIC_VAR_INIT(CreateDCW)
STATIC_VAR_INIT(CreateCompatibleDC)
STATIC_VAR_INIT(GetDC)			// USER32.DLL
STATIC_VAR_INIT(GetDCEx)		// USER32.DLL
STATIC_VAR_INIT(GetWindowDC)	// USER32.DLL
STATIC_VAR_INIT(CreateICA)
STATIC_VAR_INIT(CreateICW)
STATIC_VAR_INIT(BeginPaint)		// USER32.DLL

// FONT
STATIC_VAR_INIT(CreateFontA)
STATIC_VAR_INIT(CreateFontW)
STATIC_VAR_INIT(CreateFontIndirectA)
STATIC_VAR_INIT(CreateFontIndirectW)
//STATIC_VAR_INIT(CreateFontIndirectExA)
//STATIC_VAR_INIT(CreateFontIndirectExW)

// PALETTE
STATIC_VAR_INIT(CreatePalette)
STATIC_VAR_INIT(CreateHalftonePalette)

// PEN
STATIC_VAR_INIT(CreatePen)
STATIC_VAR_INIT(CreatePenIndirect)
STATIC_VAR_INIT(ExtCreatePen)

// REGION
STATIC_VAR_INIT(CreateEllipticRgn)
STATIC_VAR_INIT(CreateEllipticRgnIndirect)
STATIC_VAR_INIT(CreatePolygonRgn)
STATIC_VAR_INIT(CreatePolyPolygonRgn)
STATIC_VAR_INIT(CreateRectRgn)
STATIC_VAR_INIT(CreateRectRgnIndirect)
STATIC_VAR_INIT(CreateRoundRectRgn)
STATIC_VAR_INIT(PathToRegion)
STATIC_VAR_INIT(ExtCreateRegion)

// RELEASE API
STATIC_VAR_INIT(ReleaseDC)

// DELETE API
STATIC_VAR_INIT(DeleteObject)
STATIC_VAR_INIT(DeleteDC)
STATIC_VAR_INIT(DeleteMetaFile)
STATIC_VAR_INIT(DeleteEnhMetaFile)

// END API
STATIC_VAR_INIT(EndPaint)

// CLOSE API
STATIC_VAR_INIT(CloseMetaFile)
STATIC_VAR_INIT(CloseEnhMetaFile)







CGdiDetector::CGdiDetector(void){



	HMODULE hKernel32 = ::GetModuleHandle(TEXT("KERNEL32.DLL"));
	HMODULE hUser32 = ::GetModuleHandle(TEXT("USER32.DLL"));
	HMODULE hGdi32 = ::GetModuleHandle(TEXT("GDI32.DLL"));

	// BITMAP
	GET_OLD_FUNC_ADDR(CreateBitmap, hGdi32)
	GET_OLD_FUNC_ADDR(CreateBitmapIndirect, hGdi32)
	GET_OLD_FUNC_ADDR(CreateCompatibleBitmap, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDIBitmap, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDIBSection, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDiscardableBitmap, hGdi32)
	GET_OLD_FUNC_ADDR(LoadBitmapA, hUser32)	// USER32.DLL
	GET_OLD_FUNC_ADDR(LoadBitmapW, hUser32)	// USER32.DLL

	// BRUSH
	GET_OLD_FUNC_ADDR(CreateBrushIndirect, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDIBPatternBrush, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDIBPatternBrushPt, hGdi32)
	GET_OLD_FUNC_ADDR(CreateHatchBrush, hGdi32)
	GET_OLD_FUNC_ADDR(CreatePatternBrush, hGdi32)
	GET_OLD_FUNC_ADDR(CreateSolidBrush, hGdi32)


 	// DEVICE CONTENT
	
	GET_OLD_FUNC_ADDR(CreateDCA, hGdi32)
	GET_OLD_FUNC_ADDR(CreateDCW, hGdi32)
	GET_OLD_FUNC_ADDR(CreateCompatibleDC, hGdi32)
	GET_OLD_FUNC_ADDR(GetDC, hUser32)			// USER32.DLL
	GET_OLD_FUNC_ADDR(GetDCEx, hUser32)		// USER32.DLL
	GET_OLD_FUNC_ADDR(GetWindowDC, hUser32)	// USER32.DLL
	GET_OLD_FUNC_ADDR(CreateICA, hGdi32)
	GET_OLD_FUNC_ADDR(CreateICW, hGdi32)
	GET_OLD_FUNC_ADDR(BeginPaint, hUser32)		// USER32.DLL


	// FONT
	
	GET_OLD_FUNC_ADDR(CreateFontA, hGdi32)
	GET_OLD_FUNC_ADDR(CreateFontW, hGdi32)
	GET_OLD_FUNC_ADDR(CreateFontIndirectA, hGdi32)
	GET_OLD_FUNC_ADDR(CreateFontIndirectW, hGdi32)
	
	// PALETTE
	GET_OLD_FUNC_ADDR(CreatePalette, hGdi32)
	GET_OLD_FUNC_ADDR(CreateHalftonePalette, hGdi32)
		
	// PEN
	GET_OLD_FUNC_ADDR(CreatePen, hGdi32)
	GET_OLD_FUNC_ADDR(CreatePenIndirect, hGdi32)
	GET_OLD_FUNC_ADDR(ExtCreatePen, hGdi32)



	// REGION
	GET_OLD_FUNC_ADDR(CreateEllipticRgn, hGdi32)
	GET_OLD_FUNC_ADDR(CreateEllipticRgnIndirect, hGdi32)
	GET_OLD_FUNC_ADDR(CreatePolygonRgn, hGdi32)
	GET_OLD_FUNC_ADDR(CreatePolyPolygonRgn, hGdi32)
	GET_OLD_FUNC_ADDR(CreateRectRgn, hGdi32)
	GET_OLD_FUNC_ADDR(CreateRectRgnIndirect, hGdi32)
	GET_OLD_FUNC_ADDR(CreateRoundRectRgn, hGdi32)
	GET_OLD_FUNC_ADDR(PathToRegion, hGdi32)
	GET_OLD_FUNC_ADDR(ExtCreateRegion, hGdi32)
 

	// RELEASE API
	GET_OLD_FUNC_ADDR(ReleaseDC, hUser32)

	// DELETE API
	GET_OLD_FUNC_ADDR(DeleteObject, hGdi32)
	GET_OLD_FUNC_ADDR(DeleteDC, hGdi32)
	GET_OLD_FUNC_ADDR(DeleteMetaFile, hGdi32)
	GET_OLD_FUNC_ADDR(DeleteEnhMetaFile, hGdi32)

	// END API
	GET_OLD_FUNC_ADDR(EndPaint, hUser32)

	// CLOSE API
	GET_OLD_FUNC_ADDR(CloseMetaFile, hGdi32)
	GET_OLD_FUNC_ADDR(CloseEnhMetaFile, hGdi32)

// 	//RedirectApi("USER32.dll", "GetDC", GetMemberFuncAddr(dwAddr, &CGdiDetector::_GetDC), &hItem_GetDC);
// 	//RedirectApi("USER32.dll", "GetDC", (DWORD)_GetDC, &hItem_GetDC);
// 	
// 	
// 
// 
	// BITMAP
	REDIRECTAPI(GDI32.dll, CreateBitmap)
	REDIRECTAPI(GDI32.dll, CreateBitmapIndirect)
	REDIRECTAPI(GDI32.dll, CreateCompatibleBitmap)
	REDIRECTAPI(GDI32.dll, CreateDIBitmap)
	REDIRECTAPI(GDI32.dll, CreateDIBSection)
	REDIRECTAPI(GDI32.dll, CreateDiscardableBitmap)
	REDIRECTAPI(USER32.dll, LoadBitmapA)
	REDIRECTAPI(USER32.dll, LoadBitmapW)

	// BRUSH
	REDIRECTAPI(GDI32.dll, CreateBrushIndirect)
	REDIRECTAPI(GDI32.dll, CreateDIBPatternBrush)
	REDIRECTAPI(GDI32.dll, CreateDIBPatternBrushPt)
	REDIRECTAPI(GDI32.dll, CreateHatchBrush)
	REDIRECTAPI(GDI32.dll, CreatePatternBrush)
	REDIRECTAPI(GDI32.dll, CreateSolidBrush)
	

	// DEVICE CONTENT
	REDIRECTAPI(GDI32.dll, CreateDCA)
	REDIRECTAPI(GDI32.dll, CreateDCW)
	REDIRECTAPI(GDI32.dll, CreateCompatibleDC)
	REDIRECTAPI(USER32.dll, GetDC)			// USER32.dll
	REDIRECTAPI(USER32.dll, GetDCEx)		// USER32.dll
	REDIRECTAPI(USER32.dll, GetWindowDC)	// USER32.dll
	REDIRECTAPI(GDI32.dll, CreateICA)
	REDIRECTAPI(GDI32.dll, CreateICW)
	REDIRECTAPI(USER32.dll, BeginPaint)		// USER32.dll
	
	// FONT
	
	REDIRECTAPI(GDI32.dll, CreateFontA)
	REDIRECTAPI(GDI32.dll, CreateFontW)
	REDIRECTAPI(GDI32.dll, CreateFontIndirectA)
	REDIRECTAPI(GDI32.dll, CreateFontIndirectW)
	
	// PALETTE
	REDIRECTAPI(GDI32.dll, CreatePalette)
	REDIRECTAPI(GDI32.dll, CreateHalftonePalette)
	
	
	// PEN
	REDIRECTAPI(GDI32.dll, CreatePen)
	REDIRECTAPI(GDI32.dll, CreatePenIndirect)
	REDIRECTAPI(GDI32.dll, ExtCreatePen)




	// REGION
	REDIRECTAPI(GDI32.dll, CreateEllipticRgn)
	REDIRECTAPI(GDI32.dll, CreateEllipticRgnIndirect)
	REDIRECTAPI(GDI32.dll, CreatePolygonRgn)
	REDIRECTAPI(GDI32.dll, CreatePolyPolygonRgn)
	REDIRECTAPI(GDI32.dll, CreateRectRgn)
	REDIRECTAPI(GDI32.dll, CreateRectRgnIndirect)
	REDIRECTAPI(GDI32.dll, CreateRoundRectRgn)
	REDIRECTAPI(GDI32.dll, PathToRegion)
	REDIRECTAPI(GDI32.dll, ExtCreateRegion)


	// RELEASE API
	REDIRECTAPI(USER32.dll, ReleaseDC)

	// DELETE API
	REDIRECTAPI(GDI32.dll, DeleteObject)
	REDIRECTAPI(GDI32.dll, DeleteDC)
	REDIRECTAPI(GDI32.dll, DeleteMetaFile)
	REDIRECTAPI(GDI32.dll, DeleteEnhMetaFile)

	// END API
	REDIRECTAPI(USER32.dll, EndPaint)


	// CLOSE API
	REDIRECTAPI(GDI32.dll, CloseMetaFile)
	REDIRECTAPI(GDI32.dll, CloseEnhMetaFile)


	
}

CGdiDetector::~CGdiDetector(void){

// 	HANDLE_INFORAMTION handleInfo;
// 	handleInfo.handle = 325352343;
// 	handleInfo.uType = 4;
// 	strcpy(handleInfo.szParaInfo, "RTN_HDC:321354 | HWND:12312 | LPCSTR:hello");
// 
// 	//handleInfo.registers
// 	REGISTERS_INFORMATION regs;
// 	regs.Eax	= 0x7D30A00C;
// 	regs.Ebx	= 0x7D30A00C;
// 	regs.Ecx	= 0x7D30A00C;
// 	regs.Edx	= 0x7D30A00C;
// 	regs.EFlags = 0x7D30A00C;
// 	regs.Edi	= 0x7D30A00C;
// 	regs.Esi	= 0x7D30A00C;
// 	regs.Esp	= 0x7D30A00C;
// 	regs.Ebp	= 0x7D30A00C;
// 	regs.SegCs	= 0x7D30A00C;
// 	regs.SegSs	= 0x7D30A00C;
// 
// 	std::vector<MODULE_INFORMATION> moduleInfo;
// 	MODULE_INFORMATION m1 = {0};
// 	MODULE_INFORMATION m2 = {0};
// 	m1.BaseOfImage		= 0x034B0000;
// 	m1.FileVersion		= 13247007880135;
// 	strcpy(m1.ImageName, "C:\\WINDOWS\\system32\\MSIMTF.dll");
// 	m1.ImageSize		= 234231;
// 	//m1.ModuleName		= ;
// 	strcpy(m1.ModuleName, "MSIMTF.DLL");
// 	m1.TimeDateStamp	= 1141235151;
// 	moduleInfo.push_back(m1);
// 	moduleInfo.push_back(m2);
// 
// 	CALL_STACK callStack;
// 	callStack.callStack.push_back("[0x31000000, 0x34194B] IM.dll --> Func1");
// 	callStack.callStack.push_back("[0x31000000, 0x12B967] IM.dll --> Func2");
// 
// 	handleInfo.registers = regs;
// 	handleInfo.moduleInfo = moduleInfo;
// 	handleInfo.callStack = callStack;
// 	

 	char szPath[256];

 	sprintf(szPath, "%s\\InfoOfLeakObject.xml", szMainProPath);

	xmloperator xml;
	xml.SetFileName(szPath);
	xml.Createxml();

	std::map<DWORD, HANDLE_INFORAMTION>::iterator it;
	
	xml.Start(szPath);

	for(it=g_handleInfo.begin(); it != g_handleInfo.end(); it++){
		xml.Add(&((*it).second));	
	}

	xml.End(szPath);

	//SendMessage一直等待目标程序回应，会陷入等待状态，造成程序假死
	//::SendMessage(g_hMain, WM_DLLUNLOAD, NULL, NULL);

	::PostMessage(g_hMain, WM_DLLUNLOAD, NULL, NULL);

	if(g_pBuf){
		UnmapViewOfFile(g_pBuf);
		CloseHandle(g_hMapFile);
	}
	
}

