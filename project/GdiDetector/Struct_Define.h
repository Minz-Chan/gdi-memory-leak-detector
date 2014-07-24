#ifndef _STRUCT_DEFINE_H
#define _STRUCT_DEFINE_H

#include <vector>

typedef struct _CALL_STACK{
	std::vector<std::string> callStack;
}CALL_STACK, *PCALL_STACK;

// 寄存器信息
typedef struct _REGISTERS_INFORMATION{
	unsigned long Edi;
	unsigned long Esi;
	unsigned long Eax;
	unsigned long Ebx;
	unsigned long Ecx;
	unsigned long Edx;
	unsigned long Eip;
	unsigned long Ebp;
	unsigned long SegCs;
	unsigned long EFlags;
	unsigned long Esp;
	unsigned long SegSs;
}REGISTERS_INFORMATION, PREGISTERS_INFORMATION;



// 模块信息
typedef struct _MODULE_INFORMATION{
	unsigned long		BaseOfImage;			// 映像基址
	unsigned long		ImageSize;				// 映像大小
	unsigned long		TimeDateStamp;			// 时间戳
	unsigned long		CheckSum;				// 检验值
	char				ModuleName[32];			// 模块名
	char				ImageName[256];			// 映像路径
	char				LoadedImageName[256];	// 装载的映像名
	char				LoadedPdbName[256];		// 装载的PDB名
	unsigned __int64	FileVersion;			// 文件版本值
}MODULE_INFORMATION, *PMODULE_INFORMATION;


// 句柄类型
typedef enum _HANDLE_TYPE{
	_BITMAP = 1,
		_BRUSH = 2,
		_COLORSPACE = 3,
		_DC = 4,
		_ENHMETADC = 5,
		_ENHMETAFILE = 6,
		_EXTPEN = 7,
		_FONT = 8,
		_MEMDC = 9,
		_METAFILE = 10,
		_METADC = 11,
		_PAL = 12,
		_PEN = 13,
		_REGION = 14,
		_UNKNOWN = 15
}HANDLE_TYPE;

// 对应句柄的相关信息
typedef struct _HANDLE_INFORAMTION{
	unsigned long handle;						// 句柄值
	unsigned uType;								// 句柄类型
	char szParaInfo[256];						// 参数信息
	CALL_STACK callStack;						// 函数调用栈信息
	REGISTERS_INFORMATION registers;			// 寄存器信息
	std::vector<MODULE_INFORMATION> moduleInfo;	// 模块信息
}HANDLE_INFORAMTION, *PHANDLE_INFORMATION;


#endif //_STRUCT_DEFINE_H