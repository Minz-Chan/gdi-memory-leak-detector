// Gdi.h: interface for the CGdi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDI_H__7F316BC5_4FC2_47FE_AE36_2452E39746C3__INCLUDED_)
#define AFX_GDI_H__7F316BC5_4FC2_47FE_AE36_2452E39746C3__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<Tlhelp32.h>
#include "NtQuery.h"
#include "psapi.h"
#include <queue>
#include <vector>
#include <string>

using namespace std;


class Gdi  
{
///////////////////////////////////////////////////////////////
//构造函数
public:
	Gdi();
	virtual ~Gdi();

////////////////////////////////////////////////////////////////
//更新数据  （本程序每秒调用Updata一次   并由Updata调用各个 更新函数 来更新数据)
public:
	void Updata(DWORD ProcessID,HANDLE Process, BOOL OpenProcess); //更新数据
private:
	void UpdataGdi(HANDLE process);  //更新GDI
	void UpdataCPU(DWORD ProcessID,HANDLE Process, BOOL OpenProcess); //更新CPU 
	void UpdataGdiType(DWORD ProcessID,HANDLE Handle); //更新GDI类别
	void UpdataThread(DWORD ProcessID); //更新线程队列
///////////////////////////////////////////////////////////////
//画图 
public:
	void SetDraw(CDC *pdc, int cursel,  bool bDraw = true); // true为画图 false为改变cursel值
private:
	void SetBitmap(); //此函数设置 曲线图大小 位置等
	Draw(int x, int y, int weight, int height, bool choose=0); //此函数对曲线图相关条件进行判断 
	void DrawGdiTypeRect(int j,int x,int y, double xArgument, double yArgument, CDC *pDC, int num);
	void DrawGdiType(CDC *MemDC,int weight,int height);
	void DrawGDI(CDC *MemDC,int weight,int height);
	void DrawThreadNum(CDC *MemDC,int weight,int height);
	void DrawCpu2(CDC *pDC,int weight,int height);
	void DrawCpu(CDC *pDC,int weight,int height);
	void DrawMemory(CDC *pDC, int weight, int height);

////////////////////////////////////////////////////////////////
// Get Set函数

public:
	DWORD GetProcessMem();
	bool GetFindProcess();
	CString GetSP(TCHAR  sp[128]);
	vector<THREADINFO> GetThreadInfo();
	DWORD GetGdiNum();
	DWORD GetCursel();
	vector<GDITYPEINFO> GetGDITypeInfo();
	vector<GDIINFO> GetGDIInfo();
	CString GetChineseGdiType(DWORD i);
	CString GetEnglishGdiType(DWORD i);
	int GetMedian(int i);
	queue<DWORD> GetProcessQueueWokingSize();
	DWORD GetProcessWokingSize();
	void GetGdiTypeNum(int (&type)[15]);
	DWORD GetWin32GDIType(BYTE InternalType);
	queue<int> GetQueueGdi();
	DWORD GetPageFileSize();
	DWORD GetPagePath();
	DWORD GetAvailVirtual();
	queue<int> GetQueueThreadNum(); 
	DWORD GetVirtualMem();
	CString GetVersion();
	DWORD GetAvalMem();
	DWORD GetPhyMem();
	int GetCpuUsedAge();
	queue<int> GetQueueCpuUsedAge();
	int GetCpuNumber();

public:
	void SetCursel(int cursel);
	void SetFirstTime(bool ft);
	double GetArgument(DWORD max);
	void SetFindProcess(BOOL i);
	void SetQueueCpuUsedAge(int age); //更新CPU使用率队列
	BOOL IsProcessWorking(); //判断进程是否关闭。（未写）


protected:
	//获取API
	typedef   LONG   (WINAPI   *PROCNTQSI)(UINT,PVOID,ULONG,PULONG); 
	typedef   DWORD  (WINAPI  *GETGUIRESOURCES) (HANDLE,DWORD);
	typedef unsigned (CALLBACK * Proc0) (void);          //GDI
	
    PROCNTQSI   NtQuerySystemInformation; //API
	PROCNTQSI   ZwQuerySystemInformation; //API
	GETGUIRESOURCES      GetGuiResources;//API
	Proc0                 GdiQueryTable;//API

private:
//////////////////////////////////////////////////////////////
//  变量

	vector<THREADINFO> vThreadInfo;
	int iGdiInfoNum;
	BOOL bStart;// 判断是否第一次
	/////////////////////////////////////////
    //CPU
	SYSTEM_PERFORMANCE_INFORMATION   SysPerfInfo;  //系统性能信息
    SYSTEM_TIME_INFORMATION          SysTimeInfo;  //系统时间信息
    SYSTEM_BASIC_INFORMATION         SysBaseInfo;  //系统基本信息
	PSYSTEM_HANDLE_INFORMATION        pSysHandleInfo;//句柄
	vector <SYSTEM_HANDLE_INFORMATION>  vSysHandleInfo;//
	PSYSTEM_PROCESSES                pSp;//第5号
    double                           dbIdleTime;   //空闲时间
    double                           dbSystemTime; //系统时间
    LARGE_INTEGER                    liOldIdleTime;   //旧空闲时间
    LARGE_INTEGER                    liOldSystemTime; //旧系统时间
	queue<int> qiCpuUseAge;   //存放单位时间CPU使用情况
	//////////////////////////////////////////
	//内存 
	LPMEMORYSTATUS pmst;
	PPROCESS_MEMORY_COUNTERS ppmc;   //进程内存
	queue<DWORD> qdWorkingSize;   //进程内存队列 
	DWORD dwWorkingSize;  //此刻进程使用内存
	DWORD m_NowMemory;//此刻内存
	//////////////////////////////////////////
	//系统版本
	OSVERSIONINFO osvi; 
	//////////////////////////////////////////
	//线程
	queue<int> qiThreadNum;
	DWORD m_NowThread;//此刻线程
	
	//////////////////////////////////////////
	//GDI
	queue<int> qiGdi;
	int GdiType[15]; // 1-14 进程各个GDI类型的GDI个数
	GDITableCell * pGDITable;  
	vector<GDIINFO>  sss	; //句柄信息 
	vector<GDIINFO>  vGdiInfo; //句柄信息 
	vector<GDITYPEINFO> vGdiTypeInfo; //句柄类型信息
  
	DWORD m_NowGdi;   //此刻GDI
	////////////////////////////////////////
	//画图参数
	int m_Cursel; //哪张图
	bool FindProcess; //是否找到进程
	bool FirstTime; //是否第1秒
	CDC *pDC; //此时屏幕DC
	DWORD m_rgb[15];//GDI对象类型表 矩形颜色
	int LineNumGDI;    // 画竖线用的参数
	int LineNumCPU; //CPU竖线
	int LineNumThread; //线程竖线
	int LineNumMemory; //句柄竖线参数
	///////////////////////////////////////

};


/*
#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13
*/
//pen brush dc metadc pal font bitmap region metafile memdc extpen enhmetadc enhmetafile other

#endif // !defined(AFX_GDI_H__7F316BC5_4FC2_47FE_AE36_2452E39746C3__INCLUDED_)
