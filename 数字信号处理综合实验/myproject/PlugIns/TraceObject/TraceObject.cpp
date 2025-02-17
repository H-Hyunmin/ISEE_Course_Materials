// TraceObject.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TraceObject.h"
#include "BufStruct.h"
#include "ImageProc.h"
//#include "TraceFeature.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
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
// CTraceObjectApp

BEGIN_MESSAGE_MAP(CTraceObjectApp, CWinApp)
	//{{AFX_MSG_MAP(CTraceObjectApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceObjectApp construction

CTraceObjectApp::CTraceObjectApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTraceObjectApp object

CTraceObjectApp theApp;
#define GOOD_TRACE_DIST 40
// #define MIDD_TRACE_DIST 80
#define BAD_TRACE_DIST 80
#define MAX_DIST_TO_ORG 150

//初始化搜索区域大小
int LefteyeSearchSize=5;
int RighteyeSearchSize=5;
int NoseSearchSize=5;

bool bLastPlugin = false;

DLL_INP void InitFeatureVector(FeatureVector* pThis);
DLL_INP bool UpdateVectorsFrom(FeatureVector* pFV, FeatureVector* aFV,int nOrgWeight);
DLL_INP aPOINT CompareFromImage(FeatureVector* pFV, aBYTE* pImageBits, int nLineW,int nH,
                             aRect rcSampleRC, aRect rcRange,int* nMinDist,FeatureVector* theMinFV);
DLL_INP int FV_Distance(FeatureVector* pFV,FeatureVector* aFV,int nFaceClrWeight,int nLevelWeight);
//DLL_INP bool CopyVectorsFrom4P(FeatureVector* pThis, FeatureVector* fv4p);

DLL_EXP void ON_PLUGIN_BELAST(bool bLast)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	bLastPlugin = bLast;
}

char sInfo[] = "人脸跟踪-眼睛鼻子跟踪处理插件";

DLL_EXP LPCTSTR ON_PLUGININFO(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	return sInfo;
}

DLL_EXP void ON_INITPLUGIN(LPVOID lpParameter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.Create(IDD_PLUGIN_SETUP);
	//theApp.dlg.ShowWindow(SW_HIDE);
}

DLL_EXP int ON_PLUGINCTRL(int nMode,void* pParameter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	int nRet = 0;
	switch(nMode)
	{
	case 0:
		{
			//theApp.dlg.ShowWindow(SW_SHOWNORMAL);
			//theApp.dlg.SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
		}
		break;
	}
	return nRet;
}




/*******************************************************************/
//							myfunction
/*******************************************************************/

//计算灰色图像中某个区域对于某个原点的灰色重心函数
//参数：graybmp,指向灰度图像的指针
//w,h,灰度图像的宽和高
//x,y,原点坐标
//left,top,区域左上角坐标
//width,height,区域宽和高
//outX,outY,计算出的灰度重心在graybmp上的坐标。
//_BUF_STRUCT 用来debug
DLL_EXP void CalGrayGravity(BYTE* graybmp, int w, int h, int x, int y, int left, int top, int width, int height, int* outX, int* outY, _BUF_STRUCT *pBufStruct) {
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
    int absoluteGravityX = static_cast<int>(relativeGravityX + x);
    int absoluteGravityY = static_cast<int>(relativeGravityY + y);
    //debug
    // DrawCross(pBufStruct->displayImage, w, h, absoluteGravityX, absoluteGravityY, 10, TYUV1(0, 151, 120), false);

    // 转换为归一化为(-1024,1024)
    double normalizationGrayx = relativeGravityX * 1024 / (w / 2);
    double normalizationGrayy = relativeGravityY * 1024 / (h / 2);

    //舍入取整
    *outX = static_cast<int>(normalizationGrayx + 0.5);
    *outY = static_cast<int>(normalizationGrayy + 0.5);
}


//计算特征值
//参数：_BUF_STRUCT *pBufStruct指针
//w,h,灰度图像的宽和高
//left,top,区域左上角坐标
//width,height,区域宽和高
//fvp：FeatureVector4P指针指向特征向量处
DLL_EXP void CalEigen(_BUF_STRUCT *pBufStruct,int w, int h,  int left, int top, int width, int height,FeatureVector4P* fvp){
    //四个地址指针
    FeatureVector4P* fvp_pNL_LeftTop=fvp + 1;
    FeatureVector4P* fvp_pNL_RightTop=fvp+2;
    FeatureVector4P* fvp_pNL_LeftBottom=fvp+3;
    FeatureVector4P* fvp_pNL_RightBottom=fvp+4;
    int Grayx=0,Grayy=0;//临时存储
    //计算跟踪体左上角区域的灰度重心
    //计算跟踪体中心
    int centreX = left + width / 2;
    int centreY = top + height / 2;
    //计算左上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[0].x = Grayx;
    fvp->Vector[0].y = Grayy;
    //计算右上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[1].x = Grayx;
    fvp->Vector[1].y = Grayy;
    //计算左下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 2, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[2].x = Grayx;
    fvp->Vector[2].y = Grayy;
    //计算右下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 2, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[3].x = Grayx;
    fvp->Vector[3].y = Grayy;
    //其它参数
    fvp->nLevels=2;
    fvp->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp->pNL_RightTop=fvp_pNL_RightTop;
    fvp->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp->pNL_RightBottom=fvp_pNL_RightBottom;

    //计算左上角区域的特征值
    centreX = left + width / 4;
    centreY = top + height / 4;
    //计算左上角区域的左上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[0].x = Grayx;
    fvp_pNL_LeftTop->Vector[0].y = Grayy;
    //计算左上角区域的右上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[1].x = Grayx;
    fvp_pNL_LeftTop->Vector[1].y = Grayy;
    //计算左上角区域的左下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[2].x = Grayx;
    fvp_pNL_LeftTop->Vector[2].y = Grayy;
    //计算左上角区域的右下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[3].x = Grayx;
    fvp_pNL_LeftTop->Vector[3].y = Grayy;
    //其它参数
    fvp_pNL_LeftTop->nLevels=1;
    fvp_pNL_LeftTop->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_LeftTop->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_LeftTop->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_LeftTop->pNL_RightBottom=fvp_pNL_RightBottom;

    //计算右上角区域的特征值
    centreX = left + width * 3 / 4;
    centreY = top + height / 4;
    //计算右上角区域的左上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[0].x = Grayx;
    fvp_pNL_RightTop->Vector[0].y = Grayy;
    //计算右上角区域的右上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[1].x = Grayx;
    fvp_pNL_RightTop->Vector[1].y = Grayy;
    //计算右上角区域的左下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[2].x = Grayx;
    fvp_pNL_RightTop->Vector[2].y = Grayy;
    //计算右上角区域的右下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[3].x = Grayx;
    fvp_pNL_RightTop->Vector[3].y = Grayy;
    //其它参数
    fvp_pNL_RightTop->nLevels=1;
    fvp_pNL_RightTop->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_RightTop->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_RightTop->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_RightTop->pNL_RightBottom=fvp_pNL_RightBottom;

    //计算左下角区域的特征值
    centreX = left + width / 4;
    centreY = top + height * 3 / 4;
    //计算左下角区域的左上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[0].x = Grayx;
    fvp_pNL_LeftBottom->Vector[0].y = Grayy;
    //计算左下角区域的右上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[1].x = Grayx;
    fvp_pNL_LeftBottom->Vector[1].y = Grayy;
    //计算左下角区域的左下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[2].x = Grayx;
    fvp_pNL_LeftBottom->Vector[2].y = Grayy;
    //计算左下角区域的右下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[3].x = Grayx;
    fvp_pNL_LeftBottom->Vector[3].y = Grayy;
    //其它参数
    fvp_pNL_LeftBottom->nLevels=1;
    fvp_pNL_LeftBottom->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_LeftBottom->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_LeftBottom->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_LeftBottom->pNL_RightBottom=fvp_pNL_RightBottom;

    //计算右下角区域的特征值
    centreX = left + width * 3 / 4;
    centreY = top + height * 3 / 4;
    //计算右下角区域的左上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[0].x = Grayx;
    fvp_pNL_RightBottom->Vector[0].y = Grayy;
    //计算右下角区域的右上角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[1].x = Grayx;
    fvp_pNL_RightBottom->Vector[1].y = Grayy;
    //计算右下角区域的左下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[2].x = Grayx;
    fvp_pNL_RightBottom->Vector[2].y = Grayy;
    //计算右下角区域的右下角区域的灰度重心
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[3].x = Grayx;
    fvp_pNL_RightBottom->Vector[3].y = Grayy;
    //其它参数
    fvp_pNL_RightBottom->nLevels=1;
    fvp_pNL_RightBottom->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_RightBottom->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_RightBottom->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_RightBottom->pNL_RightBottom=fvp_pNL_RightBottom;

    


}

//计算特征值差
DLL_EXP int CalEigenDiff(FeatureVector4P* fvp1, FeatureVector4P* fvp2) {
	int diff = 0;
	for (int i = 0; i < 4; ++i) {
		diff += abs(fvp1->Vector[i].x - fvp2->Vector[i].x);
		diff += abs(fvp1->Vector[i].y - fvp2->Vector[i].y);
		diff += abs(fvp1->pNL_LeftTop->Vector[i].x - fvp2->pNL_LeftTop->Vector[i].x);
		diff += abs(fvp1->pNL_LeftTop->Vector[i].y - fvp2->pNL_LeftTop->Vector[i].y);
		diff += abs(fvp1->pNL_RightTop->Vector[i].x - fvp2->pNL_RightTop->Vector[i].x);
		diff += abs(fvp1->pNL_RightTop->Vector[i].y - fvp2->pNL_RightTop->Vector[i].y);
		diff += abs(fvp1->pNL_LeftBottom->Vector[i].x - fvp2->pNL_LeftBottom->Vector[i].x);
		diff += abs(fvp1->pNL_LeftBottom->Vector[i].y - fvp2->pNL_LeftBottom->Vector[i].y);
		diff += abs(fvp1->pNL_RightBottom->Vector[i].x - fvp2->pNL_RightBottom->Vector[i].x);
		diff += abs(fvp1->pNL_RightBottom->Vector[i].y - fvp2->pNL_RightBottom->Vector[i].y);
	}
	return diff;
}

//中和替换
DLL_EXP void neutralization_replacement(FeatureVector4P* fvp, FeatureVector4P* fvp_org){
    double weight = 0.5;
    for (int i = 0; i < 4; ++i) {
        fvp->Vector[i].x = static_cast<int>(fvp->Vector[i].x * weight + fvp_org->Vector[i].x * (1 - weight));
        fvp->Vector[i].y = static_cast<int>(fvp->Vector[i].y * weight + fvp_org->Vector[i].y * (1 - weight));
        fvp->pNL_LeftTop->Vector[i].x = static_cast<int>(fvp->pNL_LeftTop->Vector[i].x * weight + fvp_org->pNL_LeftTop->Vector[i].x * (1 - weight));
        fvp->pNL_LeftTop->Vector[i].y = static_cast<int>(fvp->pNL_LeftTop->Vector[i].y * weight + fvp_org->pNL_LeftTop->Vector[i].y * (1 - weight));
        fvp->pNL_RightTop->Vector[i].x = static_cast<int>(fvp->pNL_RightTop->Vector[i].x * weight + fvp_org->pNL_RightTop->Vector[i].x * (1 - weight));
        fvp->pNL_RightTop->Vector[i].y = static_cast<int>(fvp->pNL_RightTop->Vector[i].y * weight + fvp_org->pNL_RightTop->Vector[i].y * (1 - weight));
        fvp->pNL_LeftBottom->Vector[i].x = static_cast<int>(fvp->pNL_LeftBottom->Vector[i].x * weight + fvp_org->pNL_LeftBottom->Vector[i].x * (1 - weight));
        fvp->pNL_LeftBottom->Vector[i].y = static_cast<int>(fvp->pNL_LeftBottom->Vector[i].y * weight + fvp_org->pNL_LeftBottom->Vector[i].y * (1 - weight));
        fvp->pNL_RightBottom->Vector[i].x = static_cast<int>(fvp->pNL_RightBottom->Vector[i].x * weight + fvp_org->pNL_RightBottom->Vector[i].x * (1 - weight));
        fvp->pNL_RightBottom->Vector[i].y = static_cast<int>(fvp->pNL_RightBottom->Vector[i].y * weight + fvp_org->pNL_RightBottom->Vector[i].y * (1 - weight));
    }
}

/*******************************************************************/
//眼鼻跟踪
/*******************************************************************/
DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	 //请编写相应处理程序
	_BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer;
	int RW=0,RH=0;
	int W=0,H=0;
	//搜索区域的左边边界坐标Rx,上面边界坐标Ry
	int Rx=0,Ry=0;
	//Tx、Ty 分别跟踪体在上一帧的左边边界坐标、上面边界坐标。
	// spdxObj、spdyObj分别为跟踪体水平方向、垂直方向运动速度，符号表示运动方向
	int Tx=0,Ty=0,spdxObj=0,spdyObj=0;
	if (!pBufStruct->bLastEyeChecked)//未建立跟踪体
	{
		return;
	}
	//左眼跟踪
	W=pBufStruct->pOtherVars->objLefteye.rcObject.width;
	H=pBufStruct->pOtherVars->objLefteye.rcObject.height; 
	RW = pBufStruct->pOtherVars->objLefteye.rcObject.width+2*LefteyeSearchSize;
	RH = pBufStruct->pOtherVars->objLefteye.rcObject.height+2*LefteyeSearchSize;
	//搜索区域的左边边界坐标 Rx、上面边界坐标 Ry
	//Tx、Ty 分别跟踪体在上一帧的左边边界坐标、上面边界坐标。
	// spdxObj、spdyObj分别为跟踪体水平方向、垂直方向运动速度，符号表示运动方向
	Tx=pBufStruct->pOtherVars->objLefteye.rcObject.left;
	Ty=pBufStruct->pOtherVars->objLefteye.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objLefteye.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objLefteye.spdyObj;
	Rx=Tx-LefteyeSearchSize+spdxObj;
	Ry=Ty-LefteyeSearchSize+spdyObj;


	// //debug绘制搜索区域
	// aRect debugrec;
	// debugrec.left=Rx;
	// debugrec.top=Ry;
	// debugrec.width=RW;
	// debugrec.height=RH;
	// DrawRectangle(pBufStruct->displayImage, w, h,pBufStruct->pOtherVars->objLefteye.rcObject, TYUV1(120, 151, 0), false);
	// DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 0, 158), false);
	// return;

	//脸部修正使整个搜索区域在脸部范围内
	//脸部坐标pBufStruct->rcnFace
	aRect facerec;
	facerec.left=pBufStruct->rcnFace.left*2;
	facerec.top=pBufStruct->rcnFace.top*4;
	facerec.width=pBufStruct->rcnFace.width*2;
	facerec.height=pBufStruct->rcnFace.height*4;


	if (Rx <facerec.left) {
		Rx = facerec.left;
	}
	if (Ry < facerec.top) {
		Ry = facerec.top;
	}
	if (Rx + RW > facerec.left + facerec.width) {
		Rx = facerec.left + facerec.width - RW;
	}
	if (Ry + RH > facerec.top + facerec.height) {
		Ry = facerec.top + facerec.height - RH;
	}


	//2.搜索最佳匹配点
	// 	在搜索区域内，自左向右、先上后下移动跟踪体，每移动一个像素，计算跟踪体的特征
	// 并计算与当前特征的差距，找到一个最佳匹配点--特征差距最小。记录最佳匹配点的中心位
	// 置 ptNew 、最小特征差距及特征值 theMinFV 。最小特征差距直接记录在结构体
	// TRACE_OBJEC 中的变量 nMinDist。
	int nMinDist=0,nDist=0,nDistFromOrg=0;
	POINT ptNew;
	FeatureVector4P* temp = new FeatureVector4P[5];//临时特征向量
	FeatureVector4P* theMinFV = new FeatureVector4P[5];//最小特征向量
	//在搜索区域内，自左向右、先上后下移动跟踪体，每移动一个像素，计算跟踪体的特征
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objLefteye.fvObject.Vector,temp);

	for (int i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//计算特征值
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//计算特征值差
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objLefteye.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//赋初值
			{
				nMinDist=nDist;
				ptNew.x=i+W/2;
				ptNew.y=j+H/2;
				memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
			}
			else
			{
				if (nDist<nMinDist)
				{
					nMinDist=nDist;
					ptNew.x=i+W/2;
					ptNew.y=j+H/2;
					memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
				}
			}
		}
	}
	
	// 3.更新跟踪体位置、运动速度及当前特征值
	//1）更新跟踪体位置矩形（object->rcObject）到最佳匹配处的位置。
	POINT ptOld;
	ptOld.x=pBufStruct->pOtherVars->objLefteye.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objLefteye.rcObject.top+H/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.width=W;
	pBufStruct->pOtherVars->objLefteye.rcObject.height=H;

	// 	2）运动速度的更新，由原速度与本帧与上帧运动加权得到，权值与最小特征差距有关，
	// 式 5.26 给出算法。
	pBufStruct->pOtherVars->objLefteye.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objLefteye.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));


	// 3.跟踪结果判断方法
	// （1）以最佳特征与当前特征的差距nMinDist为判断依据，设置GOOD_TRACE_DIST、
	// BAD_TRACE_DIST两个阈值。
	// 当匹配良好，即nMinDist≤GOOD_TRACE_DIST时，判定跟踪有效。
	// 当匹配中等，OOD_TRACE_DIST<nMinDist≤BAD_TRACE_DIST时，认为基本还
	// 能跟踪，也有可能丢失跟踪体，因此，记跟踪疑似失败一次。
	// 当匹配很差，即nMinDist>BAD_TRACE_DIST时，判定丢失跟踪体。
	// （2）当前特征与原始特征的差距nDistFromOrg太大，也判定丢失跟踪体。这里再设一
	// 个阈值MAX_DIST_TO_ORG，当nDistFromOrg>MAX_DIST_TO_ORG时，判定丢失跟踪体。
	// 当前特征更新
	// 匹配良好，用最佳特征直接替换当前特征。
	// 匹配中等，用原始特征中和替换当前特征，如式 5.27 所示。
	// 匹配很差，用原始特征直接替换当前特征。
	// 当前特征与距离原始特征太远，用原始特征直接替换当前特征。
	if (nMinDist<=GOOD_TRACE_DIST)//判断良好
	{
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objLefteye.nBrokenTimes=0;//清除跟踪疑似失败次数
		pBufStruct->pOtherVars->objLefteye.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//清除跟踪失败标志
		LefteyeSearchSize=5;//搜索区域大小
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//判断中等
	{	
		//用原始特征中和替换当前特征
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector);
		pBufStruct->pOtherVars->objLefteye.nBrokenTimes++;//跟踪疑似失败一次
		pBufStruct->pOtherVars->objLefteye.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//清除跟踪失败标志
		LefteyeSearchSize=10;//搜索区域大小
	}
	else if (nMinDist>BAD_TRACE_DIST)//判断很差
	{
		//用原始特征替换当前特征
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		LefteyeSearchSize=15;//搜索区域大小
		if (pBufStruct->pOtherVars->objLefteye.bSaveit)//已经丢失过跟踪体
		{
			pBufStruct->pOtherVars->objLefteye.bBrokenTrace=true;//跟踪失败
		}
		else
		{
			pBufStruct->pOtherVars->objLefteye.bSaveit=true;//丢失跟踪体
			pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//清除跟踪失败标志
			pBufStruct->pOtherVars->objLefteye.nBrokenTimes++;//跟踪疑似失败一次
		}
	}
	if (nDistFromOrg>MAX_DIST_TO_ORG)//距离原始特征太远
	{
		//用原始特征替换当前特征
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objLefteye.bSaveit=true;//置丢失跟踪体
	}
	if (pBufStruct->pOtherVars->objLefteye.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=true;//跟踪失败
	}
	// 绘制跟踪体中心
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// 绘制跟踪体矩形
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objLefteye.rcObject, TYUV1(120, 151, 255), false);
	// 绘制左眼搜索框
	aRect debugrec;
	debugrec.left=Rx;
	debugrec.top=Ry;
	debugrec.width=RW;
	debugrec.height=RH;
	DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 151, 0), false);

	//右眼跟踪
	W=pBufStruct->pOtherVars->objRighteye.rcObject.width;
	H=pBufStruct->pOtherVars->objRighteye.rcObject.height;
	RW = pBufStruct->pOtherVars->objRighteye.rcObject.width+2*RighteyeSearchSize;
	RH = pBufStruct->pOtherVars->objRighteye.rcObject.height+2*RighteyeSearchSize;
	//搜索区域的左边边界坐标 Rx、上面边界坐标 Ry
	//Tx、Ty 分别跟踪体在上一帧的左边边界坐标、上面边界坐标。
	// spdxObj、spdyObj分别为跟踪体水平方向、垂直方向运动速度，符号表示运动方向
	Tx=pBufStruct->pOtherVars->objRighteye.rcObject.left;
	Ty=pBufStruct->pOtherVars->objRighteye.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objRighteye.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objRighteye.spdyObj;
	Rx=Tx-RighteyeSearchSize+spdxObj;
	Ry=Ty-RighteyeSearchSize+spdyObj;

	//脸部修正使整个搜索区域在脸部范围内
	//脸部坐标pBufStruct->rcnFace
	facerec.left=pBufStruct->rcnFace.left*2;
	facerec.top=pBufStruct->rcnFace.top*4;
	facerec.width=pBufStruct->rcnFace.width*2;
	facerec.height=pBufStruct->rcnFace.height*4;
	if (Rx<facerec.left)
	{
		Rx=facerec.left;
	}
	if (Ry<facerec.top)
	{
		Ry=facerec.top;
	}
	if (Rx+RW>facerec.left+facerec.width)
	{
		Rx=facerec.left+facerec.width-RW;
	}
	if (Ry+RH>facerec.top+facerec.height)
	{
		Ry=facerec.top+facerec.height-RH;
	}
	
	//在搜索区域内，自左向右、先上后下移动跟踪体，每移动一个像素，计算跟踪体的特征
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objRighteye.fvObject.Vector,temp);

	for ( i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//计算特征值
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//计算特征值差
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objRighteye.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//赋初值
			{
				nMinDist=nDist;
				ptNew.x=i+W/2;
				ptNew.y=j+H/2;
				memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
			}
			else
			{
				if (nDist<nMinDist)
				{
					nMinDist=nDist;
					ptNew.x=i+W/2;
					ptNew.y=j+H/2;
					memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
				}
			}
		}

	}

	// 3.更新跟踪体位置、运动速度及当前特征值
	//1）更新跟踪体位置矩形（object->rcObject）到最佳匹配处的位置。
	ptOld.x=pBufStruct->pOtherVars->objRighteye.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objRighteye.rcObject.top+H/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.width=W;
	pBufStruct->pOtherVars->objRighteye.rcObject.height=H;

	// 	2）运动速度的更新，由原速度与本帧与上帧运动加权得到，权值与最小特征差距有关，
	// 式 5.26 给出算法。
	pBufStruct->pOtherVars->objRighteye.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objRighteye.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));

	// 	3）更新当前特征值。
	if (nMinDist<=GOOD_TRACE_DIST)//判断良好
	{
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objRighteye.nBrokenTimes=0;//清除跟踪疑似失败次数
		pBufStruct->pOtherVars->objRighteye.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//清除跟踪失败标志
		RighteyeSearchSize=5;//搜索区域大小
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//判断中等
	{	
		//用原始特征中和替换当前特征
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector);
		pBufStruct->pOtherVars->objRighteye.nBrokenTimes++;//跟踪疑似失败一次
		pBufStruct->pOtherVars->objRighteye.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//清除跟踪失败标志
		RighteyeSearchSize=10;//搜索区域大小
	}
	else if (nMinDist>BAD_TRACE_DIST)//判断很差
	{
		//用原始特征替换当前特征
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		RighteyeSearchSize=15;//搜索区域大小
		if (pBufStruct->pOtherVars->objRighteye.bSaveit)//已经丢失过跟踪体
		{
			pBufStruct->pOtherVars->objRighteye.bBrokenTrace=true;//跟踪失败
		}
		else
		{
			pBufStruct->pOtherVars->objRighteye.bSaveit=true;//丢失跟踪体
			pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//清除跟踪失败标志
			pBufStruct->pOtherVars->objRighteye.nBrokenTimes++;//跟踪疑似失败一次
		}
	}
	if (nDistFromOrg>MAX_DIST_TO_ORG)//距离原始特征太远
	{
		//用原始特征替换当前特征
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objRighteye.bSaveit=true;//置丢失跟踪体
	}
	if (pBufStruct->pOtherVars->objRighteye.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=true;//跟踪失败
	}
	// 绘制跟踪体中心
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// 绘制跟踪体矩形
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objRighteye.rcObject, TYUV1(120, 151, 255), false);
	// 绘制右眼搜索框
	debugrec.left=Rx;
	debugrec.top=Ry;
	debugrec.width=RW;
	debugrec.height=RH;
	DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 151, 0), false);

	//鼻子跟踪
	W=pBufStruct->pOtherVars->objNose.rcObject.width;
	H=pBufStruct->pOtherVars->objNose.rcObject.height;
	RW = pBufStruct->pOtherVars->objNose.rcObject.width+2*NoseSearchSize;
	RH = pBufStruct->pOtherVars->objNose.rcObject.height+2*NoseSearchSize;
	//搜索区域的左边边界坐标 Rx、上面边界坐标 Ry
	//Tx、Ty 分别跟踪体在上一帧的左边边界坐标、上面边界坐标。
	// spdxObj、spdyObj分别为跟踪体水平方向、垂直方向运动速度，符号表示运动方向
	Tx=pBufStruct->pOtherVars->objNose.rcObject.left;
	Ty=pBufStruct->pOtherVars->objNose.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objNose.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objNose.spdyObj;
	Rx=Tx-NoseSearchSize+spdxObj;
	Ry=Ty-NoseSearchSize+spdyObj;

	//脸部修正使整个搜索区域在脸部范围内
	//脸部坐标pBufStruct->rcnFace
	facerec.left=pBufStruct->rcnFace.left*2;
	facerec.top=pBufStruct->rcnFace.top*4;
	facerec.width=pBufStruct->rcnFace.width*2;
	facerec.height=pBufStruct->rcnFace.height*4;
	if (Rx<facerec.left)
	{
		Rx=facerec.left;
	}
	if (Ry<facerec.top)
	{
		Ry=facerec.top;
	}
	if (Rx+RW>facerec.left+facerec.width)
	{
		Rx=facerec.left+facerec.width-RW;
	}
	if (Ry+RH>facerec.top+facerec.height)
	{
		Ry=facerec.top+facerec.height-RH;
	}
	//计算鼻子特征向量
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objNose.fvObject.Vector,temp);
	
	for ( i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//计算特征值
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//计算特征值差
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objNose.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//赋初值
			{
				nMinDist=nDist;
				ptNew.x=i+W/2;
				ptNew.y=j+H/2;
				memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
			}
			else
			{
				if (nDist<nMinDist)
				{
					nMinDist=nDist;
					ptNew.x=i+W/2;
					ptNew.y=j+H/2;
					memcpy(theMinFV,temp,sizeof(FeatureVector4P)*5);
				}
			}
		}

	}

	// 3.更新跟踪体位置、运动速度及当前特征值
	//1）更新跟踪体位置矩形（object->rcObject）到最佳匹配处的位置。
	ptOld.x=pBufStruct->pOtherVars->objNose.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objNose.rcObject.top+H/2;
	pBufStruct->pOtherVars->objNose.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objNose.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objNose.rcObject.width=W;
	pBufStruct->pOtherVars->objNose.rcObject.height=H;

	// 	2）运动速度的更新，由原速度与本帧与上帧运动加权得到，权值与最小特征差距有关，
	// 式 5.26 给出算法。
	pBufStruct->pOtherVars->objNose.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objNose.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));



	// 	3）更新当前特征值。
	if (nMinDist<=GOOD_TRACE_DIST)//判断良好
	{
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objNose.nBrokenTimes=0;//清除跟踪疑似失败次数
		pBufStruct->pOtherVars->objNose.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//清除跟踪失败标志
		NoseSearchSize=5;//搜索区域大小
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//判断中等
	{	
		//用原始特征中和替换当前特征
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject_org.Vector);
		pBufStruct->pOtherVars->objNose.nBrokenTimes++;//跟踪疑似失败一次
		pBufStruct->pOtherVars->objNose.bSaveit=false;//清除跟踪丢失标志
		pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//清除跟踪失败标志
		NoseSearchSize=10;//搜索区域大小
	}


	else if (nMinDist>BAD_TRACE_DIST)//判断很差
	{
		//用原始特征替换当前特征
		// TODO:这段有bug？？？
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,pBufStruct->pOtherVars->objNose.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		NoseSearchSize=15;//搜索区域大小
		if (pBufStruct->pOtherVars->objNose.bSaveit)//已经丢失过跟踪体
		{
			pBufStruct->pOtherVars->objNose.bBrokenTrace=true;//跟踪失败标志
		}
		else
		{
			pBufStruct->pOtherVars->objNose.bSaveit=true;//丢失跟踪体
			pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//清除跟踪失败标志
			pBufStruct->pOtherVars->objNose.nBrokenTimes++;//跟踪疑似失败一次
		}
	}


	if (nDistFromOrg>MAX_DIST_TO_ORG)//距离原始特征太远
	{
		//用原始特征替换当前特征
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,pBufStruct->pOtherVars->objNose.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objNose.bSaveit=true;//置丢失跟踪体
	}
	if (pBufStruct->pOtherVars->objNose.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objNose.bBrokenTrace=true;//跟踪失败
	}


	// 绘制跟踪体中心
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// 绘制跟踪体矩形
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objNose.rcObject, TYUV1(120, 151, 255), false);
	// 绘制鼻子搜索框
	debugrec.left=Rx;
	debugrec.top=Ry;
	debugrec.width=RW;
	debugrec.height=RH;
	DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 151, 0), false);









	delete[] temp;
	delete[] theMinFV;
	
}
/*******************************************************************/

/*******************************************************************/
DLL_EXP void ON_PLUGINEXIT()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.DestroyWindow();
}

