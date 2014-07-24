// CodeShow.cpp : implementation file
//

#include "stdafx.h"
#include "gdileakdetector.h"
#include "CodeShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeShow dialog


CCodeShow::CCodeShow(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCodeShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCodeShow, CDialog)
	//{{AFX_MSG_MAP(CCodeShow)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeShow message handlers

BOOL CCodeShow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	CRect rect;

	this->GetWindowRect(&rect);



	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CODETEXT);

	pEdit->MoveWindow(0, 0, rect.right - rect.left, 
		rect.bottom - rect.top, TRUE);

	if(::GetFileAttributes(m_szFileName) == -1){
		::MessageBox(NULL, "目标文件不存在 !", "错误", MB_ICONINFORMATION);
		return FALSE;
	}

	HANDLE hFile = CreateFile(m_szFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL,                  // default security
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL); 
	
	DWORD fileLen = GetFileSize(hFile, NULL);
	char* text = (char*)VirtualAlloc(NULL, fileLen+1/*NULL*/, MEM_COMMIT, PAGE_READWRITE);
	if(text)
	{
		ReadFile(hFile, text, fileLen, &fileLen, NULL);
		text[fileLen] = 0;
		//		MessageBox(text);
		::SetWindowText(pEdit->GetSafeHwnd(), text);
		VirtualFree(text, 0, MEM_RELEASE);
		pEdit->SetModify(FALSE);
	}

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCodeShow::SetLineNum(int nLine)
{
	m_nLineNum = nLine;
}

void CCodeShow::SetFileName(CString szFileName)
{
	m_szFileName = szFileName;
}


void CCodeShow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CODETEXT);
	
	int nLine = m_nLineNum;
	if(nLine > 0){
		int lineStart = pEdit->LineIndex(nLine - 1);
		int lineEnd = pEdit->LineIndex(nLine);
		pEdit->SetSel(lineStart, lineEnd);
		pEdit->SetFocus();
	}

	KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}
