#include <windows.h>
#include <tchar.h>

#ifndef _INJECT_H
#define _INJECT_H


// 定义API挂接项结构
// typedef struct _HOOK_ITEM {
// 	DWORD	dwAddr ;			// IAT项所在地址
// 	DWORD	dwOldValue ;		// IAT项的原始函数地址
// 	DWORD	dwNewValue ;		// IAT项的新函数地址
// } HOOK_ITEM, *PHOOK_ITEM ;

#ifndef _STRUCT_HOOK_ITEM
#define _STRUCT_HOOK_ITEM

typedef struct _HOOK_ITEM{
	DWORD dwIATAddr;			// IAT项所在地址
	DWORD dwOldImportAddr;		// IAT项的原始函数地址
	DWORD dwEATAddr;			// 目标模块的EAT项所在地址
	DWORD dwOldExportAddr;		// 目标模块的EAT项的原始函数地址
	DWORD dwNewFuncAddr;		// 指向的新函数地址
}HOOK_ITEM, *PHOOK_ITEM;

#endif //_STRUCT_HOOK_ITEM

// 定义重定向API的实现函数
// BOOL WINAPI SetImportAddress(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem);
// BOOL WINAPI SetExportAddress(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem);
// BOOL WINAPI RedirectApi(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem);

// 设置输出表函数入口点
// 参数pDllName:目标API所在的DLL名称
// 参数pFunName:目标API名称
// 参数dwNewProc:自定义的函数地址
// 参数pItem:用于保存EAT项信息
BOOL WINAPI SetExportAddress(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem){

	// 检查参数是否合法
	if (pDllName == NULL || pFunName == NULL || !dwNewProc || !pItem)
		return FALSE;
	
	// 检测目标模块是否存在
	char szTempDllName[256] = {0};
	DWORD dwBaseImage = (DWORD)GetModuleHandle(pDllName);	// 已加载的DLL基址
	if (dwBaseImage == 0)
		return FALSE;
	
	// 取得PE文件头信息指针
	PIMAGE_DOS_HEADER			pDosHeader = (PIMAGE_DOS_HEADER)dwBaseImage;
	PIMAGE_NT_HEADERS			pNtHeader = (PIMAGE_NT_HEADERS)(dwBaseImage + (pDosHeader->e_lfanew));
	PIMAGE_OPTIONAL_HEADER32	pOptionalHeader = &(pNtHeader->OptionalHeader);
	PIMAGE_SECTION_HEADER		pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeader + 0x18 \
		+ pNtHeader->FileHeader.SizeOfOptionalHeader);
	
	// 遍历输出表
	//PIMAGE_THUNK_DATA pThunk, pEAT;
	PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)(dwBaseImage + \
		pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	unsigned ord = 0;
	if ((unsigned) pFunName < 0xFFFF) // ordinal ?
		ord = (unsigned) pFunName;
	else{

		const DWORD * pNames = (const DWORD *)(dwBaseImage + pIED->AddressOfNames);
		const WORD * pOrds = (const WORD *)(dwBaseImage + pIED->AddressOfNameOrdinals);
		

		// 取得在函数地址表数组中的下标序号ord
		for(unsigned i=0; i < pIED->AddressOfNames; i++){
			
			if(strcmp(pFunName, (const char *)(dwBaseImage + pNames[i])) == 0){
				ord = pIED->Base + pOrds[i];
				break;
			}
		}
	}
	if(ord<pIED->Base || ord>pIED->NumberOfNames)
		return FALSE;
	
	// 指向输出表的函数地址表
	// 注: 输出表中的函数地址表中的地址值是RVA地址
	// 需加上模块基址的值才能得到真正的函数地址值
	DWORD * pFuncAddr = (DWORD *)(dwBaseImage + pIED->AddressOfFunctions);

	
	// 填充重定向信息
	pItem->dwEATAddr = (DWORD)pIED;
	pItem->dwOldExportAddr = dwBaseImage + pFuncAddr[ord - pIED->Base] ;
	pItem->dwNewFuncAddr = dwNewProc;

	
	DWORD dwNewFuncRVA = dwNewProc - dwBaseImage;
	DWORD dwOldProtect = 0;

	// 修改EAT项
	VirtualProtect((DWORD *)&pFuncAddr[ord - pIED->Base], 4, PAGE_READWRITE, &dwOldProtect);
	pFuncAddr[ord - pIED->Base]=dwNewFuncRVA;
	VirtualProtect((DWORD *)&pFuncAddr[ord - pIED->Base], 4, PAGE_READWRITE, &dwOldProtect);

	return TRUE;
}



// 设置输入表函数入口点
// 参数pDllName:目标API所在的DLL名称
// 参数pFunName:目标API名称
// 参数dwNewProc:自定义的函数地址
// 参数pItem:用于保存IAT项信息
BOOL WINAPI SetImportAddress(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem){

	// 检查参数是否合法
	if (pDllName == NULL || pFunName == NULL || !dwNewProc || !pItem)
		return FALSE;
	
	// 检测目标模块是否存在
	char szTempDllName[256] = {0};
	DWORD dwBaseImage = (DWORD)GetModuleHandle(NULL);	// 目标进程的基址
	if (dwBaseImage == 0)
		return FALSE;
	
	// 取得PE文件头信息指针
	PIMAGE_DOS_HEADER			pDosHeader = (PIMAGE_DOS_HEADER)dwBaseImage;
	PIMAGE_NT_HEADERS			pNtHeader = (PIMAGE_NT_HEADERS)(dwBaseImage + (pDosHeader->e_lfanew));
	PIMAGE_OPTIONAL_HEADER32	pOptionalHeader = &(pNtHeader->OptionalHeader);
	PIMAGE_SECTION_HEADER		pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeader + 0x18 \
											+ pNtHeader->FileHeader.SizeOfOptionalHeader);
	
	// 遍历输入表
	PIMAGE_THUNK_DATA pThunk, pIAT;
	PIMAGE_IMPORT_DESCRIPTOR pIID = (PIMAGE_IMPORT_DESCRIPTOR)(dwBaseImage + \
		pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	while (pIID->FirstThunk )
	{
		// 检测是否目标模块
		if (strcmp((PCHAR)(dwBaseImage+pIID->Name), pDllName))
		{
			pIID++;
			continue;
		}
		
		// 获取IAT表的地址
		pIAT = (PIMAGE_THUNK_DATA)(dwBaseImage + pIID->FirstThunk);
		if (pIID->OriginalFirstThunk){
			pThunk = (PIMAGE_THUNK_DATA)(dwBaseImage + pIID->OriginalFirstThunk);
		}
		else
			pThunk = pIAT;
		
		// 遍历IAT
		DWORD	dwThunkValue = 0;
		while ((dwThunkValue = *((DWORD*)pThunk)) != 0)
		{
			// 当 dwThunkValue 最高位(MSB位)为0, 说明函数不是由序数引入.
			// 此时存在 IMAGE_IMPORT_BY_NAME 结构, 可通过将该值作为RVA转入 
			// IMAGE_IMPORT_BY_NAME 数组, 跳过 Hint 就是函数名字.
			// 注: 通过 Depends 可知系统的几个核心模块Kernel32.dll, User32.dll,
			// Gdi32.dll等不是由序号引入的.
			if (( dwThunkValue & IMAGE_ORDINAL_FLAG32 ) == 0)
			{
				// 检测是否目标函数
				if (strcmp ( (PCHAR)(dwBaseImage+dwThunkValue+2), pFunName ) == 0)
				{
					// 填充函数重定向信息
					pItem->dwIATAddr = (DWORD)pIAT;
					pItem->dwOldImportAddr = *((DWORD*)pIAT);
					pItem->dwNewFuncAddr = dwNewProc;
					
					// 修改IAT项
					DWORD dwOldProtect = 0;
					VirtualProtect (pIAT, 4, PAGE_READWRITE, &dwOldProtect);
					*((DWORD*)pIAT) = dwNewProc;
					VirtualProtect (pIAT, 4, PAGE_READWRITE, &dwOldProtect);
					return TRUE;
				}
			}
			
			pThunk ++;
			pIAT ++;
		}
		
		pIID ++;
	}
	
	return FALSE;
}

// 设置API函数入口点
// 参数pDllName:目标API所在的DLL名称
// 参数pFunName:目标API名称
// 参数dwNewProc:自定义的函数地址
// 参数pItem:用于保存HOOK_ITEM信息
BOOL WINAPI RedirectApi(PCHAR pDllName, PCHAR pFunName, DWORD dwNewProc, PHOOK_ITEM pItem){
	
	if(SetImportAddress(pDllName, pFunName, dwNewProc, pItem)
		&& SetExportAddress(pDllName, pFunName, dwNewProc, pItem))
		return TRUE;
	else
		return FALSE;
}

#endif