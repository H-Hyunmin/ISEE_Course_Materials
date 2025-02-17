#include "stdafx.h"
#include "ImagePrepare.h"
//
#include "BufStruct.h"
#include "ImageProc.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
/////////////////////////////////////////////////////////////////////////////
// CImagePrepareApp
BEGIN_MESSAGE_MAP(CImagePrepareApp, CWinApp)
	//{{AFX_MSG_MAP(CImagePrepareApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//
/////////////////////////////////////////////////////////////////////////////
// CImagePrepareApp construction
CImagePrepareApp::CImagePrepareApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CImagePrepareApp object
CImagePrepareApp theApp;

char sInfo[] = "人脸跟踪-摄像头视频流图片截取处理插件";
bool bLastPlugin = false;

DLL_EXP void ON_PLUGIN_BELAST(bool bLast)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	bLastPlugin = bLast;
}
//插件名称
DLL_EXP LPCTSTR ON_PLUGININFO(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	return sInfo;
}
//
DLL_EXP void ON_INITPLUGIN(LPVOID lpParameter)
{   
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.Create(IDD_PLUGIN_SETUP);
	//theApp.dlg.ShowWindow(SW_HIDE);
}
DLL_EXP int ON_PLUGINCTRL(int nMode,void* pParameter)
{
//模块状态切换
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nRet = 0;
	return nRet;
}

//测试函数***************************************************************
DLL_EXP void CalGrayGravity(BYTE* graybmp, int w, int h, int x, int y, int left, int top, int width, int height, int* outX, int* outY) {
    // 累加变量
    int sumGray = 0;
    int sumGrayX = 0;
    int sumGrayY = 0;

    // 遍历指定区域
    for (int j = top; j < top + height; ++j) {
        for (int i = left; i < left + width; ++i) {
            // 获取当前像素的灰度值
            int grayValue = graybmp[j * w + i];

            // 累加灰度值和加权坐标
            sumGray += grayValue;
            sumGrayX += grayValue * (i - x);
            sumGrayY += grayValue * (j - y);
        }
    }

    // 计算相对于 (x, y) 的灰度重心坐标
    double relativeGravityX = 0;
    double relativeGravityY = 0;
    if (sumGray != 0) {
        relativeGravityX = static_cast<double>(sumGrayX) / sumGray;
        relativeGravityY = static_cast<double>(sumGrayY) / sumGray;
    }

    // 转换为 graybmp 上的绝对坐标
    double absoluteGravityX = relativeGravityX + x;
    double absoluteGravityY = relativeGravityY + y;

    //舍入取整
    *outX = static_cast<int>(absoluteGravityX + 0.5);
    *outY = static_cast<int>(absoluteGravityY + 0.5);
}
//测试函数***************************************************************

/****************************************************************************************/
/*                             摄像头视频流图片截取、重采样等处理                       */
/****************************************************************************************/



DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
//pYBits 大小为w*h
//pUBits 和 pVBits 的大小为 w*h/2
//pBuffer 的大小为 w*h*6
//下面算法都基于一个假设，即w是16的倍数
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
   
	//ShowDebugMessage("与printf函数用法相似,X=%d,Y=%d\n",10,5);
    //请编写相应处理程序
	_BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer; //pBuffer指向的内存块中存放了图像处理过程中需要用到的数据结构

    if (pBufStruct->bNotInited)
    {	
		//(1)
        pBufStruct->colorBmp = pBuffer + sizeof(BUF_STRUCT);
        pBufStruct->grayBmp = pBufStruct->colorBmp;
        pBufStruct->clrBmp_1d8 = pBufStruct->grayBmp + w * h * 2;
        pBufStruct->grayBmp_1d16 = pBufStruct->clrBmp_1d8 + w * h / 4;
        pBufStruct->TempImage1d8 = pBufStruct->grayBmp_1d16 + w * h / 16;
        pBufStruct->lastImageQueue1d16m8 = pBufStruct->TempImage1d8 + w * h / 8;
        pBufStruct->pOtherVars = (OTHER_VARS*)(pBufStruct->lastImageQueue1d16m8 + w * h / 2);
        pBufStruct->pOtherData = (aBYTE*)(pBufStruct->pOtherVars) + sizeof(OTHER_VARS);


        for (int i = 0; i < 8; ++i)
        {
            pBufStruct->pImageQueue[i] = pBufStruct->lastImageQueue1d16m8 + i * (w * h / 16);
        }
		//(2)
		pBufStruct->W = w;
		pBufStruct->H = h;
		pBufStruct->cur_allocSize = 0;
		pBufStruct->allocTimes = 0;
		pBufStruct->cur_maxallocsize = 0;
		pBufStruct->bLastEyeChecked = false;
		pBufStruct->EyeBallConfirm = true;
		pBufStruct->EyePosConfirm = true;	
		pBufStruct->nImageQueueIndex = -1;
		pBufStruct->nLastImageIndex = -1;

		//(3)
		for (i=0; i<=255; i++)
		{
			pBufStruct->pOtherVars->byHistMap_U[i] = 0;
			pBufStruct->pOtherVars->byHistMap_V[i] = 0;
		}
		for (i=85; i<=126; i++)
		{
			pBufStruct->pOtherVars->byHistMap_U[i] = 1;
		}
		for (i=130; i<=165; i++)
		{
			pBufStruct->pOtherVars->byHistMap_V[i] = 1;
		}
		//(4)初始化眼鼻跟踪体
		aRect tempaRect={0,0,0,0};
		FeatureVector tempFeatureVector= {sizeof(FeatureVector4P)*5,0};

		pBufStruct->pOtherVars->objNose.rcObject = tempaRect;
		//把FeatureVector初始化为sizeof(FeatureVector4P)*5和全为0的Vector向量，
		pBufStruct->pOtherVars->objNose.fvObject = tempFeatureVector;
		pBufStruct->pOtherVars->objNose.fvObject_org=tempFeatureVector;
		pBufStruct->pOtherVars->objNose.spdxObj = 0;
		pBufStruct->pOtherVars->objNose.spdyObj = 0;
		pBufStruct->pOtherVars->objNose.nMinDist = 0x7fffffff;
		pBufStruct->pOtherVars->objNose.bBrokenTrace = false;
		pBufStruct->pOtherVars->objNose.bSaveit = false;
		pBufStruct->pOtherVars->objNose.nBrokenTimes = 0;
		strcpy(pBufStruct->pOtherVars->objNose.sName,"Nose");
		
		pBufStruct->pOtherVars->objLefteye.rcObject = tempaRect;
		//把FeatureVector初始化为sizeof(FeatureVector4P)*5和全为0的Vector向量，
		pBufStruct->pOtherVars->objLefteye.fvObject = tempFeatureVector;
		pBufStruct->pOtherVars->objLefteye.fvObject_org = tempFeatureVector;
		pBufStruct->pOtherVars->objLefteye.spdxObj = 0;
		pBufStruct->pOtherVars->objLefteye.spdyObj = 0;
		pBufStruct->pOtherVars->objLefteye.nMinDist = 0x7fffffff;
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace = false;
		pBufStruct->pOtherVars->objLefteye.bSaveit = false;
		pBufStruct->pOtherVars->objLefteye.nBrokenTimes = 0;
		strcpy(pBufStruct->pOtherVars->objLefteye.sName, "Leye");

		pBufStruct->pOtherVars->objRighteye.rcObject = tempaRect;
		//把FeatureVector初始化为sizeof(FeatureVector4P)*5和全为0的Vector向量，
		pBufStruct->pOtherVars->objRighteye.fvObject = tempFeatureVector;
		pBufStruct->pOtherVars->objRighteye.fvObject_org = tempFeatureVector;
		pBufStruct->pOtherVars->objRighteye.spdxObj = 0;
		pBufStruct->pOtherVars->objRighteye.spdyObj = 0;
		pBufStruct->pOtherVars->objRighteye.nMinDist = 0x7fffffff;
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace = false;
		pBufStruct->pOtherVars->objRighteye.bSaveit = false;
		pBufStruct->pOtherVars->objRighteye.nBrokenTimes = 0;
		strcpy(pBufStruct->pOtherVars->objRighteye.sName, "Reye");

		//(5)待处理
		//pBufStruct->pOtherVars->FeaProcBuf
		//pBufStruct->pOtherVars->TraceProcBuf
		//(6)
		pBufStruct->max_allocSize = w * h * 17 / 16 - sizeof(BUF_STRUCT) - sizeof(OTHER_VARS);//最大分配大小，注意要减去BUF_STRUCT和OTHER_VARS的大小
        pBufStruct->bNotInited = false;
		//(7)
		myHeapAllocInit(pBufStruct);
    }
	pBufStruct->displayImage=pYBits;

	//将pYBits中的数据复制到colorBmp中
	memcpy(pBufStruct->colorBmp, pYBits, w * h );
	memcpy(pBufStruct->colorBmp + w * h, pUBits, w * h / 2);
	memcpy(pBufStruct->colorBmp + w * h + w * h / 2, pVBits, w * h / 2);

	ReSample(pBufStruct->colorBmp, w, h, w/2, h/4, false, false, pBufStruct->clrBmp_1d8);
	ReSample(pBufStruct->grayBmp, w, h, w/4, h/4, false, true, pBufStruct->grayBmp_1d16);


	//下面的步骤用于测试图像产生的结果是否正确。
	
		if( bLastPlugin )
		{
		
			//测试grayBmp_1d16:将grayBmp_1d16复制到显示图片的左上角
			CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, 0, 0, true);
			//测试colorBmp:将clrBmp_1d8复制到显示图片的右上角
			CopyToRect(pBufStruct->clrBmp_1d8,  pYBits, w/2, h/4, w, h, w/2, 0, false);

			//测试CalGrayGravity函数
			// int Grayx=0,Grayy=0;
			// CalGrayGravity(pBufStruct->grayBmp,w,h,w/2,h/2,w/2,h/2,w/2,h/2,&Grayx,&Grayy);
			// DrawCross(pBufStruct->displayImage, w, h, Grayx, Grayy, 20, TYUV1(0, 191, 255), false);

			// CalGrayGravity(pBufStruct->grayBmp,w,h,w/2,h/2,0,0,w/2,h/2,&Grayx,&Grayy);
			// DrawCross(pBufStruct->displayImage, w, h, Grayx, Grayy, 20, TYUV1(0, 191, 255), false);

			// CalGrayGravity(pBufStruct->grayBmp,w,h,w/2,h/2,w/2,0,w/2,h/2,&Grayx,&Grayy);
			// DrawCross(pBufStruct->displayImage, w, h, Grayx, Grayy, 20, TYUV1(0, 191, 255), false);

			// CalGrayGravity(pBufStruct->grayBmp,w,h,w/2,h/2,0,h/2,w/2,h/2,&Grayx,&Grayy);
			// DrawCross(pBufStruct->displayImage, w, h, Grayx, Grayy, 20, TYUV1(0, 191, 255), false);
		}
    
}



DLL_EXP void ON_PLUGINEXIT()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.DestroyWindow();
}
