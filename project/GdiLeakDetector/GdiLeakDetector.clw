; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRightView2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "gdileakdetector.h"
LastPage=0

ClassCount=20
Class1=CBitmapTabCtrl
Class2=CCategoryStatisticsPage
Class3=CCpuPage
Class4=CCurveSheet
Class5=CCurveView
Class6=CGdiLeakDetectorApp
Class7=CAboutDlg
Class8=CGdiLeakDetectorDoc
Class9=CGdiLeakDetectorView
Class10=CGdiNumberPage
Class11=CGdiObjWatchPage
Class12=CInfoView
Class13=CLeftView
Class14=CMainFrame
Class15=CMemoryPage
Class16=COutputPage
Class17=CRightView
Class18=CRightView2
Class19=CThreadPage

ResourceCount=13
Resource1=IDT_LOADVIEW
Resource2=IDR_MAINFRAME
Resource3=IDD_THREAD
Resource4=IDD_FORMVIEW1
Resource5=IDD_ABOUTBOX
Resource6=IDD_GDINUMBER
Resource7=IDD_GDIOBJWATCH
Resource8=IDD_OUTPUT
Resource9=IDD_MEMORY
Resource10=IDD_CATEGORY_STATISTICS
Resource11=IDD_FORMVIEW
Resource12=IDD_CPU
Class20=CCodeShow
Resource13=IDD_SHOWCODE

[CLS:CBitmapTabCtrl]
Type=0
BaseClass=CTabCtrl
HeaderFile=BitmapTabCtrl.h
ImplementationFile=BitmapTabCtrl.cpp

[CLS:CCategoryStatisticsPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=CategoryStatisticsPage.h
ImplementationFile=CategoryStatisticsPage.cpp

[CLS:CCpuPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=CpuPage.h
ImplementationFile=CpuPage.cpp

[CLS:CCurveSheet]
Type=0
BaseClass=CPropertySheet
HeaderFile=CurveSheet.h
ImplementationFile=CurveSheet.cpp

[CLS:CCurveView]
Type=0
BaseClass=CView
HeaderFile=CurveView.h
ImplementationFile=CurveView.cpp

[CLS:CGdiLeakDetectorApp]
Type=0
BaseClass=CWinApp
HeaderFile=GdiLeakDetector.h
ImplementationFile=GdiLeakDetector.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=GdiLeakDetector.cpp
ImplementationFile=GdiLeakDetector.cpp
LastObject=CAboutDlg

[CLS:CGdiLeakDetectorDoc]
Type=0
BaseClass=CDocument
HeaderFile=GdiLeakDetectorDoc.h
ImplementationFile=GdiLeakDetectorDoc.cpp

[CLS:CGdiLeakDetectorView]
Type=0
BaseClass=CListView
HeaderFile=GdiLeakDetectorView.h
ImplementationFile=GdiLeakDetectorView.cpp

[CLS:CGdiNumberPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GdiNumberPage.h
ImplementationFile=GdiNumberPage.cpp

[CLS:CGdiObjWatchPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=GdiObjWatchPage.h
ImplementationFile=GdiObjWatchPage.cpp

[CLS:CInfoView]
Type=0
BaseClass=CListView
HeaderFile=InfoView.h
ImplementationFile=InfoView.cpp

[CLS:CLeftView]
Type=0
BaseClass=CFormView
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame

[CLS:CMemoryPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=MemoryPage.h
ImplementationFile=MemoryPage.cpp

[CLS:COutputPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=OutputPage.h
ImplementationFile=OutputPage.cpp
Filter=D
VirtualFilter=idWC

[CLS:CRightView]
Type=0
BaseClass=CDialog
HeaderFile=RightView.h
ImplementationFile=RightView.cpp

[CLS:CRightView2]
Type=0
BaseClass=CFormView
HeaderFile=RightView2.h
ImplementationFile=RightView2.cpp
Filter=D
VirtualFilter=VWC
LastObject=CRightView2

[CLS:CThreadPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ThreadPage.h
ImplementationFile=ThreadPage.cpp

[DLG:IDD_CATEGORY_STATISTICS]
Type=1
Class=CCategoryStatisticsPage
ControlCount=1
Control1=IDC_NOTICESTATIC,static,1342308352

[DLG:IDD_CPU]
Type=1
Class=CCpuPage
ControlCount=0

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GDINUMBER]
Type=1
Class=CGdiNumberPage
ControlCount=1
Control1=IDC_NOTICESTATIC,static,1342308352

[DLG:IDD_GDIOBJWATCH]
Type=1
Class=CGdiObjWatchPage
ControlCount=1
Control1=IDLC_HANDLE,SysListView32,1350633477

[DLG:IDD_FORMVIEW]
Type=1
Class=CLeftView
ControlCount=17
Control1=IDC_EDIT_VERSION,edit,1350633600
Control2=IDC_EDIT_PRO,edit,1350633600
Control3=IDC_EDIT_PHYMEM,edit,1350633600
Control4=IDC_EDIT_AVALMEM,edit,1350633600
Control5=IDC_EDIT_VIRTUALMEM,edit,1350633600
Control6=IDC_EDIT_AVALVMEM,edit,1350633600
Control7=IDC_EDIT_PAGEFILESIZE,edit,1350633600
Control8=IDC_EDIT_PAGEPATH,edit,1350633600
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287

[DLG:IDD_MEMORY]
Type=1
Class=CMemoryPage
ControlCount=1
Control1=IDC_NOTICESTATIC,static,1342308352

[DLG:IDD_OUTPUT]
Type=1
Class=COutputPage
ControlCount=1
Control1=IDLC_LEAKHANDLE,SysListView32,1350633477

[DLG:IDD_FORMVIEWINFO]
Type=1
Class=CRightView

[DLG:IDD_FORMVIEW1]
Type=1
Class=CRightView2
ControlCount=8
Control1=IDC_LIST3,SysListView32,1350631425
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_LIST2,SysListView32,1350631425
Control4=IDC_OUTPUTLIST,SysListView32,1350631681
Control5=IDC_WATCHEDIT,edit,1352728708
Control6=IDC_MEMORYEDIT,edit,1352728708
Control7=IDC_NOTICESTATIC,static,1342308352
Control8=IDC_NOTICESTATIC2,static,1342308352

[DLG:IDD_THREAD]
Type=1
Class=CThreadPage
ControlCount=1
Control1=IDC_NOTICESTATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_VIEW_LARGEICON
Command9=ID_VIEW_SMALLICON
Command10=ID_VIEW_LIST
Command11=ID_VIEW_DETAILS
Command12=ID_APP_ABOUT
CommandCount=12

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDT_LOADVIEW]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDE_FILEPATH,edit,1350631424
Control3=IDBN_LOADFILE,button,1342259201
Control4=IDBN_DEBUG,button,1342259201

[DLG:IDD_SHOWCODE]
Type=1
Class=CCodeShow
ControlCount=1
Control1=IDC_CODETEXT,edit,1345388548

[CLS:CCodeShow]
Type=0
HeaderFile=CodeShow.h
ImplementationFile=CodeShow.cpp
BaseClass=CDialog
Filter=D
LastObject=CCodeShow
VirtualFilter=dWC

