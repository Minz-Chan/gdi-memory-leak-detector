// Gdi.cpp: implementation of the CGdi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gdileakdetector.h"
#include "Gdi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  MAX_GDI_HANDLE  0x4000

Gdi g_Gdi;// GDI类全局变量


Gdi::Gdi() //构造函数
{
	liOldIdleTime.QuadPart=0;
	liOldSystemTime.QuadPart=0;

	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress       
	          ( GetModuleHandle("ntdll"), "NtQuerySystemInformation" );
	ZwQuerySystemInformation = (PROCNTQSI)GetProcAddress               
	          ( GetModuleHandle("ntdll"), "ZwQuerySystemInformation" );
						
	HMODULE hMod = ::LoadLibrary("USER32.dll"); 
	GetGuiResources = (GETGUIRESOURCES)::GetProcAddress(hMod, "GetGuiResources"); 

	HMODULE hGDI = GetModuleHandle(_T("GDI32.dll"));
    GdiQueryTable = (Proc0) GetProcAddress(hGDI, "GdiQueryTable");
	
	pmst=new MEMORYSTATUS;
	ppmc=new PROCESS_MEMORY_COUNTERS;
	ZeroMemory( &osvi, sizeof( osvi ) ); 
	bStart=true;
	FirstTime=true;

	m_rgb[0]=RGB(0XFF,0X00,0X00);
	m_rgb[1]=RGB(0x66,0xFF,0x00);
	m_rgb[2]=RGB(0x8c,0xFB,0xFB);
	m_rgb[3]=RGB(251 , 23 ,146 );
	m_rgb[4]=RGB(0xFF,0xFF,0x44);
	m_rgb[5]=RGB(0x66,0xFF,0x00);
	m_rgb[6]=RGB(0x7C,0xB8,0x7C);
	m_rgb[7]=RGB(0xBB,0x89,0xB1);
	m_rgb[8]=RGB(0x3E,0xF0,0x7D);
	m_rgb[9]=RGB(0x56,0x6D,0xCC);


	LineNumGDI    =0;    // 画竖线用的参数
	LineNumCPU   =0; //CPU竖线
	LineNumThread =0; //线程竖线
	LineNumMemory =0; //句柄竖线参数

}

Gdi::~Gdi() //析构函数
{
	delete pmst;
}

void Gdi::Updata(DWORD ProcessID,HANDLE Process, BOOL OpenProcess)
{
	////////////////////////////////////////////////////////////////////////////////
	//更新CPU
	UpdataCPU(ProcessID, Process, OpenProcess);



	////////////////////////////////////////////////////////////////////////////////
	//更新内存
	GlobalMemoryStatus(pmst);
	////////////////////////////////////////////////////////////////////////////////
	//获取系统版本


	///////////////////////////////////////////////////////////////////////////////
	//更新5号功能（虚拟内存等）
    ULONG returnSize=0;   
	ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, NULL, 0, &returnSize);

	unsigned char *buf = new unsigned char[returnSize];  
	ZwQuerySystemInformation( SystemProcessesAndThreadsInformation, 
		(PVOID)buf, returnSize, NULL); 
	pSp = (PSYSTEM_PROCESSES)buf;

	////////////////////////////////////////////////////////////////////////////////
	//更新线程
	if(OpenProcess)
	{
		UpdataThread(ProcessID);
	}
	////////////////////////////////////////////////////////////////////////////////
	//更新Gdi数
	if(OpenProcess)
	{
		UpdataGdi(Process);
	}
	////////////////////////////////////////////////////////////////////////////////
	//更新GDI类别
	if(OpenProcess)
	{
		UpdataGdiType(ProcessID,Process);
	}
	////////////////////////////////////////////////////////////////////////////////
	//更新进程使用内存
	if(OpenProcess)
	{
		GetProcessMemoryInfo(Process,ppmc, sizeof(PROCESS_MEMORY_COUNTERS));

		dwWorkingSize = ppmc->WorkingSetSize / 1024; //更新此刻内存使用情况
		if(qdWorkingSize.size()>=60)
			qdWorkingSize.pop();
		qdWorkingSize.push( ppmc->WorkingSetSize / 1024 );
	}
	/////////////////////////////////////////////////////////
	//画图竖线参数更新
	if(OpenProcess)
	{
		LineNumGDI--;    // 画竖线用的参数
		LineNumThread--; //线程竖线
		LineNumMemory--; //句柄竖线参数
		if(LineNumGDI == -3)
		{
			LineNumGDI = 0 ;
		}
		if(LineNumThread == -3)
		{
			LineNumThread = 0;
		}
		if(LineNumMemory == -3)
		{
			LineNumMemory = 0;
		}
		
	}
	LineNumCPU--; //CPU竖线
	if(LineNumCPU == -3)
	{
		LineNumCPU = 0;
	}

}

int Gdi::GetCpuNumber()        //获得CPU个数
{
	return SysBaseInfo.bKeNumberProcessors;
}


void Gdi::SetQueueCpuUsedAge(int age) //添加到CPU使用率队列
{
	if(qiCpuUseAge.size()>=60)
		qiCpuUseAge.pop();
	qiCpuUseAge.push(age);
}

queue<int> Gdi::GetQueueCpuUsedAge() //获得CPU使用率队列
{
	return qiCpuUseAge;
}

int Gdi::GetCpuUsedAge()   //获得CPU使用率
{
	return dbIdleTime;
}

DWORD Gdi::GetPhyMem()
{
	return pmst->dwAvailVirtual / 1024;
}

DWORD Gdi::GetAvalMem()
{
	return pmst->dwAvailPhys / 1024;
}

CString Gdi::GetVersion()
{
	osvi.dwOSVersionInfoSize = sizeof( osvi ); // 实例所占字节数
	if (GetVersionEx(&osvi))   
	{   
		CString str;	
		str.Format("%ld.%ld.%ld",osvi.dwMajorVersion , osvi.dwMinorVersion ,osvi.dwBuildNumber);
		str += GetSP(osvi.szCSDVersion);
		return str;

	} 
	return "0";
}

DWORD Gdi::GetVirtualMem() //虚拟内存总量
{
	return pmst->dwTotalVirtual / 1024; //
}



queue<int> Gdi::GetQueueThreadNum() //返回线程数队列
{
	return qiThreadNum;
}

void Gdi::UpdataThread(DWORD ProcessID)
{

	HANDLE   hProcessSnap;  //句柄
	PROCESSENTRY32   pe32;  
	
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS,0 ); //创建快照 
	
	if( hProcessSnap == INVALID_HANDLE_VALUE ) //INVALID_HANDLE_VALUE 无效句柄
		//	break;
		
		//使用前先设置结构大小
		pe32.dwSize = sizeof(PROCESSENTRY32); 
	//获取信息
	BOOL bMore = Process32First( hProcessSnap,&pe32 ) ; 
		//	break; 
	while(bMore)
	{
		if(pe32.th32ProcessID == ProcessID)
		{
			if(qiThreadNum.size()>=60)
				qiThreadNum.pop();
				qiThreadNum.push(pe32.cntThreads);	
			break;
		}
		bMore = Process32Next( hProcessSnap,&pe32 ) ;
	}
	
	CloseHandle( hProcessSnap ); 
	CloseHandle( hProcessSnap );           //   快照项必须清空
	//-----------------------------------------------------------
	HANDLE   h   =   CreateToolhelp32Snapshot   (TH32CS_SNAPTHREAD,   0);  //创建线程快照
	THREADENTRY32   te; 
	te.dwSize   =   sizeof   (te); 

	BOOL b = Thread32First  (h,   &te) ; 
		//	break;
	vector <THREADINFO> threadinfo ;

	while(b)
	{
		if(te.th32OwnerProcessID == ProcessID)
		{
			
			//UpdateThreadInfo (te.th32ThreadID)) ;
			
			THREADINFO info;
			info.id = te.th32ThreadID;
			info.pri = te.tpBasePri + te.tpDeltaPri;
			threadinfo.push_back ( info );
		}


		b = (Thread32Next   (h,   &te) );
	}
	vThreadInfo = threadinfo;
}

DWORD Gdi::GetAvailVirtual() //可用虚拟内存
{
	return  pmst->dwAvailVirtual / 1024 ;
}

DWORD Gdi::GetPagePath() //可用页文件
{
	return pmst->dwAvailPageFile / 1024;
}

DWORD Gdi::GetPageFileSize() //分页文件空间
{
	return pmst->dwTotalPageFile / 1024;
}

void Gdi::UpdataGdi(HANDLE hProcess)
{
	if(qiGdi.size()>=60)
		qiGdi.pop();
	qiGdi.push(GetGuiResources(hProcess,0));
}

queue<int> Gdi::GetQueueGdi()//获取GDI队列
{
	return qiGdi;
}

void Gdi::UpdataGdiType(DWORD ProcessID, HANDLE Handle) 
{

	for(int i = 0; i<15; i++)
	{
		GdiType[i] = 0;
	}

	pGDITable = (GDITableCell *) GdiQueryTable();   //获取 GDI句柄表

	WORD index;
	vGdiInfo.clear();

	for(index = 0; index < MAX_GDI_HANDLE; index++)
	{
		if( pGDITable[index].nProcess == ProcessID   ) //判断是否属于此进程 
		{
			iGdiInfoNum++;	
			GDIINFO   gdiinfo;

			//获取GDI详细信息
			gdiinfo.handle  =  MAKELONG(ProcessID, pGDITable[index].nUpper); //句柄
			gdiinfo.type    =  GetChineseGdiType(  GetWin32GDIType( (LOBYTE(pGDITable[index].nUpper) & 0x7F) )  );  //类型
			gdiinfo.pKernel =  pGDITable[index].pKernel; //内核空间
			
			vGdiInfo.push_back( gdiinfo ); //添加

		}
	}

	vGdiTypeInfo.clear();
	for(index = 0; index <=14; index++)
	{
		GDITYPEINFO gditypeinfo;
		if(GdiType[index] != 0)
		{
			gditypeinfo.type = GetEnglishGdiType( index );
			gditypeinfo.num = GdiType[index];
			gditypeinfo.Expand = GetChineseGdiType( index );
			vGdiTypeInfo.push_back(gditypeinfo);
		}
	}

}


/*	Color space
OBJ_BITMAP	Bitmap
OBJ_DC	Device context
OBJ_ENHMETADC	Enhanced metafile DC
OBJ_ENHMETAFILE	Enhanced metafile
OBJ_EXTPEN	Extended pen
OBJ_FONT	Font
OBJ_MEMDC	Memory DC
OBJ_METAFILE	Metafile
OBJ_METADC	Metafile DC
OBJ_PAL	Palette
OBJ_PEN	Pen
OBJ_REGION	Region*/


// 0 1 2 3 5 6 7 8 11 13 
//pen brush dc  pal font bitmap region extpen  enhmetafile other
DWORD Gdi::GetWin32GDIType(BYTE InternalType)
{
   switch(InternalType)
   {
      case 1:
		 GdiType[3]++;
         return(OBJ_DC); 
      break;

      case 4:
		  GdiType[8]++;
         return(OBJ_REGION);
      break;

      case 5:
		 GdiType[7]++;
         return(OBJ_BITMAP);
      break;

      case 8:
		  GdiType[5]++;
         return(OBJ_PAL);
      break;

      case 10:
		  GdiType[6]++;
         return(OBJ_FONT);
      break;

      case 16:
		  GdiType[2]++;
         return(OBJ_BRUSH);
      break;

      case 33:
		 GdiType[13]++;
         return(OBJ_ENHMETAFILE);
      break;

      case 48:
		  GdiType[1]++;
         return(OBJ_PEN);
      break;

      case 80:
		  GdiType[11]++;
         return(OBJ_EXTPEN);
      break;

      default:
		 GdiType[0]++; //其它GDI
         return(0);
      break;
   }
}

void Gdi::GetGdiTypeNum(int (&type)[15]) //获取GDI类别数组
{
	for(int i=0;i<15;i++)
		type[i] = GdiType[i];
}

DWORD Gdi::GetProcessWokingSize() //返回进程内存使用情况
{
	return ppmc->WorkingSetSize / 1024;
}

queue<DWORD> Gdi::GetProcessQueueWokingSize()//返回进程内存使用情况 队列
{
	return qdWorkingSize;
}


//判断进程是否关闭。(未写)
BOOL Gdi::IsProcessWorking()
{
	return true;
}



void Gdi::SetBitmap()
{
	//---------------------------------------
	int x , y ; //右上角位置
	int weight, height;//图片大小
	//---------------------------------------
	
	switch(m_Cursel)
	{
	case 1:	//CPU
		if(!FirstTime)//第一次调用
		{
			//画CPU小图
			weight = 100, height = 130 ; //图形框架大小
			
			x = 80, y = 50;// 图形右上角位置
	
			Draw(x,y,weight,height,0);
			/////////////////////////////////////////////
			//花CPU大图
			weight = 420, height = 130 ; //图形大小
			x = 230, y = 50;
	
			Draw(x,y,weight,height,1);
		}
		break;  
	case 2:	//内存
		if(FindProcess)
		{
			weight = 550, height = 130 ; //图形大小 
			x = 100, y = 50;
			Draw(x,y,weight,height);
		}
		break;
	case 3:	//GDI对象总数
		if(FindProcess)
		{
			weight = 550, height = 130 ; //图形大小 
			x = 100, y = 50;
	
			Draw(x,y,weight,height);
		}
		break;
	case 4:	//线程数实时监测

		if(FindProcess)
		{
			weight = 550, height = 130 ; //图形大小 
			x = 100, y = 50;
	
			Draw(x,y,weight,height);
		}
		break;
	case 5:	//GDI对象分类统计实时监测图

		if(FindProcess)
		{
			weight = 600, height = 170 ; //图形大小 
			x = 70, y = 40;

			Draw(x,y,weight,height);
      	}
		break;
	}
}

void Gdi::SetDraw(CDC *pdc, int cursel, bool bDraw ) //bDraw = true为画图 bDraw = false为改变cursel值
{
	m_Cursel = cursel; //改变当前属性页值
	pDC = pdc;  //DC

	if(bDraw == true) //画图
	{
		SetBitmap();
	}
}

void Gdi::DrawMemory(CDC *pDC, int weight, int height)
{
	CRect rectangle; 
	int	 weight0 = 240, height0 = 90 ; // 基数
	double xArgument = weight/1.0 / weight0, 
		yArgument = height/1.0 / height0;
	
	int x,y; //参数
	x=0;
	y=0;
	
	//画背景
	CBrush newbrush(RGB(0xf0,0xff,0xff));
	CBrush *oldbrush;
	oldbrush=pDC->SelectObject(&newbrush); 
	pDC->Rectangle(CRect(x+0,y+0,x+weight,y+height));
	pDC->SelectObject(oldbrush); 
	
	int k;
	for( k=1 ; k<height-1 ; k++)
	{
		rectangle.SetRect(1, k , weight-1, k+1);
		pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
	}
	CPen gridpen(PS_SOLID,1,RGB(0,128,64));//笔 
	CPen* oldpen; 
	oldpen=pDC->SelectObject(&gridpen); 
	
	//画横线
	int i;
	for(i=0;i<=9;i++)
	{
		pDC->MoveTo(x + 0 * xArgument , y +  9*yArgument*i );
		pDC->LineTo(x + weight * xArgument , y + 9*yArgument*i );
	}
	//画竖线
	for(i=weight0/12; i>0; i--)
	{
		pDC->MoveTo(x+12*i*xArgument+LineNumMemory*4*xArgument+xArgument, y+0*yArgument);
		pDC->LineTo(x+12*i*xArgument+LineNumMemory*4*xArgument+xArgument , y+height0*yArgument);
	}


	pDC->SelectObject(&oldpen);
	
	//取最大值
	queue<DWORD> qiMemory2=GetProcessQueueWokingSize();
	
	queue<DWORD> qiMemory = qiMemory2;
	
	
	DWORD max = 0;
	for(i=0;qiMemory2.size();i++)
	{
		if(max < qiMemory2.front())
		{
			max = qiMemory2.front();
		}
		qiMemory2.pop();
	}
	
	double argument=GetArgument(max);
	//画动态内存数据线
	CPoint oldpoint; //前一点 内存
	CPoint newpoint;//当前 内存
	
	CPen newpen(PS_SOLID,1,RGB(254,0,0));//笔 
	oldpen=pDC->SelectObject(&newpen); 
	
	int n;
	n=243-qiMemory.size()*4;
	
	for(i=0;qiMemory.size();i++)
	{
		
		newpoint.x = x+(n+i*4)*xArgument;
		m_NowMemory = qiMemory.front();
		newpoint.y =  y+( 90-(int)(qiMemory.front() * 0.9 * argument ) ) * yArgument  ;
		
		pDC->MoveTo( newpoint.x,newpoint.y );
		pDC->LineTo(oldpoint.x,oldpoint.y);
		
		oldpoint=newpoint;
		
		qiMemory.pop();
	}
	pDC->SelectObject(oldpen);
	
	
	newbrush.DeleteObject();
	newpen.DeleteObject();
	gridpen.DeleteObject();
}
               // 右上角起始点        //大小    //CPU图使用参数
Gdi::Draw(int x, int y, int weight, int height, bool choose)
{
	CDC MemDC;
	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC, weight , height );  //创立一个合适大小的背景

	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(&BmpMem); //选入内存DC


	switch(m_Cursel)
	{
		case 1:	//CPU

			//画CPU
			if(!FirstTime)
			{
				if(choose == 0 )
				{
					DrawCpu(&MemDC,weight,height);
				}
				else
				{
					DrawCpu2(&MemDC,weight,height);
				}
			}
			break;

		case 2:	//内存
			if(FindProcess)
			{
				DrawMemory(&MemDC,weight,height);
			}
			break;
		case 3:	//GDI对象总数

			if(FindProcess)
			{
				DrawGDI(&MemDC,weight,height);
			}
			
			break;
		case 4:	//线程数实时监测
			if(FindProcess)
			{
				DrawThreadNum(&MemDC,weight,height);
			}
			break;
			
		case 5:	//GDI对象分类统计实时监测图

			if(FindProcess)
			{
				DrawGdiType(&MemDC,weight,height);
			}
	}
	pDC->BitBlt(x, y, weight , height , &MemDC, 
		        0, 0 , SRCCOPY);
}

void Gdi::SetFindProcess(BOOL i) //设置是否找到进程
{
	FindProcess = true;
}

double Gdi::GetArgument(DWORD max)
{
		double argument;
		if(max <= 10 )
		{
			argument = 10;
		}
		else if(max <= 20)
		{
			argument = 5;
		}
		else if(max <= 30)
		{
			argument = 3.3;
		}
		else if(max <= 50)
		{
			argument = 2;
		}
		else if(max <= 100)
		{
			argument = 1;
		}
		else if(max<= 200)
		{
			argument = 0.5;
		}
		else if(max <= 500)
		{
			argument = 0.2;
		}
		else if(max <= 1000)
		{
			argument = 0.1;
		}
		else if(max <= 5000)
		{
			argument = 0.02;
		}
		else if(max <= 10000)
		{
			argument = 0.01;
		}
		else if(max <= 20000)
		{
			argument = 0.005;
		}
		else if(max <= 50000)
		{
			argument = 0.002;
		}
		else if(max <= 100000)
		{
			argument = 0.001;
		}
		else if(max <= 500000)
		{
			argument = 0.0002;
		}
		else if(max <= 1000000)
		{
			argument = 0.0001;
		}

		return argument;
}

void Gdi::DrawCpu(CDC *pDC,int weight,int height)
{

		CRect rectangle; 

		int	 weight0 = 75, height0 = 90 ; // 基数

		double xArgument = weight/1.0 / weight0, 
			   yArgument = height/1.0 / height0;

		int x,y;//参数
		x=0;
		y=0;

		//画背景

		CBrush newbrush(RGB(0xf0,0xff,0xff));
		CBrush *oldbrush;
		oldbrush=pDC->SelectObject(&newbrush); 
		pDC->Rectangle(CRect(x,y,x+weight,y+height));
		pDC->SelectObject(oldbrush); 

		int k;
		for( k=1 ; k<height-1 ; k++)
		{
			rectangle.SetRect(1, k , weight-1, k+1);
			pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
		}
		
		//画浅色背景
		CBrush bkagebrush(RGB(0xc1,0xff,0xc1));
		oldbrush=pDC->SelectObject(&bkagebrush);
		pDC->Rectangle(CRect(x+5*xArgument,y+5*yArgument,x+70*xArgument,y+70*yArgument));          //
		pDC->SelectObject(oldbrush);
		

		//画CPU使用率
		CBrush cpuagebrush(RGB(0xad,0xd8,0xa6));
		
		oldbrush=pDC->SelectObject(&cpuagebrush);
		pDC->Rectangle(CRect(x+5*xArgument,y+(70-GetCpuUsedAge()*0.65)*yArgument,x+70*xArgument,y+70*yArgument));
		pDC->SelectObject(oldbrush);
	    

		//写字 -- CPU使用率
		CPen gridpen(PS_SOLID,1,RGB(0,0,0));//笔 
		CPen* oldpen; 
		oldpen=pDC->SelectObject(&gridpen); 
    	pDC->SetTextColor(RGB(0x87,0xCE,0xE8));
		pDC->SetBkMode(TRANSPARENT);
		CString str;
		str.Format("%d ",GetCpuUsedAge());
		str+=" %";
		pDC->TextOut(x+25*xArgument,y+74*yArgument,str);



		newbrush.DeleteObject();
		bkagebrush.DeleteObject();
		cpuagebrush.DeleteObject();
		gridpen.DeleteObject();
}

void Gdi::DrawCpu2(CDC *pDC,int weight,int height)
{
		CRect rectangle; 

		int	 weight0 = 240, height0 = 90 ; // 基数
		double xArgument = weight/1.0 / weight0, 
			   yArgument = height/1.0 / height0;
		
		int x,y; //参数
		x=0;
		y=0;

		//画背景
		CBrush newbrush(RGB(0xf0,0xff,0xff));
		CBrush *oldbrush;
		oldbrush=pDC->SelectObject(&newbrush); 
		pDC->Rectangle(CRect(x,y,x+weight,y+height));
		pDC->SelectObject(oldbrush); 

		int k;
		for( k=1 ; k<height-1 ; k++)
		{
			rectangle.SetRect(1, k , weight-1, k+1);
			pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
		}
		
		CPen gridpen(PS_SOLID,1,RGB(0,128,64));//笔 
		CPen* oldpen; 
		oldpen=pDC->SelectObject(&gridpen); 
		
		//画横线
		int i;
		for(i=0;i<=9;i++)
		{
			pDC->MoveTo(x+0*xArgument,y+9*i*yArgument);
			pDC->LineTo(x+240*xArgument,y+9*i*yArgument);
		}
		pDC->SelectObject(&oldpen);

			//画竖线
		for(i=weight0/12; i>0; i--)
		{
			pDC->MoveTo(x+12*i*xArgument+LineNumCPU*4*xArgument +xArgument, y+0*yArgument);
			pDC->LineTo(x+12*i*xArgument+LineNumCPU*4*xArgument+xArgument , y+height0*yArgument);
		}

		//画动态CPU数据线
		queue<int> qiCpuAge=GetQueueCpuUsedAge();
		CPoint oldpoint; //前一点 CPU使用率
		CPoint newpoint;//当前 CPU使用率

		CPen newpen(PS_SOLID,1,RGB(254,0,0));//笔 
		oldpen=pDC->SelectObject(&newpen); 
		int n;
		n=243-qiCpuAge.size()*4;
		for(i=0;qiCpuAge.size();i++)
		{
			
			
			newpoint.x=x+(n+i*4)*xArgument;
			newpoint.y=y+(90-qiCpuAge.front()*0.9)*yArgument;
			
			pDC->MoveTo( newpoint.x,newpoint.y );
			pDC->LineTo(oldpoint.x,oldpoint.y);

			oldpoint=newpoint;

			qiCpuAge.pop();
		}

		


		newbrush.DeleteObject();
		newpen.DeleteObject();
		gridpen.DeleteObject();
}

void Gdi::SetFirstTime(bool ft) 
{
	if(FirstTime != ft)
		FirstTime = ft;
}

void Gdi::DrawThreadNum(CDC *pDC,int weight,int height)
{
	CRect rectangle; 
	int	 weight0 = 240, height0 = 90 ; // 基数
	double xArgument = weight/1.0 / weight0, 
		yArgument = height/1.0 / height0;
	
	int x,y; //参数
	x=0;
	y=0;
	
	int k;
	//画背景
	CBrush newbrush(RGB(0xf0,0xff,0xff));
	CBrush *oldbrush;
	oldbrush=pDC->SelectObject(&newbrush); 
	pDC->Rectangle(CRect(x,y,x+weight,y+height));
	pDC->SelectObject(oldbrush); 
	
	for( k=1 ; k<height-1 ; k++)
	{
		rectangle.SetRect(1, k , weight-1, k+1);
		pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
	}
	
	CPen gridpen(PS_SOLID,1,RGB(0,128,64));//笔 
	CPen* oldpen; 
	oldpen=pDC->SelectObject(&gridpen); 
	
	//画横线
	int i;
	for(i=0;i<=9;i++)
	{
		pDC->MoveTo(x+0*xArgument,y+9*i*yArgument);
		pDC->LineTo(x+240*xArgument,y+9*i*yArgument);
	}

	//画竖线
	for(i=weight0/12; i>0; i--)
	{
		pDC->MoveTo(x+12*i*xArgument+LineNumThread*4*xArgument+xArgument , y+0*yArgument);
		pDC->LineTo(x+12*i*xArgument+LineNumThread*4*xArgument+xArgument , y+height0*yArgument);
	}
	pDC->SelectObject(&oldpen);
	
	//取最大值
	queue<int> qiCpuAge2=GetQueueThreadNum();
	
	queue<int> qiCpuAge = qiCpuAge2;
	
	
	int max = 0;
	for(i=0;qiCpuAge2.size();i++)
	{
		if(max < qiCpuAge2.front())
		{
			max = qiCpuAge2.front();
		}
		qiCpuAge2.pop();
	}
	
	double argument=GetArgument(max);
	//画动态CPU数据线
	
	CPoint oldpoint; //前一点 CPU使用率
	CPoint newpoint;//当前 CPU使用率
	
	CPen newpen(PS_SOLID,1,RGB(254,0,0));//笔 
	oldpen=pDC->SelectObject(&newpen); 
	int n=0;
	n=243-qiCpuAge.size()*4;
	for(i=0;qiCpuAge.size();i++)
	{
		
		newpoint.x=x+(n+i*4)*xArgument;
		m_NowThread=qiCpuAge.front();
		newpoint.y=y+(90-qiCpuAge.front() * 0.9 * argument)*yArgument;
		
		pDC->MoveTo( newpoint.x,newpoint.y );
		pDC->LineTo(oldpoint.x,oldpoint.y);
		
		oldpoint=newpoint;
		
		qiCpuAge.pop();
	}
	
	pDC->SelectObject(oldpen);
	newbrush.DeleteObject();
	newpen.DeleteObject();
	gridpen.DeleteObject();
}

void Gdi::DrawGDI(CDC *pDC,int weight,int height) //画GDI总数
{
	CRect rectangle; 
	
	int	 weight0 = 240, height0 = 90 ; // 基数
	double xArgument = weight/1.0 / weight0, 
		yArgument = height/1.0 / height0;
	
	int x,y; //参数
	x=0;
	y=0;
	
	int k;
	//画背景
	CBrush newbrush(RGB(0xf0,0xff,0xff));
	CBrush *oldbrush;
	oldbrush=pDC->SelectObject(&newbrush); 
	pDC->Rectangle(CRect(x,y,x+weight,y+height));
	pDC->SelectObject(oldbrush); 
	
	
	for( k=1 ; k<height-1 ; k++)
	{
		rectangle.SetRect(1, k , weight-1, k+1);
		pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
	}
	
	
	CPen gridpen(PS_SOLID,1,RGB(0,128,64));//笔 
	CPen* oldpen; 
	oldpen=pDC->SelectObject(&gridpen); 
	
	//画横线
	int i;
	for(i=0;i<=9;i++)
	{
		pDC->MoveTo(x+0*xArgument,y+9*i*yArgument);
		pDC->LineTo(x+240*xArgument,y+9*i*yArgument);
	}
	
	//画竖线
	for(i=weight0/12; i>0; i--)
	{
		pDC->MoveTo(x+12*i*xArgument+LineNumGDI*4*xArgument+xArgument , y+0*yArgument);
		pDC->LineTo(x+12*i*xArgument+LineNumGDI*4*xArgument+xArgument , y+height0*yArgument);
	}


	pDC->SelectObject(&oldpen);
	
	//取最大值
	queue<int> qiCpuAge2=GetQueueGdi();
	
	queue<int> qiCpuAge = qiCpuAge2;
	
	
	int max = 0;
	for(i=0;qiCpuAge2.size();i++)
	{
		if(max < qiCpuAge2.front())
		{
			max = qiCpuAge2.front();
		}
		qiCpuAge2.pop();
	}
	
	double argument=GetArgument(max);
	
	//画动态GDI数据线
	CPoint oldpoint; //前一点 GDI
	CPoint newpoint;//当前 GDI
	CPen newpen(PS_SOLID,1,RGB(254,0,0));//笔 
	oldpen=pDC->SelectObject(&newpen); 
	int n;
	n= 243-qiCpuAge.size()*4;
	for(i=0;qiCpuAge.size();i++)
	{
		
		newpoint.x=x+(n+i*4)*xArgument;
		m_NowGdi=qiCpuAge.front();
		newpoint.y=y+(90-qiCpuAge.front() * 0.9 * argument)*yArgument;
		
		pDC->MoveTo( newpoint.x,newpoint.y );
		pDC->LineTo(oldpoint.x,oldpoint.y);
		
		oldpoint=newpoint;
		
		qiCpuAge.pop();
	}
	pDC->SelectObject(oldpen);
	
	
	
	
	newbrush.DeleteObject();
	newpen.DeleteObject();
	gridpen.DeleteObject();
}

void Gdi::DrawGdiType(CDC *pDC,int weight,int height) //画GDI分类统计
{
			CRect rectangle; 
		int	 weight0 = 348, height0 = 133 ; // 基数
		double xArgument = weight/1.0 / weight0, 
			   yArgument = height/1.0 / height0;


		int x,y; //参数
		x=0;
		y=0;

		//画背景
		CBrush newbrush(RGB(0xf0,0xff,0xff));
		CBrush *oldbrush;
		oldbrush=pDC->SelectObject(&newbrush); 
		pDC->Rectangle(CRect(x,y,x+weight,y+height));
		pDC->SelectObject(oldbrush); 

		int k;
		for( k=1 ; k<height-1 ; k++)
		{
			rectangle.SetRect(1, k , weight-1, k+1);
			pDC->FillSolidRect(&rectangle,RGB(0xff,0xff,205+MulDiv(k, 50, height)) );
		}
		

		int GdiType[15]; //GDI大小数组
		GetGdiTypeNum(GdiType);

		// 348-339 = 9  
		//5 * 7 = 35  7个字母
		//写字

		int strweight = 32*xArgument;  //每个矩形 的 宽 
		int fontweight = (strweight - 34) / 6;  // 字体大小  

		CFont *m_Font = new CFont;
		m_Font->CreateFont(-11-fontweight*2,4+fontweight,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"Arial");
		CFont *oldFont;
		pDC->SetBkMode(TRANSPARENT);
		oldFont = pDC->SelectObject(m_Font);
		int i;
		
		CString str1[10]={"other" , "pen" , "brush" , "dc" , "pal" , "font" ,
			"bitmap", "region" , "extpen", "enhmeta"};



		for(i = 0; i <= 9 ;i ++)
		{
		int addweight = strweight -  ( str1[i].GetLength() * (4+fontweight) );//增加的空白
		
		pDC->TextOut(x+ + addweight/2 + (20+i*32)*xArgument, (y+118)*yArgument, str1[i]);
		}
		pDC->SelectObject(oldFont);

		
		// 1 2 3 5 6 7 8 11 13 0

		//画横线
		CPen gridpen(PS_SOLID,1,RGB(0,128,64));//笔
		CPen* oldpen; 
		oldpen=pDC->SelectObject(&gridpen); 
		

		for(i=0;i<=10;i++)
		{
			pDC->MoveTo(x+19*xArgument , (y+13+10*i) * yArgument);
			pDC->LineTo(x+339*xArgument , (y+13+10*i) * yArgument);
		}
		pDC->SelectObject(&oldpen);

		//画外框线

		CPen gridpen2(PS_SOLID,1,RGB(128,128,128));//笔
		oldpen=pDC->SelectObject(&gridpen2); 
        pDC->MoveTo(x+19*xArgument, (y+3) * yArgument);
		pDC->LineTo(x+19*xArgument, (y+113) * yArgument);

        pDC->MoveTo(x+19*xArgument, (y+113) * yArgument);
		pDC->LineTo(x+344*xArgument, (y+113) * yArgument);
		pDC->SelectObject(oldFont);

// x( 11 - 330 ) 每个GDI 占用 32像素 
//		11-42
//		43-74

		//获取 argument指数
		int  j=0,max=0;
		double argument=0;
		CString str;
		for( i = 0; i <= 14 ; i++)
		{
			if(GdiType[i]>max)
				max = GdiType[i];
		}

		argument=GetArgument(max);
		
		int ordinateweight = 0 ; 
		if(	19*xArgument > 22)
			ordinateweight = (19*xArgument-22)/4;

		
		
		//画纵坐标
		CFont *m_Font3 = new CFont;
		m_Font3->CreateFont(-11-ordinateweight*2,5+ordinateweight,0,0,10,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"Arial");
		
		DWORD numargument = (DWORD) 10.0 / argument ;
		oldFont = pDC->SelectObject(m_Font3);
		str.Format("%ld",numargument);

		for( i = 1; i <= 5; i++ )
		{
			str.Format("%ld",numargument*i*2);
			pDC->TextOut(x+1,(y+105-20*i) * yArgument,str);
		}
			pDC->SelectObject(oldFont);
	
		//写GDI个数
		//画GDI矩形
		CFont *m_Font2 = new CFont;


		m_Font2->CreateFont(-11-fontweight*2,4+fontweight,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"Arial");
		pDC->SetBkMode(TRANSPARENT);
		oldFont = pDC->SelectObject(m_Font2);
		
		

		//0 1 2 3 5 6 7 8 11 13 

		for( i = 0; i <= 14 ; i++) //j =GDI种类
		{
			if( i != 4 && i != 9 && i !=10 && i != 12 && i != 14 ) //不支持的GDI
			{
				int addweight = strweight -  ( (4+fontweight) * GetMedian(GdiType[i]) );//增加的空白
				str.Format("%d",GdiType[i]);
				pDC->TextOut(x+addweight/2+((20+32*j)*xArgument), (y - 1)*yArgument , str);  //写GDI个数
				int PictureType =(int) GdiType[i]*argument ;//图像高
				DrawGdiTypeRect(j, x, y, xArgument,yArgument,pDC, PictureType); //与指数argument相乘 范围在100以内。 
				j++; 

			}
		}
		pDC->SelectObject(oldFont);
		

		m_Font->DeleteObject();
		m_Font2->DeleteObject();
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldFont);
		newbrush.DeleteObject();
		gridpen.DeleteObject();
		gridpen2.DeleteObject();
		delete m_Font;
}


void Gdi::DrawGdiTypeRect(int j,int x,int y, double xArgument, double yArgument, CDC *pDC, int num)
{
		CPoint r1( x+(24 + 32*j) * xArgument , (y+113) * yArgument);
		CPoint r2( x+(47 + 32*j) * xArgument , (y+113-num) * yArgument);
		pDC->FillSolidRect( CRect(r1,r2) , m_rgb[j]);
}

int Gdi::GetMedian(int i)//返回位数
{
	if(i<10)
		return 1;
	if(i<100)
		return 2;
	if(i<1000)
		return 3;
	if(i<10000)
		return 4;

	return 5;
}

CString Gdi::GetChineseGdiType(DWORD i) //GDI类型 -> 中文
{
	CString str;
	if(i ==OBJ_PEN)
		str = "画笔" ;
	else if( i  == OBJ_BRUSH )
		str = "画刷" ;
	else if( i  == OBJ_DC )
		str = "设备上下文";
	else if( i  == OBJ_PAL)
		str = "调色板";
	else if( i  == OBJ_FONT)
		str = "字体";
	else if( i  == OBJ_BITMAP)
		str = "位图";
	else if( i  == OBJ_REGION)
		str = "区域";
	else if( i  == OBJ_METAFILE )
		str = "图元文件";
	else if( i  ==OBJ_EXTPEN )
		str = "扩展笔";
	else if( i  == OBJ_ENHMETAFILE)
		str = "增强图元文件";
	else
		str = "其它图形对象";
	return str;
				
}
CString Gdi::GetEnglishGdiType(DWORD i) //GDI类型 -> 中文
{
	CString str;
	if(i ==OBJ_PEN)
		str = "pen" ;
	else if( i  == OBJ_BRUSH )
		str = "brush" ;
	else if( i  == OBJ_DC )
		str = "dc";
	else if( i  == OBJ_PAL)
		str = "pal";
	else if( i  == OBJ_FONT)
		str = "font";
	else if( i  == OBJ_BITMAP)
		str = "bitmap";
	else if( i  == OBJ_REGION)
		str = "region";
	else if( i  == OBJ_METAFILE )
		str = "metafile";
	else if( i  ==OBJ_EXTPEN )
		str = "extpen";
	else if( i  == OBJ_ENHMETAFILE)
		str = "enhmeta";
	else
		str = "other";
	return str;
				
}
vector<GDIINFO> Gdi::GetGDIInfo() //返回GDI句柄信息表
{
	return vGdiInfo;
}
vector<GDITYPEINFO> Gdi::GetGDITypeInfo()//返回GDI句柄类型信息表
{
	return vGdiTypeInfo;
}

DWORD Gdi::GetCursel()
{
	return m_Cursel;
}

DWORD Gdi::GetGdiNum()
{
	int sum = 0, i;
	for(i = 0 ; i <= 14 ; i++)
	{
		sum += GdiType[i];
	}
	return sum;
}



vector< THREADINFO > Gdi::GetThreadInfo()
{
	return vThreadInfo;
}

CString Gdi::GetSP(TCHAR sp[128])
{
	int i;
	CString str ="";
	for(i = 0 ;osvi.szCSDVersion[i]!='\0';i++)
	{
		if( osvi.szCSDVersion[i] <='9' && osvi.szCSDVersion[i] >='0')
		{
			str.Format(".SP%c",osvi.szCSDVersion[i]);
			break;
		}
	}
	return str;
}
  
void Gdi::SetCursel(int cursel) //  设置Cursel  等同于 SetDraw(NULL,cursel,FALSE)
{
	m_Cursel = cursel ;
}





bool Gdi::GetFindProcess()  //返回是否找到进程
{
	return FindProcess;
}

DWORD Gdi::GetProcessMem() //返回此刻进程的内存使用情况
{
	return dwWorkingSize;
}

void Gdi::UpdataCPU(DWORD ProcessID, HANDLE Process, BOOL OpenProcess)
{
	//更新基本信息(CPU个数)
	int status;
	status=NtQuerySystemInformation(SystemBasicInformation, &SysBaseInfo, sizeof(SysBaseInfo),NULL); 
    if(status != NO_ERROR) 
	return;
	//获得新系统时间
	status  = NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo, sizeof(SysTimeInfo),0); 
	if(status!=NO_ERROR) 
	return ; 
	
	// 获得新CPU空闲时间
	status   =   NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if   (status   !=   NO_ERROR) 
	return ; 
	//新CPU空余时间= 新CPU空余时间-旧CPU空余时间
	dbIdleTime   =   Li2Double(SysPerfInfo.liIdleTime)   -   Li2Double(liOldIdleTime); 
	////此段系统时间 = 新总系统时间-旧系统时间
	dbSystemTime   =   Li2Double(SysTimeInfo.liKeSystemTime)   -  Li2Double(liOldSystemTime); 
	// 当前平均CPU空闲率 =  空间时间/CPU核心数/系统时间 
	dbIdleTime   =   dbIdleTime / SysBaseInfo.bKeNumberProcessors    /   dbSystemTime; 
	//CPU平均使用率= 100-(当前CPU空闲率*100 )
	dbIdleTime   =   100.0   -   dbIdleTime   *  100.0 ; //CPU使用率
	liOldIdleTime     =   SysPerfInfo.liIdleTime;  //CPU空闲时间  
	liOldSystemTime   =   SysTimeInfo.liKeSystemTime; //系统时间
	
	if(!bStart)
		SetQueueCpuUsedAge(dbIdleTime);//加入队列
	else
		bStart=false;

}
