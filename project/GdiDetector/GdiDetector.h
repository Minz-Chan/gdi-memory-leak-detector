#pragma once

#define _WIN32_WINNT 0x0501

#include <windows.h>


//定义API挂接项结构

#ifndef _STRUCT_HOOK_ITEM
#define _STRUCT_HOOK_ITEM

typedef struct _HOOK_ITEM {
	DWORD	dwAddr ;			// IAT项所在地址
	DWORD	dwOldValue ;		// IAT项的原始函数地址
	DWORD	dwNewValue ;		// IAT项的新函数地址
} HOOK_ITEM, *PHOOK_ITEM ;

#endif //_STRUCT_HOOK_ITEM


// 类静态成员的初始化
#define STATIC_VAR_INIT(Name)	\
	CGdiDetector::Name##Proc CGdiDetector::__##Name = 0;

// 获取函数在原模块中的地址
#define GET_OLD_FUNC_ADDR(FuncName, hModule)	\
	__##FuncName = (FuncName##Proc)::GetProcAddress(hModule, #FuncName);






// 由于类成员函数引起的一些特殊问题, 故不将重定向指向的新函数声明为类成员函数 
#define DECLARE_CREATE_API1(Name, ResultType, vType)\
	typedef ResultType (WINAPI *Name##Proc)(vType);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API2(Name, ResultType, vType1, vType2)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API3(Name, ResultType, vType1, vType2, vType3)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2, vType3);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API4(Name, ResultType, vType1, vType2, vType3, vType4)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2, vType3, vType4);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API5(Name, ResultType, vType1, vType2, vType3, vType4, vType5)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2, vType3, vType4, vType5);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API6(Name, ResultType, vType1, vType2, vType3, vType4, \
	vType5, vType6)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2, vType3, vType4, \
	vType5, vType6);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CREATE_API14(Name, ResultType, vType1, vType2, vType3, vType4, \
	vType5, vType6, vType7, vType8, vType9, vType10, vType11, vType12, vType13,\
	vType14)\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2, vType3, vType4, \
	vType5, vType6, vType7, vType8, vType9, vType10, vType11, vType12, vType13,\
	vType14);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;

#define DECLARE_DELETE_API1(Name, ResultType, vType1)	\
	typedef ResultType (WINAPI *Name##Proc)(vType1);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;

#define DECLARE_RELEASE_API2(Name, ResultType, vType1, vType2)	\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;

#define DECLARE_END_API2(Name, ResultType, vType1, vType2)	\
	typedef ResultType (WINAPI *Name##Proc)(vType1, vType2);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;


#define DECLARE_CLOSE_API1(Name, ResultType, vType1)	\
	typedef ResultType (WINAPI *Name##Proc)(vType1);\
	static Name##Proc __##Name;\
	HOOK_ITEM hItem_##Name;





// 使用union类型来逃避C++的类型转换检测, 获取类非静态成员函数地址
template<typename ToType, typename FromType>
DWORD GetMemberFuncAddr(ToType& addr,FromType f)
{
    union 
    {
		FromType _f;
		ToType   _t;
    }ut;
	
    ut._f = f;
	
    addr = ut._t;

	return (DWORD)addr;
}




class CGdiDetector{

	CGdiDetector(void);
	static CGdiDetector g_CGdiDetector;
	
public:
	~CGdiDetector(void);

	// BITMAP
	DECLARE_CREATE_API5(CreateBitmap, HBITMAP, int, int, UINT, UINT, CONST VOID*)
	DECLARE_CREATE_API1(CreateBitmapIndirect, HBITMAP, CONST BITMAP*)
	DECLARE_CREATE_API3(CreateCompatibleBitmap, HBITMAP, HDC, int, int)
	DECLARE_CREATE_API6(CreateDIBitmap, HBITMAP, HDC, CONST BITMAPINFOHEADER *, DWORD,
	CONST VOID *, CONST BITMAPINFO *, UINT)
	DECLARE_CREATE_API6(CreateDIBSection, HBITMAP, HDC, CONST BITMAPINFO*, UINT,
	VOID **, HANDLE, DWORD)
	DECLARE_CREATE_API3(CreateDiscardableBitmap, HBITMAP, HDC, int, int)
	DECLARE_CREATE_API2(LoadBitmapA, HBITMAP, HINSTANCE, LPSTR)
	DECLARE_CREATE_API2(LoadBitmapW, HBITMAP, HINSTANCE, LPWSTR)

	// ICON


	// BRUSH
	DECLARE_CREATE_API1(CreateBrushIndirect, HBRUSH, CONST LOGBRUSH*)
	DECLARE_CREATE_API2(CreateDIBPatternBrush, HBRUSH, HGLOBAL, UINT)
	DECLARE_CREATE_API2(CreateDIBPatternBrushPt, HBRUSH, CONST VOID*, UINT)
	DECLARE_CREATE_API2(CreateHatchBrush, HBRUSH, int, COLORREF)
	DECLARE_CREATE_API1(CreatePatternBrush, HBRUSH, HBITMAP)
	DECLARE_CREATE_API1(CreateSolidBrush, HBRUSH, COLORREF)


	// DEVICE CONTEXT
	DECLARE_CREATE_API4(CreateDCA, HDC, LPCSTR, LPCSTR, LPCSTR, CONST DEVMODE*)
	DECLARE_CREATE_API4(CreateDCW, HDC, LPCWSTR, LPCWSTR, LPCWSTR, CONST DEVMODE*)
	DECLARE_CREATE_API1(CreateCompatibleDC, HDC, HDC)
	DECLARE_CREATE_API1(GetDC, HDC, HWND)
	DECLARE_CREATE_API3(GetDCEx, HDC, HWND, HRGN, DWORD)
	DECLARE_CREATE_API1(GetWindowDC, HDC, HWND)
	DECLARE_CREATE_API4(CreateICA, HDC, LPCSTR, LPCSTR, LPCSTR, CONST DEVMODE*)
	DECLARE_CREATE_API4(CreateICW, HDC, LPCWSTR, LPCWSTR, LPCWSTR, CONST DEVMODE*)
	DECLARE_CREATE_API2(BeginPaint, HDC, HWND, LPPAINTSTRUCT)
	

	// FONT
	DECLARE_CREATE_API14(CreateFontA, HFONT, int, int, int, int, int, DWORD, DWORD,
	DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR)
	DECLARE_CREATE_API14(CreateFontW, HFONT, int, int, int, int, int, DWORD, DWORD,
	DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCWSTR)
	DECLARE_CREATE_API1(CreateFontIndirectA, HFONT, CONST LOGFONTA*)
	DECLARE_CREATE_API1(CreateFontIndirectW, HFONT, CONST LOGFONTW*)
	//DECLARE_CREATE_API1(CreateFontIndirectExA, HFONT, CONST ENUMLOGFONTEXDVA*)
	//DECLARE_CREATE_API1(CreateFontIndirectExW, HFONT, CONST ENUMLOGFONTEXDVW*)
	

	// PALETTE
	DECLARE_CREATE_API1(CreatePalette, HPALETTE, CONST LOGPALETTE*)
	DECLARE_CREATE_API1(CreateHalftonePalette, HPALETTE, HDC)


	// PEN
	DECLARE_CREATE_API3(CreatePen, HPEN, int, int, COLORREF)
	DECLARE_CREATE_API1(CreatePenIndirect, HPEN, CONST LOGPEN*)
	DECLARE_CREATE_API5(ExtCreatePen, HPEN, DWORD, DWORD, CONST LOGBRUSH*, DWORD, CONST DWORD*)
	

	// REGION
	DECLARE_CREATE_API4(CreateEllipticRgn, HRGN, int, int, int, int)
	DECLARE_CREATE_API1(CreateEllipticRgnIndirect, HRGN, CONST RECT*)
	DECLARE_CREATE_API3(CreatePolygonRgn, HRGN, CONST POINT*, int, int)
	DECLARE_CREATE_API4(CreatePolyPolygonRgn, HRGN, CONST POINT*, CONST INT*, int, int)
	DECLARE_CREATE_API4(CreateRectRgn, HRGN, int, int, int, int)
	DECLARE_CREATE_API1(CreateRectRgnIndirect, HRGN, CONST RECT*)
	DECLARE_CREATE_API6(CreateRoundRectRgn, HRGN, int, int, int, int, int, int)
	DECLARE_CREATE_API1(PathToRegion, HRGN, HDC)
	DECLARE_CREATE_API3(ExtCreateRegion, HRGN, CONST XFORM*, DWORD, CONST RGNDATA*)


	// RELEASE API
	DECLARE_RELEASE_API2(ReleaseDC, int, HWND, HDC)


	// DELETE API
	DECLARE_DELETE_API1(DeleteObject, BOOL, HGDIOBJ)
	DECLARE_DELETE_API1(DeleteDC, BOOL, HDC)
	DECLARE_DELETE_API1(DeleteMetaFile, BOOL, HMETAFILE)
	DECLARE_DELETE_API1(DeleteEnhMetaFile, BOOL, HENHMETAFILE)


	// END API
	DECLARE_END_API2(EndPaint, BOOL, HWND, CONST PAINTSTRUCT *)


	// CLOSE API
	DECLARE_CLOSE_API1(CloseMetaFile, HMETAFILE, HDC)
	DECLARE_CLOSE_API1(CloseEnhMetaFile, HENHMETAFILE, HDC)

};
