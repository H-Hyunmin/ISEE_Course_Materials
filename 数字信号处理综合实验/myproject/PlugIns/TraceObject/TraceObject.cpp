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

//��ʼ�����������С
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	bLastPlugin = bLast;
}

char sInfo[] = "��������-�۾����Ӹ��ٴ�����";

DLL_EXP LPCTSTR ON_PLUGININFO(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	return sInfo;
}

DLL_EXP void ON_INITPLUGIN(LPVOID lpParameter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//theApp.dlg.Create(IDD_PLUGIN_SETUP);
	//theApp.dlg.ShowWindow(SW_HIDE);
}

DLL_EXP int ON_PLUGINCTRL(int nMode,void* pParameter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
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

//�����ɫͼ����ĳ���������ĳ��ԭ��Ļ�ɫ���ĺ���
//������graybmp,ָ��Ҷ�ͼ���ָ��
//w,h,�Ҷ�ͼ��Ŀ�͸�
//x,y,ԭ������
//left,top,�������Ͻ�����
//width,height,�����͸�
//outX,outY,������ĻҶ�������graybmp�ϵ����ꡣ
//_BUF_STRUCT ����debug
DLL_EXP void CalGrayGravity(BYTE* graybmp, int w, int h, int x, int y, int left, int top, int width, int height, int* outX, int* outY, _BUF_STRUCT *pBufStruct) {
    // �ۼӱ���
    int sumGray = 0;
    int sumGrayX = 0;
    int sumGrayY = 0;

    // ����ָ������
    for (int j = top; j < top + height; ++j) {
        for (int i = left; i < left + width; ++i) {
            // ��ȡ��ǰ���صĻҶ�ֵ
            int grayValue = graybmp[j * w + i];

            // �ۼӻҶ�ֵ�ͼ�Ȩ����
            sumGray += grayValue;
            sumGrayX += grayValue * (i - x);
            sumGrayY += grayValue * (j - y);
        }
    }

    // ��������� (x, y) �ĻҶ���������
    double relativeGravityX = 0;
    double relativeGravityY = 0;
    if (sumGray != 0) {
        relativeGravityX = static_cast<double>(sumGrayX) / sumGray;
        relativeGravityY = static_cast<double>(sumGrayY) / sumGray;
    }
    // ת��Ϊ graybmp �ϵľ�������
    int absoluteGravityX = static_cast<int>(relativeGravityX + x);
    int absoluteGravityY = static_cast<int>(relativeGravityY + y);
    //debug
    // DrawCross(pBufStruct->displayImage, w, h, absoluteGravityX, absoluteGravityY, 10, TYUV1(0, 151, 120), false);

    // ת��Ϊ��һ��Ϊ(-1024,1024)
    double normalizationGrayx = relativeGravityX * 1024 / (w / 2);
    double normalizationGrayy = relativeGravityY * 1024 / (h / 2);

    //����ȡ��
    *outX = static_cast<int>(normalizationGrayx + 0.5);
    *outY = static_cast<int>(normalizationGrayy + 0.5);
}


//��������ֵ
//������_BUF_STRUCT *pBufStructָ��
//w,h,�Ҷ�ͼ��Ŀ�͸�
//left,top,�������Ͻ�����
//width,height,�����͸�
//fvp��FeatureVector4Pָ��ָ������������
DLL_EXP void CalEigen(_BUF_STRUCT *pBufStruct,int w, int h,  int left, int top, int width, int height,FeatureVector4P* fvp){
    //�ĸ���ַָ��
    FeatureVector4P* fvp_pNL_LeftTop=fvp + 1;
    FeatureVector4P* fvp_pNL_RightTop=fvp+2;
    FeatureVector4P* fvp_pNL_LeftBottom=fvp+3;
    FeatureVector4P* fvp_pNL_RightBottom=fvp+4;
    int Grayx=0,Grayy=0;//��ʱ�洢
    //������������Ͻ�����ĻҶ�����
    //�������������
    int centreX = left + width / 2;
    int centreY = top + height / 2;
    //�������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[0].x = Grayx;
    fvp->Vector[0].y = Grayy;
    //�������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[1].x = Grayx;
    fvp->Vector[1].y = Grayy;
    //�������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 2, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[2].x = Grayx;
    fvp->Vector[2].y = Grayy;
    //�������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 2, width / 2, height / 2, &Grayx, &Grayy, pBufStruct);
    fvp->Vector[3].x = Grayx;
    fvp->Vector[3].y = Grayy;
    //��������
    fvp->nLevels=2;
    fvp->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp->pNL_RightTop=fvp_pNL_RightTop;
    fvp->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp->pNL_RightBottom=fvp_pNL_RightBottom;

    //�������Ͻ����������ֵ
    centreX = left + width / 4;
    centreY = top + height / 4;
    //�������Ͻ���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[0].x = Grayx;
    fvp_pNL_LeftTop->Vector[0].y = Grayy;
    //�������Ͻ���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[1].x = Grayx;
    fvp_pNL_LeftTop->Vector[1].y = Grayy;
    //�������Ͻ���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[2].x = Grayx;
    fvp_pNL_LeftTop->Vector[2].y = Grayy;
    //�������Ͻ���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftTop->Vector[3].x = Grayx;
    fvp_pNL_LeftTop->Vector[3].y = Grayy;
    //��������
    fvp_pNL_LeftTop->nLevels=1;
    fvp_pNL_LeftTop->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_LeftTop->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_LeftTop->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_LeftTop->pNL_RightBottom=fvp_pNL_RightBottom;

    //�������Ͻ����������ֵ
    centreX = left + width * 3 / 4;
    centreY = top + height / 4;
    //�������Ͻ���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[0].x = Grayx;
    fvp_pNL_RightTop->Vector[0].y = Grayy;
    //�������Ͻ���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[1].x = Grayx;
    fvp_pNL_RightTop->Vector[1].y = Grayy;
    //�������Ͻ���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[2].x = Grayx;
    fvp_pNL_RightTop->Vector[2].y = Grayy;
    //�������Ͻ���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightTop->Vector[3].x = Grayx;
    fvp_pNL_RightTop->Vector[3].y = Grayy;
    //��������
    fvp_pNL_RightTop->nLevels=1;
    fvp_pNL_RightTop->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_RightTop->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_RightTop->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_RightTop->pNL_RightBottom=fvp_pNL_RightBottom;

    //�������½����������ֵ
    centreX = left + width / 4;
    centreY = top + height * 3 / 4;
    //�������½���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[0].x = Grayx;
    fvp_pNL_LeftBottom->Vector[0].y = Grayy;
    //�������½���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[1].x = Grayx;
    fvp_pNL_LeftBottom->Vector[1].y = Grayy;
    //�������½���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[2].x = Grayx;
    fvp_pNL_LeftBottom->Vector[2].y = Grayy;
    //�������½���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 4, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_LeftBottom->Vector[3].x = Grayx;
    fvp_pNL_LeftBottom->Vector[3].y = Grayy;
    //��������
    fvp_pNL_LeftBottom->nLevels=1;
    fvp_pNL_LeftBottom->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_LeftBottom->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_LeftBottom->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_LeftBottom->pNL_RightBottom=fvp_pNL_RightBottom;

    //�������½����������ֵ
    centreX = left + width * 3 / 4;
    centreY = top + height * 3 / 4;
    //�������½���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[0].x = Grayx;
    fvp_pNL_RightBottom->Vector[0].y = Grayy;
    //�������½���������Ͻ�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height / 2, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[1].x = Grayx;
    fvp_pNL_RightBottom->Vector[1].y = Grayy;
    //�������½���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width / 2, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[2].x = Grayx;
    fvp_pNL_RightBottom->Vector[2].y = Grayy;
    //�������½���������½�����ĻҶ�����
    CalGrayGravity(pBufStruct->grayBmp, w, h, centreX, centreY, left + width * 3 / 4, top + height * 3 / 4, width / 4, height / 4, &Grayx, &Grayy, pBufStruct);
    fvp_pNL_RightBottom->Vector[3].x = Grayx;
    fvp_pNL_RightBottom->Vector[3].y = Grayy;
    //��������
    fvp_pNL_RightBottom->nLevels=1;
    fvp_pNL_RightBottom->pNL_LeftTop=fvp_pNL_LeftTop;
    fvp_pNL_RightBottom->pNL_RightTop=fvp_pNL_RightTop;
    fvp_pNL_RightBottom->pNL_LeftBottom=fvp_pNL_LeftBottom;
    fvp_pNL_RightBottom->pNL_RightBottom=fvp_pNL_RightBottom;

    


}

//��������ֵ��
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

//�к��滻
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
//�۱Ǹ���
/*******************************************************************/
DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	 //���д��Ӧ�������
	_BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer;
	int RW=0,RH=0;
	int W=0,H=0;
	//�����������߽߱�����Rx,����߽�����Ry
	int Rx=0,Ry=0;
	//Tx��Ty �ֱ����������һ֡����߽߱����ꡢ����߽����ꡣ
	// spdxObj��spdyObj�ֱ�Ϊ������ˮƽ���򡢴�ֱ�����˶��ٶȣ����ű�ʾ�˶�����
	int Tx=0,Ty=0,spdxObj=0,spdyObj=0;
	if (!pBufStruct->bLastEyeChecked)//δ����������
	{
		return;
	}
	//���۸���
	W=pBufStruct->pOtherVars->objLefteye.rcObject.width;
	H=pBufStruct->pOtherVars->objLefteye.rcObject.height; 
	RW = pBufStruct->pOtherVars->objLefteye.rcObject.width+2*LefteyeSearchSize;
	RH = pBufStruct->pOtherVars->objLefteye.rcObject.height+2*LefteyeSearchSize;
	//�����������߽߱����� Rx������߽����� Ry
	//Tx��Ty �ֱ����������һ֡����߽߱����ꡢ����߽����ꡣ
	// spdxObj��spdyObj�ֱ�Ϊ������ˮƽ���򡢴�ֱ�����˶��ٶȣ����ű�ʾ�˶�����
	Tx=pBufStruct->pOtherVars->objLefteye.rcObject.left;
	Ty=pBufStruct->pOtherVars->objLefteye.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objLefteye.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objLefteye.spdyObj;
	Rx=Tx-LefteyeSearchSize+spdxObj;
	Ry=Ty-LefteyeSearchSize+spdyObj;


	// //debug������������
	// aRect debugrec;
	// debugrec.left=Rx;
	// debugrec.top=Ry;
	// debugrec.width=RW;
	// debugrec.height=RH;
	// DrawRectangle(pBufStruct->displayImage, w, h,pBufStruct->pOtherVars->objLefteye.rcObject, TYUV1(120, 151, 0), false);
	// DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 0, 158), false);
	// return;

	//��������ʹ��������������������Χ��
	//��������pBufStruct->rcnFace
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


	//2.�������ƥ���
	// 	�����������ڣ��������ҡ����Ϻ����ƶ������壬ÿ�ƶ�һ�����أ���������������
	// �������뵱ǰ�����Ĳ�࣬�ҵ�һ�����ƥ���--���������С����¼���ƥ��������λ
	// �� ptNew ����С������༰����ֵ theMinFV ����С�������ֱ�Ӽ�¼�ڽṹ��
	// TRACE_OBJEC �еı��� nMinDist��
	int nMinDist=0,nDist=0,nDistFromOrg=0;
	POINT ptNew;
	FeatureVector4P* temp = new FeatureVector4P[5];//��ʱ��������
	FeatureVector4P* theMinFV = new FeatureVector4P[5];//��С��������
	//�����������ڣ��������ҡ����Ϻ����ƶ������壬ÿ�ƶ�һ�����أ���������������
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objLefteye.fvObject.Vector,temp);

	for (int i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//��������ֵ
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//��������ֵ��
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objLefteye.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//����ֵ
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
	
	// 3.���¸�����λ�á��˶��ٶȼ���ǰ����ֵ
	//1�����¸�����λ�þ��Σ�object->rcObject�������ƥ�䴦��λ�á�
	POINT ptOld;
	ptOld.x=pBufStruct->pOtherVars->objLefteye.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objLefteye.rcObject.top+H/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objLefteye.rcObject.width=W;
	pBufStruct->pOtherVars->objLefteye.rcObject.height=H;

	// 	2���˶��ٶȵĸ��£���ԭ�ٶ��뱾֡����֡�˶���Ȩ�õ���Ȩֵ����С��������йأ�
	// ʽ 5.26 �����㷨��
	pBufStruct->pOtherVars->objLefteye.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objLefteye.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));


	// 3.���ٽ���жϷ���
	// ��1������������뵱ǰ�����Ĳ��nMinDistΪ�ж����ݣ�����GOOD_TRACE_DIST��
	// BAD_TRACE_DIST������ֵ��
	// ��ƥ�����ã���nMinDist��GOOD_TRACE_DISTʱ���ж�������Ч��
	// ��ƥ���еȣ�OOD_TRACE_DIST<nMinDist��BAD_TRACE_DISTʱ����Ϊ������
	// �ܸ��٣�Ҳ�п��ܶ�ʧ�����壬��ˣ��Ǹ�������ʧ��һ�Ρ�
	// ��ƥ��ܲ��nMinDist>BAD_TRACE_DISTʱ���ж���ʧ�����塣
	// ��2����ǰ������ԭʼ�����Ĳ��nDistFromOrg̫��Ҳ�ж���ʧ�����塣��������һ
	// ����ֵMAX_DIST_TO_ORG����nDistFromOrg>MAX_DIST_TO_ORGʱ���ж���ʧ�����塣
	// ��ǰ��������
	// ƥ�����ã����������ֱ���滻��ǰ������
	// ƥ���еȣ���ԭʼ�����к��滻��ǰ��������ʽ 5.27 ��ʾ��
	// ƥ��ܲ��ԭʼ����ֱ���滻��ǰ������
	// ��ǰ���������ԭʼ����̫Զ����ԭʼ����ֱ���滻��ǰ������
	if (nMinDist<=GOOD_TRACE_DIST)//�ж�����
	{
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objLefteye.nBrokenTimes=0;//�����������ʧ�ܴ���
		pBufStruct->pOtherVars->objLefteye.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//�������ʧ�ܱ�־
		LefteyeSearchSize=5;//���������С
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//�ж��е�
	{	
		//��ԭʼ�����к��滻��ǰ����
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector);
		pBufStruct->pOtherVars->objLefteye.nBrokenTimes++;//��������ʧ��һ��
		pBufStruct->pOtherVars->objLefteye.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//�������ʧ�ܱ�־
		LefteyeSearchSize=10;//���������С
	}
	else if (nMinDist>BAD_TRACE_DIST)//�жϺܲ�
	{
		//��ԭʼ�����滻��ǰ����
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		LefteyeSearchSize=15;//���������С
		if (pBufStruct->pOtherVars->objLefteye.bSaveit)//�Ѿ���ʧ��������
		{
			pBufStruct->pOtherVars->objLefteye.bBrokenTrace=true;//����ʧ��
		}
		else
		{
			pBufStruct->pOtherVars->objLefteye.bSaveit=true;//��ʧ������
			pBufStruct->pOtherVars->objLefteye.bBrokenTrace=false;//�������ʧ�ܱ�־
			pBufStruct->pOtherVars->objLefteye.nBrokenTimes++;//��������ʧ��һ��
		}
	}
	if (nDistFromOrg>MAX_DIST_TO_ORG)//����ԭʼ����̫Զ
	{
		//��ԭʼ�����滻��ǰ����
		memcpy(pBufStruct->pOtherVars->objLefteye.fvObject.Vector,pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objLefteye.bSaveit=true;//�ö�ʧ������
	}
	if (pBufStruct->pOtherVars->objLefteye.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objLefteye.bBrokenTrace=true;//����ʧ��
	}
	// ���Ƹ���������
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// ���Ƹ��������
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objLefteye.rcObject, TYUV1(120, 151, 255), false);
	// ��������������
	aRect debugrec;
	debugrec.left=Rx;
	debugrec.top=Ry;
	debugrec.width=RW;
	debugrec.height=RH;
	DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 151, 0), false);

	//���۸���
	W=pBufStruct->pOtherVars->objRighteye.rcObject.width;
	H=pBufStruct->pOtherVars->objRighteye.rcObject.height;
	RW = pBufStruct->pOtherVars->objRighteye.rcObject.width+2*RighteyeSearchSize;
	RH = pBufStruct->pOtherVars->objRighteye.rcObject.height+2*RighteyeSearchSize;
	//�����������߽߱����� Rx������߽����� Ry
	//Tx��Ty �ֱ����������һ֡����߽߱����ꡢ����߽����ꡣ
	// spdxObj��spdyObj�ֱ�Ϊ������ˮƽ���򡢴�ֱ�����˶��ٶȣ����ű�ʾ�˶�����
	Tx=pBufStruct->pOtherVars->objRighteye.rcObject.left;
	Ty=pBufStruct->pOtherVars->objRighteye.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objRighteye.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objRighteye.spdyObj;
	Rx=Tx-RighteyeSearchSize+spdxObj;
	Ry=Ty-RighteyeSearchSize+spdyObj;

	//��������ʹ��������������������Χ��
	//��������pBufStruct->rcnFace
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
	
	//�����������ڣ��������ҡ����Ϻ����ƶ������壬ÿ�ƶ�һ�����أ���������������
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objRighteye.fvObject.Vector,temp);

	for ( i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//��������ֵ
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//��������ֵ��
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objRighteye.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//����ֵ
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

	// 3.���¸�����λ�á��˶��ٶȼ���ǰ����ֵ
	//1�����¸�����λ�þ��Σ�object->rcObject�������ƥ�䴦��λ�á�
	ptOld.x=pBufStruct->pOtherVars->objRighteye.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objRighteye.rcObject.top+H/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objRighteye.rcObject.width=W;
	pBufStruct->pOtherVars->objRighteye.rcObject.height=H;

	// 	2���˶��ٶȵĸ��£���ԭ�ٶ��뱾֡����֡�˶���Ȩ�õ���Ȩֵ����С��������йأ�
	// ʽ 5.26 �����㷨��
	pBufStruct->pOtherVars->objRighteye.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objRighteye.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));

	// 	3�����µ�ǰ����ֵ��
	if (nMinDist<=GOOD_TRACE_DIST)//�ж�����
	{
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objRighteye.nBrokenTimes=0;//�����������ʧ�ܴ���
		pBufStruct->pOtherVars->objRighteye.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//�������ʧ�ܱ�־
		RighteyeSearchSize=5;//���������С
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//�ж��е�
	{	
		//��ԭʼ�����к��滻��ǰ����
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector);
		pBufStruct->pOtherVars->objRighteye.nBrokenTimes++;//��������ʧ��һ��
		pBufStruct->pOtherVars->objRighteye.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//�������ʧ�ܱ�־
		RighteyeSearchSize=10;//���������С
	}
	else if (nMinDist>BAD_TRACE_DIST)//�жϺܲ�
	{
		//��ԭʼ�����滻��ǰ����
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		RighteyeSearchSize=15;//���������С
		if (pBufStruct->pOtherVars->objRighteye.bSaveit)//�Ѿ���ʧ��������
		{
			pBufStruct->pOtherVars->objRighteye.bBrokenTrace=true;//����ʧ��
		}
		else
		{
			pBufStruct->pOtherVars->objRighteye.bSaveit=true;//��ʧ������
			pBufStruct->pOtherVars->objRighteye.bBrokenTrace=false;//�������ʧ�ܱ�־
			pBufStruct->pOtherVars->objRighteye.nBrokenTimes++;//��������ʧ��һ��
		}
	}
	if (nDistFromOrg>MAX_DIST_TO_ORG)//����ԭʼ����̫Զ
	{
		//��ԭʼ�����滻��ǰ����
		memcpy(pBufStruct->pOtherVars->objRighteye.fvObject.Vector,pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objRighteye.bSaveit=true;//�ö�ʧ������
	}
	if (pBufStruct->pOtherVars->objRighteye.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objRighteye.bBrokenTrace=true;//����ʧ��
	}
	// ���Ƹ���������
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// ���Ƹ��������
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objRighteye.rcObject, TYUV1(120, 151, 255), false);
	// ��������������
	debugrec.left=Rx;
	debugrec.top=Ry;
	debugrec.width=RW;
	debugrec.height=RH;
	DrawRectangle(pBufStruct->displayImage, w, h, debugrec, TYUV1(120, 151, 0), false);

	//���Ӹ���
	W=pBufStruct->pOtherVars->objNose.rcObject.width;
	H=pBufStruct->pOtherVars->objNose.rcObject.height;
	RW = pBufStruct->pOtherVars->objNose.rcObject.width+2*NoseSearchSize;
	RH = pBufStruct->pOtherVars->objNose.rcObject.height+2*NoseSearchSize;
	//�����������߽߱����� Rx������߽����� Ry
	//Tx��Ty �ֱ����������һ֡����߽߱����ꡢ����߽����ꡣ
	// spdxObj��spdyObj�ֱ�Ϊ������ˮƽ���򡢴�ֱ�����˶��ٶȣ����ű�ʾ�˶�����
	Tx=pBufStruct->pOtherVars->objNose.rcObject.left;
	Ty=pBufStruct->pOtherVars->objNose.rcObject.top;
	spdxObj=pBufStruct->pOtherVars->objNose.spdxObj;
	spdyObj=pBufStruct->pOtherVars->objNose.spdyObj;
	Rx=Tx-NoseSearchSize+spdxObj;
	Ry=Ty-NoseSearchSize+spdyObj;

	//��������ʹ��������������������Χ��
	//��������pBufStruct->rcnFace
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
	//���������������
	CalEigen(pBufStruct,w,h,Tx,Ty,W,H,temp);
	nDistFromOrg=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objNose.fvObject.Vector,temp);
	
	for ( i=Rx;i<Rx+RW-W;i++)
	{
		for (int j=Ry;j<Ry+RH-H;j++)
		{
			//��������ֵ
			CalEigen(pBufStruct,w,h,i,j,W,H,temp);
			//��������ֵ��
			nDist=CalEigenDiff((FeatureVector4P*)&pBufStruct->pOtherVars->objNose.fvObject.Vector,temp);
			if (i==Rx && j==Ry)//����ֵ
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

	// 3.���¸�����λ�á��˶��ٶȼ���ǰ����ֵ
	//1�����¸�����λ�þ��Σ�object->rcObject�������ƥ�䴦��λ�á�
	ptOld.x=pBufStruct->pOtherVars->objNose.rcObject.left+W/2;
	ptOld.y=pBufStruct->pOtherVars->objNose.rcObject.top+H/2;
	pBufStruct->pOtherVars->objNose.rcObject.left=ptNew.x-W/2;
	pBufStruct->pOtherVars->objNose.rcObject.top=ptNew.y-H/2;
	pBufStruct->pOtherVars->objNose.rcObject.width=W;
	pBufStruct->pOtherVars->objNose.rcObject.height=H;

	// 	2���˶��ٶȵĸ��£���ԭ�ٶ��뱾֡����֡�˶���Ȩ�õ���Ȩֵ����С��������йأ�
	// ʽ 5.26 �����㷨��
	pBufStruct->pOtherVars->objNose.spdxObj=(ptNew.x-ptOld.x)*(256/(nMinDist+256))+spdxObj*(nMinDist/(nMinDist+256));
	pBufStruct->pOtherVars->objNose.spdyObj=(ptNew.y-ptOld.y)*(256/(nMinDist+256))+spdyObj*(nMinDist/(nMinDist+256));



	// 	3�����µ�ǰ����ֵ��
	if (nMinDist<=GOOD_TRACE_DIST)//�ж�����
	{
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,theMinFV,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objNose.nBrokenTimes=0;//�����������ʧ�ܴ���
		pBufStruct->pOtherVars->objNose.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//�������ʧ�ܱ�־
		NoseSearchSize=5;//���������С
	}
	else if (nMinDist>GOOD_TRACE_DIST && nMinDist<=BAD_TRACE_DIST)//�ж��е�
	{	
		//��ԭʼ�����к��滻��ǰ����
		neutralization_replacement((FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject.Vector,(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject_org.Vector);
		pBufStruct->pOtherVars->objNose.nBrokenTimes++;//��������ʧ��һ��
		pBufStruct->pOtherVars->objNose.bSaveit=false;//������ٶ�ʧ��־
		pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//�������ʧ�ܱ�־
		NoseSearchSize=10;//���������С
	}


	else if (nMinDist>BAD_TRACE_DIST)//�жϺܲ�
	{
		//��ԭʼ�����滻��ǰ����
		// TODO:�����bug������
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,pBufStruct->pOtherVars->objNose.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		NoseSearchSize=15;//���������С
		if (pBufStruct->pOtherVars->objNose.bSaveit)//�Ѿ���ʧ��������
		{
			pBufStruct->pOtherVars->objNose.bBrokenTrace=true;//����ʧ�ܱ�־
		}
		else
		{
			pBufStruct->pOtherVars->objNose.bSaveit=true;//��ʧ������
			pBufStruct->pOtherVars->objNose.bBrokenTrace=false;//�������ʧ�ܱ�־
			pBufStruct->pOtherVars->objNose.nBrokenTimes++;//��������ʧ��һ��
		}
	}


	if (nDistFromOrg>MAX_DIST_TO_ORG)//����ԭʼ����̫Զ
	{
		//��ԭʼ�����滻��ǰ����
		memcpy(pBufStruct->pOtherVars->objNose.fvObject.Vector,pBufStruct->pOtherVars->objNose.fvObject_org.Vector,sizeof(FeatureVector4P)*5);
		pBufStruct->pOtherVars->objNose.bSaveit=true;//�ö�ʧ������
	}
	if (pBufStruct->pOtherVars->objNose.nBrokenTimes>=10)
	{
		pBufStruct->pOtherVars->objNose.bBrokenTrace=true;//����ʧ��
	}


	// ���Ƹ���������
	DrawCross(pBufStruct->displayImage, w, h, ptNew.x, ptNew.y, 15, TYUV1(42, 100, 105), true);
	// ���Ƹ��������
	DrawRectangle(pBufStruct->displayImage, w, h, pBufStruct->pOtherVars->objNose.rcObject, TYUV1(120, 151, 255), false);
	// ���Ʊ���������
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//theApp.dlg.DestroyWindow();
}

