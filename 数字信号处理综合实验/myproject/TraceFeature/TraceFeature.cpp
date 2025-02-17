// TraceFeature.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "TraceFeature.h"
#include "bufstruct.h"
#include "ImageProc.h"

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
// CTraceFeatureApp

BEGIN_MESSAGE_MAP(CTraceFeatureApp, CWinApp)
	//{{AFX_MSG_MAP(CTraceFeatureApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceFeatureApp construction

CTraceFeatureApp::CTraceFeatureApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTraceFeatureApp object
CTraceFeatureApp theApp;
#define ByteToVectorDouble(a) ((int)(a))/255.0
#define WordToVectorDouble(a) ((int)(a))/65535.0
#define MAX_RH	256			//最大特征匹配搜索范围的高度
aBYTE* pBuffer;//256*4*8=8196字节的缓冲区备用
//pLineSumC和pLineSumD是为了快速搜索一个邻域而设置的，具体请看InitFromImageCols函数的说明
aPOINT* pLineSumC[2]={NULL,NULL};
aPOINT* pLineSumD[2][2]={{NULL,NULL},{NULL,NULL}};
/*******************************************************************/
//初始化全局指针变量 pLineSumC、pLineSumD
/*******************************************************************/
DLL_EXP void InitFeatureBuffer(BUF_STRUCT* pBS)
{
	pBuffer = (aBYTE*)pBS->pOtherVars->FeaProcBuf;
	pLineSumC[0] = (aPOINT*)pBuffer;
	pLineSumC[1] = pLineSumC[0]+MAX_RH;
	pLineSumD[0][0] = pLineSumC[1]+MAX_RH;
	pLineSumD[0][1] = pLineSumD[0][0]+MAX_RH;
	pLineSumD[1][0] = pLineSumD[0][1]+MAX_RH;
	pLineSumD[1][1] = pLineSumD[1][0]+MAX_RH;
	//以上占据MAX_RH*6*8字节=12K bytes,剩余4K bytes
	pBuffer = (aBYTE*)(pLineSumD[1][1]+MAX_RH);
}
/*******************************************************************/
//初始化跟踪体的特征，参数为跟踪体结构变量
/*******************************************************************/
DLL_EXP void InitFeatureVector( FeatureVector* pThis)
{
	int i;
	ASSERT(pThis);
    //特征向量以FeatureVector4P形式存放
	FeatureVector4P* pVector = (FeatureVector4P*)pThis->Vector;
	pVector->pNL_LeftTop		= (FeatureVector4P*)((aBYTE*)pVector+sizeof(FeatureVector4P));
	pVector->pNL_RightTop		= (FeatureVector4P*)((aBYTE*)pVector+2*sizeof(FeatureVector4P));
	pVector->pNL_LeftBottom		= (FeatureVector4P*)((aBYTE*)pVector+3*sizeof(FeatureVector4P));
	pVector->pNL_RightBottom	= (FeatureVector4P*)((aBYTE*)pVector+4*sizeof(FeatureVector4P));
	pThis->size = sizeof(FeatureVector4P)*5;//
	pVector->nLevels = 2;
	pVector->pNL_LeftTop->nLevels = 1;
	pVector->pNL_RightTop->nLevels = 1;
	pVector->pNL_LeftBottom->nLevels = 1;
	pVector->pNL_RightBottom->nLevels = 1;
    for(i=0;i<4;i++)
    	{
        pVector->Vector[i].x = pVector->Vector[i].y = 0;//四角重心
        //子区域的四角重心
        pVector->pNL_LeftTop->Vector[i].x = pVector->pNL_LeftTop->Vector[i].y = 0;
        pVector->pNL_RightTop->Vector[i].x = pVector->pNL_RightTop->Vector[i].y = 0;
        pVector->pNL_LeftBottom->Vector[i].x = pVector->pNL_LeftBottom->Vector[i].y = 0;
        pVector->pNL_RightBottom->Vector[i].x = pVector->pNL_RightBottom->Vector[i].y = 0;
    	}
}

