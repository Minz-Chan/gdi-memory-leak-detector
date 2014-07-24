#ifndef _REDIRECTEDAPIFUNC_H
#define _REDIRECTEDAPIFUNC_H

#include <windows.h>
#include "Struct_Define.h"
#include <string>
#include <vector>
#include <map>


using namespace std;



#define PARA_DECLARE_REGION \
	HANDLE_INFORAMTION handleInformation;\
	char sInfo[256];\
	char szPara[256];
	

// 获取函数调用栈
#define GET_CALLSTACK_TRACE(handleInfo) \
	StackWalker stackWalker;\
	stackWalker.GetCallstack(&handleInfo);




// 从MAP中删除句柄对应的信息
// #define REMOVE_HANDLE_INFORMATION(handle) \
// 	g_handleInfo.


#define TRACE_INFO_VIEW \
	::MessageBox(NULL,sInfo,"Message", MB_ICONINFORMATION);\
	string szCallStack;\
	std::vector<std::string>::iterator it;\
	for(it=handleInformation.callStack.callStack.begin()\
		; it != handleInformation.callStack.callStack.end()\
		; it++){\
		szCallStack+=*it;\
		szCallStack+="\n";\
	}\
	::MessageBox(NULL,szCallStack.c_str(),"CallStack Message", MB_ICONINFORMATION);


// 将结构体HANDLE_INFORMATION信息写入内存映射区域
BOOL WriteToMemory(PHANDLE_INFORMATION lpHandleInfo);

void GetParaInfo(string s, PHANDLE_INFORMATION pHandleInfo);

HBITMAP WINAPI _CreateBitmap(
					 int nWidth,         // bitmap width, in pixels
					 int nHeight,        // bitmap height, in pixels
					 UINT cPlanes,       // number of color planes used by device
					 UINT cBitsPerPel,   // number of bits required to identify a color
					 CONST VOID *lpvBits // pointer to array containing color data
					 );

HBITMAP WINAPI _CreateBitmapIndirect(
							 CONST BITMAP *lpbm    // pointer to the bitmap data
							 );

HBITMAP WINAPI _CreateCompatibleBitmap(
							   HDC hdc,        // handle to device context
							   int nWidth,     // width of bitmap, in pixels
							   int nHeight     // height of bitmap, in pixels
							   );

HBITMAP WINAPI _CreateDIBitmap(
					   HDC hdc,                  // handle to device context
					   CONST BITMAPINFOHEADER *lpbmih,  // pointer to bitmap size and
					   // format data
					   DWORD fdwInit,            // initialization flag
					   CONST VOID *lpbInit,      // pointer to initialization data
					   CONST BITMAPINFO *lpbmi,  // pointer to bitmap color-format data
					   UINT fuUsage              // color-data usage
					   );

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
						 );

HBITMAP WINAPI _CreateDiscardableBitmap(
								HDC hdc,     // handle to device context
								int nWidth,  // bitmap width
								int nHeight  // bitmap height
								);

HBITMAP WINAPI _LoadBitmapA(
				   HINSTANCE hInstance,  // handle to application instance
				   LPSTR lpBitmapName  // address of bitmap resource name
				   );

HBITMAP WINAPI _LoadBitmapW(
						   HINSTANCE hInstance,  // handle to application instance
						   LPWSTR lpBitmapName  // address of bitmap resource name
				   );


HBRUSH WINAPI _CreateBrushIndirect(
						   CONST LOGBRUSH *lplb   // pointer to structure describing brush
						   );


HBRUSH WINAPI _CreateDIBPatternBrush(
							 HGLOBAL hglbDIBPacked,  // handle to device-independent bitmap
							 UINT fuColorSpec        // color table data
							 );

HBRUSH WINAPI _CreateDIBPatternBrushPt(
							   CONST VOID *lpPackedDIB,  // pointer to structure and bitmap bits
							   UINT iUsage               // usage flags
							   );

HBRUSH WINAPI _CreateHatchBrush(
						int fnStyle,      // hatch style
						COLORREF clrref   // color value
						);


HBRUSH WINAPI _CreatePatternBrush(
						  HBITMAP hbmp   // handle to bitmap
						  );

HBRUSH WINAPI _CreateSolidBrush(
						COLORREF crColor   // brush color value
						);






HDC WINAPI _CreateDCA(
			 LPCSTR lpszDriver,  // pointer to string specifying driver name
			 LPCSTR lpszDevice,  // pointer to string specifying device name
			 LPCSTR lpszOutput,  // do not use; set to NULL
			 CONST DEVMODE *lpInitData 
			 // pointer to optional printer data
			 );

HDC WINAPI _CreateDCW(
					 LPCWSTR lpszDriver,  // pointer to string specifying driver name
					 LPCWSTR lpszDevice,  // pointer to string specifying device name
					 LPCWSTR lpszOutput,  // do not use; set to NULL
					 CONST DEVMODE *lpInitData 
					 // pointer to optional printer data
			 );
HDC WINAPI _CreateCompatibleDC(
					   HDC hdc   // handle to the device context
					   );

HDC WINAPI _GetDC(HWND hwnd);

HDC WINAPI _GetDCEx(
			HWND hWnd,      // handle of window
			HRGN hrgnClip,  // handle of clip region
			DWORD flags     // device-context creation flags
			);

HDC WINAPI _GetWindowDC(
				HWND hWnd   // handle of window
				);

HDC WINAPI _CreateICA(
			 LPCSTR lpszDriver,  // pointer to string specifying driver name
			 LPCSTR lpszDevice,  // pointer to string specifying device name
			 LPCSTR lpszOutput,  // pointer to string specifying port or file name
			 CONST DEVMODE *lpdvmInit 
			 // pointer to optional initialization data
			 );

HDC WINAPI _CreateICW(
					 LPCWSTR lpszDriver,  // pointer to string specifying driver name
					 LPCWSTR lpszDevice,  // pointer to string specifying device name
					 LPCWSTR lpszOutput,  // pointer to string specifying port or file name
					 CONST DEVMODE *lpdvmInit 
					 // pointer to optional initialization data
			 );

HDC WINAPI _BeginPaint(
			   HWND hwnd,  // handle to window
			   LPPAINTSTRUCT lpPaint
			   // pointer to structure for paint information
			   );



//HDC WINAPI _GetDC(HWND hwnd);

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
				 );


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
				 );

HFONT WINAPI _CreateFontIndirectA(
						 CONST LOGFONTA *lplf   // pointer to logical font structure
						 );

HFONT WINAPI _CreateFontIndirectW(
								 CONST LOGFONTW *lplf   // pointer to logical font structure
						 );

HPALETTE WINAPI _CreatePalette(
					   CONST LOGPALETTE *lplgpl   // pointer to logical color palette
					   );


HPALETTE WINAPI _CreateHalftonePalette(
							   HDC hdc   // handle to device context
							   );

HPEN WINAPI _CreatePen(
			   int fnPenStyle,    // pen style
			   int nWidth,        // pen width
			   COLORREF crColor   // pen color
			   );

HPEN WINAPI _CreatePenIndirect(
					   CONST LOGPEN *lplgpn   // pointer to LOGPEN structure
					   );

HPEN WINAPI _ExtCreatePen(
				  DWORD dwPenStyle,      // pen style
				  DWORD dwWidth,         // pen width
				  CONST LOGBRUSH *lplb,  // pointer to structure for brush attributes
				  DWORD dwStyleCount,    // length of array containing custom style bits
				  CONST DWORD *lpStyle   // optional array of custom style bits
				  );













HRGN WINAPI _CreateEllipticRgn(
					   int nLeftRect,   // x-coord of the upper-left corner of the bounding rectangle
					   int nTopRect,    // y-coord of the upper-left corner of the bounding rectangle
					   int nRightRect,  // x-coord of the lower-right corner of the bounding rectangle
					   int nBottomRect  // y-coord of the lower-right corner of the bounding rectangle
					   );

HRGN WINAPI _CreateEllipticRgnIndirect(
							   CONST RECT *lprc   // pointer to bounding rectangle
							   );

HRGN WINAPI _CreatePolygonRgn(
					  CONST POINT *lppt,  // pointer to array of points
					  int cPoints,        // number of points in array
					  int fnPolyFillMode  // polygon-filling mode
					  );

HRGN WINAPI _CreatePolyPolygonRgn(
						  CONST POINT *lppt,        // pointer to array of points
						  CONST INT *lpPolyCounts,  // pointer to count of vertices
						  int nCount,               // number of integers in array
						  int fnPolyFillMode        // polygon fill mode
						  );

HRGN WINAPI _CreateRectRgn(
				   int nLeftRect,   // x-coordinate of region's upper-left corner
				   int nTopRect,    // y-coordinate of region's upper-left corner
				   int nRightRect,  // x-coordinate of region's lower-right corner
				   int nBottomRect  // y-coordinate of region's lower-right corner
				   );


HRGN WINAPI _CreateRectRgnIndirect(
						   CONST RECT *lprc   // pointer to the rectangle
						   );


HRGN WINAPI _CreateRoundRectRgn(
						int nLeftRect,      // x-coordinate of the region's upper-left corner
						int nTopRect,       // y-coordinate of the region's upper-left corner
						int nRightRect,     // x-coordinate of the region's lower-right corner
						int nBottomRect,    // y-coordinate of the region's lower-right corner
						int nWidthEllipse,  // height of ellipse for rounded corners
						int nHeightEllipse  // width of ellipse for rounded corners
						);

HRGN WINAPI _PathToRegion(
				  HDC hdc   // handle to device context
				  );

HRGN WINAPI _ExtCreateRegion(
					 CONST XFORM *lpXform,     // pointer to transformation data
					 DWORD nCount,             // size of structure containing region data
					 CONST RGNDATA *lpRgnData  // pointer to region data
					 );
 








int WINAPI _ReleaseDC(
			  HWND hWnd,  // handle to window
			  HDC hDC     // handle to device context
			  );
 


BOOL WINAPI _DeleteObject(
				  HGDIOBJ hObject   // handle to graphic object
				  );

BOOL WINAPI _DeleteDC(
			  HDC hdc   // handle to device context
			  );
 
BOOL WINAPI _DeleteMetaFile(
					HMETAFILE hmf   // handle to Windows-format metafile
					);


BOOL WINAPI _DeleteEnhMetaFile(
					   HENHMETAFILE hemf   // handle to an enhanced metafile
					   );


 

BOOL WINAPI _EndPaint(
			  HWND hWnd,  // handle to window
			  CONST PAINTSTRUCT *lpPaint 
              // pointer to structure for paint data
			  );


HMETAFILE WINAPI _CloseMetaFile(
						HDC hdc   // handle to metafile device context
						);


HENHMETAFILE WINAPI _CloseEnhMetaFile(
							  HDC hdc   // handle to an enhanced-metafile device context
							  );
 








#endif // _REDIRECTEDAPIFUNC_H
