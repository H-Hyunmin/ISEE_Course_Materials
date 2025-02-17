// VFWCAPTUREDlg.cpp : implementation file
#include "stdafx.h"
#include "VFWCAPTURE.h"
#include "VFWCAPTUREDlg.h"
#include "vfw.h"
#include "BufStruct.h"
#include "ImageProc.h"
#pragma comment(lib,"vfw32.lib")
#pragma comment(lib,"ImageProc.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define YUV2 844715353
#define VUY2 353517448
#define YUY2 842094158
#define NV12 842094159
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVFWCAPTUREDlg dialog
int CVFWCAPTUREDlg::nVideoW=0;
int CVFWCAPTUREDlg::nVideoH=0;
DWORD CVFWCAPTUREDlg::dwFormat = 0;
extern BYTE* pImgProcBuf;

CVFWCAPTUREDlg* theDlg=NULL;
void AddMessageToList(char* message)
{
	if( theDlg )
		theDlg->AddLogInfo(message);
}

CVFWCAPTUREDlg::CVFWCAPTUREDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVFWCAPTUREDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVFWCAPTUREDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    nPlugInNum = 0;
	m_nVideoWidth = 0;
	m_nVideoHeight = 0;
	m_sVideoFmt = _T("");
    pBuffer = NULL;
	m_bDSPFormatSim = FALSE;
	theDlg = this;
}

void CVFWCAPTUREDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVFWCAPTUREDlg)
    DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_LIST2, m_PluginList);
	DDX_Text(pDX, IDC_VIDEOW, m_nVideoWidth);
	DDX_Text(pDX, IDC_VIDEOH, m_nVideoHeight);
	DDX_Text(pDX, IDC_VIDEOFMTID, m_sVideoFmt);
    DDX_Check(pDX, IDC_DSPFORMAT, m_bDSPFormatSim);
    DDX_Control(pDX, IDC_PLUGIN_ENABLE, m_PluginEnable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVFWCAPTUREDlg, CDialog)
	//{{AFX_MSG_MAP(CVFWCAPTUREDlg)	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVEIMG, OnSaveimg)
	ON_BN_CLICKED(IDC_VFSETUP, OnVfsetup)
	ON_BN_CLICKED(IDC_VFWSETUP, OnVfwsetup)
	ON_BN_CLICKED(IDC_DSPFORMAT, OnDspformat)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_BN_CLICKED(IDC_PLUGIN_ENABLE, OnPluginEnable)
	ON_BN_CLICKED(IDC_ACTIVEALL, OnActiveall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CVFWCAPTUREDlg message handlers

BOOL CVFWCAPTUREDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    InitMessageOpFunction(&AddMessageToList);
	
	HANDLE hFind = NULL;
	WIN32_FIND_DATA w32fd;//struct,存放文件的9种属性信息
	char path[256];
	GetModuleFileName(NULL,path,256);//数返回该当前应用程序全路径
	char sDrv[256],sDir[256],sFname[256],sExt[256];
	_splitpath(path,sDrv,sDir,sFname,sExt);//分解路径
   	_makepath(path,sDrv,sDir,"","");//生成路径
	strncpy(sDir,path,256);//字符串联接
	//strncpy(path,sWorkDir,256);
	strncat(path,"PlugIn\\*.dll",256);//字符串联接
	hFind = FindFirstFile(path,&w32fd);//搜索指定文件
	if( hFind!=INVALID_HANDLE_VALUE && hFind!=0 )
	{
		do 
		{
			if( nPlugInNum<32 )
			{
				AddLogInfo("One plugin found!-->%s<--",w32fd.cFileName);
				//
				strncpy(path,sDir,256);
				strncat(path,"Plugin\\",256);
				strncat(path,w32fd.cFileName,256);
				//获取插件的函数指针
				AllPlugIns[nPlugInNum].OpenPlugIn(path);
				if( AllPlugIns[nPlugInNum].bPlugInOk )
				{
					strncpy(sExt," ",256);
					strncat(sExt,w32fd.cFileName,256);
					m_PluginList.AddString(sExt);//
	     			AddLogInfo("Load PlugIn<%s> success!",path);
  					AllPlugIns[nPlugInNum].OnInitPlugIn(NULL);
                    nPlugInNum++;
				}
				else
				{
					AddLogInfo("Load PlugIn<%s> failed.",path);
				}
			}
		} while(FindNextFile(hFind,&w32fd));
		FindClose(hFind);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CVFWCAPTUREDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CVFWCAPTUREDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVFWCAPTUREDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CVFWCAPTUREDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	//	if( GetDlgItem(IDC_GRABFRAME)->IsWindowEnabled() )
//	{
//		capOverlay(m_hCapture,TRUE);
//		GetDlgItem(IDC_RECORD)->EnableWindow(TRUE);
//	}
//	else
//	{
		CAPTUREPARMS m_CaptureParams;
    //  创建"捕获窗" 
		m_hCapture = capCreateCaptureWindow("cap",WS_EX_CONTROLPARENT|WS_CHILD|WS_VISIBLE,10,6,640,480,m_hWnd,0);
	//	关联捕获窗和驱动程序
		while(!capDriverConnect(m_hCapture,0));
	//	capDriverConnect(m_hCapture,0);
    //设置视频设备的属性
		capCaptureGetSetup(m_hCapture,&m_CaptureParams,sizeof(m_CaptureParams));
		m_CaptureParams.fYield = TRUE;
		capCaptureSetSetup(m_hCapture,&m_CaptureParams,sizeof(m_CaptureParams));
   //   打开预览
		capOverlay(m_hCapture,IsDlgButtonChecked(IDC_OVERPLAY));
   //   设置相应按钮为活动状态
   		GetDlgItem(IDC_SAVEIMG)->EnableWindow(true);
	   //	GetDlgItem(IDC_RECORD)->EnableWindow(TRUE);
   //   禁止“开始”按钮
	    GetDlgItem(IDC_START)->EnableWindow(false);
   //  定义一个帧捕获回调函数
		if( capSetCallbackOnFrame(m_hCapture,(LPVOID)VideoCallbackProc) )
		 AddLogInfo("Success install capture callback!");

   // 获取视频参数
        DWORD dwSize = capGetVideoFormatSize(m_hCapture);//视频参数大小
		BYTE* pBuffer = new BYTE[dwSize];
		capGetVideoFormat(m_hCapture,pBuffer,dwSize);
	//
		LPBITMAPINFO pbi = (LPBITMAPINFO)pBuffer;
		nVideoW = pbi->bmiHeader.biWidth;
		nVideoH = pbi->bmiHeader.biHeight;
		dwFormat = pbi->bmiHeader.biCompression;
	//
		delete pBuffer;
		m_nVideoWidth = nVideoW;
		m_nVideoHeight = nVideoH;
		m_sVideoFmt.Format("0x%x",dwFormat);   
	//设置33ms定时器
	    SetTimer(1,33,NULL);
	//刷新窗口
		UpdateData(false);
}
void  CVFWCAPTUREDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//抓取一帧图像数据,与capGrabFrame相比,它不会停止预览模式和重叠
 	capGrabFrameNoStop(m_hCapture);
	CDialog::OnTimer(nIDEvent);
}


LRESULT CALLBACK CVFWCAPTUREDlg::VideoCallbackProc(HWND hWnd, LPVOID lpVHdr)
{
	 LPVIDEOHDR pVHdr = (LPVIDEOHDR)lpVHdr;
     //创建存储YUV2平面格式的破空间
     BYTE* pY = new BYTE[nVideoW*nVideoH*2];
	 BYTE* pU = pY+nVideoW*nVideoH;
	 BYTE* pV = pU+nVideoW*nVideoH/2;
				
	if( pVHdr )
	{
		if (dwFormat== YUV2  || dwFormat==RGB24)
        {
	     if( !pImgProcBuf ) 
						{
						pImgProcBuf = new BYTE[nVideoW*nVideoH*4];
						if( !pImgProcBuf )	return (LRESULT)FALSE;
						((BUF_STRUCT*)pImgProcBuf)->bNotInited = true;//设置初始化标记
						}
			//转换到YUV2平面格式
			if	 (dwFormat== YUV2)
			theDlg->SplitYUVData(nVideoW,nVideoH,pVHdr->lpData,pY,pU,pV);
            if	 (dwFormat== RGB24)
            theDlg->RGBtoYUV(nVideoW,nVideoH,pVHdr->lpData,pY,pU,pV);
		}
		else
        {
		theDlg->AddLogInfo("未知的视频格式---%d,frame_size=%d,video_size=%d",dwFormat,pVHdr->dwBufferLength,nVideoW*nVideoH);
					return FALSE;
		}
 //
			
					//找出最后有效插件
			        int theLast=-1;
					for(int i=0;i<theDlg->nPlugInNum;i++)
					{
						theDlg->AllPlugIns[i].OnPlugInBeLast(false);
						//strncmp 字符串s1与s2的前size个字符相同，函数返回值为0
						if( theDlg->AllPlugIns[i].Enabled() && strncmp(theDlg->AllPlugIns[i].sName,"PZ_FinalProc.dll",16)!=0 )
							{
							theLast = i;
							}
					}
					if( theLast>=0 )	theDlg->AllPlugIns[theLast].OnPlugInBeLast(true);
					//依次调用所有插件处理
					for(  i=0;i<theDlg->nPlugInNum;i++)
					{
						if( theDlg->AllPlugIns[i].Enabled() )
							theDlg->AllPlugIns[i].OnPlugInRun(nVideoW,nVideoH,pY,pU,pV,pImgProcBuf);
					}
					//处理完毕的图像数据转换回来显示
                if	 (dwFormat== YUV2)
					theDlg->CombineYUVData(nVideoW,nVideoH,pVHdr->lpData,pY,pU,pV);
                if	 (dwFormat== RGB24)
                theDlg->YUVtoRGB(nVideoW,nVideoH,pVHdr->lpData,pY,pU,pV);
			
	
	}
	delete pY;
    return (LRESULT) TRUE ;
}

//
void CVFWCAPTUREDlg::AddLogInfo(LPCTSTR sMessage, ...)
{
	//定义一具VA_LIST型的变量，这个变量是指向参数的指针,typedef char *  va_list;
	va_list ap;
	char sBuffer[1024];
    //VA_START宏初始化VA_LIST变量, ( ap = (va_list)&v + _INTSIZEOF(v) ) 即指向第一个变参的位置
	va_start(ap, sMessage);
    /*送格式化输出到字符串中,int vsprintf(char *string,char *format,va_list param);
	vsprintf()函数中的自变量是位于数组中的，数组元素的字符串之前都要加上百分号(%).
	这个函数是“一步一步”按顺序执行的。在第一个百分号（%）后，将插入第一个数组元素；
	在第二个%号后将插入第二个数组元素，依次类推。*/
	vsprintf(sBuffer,sMessage,ap);
	//清空va_list，即结束变参的获取
	va_end(ap);
   //
 	m_List.SetCurSel(m_List.AddString(sBuffer));
   //  m_List.AddString(sBuffer);
	if( m_List.GetCount()>10000 )
		m_List.DeleteString(0);
}
void CVFWCAPTUREDlg::OnSaveimg() 
{
	// TODO: Add your control notification handler code here
	capFileSaveDIB(m_hCapture,"d:\\VFW_Cap.bmp");
	AfxMessageBox("文件成功保存在d:\\vfw_cap.bmp");
}
void CVFWCAPTUREDlg::OnVfsetup() 
{
	// TODO: Add your control notification handler code here
    capDlgVideoFormat(m_hCapture);
	BYTE* pTemp = pImgProcBuf;
	pImgProcBuf = NULL;
	delete pTemp;
}
void CVFWCAPTUREDlg::OnVfwsetup() 
{
	// TODO: Add your control notification handler code here
	capDlgVideoSource(m_hCapture);
	BYTE* pTemp = pImgProcBuf;
	pImgProcBuf = NULL;
	delete pTemp;
}
void CVFWCAPTUREDlg::OnDspformat() 
{
	// TODO: Add your control notification handler code here
		UpdateData();
}
void CVFWCAPTUREDlg::OnSelchangeList2() 
{
	// TODO: Add your control notification handler code here
	//GetCurSel()获得当前选择序号
	//SetCheck()只对复选框与单选按钮有效，它是用来设置复选框或单选按钮的状态
	m_PluginEnable.SetCheck(AllPlugIns[m_PluginList.GetCurSel()].bEnabled);	
	// EnableWindow(): 使某个控件或窗口可用
	m_PluginEnable.EnableWindow(TRUE);
	AddLogInfo("插件信息:%s",AllPlugIns[m_PluginList.GetCurSel()].OnPlugInInfo());
}
void CVFWCAPTUREDlg::OnPluginEnable() 
{	// TODO: Add your control notification handler code here
	int id = m_PluginList.GetCurSel();
	AllPlugIns[id].bEnabled = m_PluginEnable.GetCheck();
	m_PluginList.DeleteString(id);
	CString sLine;
	sLine.Format("%c%s",AllPlugIns[id].bEnabled?'*':' ',AllPlugIns[id].sName);
	m_PluginList.InsertString(id,sLine);
	m_PluginList.SetCurSel(id);
}

void CVFWCAPTUREDlg::OnActiveall() 
{	// TODO: Add your control notification handler code here
		for(int i=0;i<nPlugInNum;i++)
	{
		AllPlugIns[i].bEnabled = IsDlgButtonChecked(IDC_ACTIVEALL);
		m_PluginList.DeleteString(i);
		CString sLine;
		sLine.Format("%c%s",AllPlugIns[i].bEnabled?'*':' ',AllPlugIns[i].sName);
		m_PluginList.InsertString(i,sLine);
		m_PluginList.SetCurSel(i);
	}
}
//
BOOL CVFWCAPTUREDlg::DestroyWindow() 
{	// TODO: Add your specialized code here and/or call the base class
	capDriverDisconnect(m_hCapture);
	return CDialog::DestroyWindow();
}
//将YUV422视频数据格式（按象素YUYVYUYV...）转换为YUV422平面图片格式(按Y-U-V存放)
void CVFWCAPTUREDlg::SplitYUVData(
	int w,int h,//图像大小
	BYTE *pYUV, //YUV422视频图像指针
	BYTE *pY, BYTE *pU, BYTE *pV//YUV422平面图片指针
	)
{
	BYTE* pLine = pYUV;
	BYTE* pLineY = pY;
 	BYTE* pLineU = pU;
	BYTE* pLineV = pV;
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)//一行处理
		{
			pLineY[i] = pLine[i*2];
		}
		for( i=0;i<w/2;i++)
		{
			pLineU[i] = pLine[i*4+1];
			pLineV[i] = pLine[i*4+3];
		}
		pLineY += w;
		pLineU += w/2;
		pLineV += w/2;
		pLine += w*2;
	}
}
//将YUV422平面图片格式(按Y-U-V存放)转换为YUV422视频数据格式（按象素YUYVYUYV...）

void CVFWCAPTUREDlg::CombineYUVData(
	    int w, int h,//图像大小
		BYTE *pYUV, //YUV422视频图像指针
		BYTE *pY, BYTE *pU, BYTE *pV//YUV422平面图片指针
		)
{
	BYTE* pLine = pYUV;
	BYTE* pLineY = pY;
	BYTE* pLineU = pU;
	BYTE* pLineV = pV;
	for(int j=0;j<h;j++)
	{
		//Y
		for(int i=0;i<w;i++)//一行处理
		{
			pLine[i*2] = pLineY[i];
		}

		for( i=0;i<w/2;i++)
		{
			pLine[i*4+1] = pLineU[i];
			pLine[i*4+3] = pLineV[i];
		}
		pLineY += w;
		pLineU += w/2;
		pLineV += w/2;
		pLine += w*2;//隔行
	}
}

//将YUV422平面图片格式(按Y-U-V存放)转换为RGB视频数据格式（按象素RGBRGB...）
void CVFWCAPTUREDlg::YUVtoRGB(
	    int w, int h,//图像大小
		BYTE *pRGB, //YUV422视频图像指针
		BYTE *pY, BYTE *pU, BYTE *pV//YUV422平面图片指针
		)
{

	BYTE* pLine = pRGB+w*(h-1)*3;//指向RGB图像的第一行，即存储空间最后一行
	BYTE* pLineY = pY;//指向第一行
	BYTE* pLineU = pU;
	BYTE* pLineV = pV;
	int r, g, b;
    int y, u, v;
	//odd
	for(int j=0;j<h;j++)
	{
		//Y
		for(int i=0;i<w;i++)//一行处理
		{
		y=(int) pLineY[i];
		u=(int) pLineU[i/2];
        v=(int) pLineV[i/2];
         r = (int) (y + (1.370705 * (v-128)));
         g = (int) (y - (0.698001 * (v-128)) - (0.337633 * (u-128)));
         b = (int) (y + (1.732446 * (u-128)));
         if(r > 255) r = 255;
         if(g > 255) g = 255;
         if(b > 255) b = 255;
         if(r < 0) r = 0;
         if(g < 0) g = 0;
         if(b < 0) b = 0;
         pLine[3*i] =  (unsigned  char) b;
		 pLine[3*i+1] =(unsigned  char ) g;
         pLine[3*i+2] =(unsigned  char ) r;
		 }
		pLineY += w;//下移一行
		pLineU += w/2;
		pLineV += w/2;
		pLine -= w*3;//上移一行
	}
}

//将YUV422平面图片格式(按Y-U-V存放)转换为RGB视频数据格式（按象素RGBRGB...）
void CVFWCAPTUREDlg::RGBtoYUV(
	    int w, int h,//图像大小
		BYTE *pRGB, //YUV422视频图像指针
		BYTE *pY, BYTE *pU, BYTE *pV//YUV422平面图片指针
		)
{
	BYTE* pLine = pRGB+w*(h-1)*3;//指向RGB图像的第一行，即存储空间最后一行
	BYTE* pLineY = pY;
	BYTE* pLineU = pU;
	BYTE* pLineV = pV;
    int r, g, b;
    int y, u, v;
	for(int j=0;j<h;j++)
	{
		//Y
		for(int i=0;i<w;i++)//一行处理
		{
		 b=(int)pLine[3*i] ;
		 g=(int)pLine[3*i+1] ;
         r=(int)pLine[3*i+2];
	     y = (int) (0.299*r + 0.587*g+0.114*b);
         if(y > 255) y = 255;
         if(y < 0) y = 0;
         pLineY[i] =(unsigned char) y;
		 if(i%2==0)
		 {
           u = (int) (- 0.1687*r- 0.3313*g+0.5*b + 128);
           v = (int) (0.5*r - 0.4187*g - 0.0813*b + 128);
           pLineU[i/2] =(unsigned char) u;
           pLineV[i/2] =(unsigned char) v;
		 }
		}

		pLineY += w;//下移一行
		pLineU += w/2;
		pLineV += w/2;
		pLine -= w*3;//上移一行
	}

}


