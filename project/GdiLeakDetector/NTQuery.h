#ifndef NTQUERY_H
#define  NTQUERY_H

///////////////////////////////////////////
//        NTQuery  头文件
///////////////////////////////////////////

#include   <Tlhelp32.h> 

#define   SystemBasicInformation         0  //系统基本信息
#define   SystemPerformanceInformation   2  //系统性能信息
#define   SystemTimeInformation          3  //系统时间信息
#define SystemHandleInformation          16 //系统句柄       
#define SystemProcessesAndThreadsInformation 5// 5号功能

#define   Li2Double(x)   ((double)((x).HighPart)   *   4.294967296E9   +   (double)((x).LowPart))  //4.294967296E9 原来是2^32
//感觉这个很多余， 为什么不直接    s.QuadPart



typedef   struct 
{ 
        DWORD       dwUnknown1; 
        ULONG       uKeMaximumIncrement;  //一个时钟的计量单位
        ULONG       uPageSize;            //一个内存页的大小
        ULONG       uMmNumberOfPhysicalPages;   //系统管理着多少个页
        ULONG       uMmLowestPhysicalPage;      //低端内存页
        ULONG       uMmHighestPhysicalPage;     //高端内存页
        ULONG       uAllocationGranularity; 
        PVOID       pLowestUserAddress;         //地端用户地址
        PVOID       pMmHighestUserAddress;      //高端用户地址
        ULONG       uKeActiveProcessors;        //激活的处理器
        BYTE         bKeNumberProcessors;  //CPU个数
        BYTE         bUnknown2; 
        WORD         wUnknown3; 
}SYSTEM_BASIC_INFORMATION; //系统基本信息

typedef   struct 
{ 
        LARGE_INTEGER       liIdleTime;       //CPU空闲时间
        DWORD            dwSpare[76];  //
}   SYSTEM_PERFORMANCE_INFORMATION;  //系统性能信息


typedef   struct 
{ 
        LARGE_INTEGER   liKeBootTime;    //系统被启动的时间 
        LARGE_INTEGER   liKeSystemTime; 
        LARGE_INTEGER   liExpTimeZoneBias; 
        ULONG                   uCurrentTimeZoneId; 
        DWORD                   dwReserved;
}   SYSTEM_TIME_INFORMATION;      //系统时间信息


                                       //类型 指针 大小 使用大小



typedef struct _SYSTEM_HANDLE_INFORMATION
{ 
	ULONG ProcessID;		      //进程的标识ID 
	UCHAR ObjectTypeNumber;      //对象类型 
	UCHAR Flags;				//0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT 
	USHORT Handle;             //对象句柄的数值 
	PVOID  Object;            //对象句柄所指的内核对象地址 
	ACCESS_MASK GrantedAccess;      //创建句柄时所准许的对象的访问权 
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION; 

typedef struct _UNICODE_STRING {
  USHORT  Length;     //UNICODE占用的内存字节数，个数*2；
  USHORT  MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING ,*PUNICODE_STRING;

typedef LONG KPRIORITY;

typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize;//虚拟存储峰值大小;
	ULONG VirtualSize;//虚拟存储大小;
	ULONG PageFaultCount;//页故障数目;
	ULONG PeakWorkingSetSize;//工作集峰值大小;
	ULONG WorkingSetSize;//工作集大小;
	ULONG QuotaPeakPagedPoolUsage;//分页池使用配额峰值;
	ULONG QuotaPagedPoolUsage; //分页池使用配额;
	ULONG QuotaPeakNonPagedPoolUsage;//非分页池使用配额峰值;
	ULONG QuotaNonPagedPoolUsage;//非分页池使用配额;
	ULONG PagefileUsage;//页文件使用情况;
	ULONG PeakPagefileUsage;//页文件使用峰值;
}VM_COUNTERS,*PVM_COUNTERS;

typedef struct _IO_COUNTERS
{
	LARGE_INTEGER ReadOperationCount;//I/O读操作数目;
	LARGE_INTEGER WriteOperationCount;//I/O写操作数目;
	LARGE_INTEGER OtherOperationCount;//I/O其他操作数目;
	LARGE_INTEGER ReadTransferCount;//I/O读数据数目;
	LARGE_INTEGER WriteTransferCount;//I/O写数据数目;
	LARGE_INTEGER OtherTransferCount;//I/O其他操作数据数目;
}IO_COUNTERS,*PIO_COUNTERS;

typedef struct _CLIENT_ID 
{ 
HANDLE UniqueProcess; 
HANDLE UniqueThread; 
}CLIENT_ID; 
typedef enum _THREAD_STATE 
{ 
StateInitialized, 
StateReady, 
StateRunning, 
StateStandby, 
StateTerminated, 
StateWait, 
StateTransition, 
StateUnknown 
}THREAD_STATE; 

typedef enum _KWAIT_REASON 
{ 
Executive, 
FreePage, 
PageIn, 
PoolAllocation, 
DelayExecution, 
Suspended, 
UserRequest, 
WrExecutive, 
WrFreePage, 
WrPageIn, 
WrPoolAllocation, 
WrDelayExecution, 
WrSuspended, 
WrUserRequest, 
WrEventPair, 
WrQueue, 
WrLpcReceive, 
WrLpcReply, 
WrVertualMemory, 
WrPageOut, 
WrRendezvous, 
Spare2, 
Spare3, 
Spare4, 
Spare5, 
Spare6, 
WrKernel 
}KWAIT_REASON; 

typedef struct _SYSTEM_THREADS
{
LARGE_INTEGER KernelTime;//CPU内核模式使用时间；
LARGE_INTEGER UserTime;//CPU用户模式使用时间；
LARGE_INTEGER CreateTime;//线程创建时间；
ULONG WaitTime;//等待时间；
PVOID StartAddress;//线程开始的虚拟地址；
CLIENT_ID ClientId;//线程标识符；
KPRIORITY Priority;//线程优先级；
KPRIORITY BasePriority;//基本优先级；
ULONG ContextSwitchCount;//环境切换数目；
THREAD_STATE State;//当前状态；
KWAIT_REASON WaitReason;//等待原因；
}SYSTEM_THREADS,*PSYSTEM_THREADS;


typedef struct _SYSTEM_PROCESSES
{
ULONG NextEntryDelta; //构成结构序列的偏移量;
ULONG ThreadCount; //线程数目;
ULONG Reserved1[6];
LARGE_INTEGER CreateTime; //创建时间;
LARGE_INTEGER UserTime;//用户模式(Ring 3)的CPU时间;
LARGE_INTEGER KernelTime; //内核模式(Ring 0)的CPU时间;
UNICODE_STRING ProcessName; //进程名称;
KPRIORITY BasePriority;//进程优先权;
ULONG ProcessId; //进程标识符;
ULONG InheritedFromProcessId; //父进程的标识符;
ULONG HandleCount; //句柄数目;
ULONG Reserved2[2];
VM_COUNTERS  VmCounters; //虚拟存储器的结构，见下;
IO_COUNTERS IoCounters; //IO计数结构，见下;
SYSTEM_THREADS Threads[1]; //进程相关线程的结构数组，见下;
}SYSTEM_PROCESSES,*PSYSTEM_PROCESSES;


typedef struct
{
	DWORD  pKernel;//指向内核地址空间
	WORD   nProcess; // NT/2000下指向进程ID
	WORD   nCount;
	WORD   nUpper;
	WORD   nType;
	DWORD  pUser;//指向用户地址空间
} GDITableCell;

/*
typedefstruct  _MEMORYSTATUSEX
{
	DWORD dwLength; //长度
	DWORD dwMemoryLoad; //繁忙等级
	unsigned int dwTotalPhys; //物理内存总量
	unsigned int dwAvailPhys; //可分配的内存总量
	unsigned int dwTotalPageFile; //硬盘页交换文件
	unsigned int dwAvailPageFile;//尚未分配的页交换文件
	unsigned int dwTotalVirtual; //地址空间被各进程私有的
	unsigned int dwAvailVirtual; //限制地址空间
}MEMORYSTATUS, *LPMEMORYSTATUS;
*/

// 线程
typedef   enum   _THREADINFOCLASS   
{ 
        ThreadBasicInformation, 
        ThreadTimes, 
        ThreadPriority, 
        ThreadBasePriority, 
        ThreadAffinityMask, 
        ThreadImpersonationToken, 
        ThreadDescriptorTableEntry, 
        ThreadEnableAlignmentFaultFixup, 
        ThreadEventPair_Reusable, 
        ThreadQuerySetWin32StartAddress, 
        ThreadZeroTlsCell, 
        ThreadPerformanceCount, 
        ThreadAmILastThread, 
        ThreadIdealProcessor, 
        ThreadPriorityBoost, 
        ThreadSetTlsArrayAddress, 
        ThreadIsIoPending, 
        ThreadHideFromDebugger, 
        ThreadBreakOnTermination, 
        MaxThreadInfoClass 
}  THREADINFOCLASS; 

//typedef  struct   _CLIENT_ID  
//{ 
//        HANDLE   UniqueProcess; 
 //       HANDLE   UniqueThread; 
//}   CLIENT_ID; 
typedef   CLIENT_ID   *PCLIENT_ID; 

typedef   struct   _THREAD_BASIC_INFORMATION   {   //   Information   Class   0 
        LONG           ExitStatus; 
        PVOID         TebBaseAddress; 
        CLIENT_ID   ClientId; 
        LONG   AffinityMask; 
        LONG   Priority; 
        LONG   BasePriority; 
}   THREAD_BASIC_INFORMATION,   *PTHREAD_BASIC_INFORMATION; 

typedef struct
{
	DWORD id; // 线程ID
	DWORD  pri;  //线程优先值

} THREADINFO, *PTHREADINFO;

//每个GDI信息
typedef struct 
{
	DWORD handle;//句柄
	CString type; //中文
	DWORD pKernel;// 内核地址空间
} GDIINFO, *PGDIINFO;

//每个GDI种类信息
typedef struct
{
	CString type;//对象类型 英文
	DWORD num; //对象数量
	CString Expand; //扩展信息
	
} GDITYPEINFO, *PGDITYPEINFO; 



#endif
