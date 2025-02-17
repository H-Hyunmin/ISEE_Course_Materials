// BlinkEyeCheck.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BlinkEyeCheck.h"
#include "BufStruct.h"
#include "ImageProc.h"

//
#define MAX_EYE_AMP         1
#define MAX_EYE_Y_DIFF      (3*MAX_EYE_AMP)//5
#define MIN_EYE_X_DIFF      (15*MAX_EYE_AMP)//4
#define MAX_EYE_X_DIFF      (40*MAX_EYE_AMP)//30
#define MAX_EYE_SIZE        (150*MAX_EYE_AMP)//200
//#define MIN_EYE_SIZE        (20*MAX_EYE_AMP)//200
//
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
// CBlinkEyeCheckApp

BEGIN_MESSAGE_MAP(CBlinkEyeCheckApp, CWinApp)
	//{{AFX_MSG_MAP(CBlinkEyeCheckApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlinkEyeCheckApp construction
CBlinkEyeCheckApp::CBlinkEyeCheckApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBlinkEyeCheckApp object

CBlinkEyeCheckApp theApp;
//
#define PC_MODE
/***************************************************************
��Щ����ֻ��PC���д��ڣ��������
��DSP�汾�в�������
***************************************************************/
#ifdef PC_MODE
aBYTE open_eye_left[32*24*2];
aBYTE open_eye_right[32*24*2];
aBYTE close_eye_left[32*24];
aBYTE close_eye_right[32*24];
aBYTE st_nose[32*48*2];
#endif


//����������


tagPOINT ptLeftEye, ptRightEye; // ����������


bool bret = false; // �Ƿ��⵽�۾�

char sInfo[] = "��������-գ�ۼ��������λ������";



bool bLastPlugin = false;

DLL_EXP void ON_PLUGIN_BELAST(bool bLast)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	bLastPlugin = bLast;
}

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
//							My Function
/*******************************************************************/

DLL_EXP void gaosi_filter(aBYTE *src, aBYTE *dst, int w, int h) {
    // 3x3 ��λ����
    float kernel[3][3] = {
        { 0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        { 0.0625, 0.125,  0.0625}
    };
    // ������ʱ�����洢�˲����
    aBYTE* tempImg = new aBYTE[w * h];
    memset(tempImg, 0, w * h * sizeof(aBYTE));

    // ��ͼ����и�˹�˲�
    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            float sum = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel = src[(y + ky) * w + (x + kx)];
                    sum += pixel * kernel[ky + 1][kx + 1];
                }
            }
            tempImg[y * w + x] = static_cast<aBYTE>(sum);
        }
    }
    // ����ʱ������ֵ������dst
    memcpy(dst, tempImg, w * h * sizeof(aBYTE));
    // �ͷ���ʱ�����ڴ�
    delete[] tempImg;
}

DLL_EXP void diff_img(aBYTE *lastimg, aBYTE *thisimg, aBYTE *dst, int w, int h) {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int index = y * w + x;
            if (lastimg[index] >= thisimg[index]) {
                dst[index] = lastimg[index] - thisimg[index];
            } else {
                dst[index] = 0;
            }
        }
    }
}



DLL_EXP void open_3x1(aBYTE *src, aBYTE *dst, int w, int h) {
    // ������ʱ�ռ䲢��ʼ��
    aBYTE* tempImage = new aBYTE[w * h];
    memset(tempImage, 0, w * h);

    // 3x1�ṹԪ�صĸ�ʴ����
    for (int y = 1; y < h - 1; ++y) {
        for (int x = 0; x < w; ++x) {
            int minVal = src[(y - 1) * w + x];
            if (src[y * w + x] < minVal) {
                minVal = src[y * w + x];
            }
            if (src[(y + 1) * w + x] < minVal) {
                minVal = src[(y + 1) * w + x];
            }
            tempImage[y * w + x] = minVal;
        }
    }
    
    // 3x1�ṹԪ�ص����Ͳ���
    for ( y = 1; y < h - 1; ++y) {
        for (int x = 0; x < w; ++x) {
            int maxVal = tempImage[(y - 1) * w + x];
            if (tempImage[y * w + x] > maxVal) {
                maxVal = tempImage[y * w + x];
            }
            if (tempImage[(y + 1) * w + x] > maxVal) {
                maxVal = tempImage[(y + 1) * w + x];
            }
            dst[y * w + x] = maxVal;
        }
    }

    // �ͷ���ʱ�ռ�
    delete[] tempImage;
}


DLL_EXP void Morphological_Processing(aBYTE *src, aBYTE *dst, int w, int h) {
	int * tempImage = new int[w * h ]; 
	memset(tempImage, 0, w * h *sizeof(int));
    int* LK = new int[1024];      
    int* LK1 = new int[1024];     
    for (int i = 0; i < 1024; i++) {
        LK[i] = i; 
        LK1[i] = 0;
    }

    int nextLabel = 1; //��һ�����
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int index = y * w + x; // ��ǰ��������
            if (src[index] == 0) {
                tempImage[index] = 0; 
                continue;
            }

            // ��ȡ�ھ����صı��
            int leftLabel = (x > 0) ? tempImage[index - 1] : 0;
            int topLabel = (y > 0) ? tempImage[index - w] : 0;

            if (leftLabel == 0 && topLabel == 0) {
                tempImage[index] = nextLabel;
                nextLabel++;
            } else if (leftLabel != 0 && topLabel == 0) {
                tempImage[index] = leftLabel;
            } else if (leftLabel == 0 && topLabel != 0) {
                tempImage[index] = topLabel;
            } else {
                // �����ھ����ض��б�ǣ�ȡ���
				tempImage[index] = leftLabel;

				// ���µȼ۱�
				int Lmax = leftLabel>topLabel?leftLabel:topLabel;
				int Lmin = leftLabel<topLabel?leftLabel:topLabel;
                
                //
                if (LK[Lmax] != Lmax) {//׷��
                    int j = Lmax;
                    while (LK[j] != j) {
                        j = LK[j];
                    }
                    if (j>Lmin) LK[j] = Lmin;
                    else LK[Lmin] = j;
                }
                else{//���ϲ��ң�ֱ���ҵ���С�ı��
                    while (LK[Lmin] != Lmin) {
                        Lmin = LK[Lmin];
                    }
                    //��¼�ȼ۹�ϵ
                    LK[Lmax] = Lmin;
                }

            }
        }
    }


    // ����ȼ۱�
	for ( i = 0; i < nextLabel; i++) {
        int j = i;
        while (LK[j] != j) {
            j = LK[j];
        }
        LK[i] =j;
	}

    //��ͨ�������±��
    int newi=1;
    for ( i=1; i<nextLabel; i++) {
        if (LK[i] == i) {
            LK1[i] = newi;
            newi++;
        }
        else {
            LK1[i] = LK1[LK[i]];
        }
    }
    //��ͨ��������
    int nconnetedcomponent = newi - 1;
    // ShowDebugMessage("��ͨ����������%d\n", nconnetedcomponent);
    //ͼƬ�滻
    for ( i = 0; i < w * h; i++) {
        tempImage[i] = LK1[tempImage[i]];
    }

        // ͳ�Ƹ�����ͨ����Ĵ�С������
    int *pixelCount = new int[nconnetedcomponent + 1]; // pixelCount[i]��¼���i����ͨ��������ظ�����pixelCount[0]����
    int *size = new int[nconnetedcomponent + 1]; // ��¼ÿ����ͨ����Ĵ�С,size=x^2+y^2, x��y�ֱ�����ͨ����Ŀ�͸�
    float *centerX = new float[nconnetedcomponent + 1]; // ��¼ÿ����ͨ���������x����,centerX[i] = sigma(x)/pixelCount[i]
    float *centerY = new float[nconnetedcomponent + 1]; // ��¼ÿ����ͨ���������y����
    
    // ��ʼ��ͳ������
    for (i = 1; i <= nconnetedcomponent; i++) {
        pixelCount[i] = 0;
        size[i] = 0;
        centerX[i] = 0.0f;
        centerY[i] = 0.0f;
    }
    
    // ͳ��ÿ����ͨ��������ظ���������
    for (y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int label = tempImage[y * w + x];
            if (label > 0) {
                pixelCount[label]++;
                centerX[label] += x;
                centerY[label] += y;
            }
        }
    }
    
    // ����ÿ����ͨ���������
    for (i = 1; i <= nconnetedcomponent; i++) {
        if (pixelCount[i] > 0) {
            centerX[i] /= pixelCount[i];
            centerY[i] /= pixelCount[i];
        }
    }
    
    // �ҵ���ÿ����ŵ���ͨ����ľ���
    int *minX = new int[nconnetedcomponent + 1];
    int *minY = new int[nconnetedcomponent + 1];
    int *maxX = new int[nconnetedcomponent + 1];
    int *maxY = new int[nconnetedcomponent + 1];
    
    // ��ʼ�����α߽�
    for (i = 1; i <= nconnetedcomponent; i++) {
        minX[i] = w;
        minY[i] = h;
        maxX[i] = 0;
        maxY[i] = 0;
    }
    
    // ����ÿ����ͨ����ľ��α߽�
    for ( y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int label = tempImage[y * w + x];
            if (label > 0) {
                if (x < minX[label]) minX[label] = x;
                if (y < minY[label]) minY[label] = y;
                if (x > maxX[label]) maxX[label] = x;
                if (y > maxY[label]) maxY[label] = y;
            }
        }
    }
    
    // ����ÿ����ͨ����Ĵ�С
    for ( i = 1; i <= nconnetedcomponent; i++) {
        int width = maxX[i] - minX[i] + 1;
        int height = maxY[i] - minY[i] + 1;
        size[i] = width * width + height * height;
    }
    
    //��������ͨ��λ˫�ۣ�������ͨ�� i��j �������м����������ɳ�����Ϊ��˫�ۡ�
    //1��������ͨ���������ͬһˮƽ���ϣ�|CenterY[i]- CenterY[j]|<4��
    //2) ������ͨ�������˫�ۼ�ࣺ15<|CenterX[i]- CenterX[j]| <30��
    // 3��������ͨ������۾���С��������̫��size[i]<200 �� size[j]<200
    int eye1 = -1, eye2 = -1;
    for ( i = 1; i <= nconnetedcomponent; i++) {
        for (int j = i + 1; j <= nconnetedcomponent; j++) {
            if ((centerY[i] - centerY[j] < 4 && centerY[i] - centerY[j] > -4) && 
                (centerX[i] - centerX[j] > 15 || centerX[j] - centerX[i] > 15) && 
                (centerX[i] - centerX[j] < 30 || centerX[j] - centerX[i] < 30) && 
                size[i] < 200 && 
                size[j] < 200) {
                eye1 = i;
                eye2 = j;
                break;
            }
        }
        if (eye1 != -1 && eye2 != -1) {
            break;
        }
    }

    if (eye1 != -1 && eye2 != -1) {
        // �ҵ����������ĵ�һ����ͨ��
        if (centerX[eye1] < centerX[eye2]) {
            ptLeftEye.x = static_cast<int>(centerX[eye1]);
            ptLeftEye.y = static_cast<int>(centerY[eye1]);
            ptRightEye.x = static_cast<int>(centerX[eye2]);
            ptRightEye.y = static_cast<int>(centerY[eye2]);
        } else {
            ptLeftEye.x = static_cast<int>(centerX[eye2]);
            ptLeftEye.y = static_cast<int>(centerY[eye2]);
            ptRightEye.x = static_cast<int>(centerX[eye1]);
            ptRightEye.y = static_cast<int>(centerY[eye1]);
        }
        bret = true; // �ҵ�˫��
        //��չ����ͼƬ
        ptLeftEye.x *= 4;
        ptLeftEye.y *= 4;
        ptRightEye.x *= 4;
        ptRightEye.y *= 4;
    }



    // �ͷ��ڴ�
    delete[] pixelCount;
    delete[] size;
    delete[] centerX;
    delete[] centerY;
    delete[] minX;
    delete[] minY;
    delete[] maxX;
    delete[] maxY;



    delete[] tempImage;
    delete[] LK;
    delete[] LK1;


}

// ��Դͼ���(x,y)�����Ͻǿ�ʼ����w1*h1��С��ͼ��Ŀ��ͼ����
// src,��w,��h��YUV422��ɫͼ��srcָ��ָ����ڴ���д�ŵ�ͼ��,ͼ���СΪ2*w*h,ǰw*hΪY������֮��w*h/2ΪU��������֮��w*h/2ΪV����
// dstĿ��ͼ��ָ�룬w1,h1ΪĿ��ͼ��Ŀ��
// x,yΪԴͼ���ĳ�����Ͻ�����

DLL_EXP void CopyImg(aBYTE *src, aBYTE *dst, int w, int h, int x, int y, int w1, int h1) {
    // ����Դͼ����Y��U��V��������ʼָ��
    aBYTE *srcY = src;
    aBYTE *srcU = src + w * h;
    aBYTE *srcV = src + w * h + (w * h) / 2;

    // ����Ŀ��ͼ����Y��U��V��������ʼָ��
    aBYTE *dstY = dst;
    aBYTE *dstU = dst + w1 * h1;
    aBYTE *dstV = dst + w1 * h1 + (w1 * h1) / 2;

    // ����Y������ÿ����һ��Yֵ����СΪw*h��
    for (int j = 0; j < h1; j++) {
        for (int i = 0; i < w1; i++) {
            dstY[j * w1 + i] = srcY[(y + j) * w + (x + i)];
        }
    }

    // ����U������ˮƽ���������ΪY��һ�룬��ֱ���򲻱䣩
    for ( j = 0; j < h1; j++) {
        for (int i = 0; i < w1 / 2; i++) {
            dstU[j * (w1 / 2) + i] = srcU[(y + j) * (w / 2) + (x / 2) + i];
        }
    }

    // ����V������ͬ����ˮƽ���������ΪY��һ�룬��ֱ���򲻱䣩
    for ( j = 0; j < h1; j++) {
        for (int i = 0; i < w1 / 2; i++) {
            dstV[j * (w1 / 2) + i] = srcV[(y + j) * (w / 2) + (x / 2) + i];
        }
    }

}


//����ɫ��У��Ŀ���ǽ�һ��ȷ���۾���λ��׼ȷ�ԣ���Ҫ�������鲿�ַǷ�ɫ�ص���С�
// ����ֱ�Ϊ����_lefteyeOpen��_righteyeOpen��32��24��С��YUV422ƽ���ʽ��˫��ͼƬ��
// ������洢��point�ṹ����ptLeftEye_cfm��ptRightEye_cfm��
// ͳ��˵��������Ϊ�Ƿ�ɫ�һҶ�ֵ�ϵͣ����U�ռ�����ʽ5.18��V�ռ�����ʽ5.19ʱ��
// ���϶�Ϊ����λ���ù��̲���U��V�ռ䣬����YUV422ƽ���ʽ��ÿ��������ȡһ��U��
// V���������U��V��СΪ16��24���أ���384�����ء�
DLL_EXP void EyeBallCheck (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, POINT* ptLeftEye_cfm, POINT* ptRightEye_cfm)
{
    int nFacePtNum=0, nEyePtNum=0;
    //1��ͳ����������ķ�ɫ��������nFacePtNum��
    //2��ͳ����������������nEyePtNum��
    BYTE *leftU = _lefteyeOpen+32*24;
    BYTE *leftV = _lefteyeOpen+32*24+32*24/2;
    BYTE *rightU = _righteyeOpen+32*24;
    BYTE *rightV = _righteyeOpen+32*24+32*24/2;

    //����
    for (int j = 0; j < 24; j++) {
        for (int i = 0; i < 16; i++) {
            if ((leftU[j * 16 + i] <= 131 && leftU[j * 16 + i] >= 124) && (leftV[j * 16 + i] <= 134 && leftV[j * 16 + i] >= 121 )) {
                nEyePtNum++;
            }
            if (leftU[j * 16 + i] <= 126 && leftU[j * 16 + i] >= 85 && leftV[j * 16 + i] <= 165 && leftV[j * 16 + i] >= 130) {
                nFacePtNum++;
            }
        }
    }

    if((nEyePtNum>=10 && nEyePtNum<=60) && nFacePtNum>=200 )
    {
        ptLeftEye_cfm->x = ptLeftEye.x;
        ptLeftEye_cfm->y = ptLeftEye.y;
    }
    // ShowDebugMessage("������������������%d\n", nEyePtNum);
    // ShowDebugMessage("���۷�ɫ����������%d\n", nFacePtNum);

    //����
    nFacePtNum = 0;
    nEyePtNum = 0;
    for ( j = 0; j < 24; j++) {
        for (int i = 0; i < 16; i++) {
            if ((rightU[j * 16 + i] <= 131 && rightU[j * 16 + i] >= 124) && (rightV[j * 16 + i] <= 134 && rightV[j * 16 + i] >= 121)) {
                nEyePtNum++;
            }
            if (rightU[j * 16 + i] <= 126 && rightU[j * 16 + i] >= 85 && rightV[j * 16 + i] <= 165 && rightV[j * 16 + i] >= 130) {
                nFacePtNum++;
                
            }
        }
    }
    if((nEyePtNum>=10 && nEyePtNum<=60) && nFacePtNum>=200 )
    {
        ptRightEye_cfm->x = ptRightEye.x;
        ptRightEye_cfm->y = ptRightEye.y;
    }


    // ShowDebugMessage("������������������%d\n", nEyePtNum);
    // ShowDebugMessage("���۷�ɫ����������%d\n", nFacePtNum);

}

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



/*******************************************************************/
//գ�ۼ�����۾���λ���
/*******************************************************************/
DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
//pYBits ��СΪw*h
//pUBits �� pVBits �Ĵ�СΪ w*h/2
//pBuffer �Ĵ�СΪ w*h*4
//�����㷨������һ�����裬 ��w��16�ı���
    AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
    _BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer; //pBufferָ����ڴ���д����ͼ�����������Ҫ�õ������ݽṹ



    //��ʼ��
    if(bret==false)
    {
        ptLeftEye.x = -1;
        ptLeftEye.y = -1;
        ptRightEye.x = -1;
        ptRightEye.y = -1;
    }

	 //���д��Ӧ�������
    //(1)��˹�˲�
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, 0, 0, true);
    gaosi_filter(pBufStruct->grayBmp_1d16, pBufStruct->grayBmp_1d16, w/4, h/4);
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, 0, h/4, true);

    //(2)nImageQueueIndex���ǰ4֡
    if(pBufStruct->nImageQueueIndex == -1)
    {   //��ʼ��
        pBufStruct->nImageQueueIndex = 4;
        pBufStruct->nLastImageIndex = 0;
        for(int i = 0; i < 8; i++)
        {
            memcpy(pBufStruct->pImageQueue[i], pBufStruct->grayBmp_1d16, w * h /16);
        }
    }
    else
    {
        pBufStruct->nImageQueueIndex = (pBufStruct->nImageQueueIndex + 1) % 8;
        pBufStruct->nLastImageIndex = (pBufStruct->nLastImageIndex + 1) % 8;
        memcpy(pBufStruct->pImageQueue[pBufStruct->nImageQueueIndex], pBufStruct->grayBmp_1d16, w * h / 16);
    }
    //(3)���ͼ��
    diff_img(pBufStruct->pImageQueue[pBufStruct->nLastImageIndex], pBufStruct->pImageQueue[pBufStruct->nImageQueueIndex], pBufStruct->TempImage1d8, w/4, h/4);
    //debug 
    // CopyToRect(pBufStruct->TempImage1d8, pYBits, w/4, h/4, w, h, 0, 0, true);
    
    
    //(4)��ֵ�� ����ֱ��ͼ������ȷ����ֵret
    int *n = new int[256];
    //���Ρ�����������������������������������
    int N0 = 40, N1=8; 
    //���Ρ�����������������������������������
    memset(n, 0, 256 * sizeof(int)); 
    //ͳ��ÿ���Ҷ�ֵ���ֵĴ���
    for(int i = 0; i < w * h/16; i++)
    {
        n[pBufStruct->TempImage1d8[i]]++;
    }
    int ret = 255, sum = 0;
    while(sum < N0 && ret >= 0)
    {
        sum += n[ret];
        ret--;
    }
    //��ֵN1��ret<N1ʱ����Ϊû��գ��
    int blinkflag = 1;
    if(ret < N1)
    {
        blinkflag = 0;
    }
    //��ֵ��
    if (blinkflag == 1)
    {
        for(int i = 0; i < w * h/16; i++)
        {
            if(pBufStruct->TempImage1d8[i] > ret)
            {
                pBufStruct->TempImage1d8[i] = 255;
            }
            else
            {
                pBufStruct->TempImage1d8[i] = 0;
            }
        }
    }
    else
    {
        memset(pBufStruct->TempImage1d8, 0, w * h / 16);
    }
    // CopyToRect(pBufStruct->TempImage1d8, pYBits, w/4, h/4, w, h, w/4, 0, true);
    //(5)��̬ѧ����
    //������
    open_3x1(pBufStruct->TempImage1d8, pBufStruct->TempImage1d8, w/4, h/4);
    // CopyToRect(pBufStruct->TempImage1d8, pYBits, w/4, h/4, w, h, 0, h/4, true);
    //�����ǣ�
    //TODO:��Ҫ��������
	Morphological_Processing(pBufStruct->TempImage1d8, pBufStruct->TempImage1d8, w/4, h/4);
    
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, w/4, h/4, true);
	
    //(6)������λ˫�ۼ���ʾ
    //����ʮ��
    if(bret)
    {
        DrawCross(pBufStruct->displayImage, w, h, ptLeftEye.x, ptLeftEye.y, 20, TYUV1(0, 191, 255), true);
        DrawCross(pBufStruct->displayImage, w, h, ptRightEye.x, ptRightEye.y, 20, TYUV1(0, 191, 255), true);
    }

    //��ӡdebug��Ϣ
    // if(bret)
    // {
    //     ShowDebugMessage("LeftEye:(%d,%d) RightEye:(%d,%d)\n", ptLeftEye.x, ptLeftEye.y, ptRightEye.x, ptRightEye.y);
    // }
    // else
    // {
    //     ShowDebugMessage("No Eye Detected\n");
    // }
    //2.��ʶ����֤˫��
    bool eyecheck = false; //��ʶ�Լ���ʶ
    //�������۾��롢�۾���Ⱥ͸߶�
    int nEyeDist=-1, nEyeWidth=-1, nEyeHeight=-1;
    int NoseWidth = -1, NoseHeight = -1;
    if(bret)
    {
        nEyeDist = ptRightEye.x - ptLeftEye.x;
        nEyeWidth = nEyeDist*2/3;
        nEyeHeight = nEyeDist/2;
    }
    //����۾���С�Ƿ����
    if(bret)
    {
        if(nEyeWidth > w/4 || nEyeHeight > h/4)
        {
            eyecheck = false;
        }
        else
        {
            eyecheck = true;
        }
    }
    //����۾��Ƿ���ͼ����
    if(bret)
    {
        if(ptLeftEye.x < 0 || ptLeftEye.y < 0 || ptRightEye.x < 0 || ptRightEye.y < 0 || ptLeftEye.x > w || ptLeftEye.y > h || ptRightEye.x > w || ptRightEye.y > h)
        {
            eyecheck = false;
        }
        else
        {
            eyecheck = true;
        }
    }
    //debug
    // ShowDebugMessage("EyeCheck:%d\n", eyecheck);
    //3.���Ʋ��ز����۾��ǲ�λͼƬ
    //  ��������������������и��Ʋ��ز����۾��ǲ�λͼƬ��
    // 1����ʶ����֤˫��ͨ����Ҳ���ǻ�������֤֡���λ˫�۵���ȷ�ԡ�
    // 2��bLastEyeCheckedΪfalseʱ���۱�������������һ������ʧ�ܣ����ʾ��û������
    // ����ģ�ͻ������ģ���ѹ�ʱ��
    aRect rcvLeftEye, rcvRightEye, rcvNose;
    if(eyecheck &&  (!pBufStruct->bLastEyeChecked ||    
        pBufStruct->pOtherVars->objLefteye.bBrokenTrace || 
        pBufStruct->pOtherVars->objRighteye.bBrokenTrace || 
        pBufStruct->pOtherVars->objNose.bBrokenTrace)
    )
    {
    //�����۾��ǲ�λͼƬ
    //�������ۡ����ۼ�������W*HͼƬʱ����Χ�����ֱ����ṹ��rcvLeftEye��rcvRightEye��rcvNose��
    // �������������Ҫ��˫�۵Ŀ�Ⱥ͸߶ȡ����ӵĿ�Ⱥ͸߶Ⱦ�ӦΪ4�ı�����
    NoseWidth = nEyeDist*3/4;
    NoseHeight = nEyeDist;

    //����˫�۵Ŀ�Ⱥ͸߶ȡ����ӵĿ�Ⱥ͸߶Ⱦ�ӦΪ4�ı�����
    nEyeWidth = (nEyeWidth/4)*4;
    nEyeHeight = (nEyeHeight/4)*4;
    NoseWidth = (NoseWidth/4)*4;
    NoseHeight = (NoseHeight/4)*4;

    //������������
    rcvLeftEye.left = ptLeftEye.x - nEyeWidth/2;
    rcvLeftEye.top = ptLeftEye.y - nEyeHeight/2;
    rcvLeftEye.width = nEyeWidth;
    rcvLeftEye.height = nEyeHeight;
    //������������
    rcvRightEye.left = ptRightEye.x - nEyeWidth/2;
    rcvRightEye.top = ptRightEye.y - nEyeHeight/2;
    rcvRightEye.width = nEyeWidth;
    rcvRightEye.height = nEyeHeight;
    //���ñ�������
    rcvNose.left = ptLeftEye.x + nEyeDist/8;
    rcvNose.top = ptLeftEye.y ;
    rcvNose.width = NoseWidth;
    rcvNose.height = NoseHeight;
    //�����ڴ�
    BYTE *temp_lefteyeOpen = new BYTE[nEyeWidth*nEyeHeight*2];
    BYTE *temp_righteyeOpen = new BYTE[nEyeWidth*nEyeHeight*2];
    BYTE *temp_nose = new BYTE[NoseWidth*NoseHeight*2];

    BYTE *_lefteyeOpen = new BYTE[24*32*2];
    BYTE *_righteyeOpen = new BYTE[24*32*2];
    BYTE *st_nose = new BYTE[48*32*2];

    //����ͼƬ�Ӳ�ɫͼƬcolorBmp�зֱ��������۲�λ��ɫͼƬ�����ز���Ϊ32��24��С��
    //YUV422ƽ���ʽ������������_lefteyeOpen��_righteyeOpen����������ɫ��У�顣

    //����CopyImg����
    // BYTE *temimg=new BYTE[w*h/2];
    // CopyImg(pBufStruct->colorBmp, temimg, w, h, w/2, h/2, w/2, h/2);
    // CopyToRect(temimg, pYBits,w/2,h/2,w,h,0,0,false);
    // CopyToRect(temimg, pYBits,w/2,h/2,w,h,w/2,0,true);
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������

    //����
    CopyImg(pBufStruct->colorBmp, temp_lefteyeOpen, w, h, rcvLeftEye.left, rcvLeftEye.top, rcvLeftEye.width, rcvLeftEye.height);
    CopyImg(pBufStruct->colorBmp, temp_righteyeOpen, w, h, rcvRightEye.left, rcvRightEye.top, rcvRightEye.width, rcvRightEye.height);
    CopyImg(pBufStruct->colorBmp, temp_nose, w, h, rcvNose.left, rcvNose.top, rcvNose.width, rcvNose.height);


    DrawRectangle(pBufStruct->displayImage, w, h, rcvLeftEye, TYUV1(0, 191, 255), false);
    DrawRectangle(pBufStruct->displayImage, w, h, rcvRightEye, TYUV1(0, 191, 255), false);
    DrawRectangle(pBufStruct->displayImage, w, h, rcvNose, TYUV1(0, 120,234), false);

    ReSample(temp_lefteyeOpen, rcvLeftEye.width, rcvLeftEye.height, 32, 24, false, false, _lefteyeOpen);
    ReSample(temp_righteyeOpen, rcvRightEye.width, rcvRightEye.height, 32, 24, false, false, _righteyeOpen);
    ReSample(temp_nose, rcvNose.width, rcvNose.height,32,48, false, false, st_nose);


    CopyToRect(_lefteyeOpen, pYBits,32,24,w,h,0,0,false);
    CopyToRect(_lefteyeOpen, pYBits,32,24,w,h,0,40,true);
    CopyToRect(_righteyeOpen, pYBits,32,24,w,h,40,0,false);
    CopyToRect(_righteyeOpen, pYBits,32,24,w,h,40,40,true);
    CopyToRect(st_nose, pYBits,32,48,w,h,16,80,false);

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
    //����ɫ��У��Ŀ���ǽ�һ��ȷ���۾���λ��׼ȷ�ԣ���Ҫ�������鲿�ַǷ�ɫ�ص���С�
    // ����ֱ�Ϊ����_lefteyeOpen��_righteyeOpen��32��24��С��YUV422ƽ���ʽ��˫��ͼƬ��
    // ������洢��point�ṹ����ptLeftEye_cfm��ptRightEye_cfm��
    POINT *ptLeftEye_cfm = new POINT;
    POINT *ptRightEye_cfm = new POINT;
    ptLeftEye_cfm->x = -1;
    ptLeftEye_cfm->y = -1;
    ptRightEye_cfm->x = -1;
    ptRightEye_cfm->y = -1;
    bool EyeballCheck_false = false;
    //����ɫ��У��
    EyeBallCheck(_lefteyeOpen, _righteyeOpen, ptLeftEye_cfm, ptRightEye_cfm);
    //�ж��Ƿ�ͨ������ɫ��У��
    if(ptLeftEye_cfm->x == -1 || ptLeftEye_cfm->y == -1 || ptRightEye_cfm->x == -1 || ptRightEye_cfm->y == -1)
    {
        EyeballCheck_false = false;
    }
    else
    {
        EyeballCheck_false = true;
    }
    // ShowDebugMessage("����ɫ�ؼ���:%d\n", EyeballCheck_false);

    //����λ��У��
    // ����λ��У�������漸�����ݣ�
    // 1�������������Ƿ�ȫ�������ڡ�ֻҪ��һ�������⣬У��ʧ�ܡ�
    // 2���۾�ˮƽ����У�飺
    // ���������е�λ��ˮƽ�������ҵ������߽磬�������left�� right���������������һ
    // �֣�У��ʧ�ܡ�
    bool EyeballPositionCheck_false = false;
    if(EyeballCheck_false){//����ɫ��У��ͨ��
        //����λ��У��
        EyeballPositionCheck_false = true;
        //����
        int fleft = pBufStruct->rcnFace.left*2;
        int ftop = pBufStruct->rcnFace.top*4;
        int fwidth = pBufStruct->rcnFace.width*2;
        int fheight = pBufStruct->rcnFace.height*4;
        int left = fleft;
        int right = fleft + fwidth;
        //��������������������ο���
        if(ptLeftEye_cfm->x < left || ptLeftEye_cfm->x > right || 
        ptLeftEye_cfm->y < ftop || ptLeftEye_cfm->y > ftop + fheight)
        {
            EyeballPositionCheck_false = false;
        }
        //����
        if(ptRightEye_cfm->x < left || ptRightEye_cfm->x > right || 
        ptRightEye_cfm->y < ftop || ptRightEye_cfm->y > ftop + fheight)
        {
            EyeballPositionCheck_false = false;
        }
    }
    // ShowDebugMessage("����λ�ü���:%d\n", EyeballPositionCheck_false);

    if(EyeballPositionCheck_false && EyeballCheck_false){
        pBufStruct->pOtherVars->objLefteye.rcObject=rcvLeftEye;
        pBufStruct->pOtherVars->objLefteyeClose.rcObject=rcvLeftEye;
        pBufStruct->pOtherVars->objRighteye.rcObject=rcvRightEye;
        pBufStruct->pOtherVars->objRighteyeClose.rcObject=rcvRightEye;
        pBufStruct->pOtherVars->objNose.rcObject=rcvNose;

        //��������ֵ���洢
        int Grayx=0,Grayy=0;//��ʱ������ŻҶ�����

        //��������
        FeatureVector4P* Lefteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvLeftEye.left,rcvLeftEye.top,rcvLeftEye.width,rcvLeftEye.height,Lefteyefvp);

        FeatureVector4P* Righteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvRightEye.left,rcvRightEye.top,rcvRightEye.width,rcvRightEye.height,Righteyefvp);

        FeatureVector4P* Nosefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvNose.left,rcvNose.top,rcvNose.width,rcvNose.height,Nosefvp);

        //����ֵ����fvObject_org��
        FeatureVector4P* Lefteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvLeftEye.left,rcvLeftEye.top,rcvLeftEye.width,rcvLeftEye.height,Lefteyefvp_org);

        FeatureVector4P* Righteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvRightEye.left,rcvRightEye.top,rcvRightEye.width,rcvRightEye.height,Righteyefvp_org);

        FeatureVector4P* Nosefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvNose.left,rcvNose.top,rcvNose.width,rcvNose.height,Nosefvp_org);
        //����bLastEyeCheckedΪTRUE
        pBufStruct->bLastEyeChecked = true;
        // //����
        // //ptLeftEye_cfm->x, ptLeftEye_cfm->yΪ��������
        // //rcvLeftEyeΪ�۾�����
        // FeatureVector4P* Lefteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector;
        // FeatureVector4P* Lefteye_pNL_LeftTop=Lefteyefvp + 1;
        // FeatureVector4P* Lefteye_pNL_pNL_RightTop=Lefteye_pNL_LeftTop+1;
        // FeatureVector4P* Lefteye_pNL_LeftBottom=Lefteye_pNL_pNL_RightTop+1;
        // FeatureVector4P* Lefteye_pNL_RightBottom=Lefteye_pNL_LeftBottom+1;


        // //������������Ͻ�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left, rcvLeftEye.top, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[0].x = Grayx;
        // Lefteyefvp->Vector[0].y = Grayy;
        // //������������Ͻ�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left+rcvLeftEye.width/2, rcvLeftEye.top, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[1].x = Grayx;
        // Lefteyefvp->Vector[1].y = Grayy;
        // //������������½�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left, rcvLeftEye.top+rcvLeftEye.height/2, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[2].x = Grayx;
        // Lefteyefvp->Vector[2].y = Grayy;
        // //������������½�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left+rcvLeftEye.width/2, rcvLeftEye.top+rcvLeftEye.height/2, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[3].x = Grayx;
        // Lefteyefvp->Vector[3].y = Grayy;
        // //��������
        // Lefteyefvp->nLevels=2;
        // Lefteyefvp->pNL_LeftTop=Lefteye_pNL_LeftTop;
        // Lefteyefvp->pNL_RightTop=Lefteye_pNL_pNL_RightTop;
        // Lefteyefvp->pNL_LeftBottom=Lefteye_pNL_LeftBottom;
        // Lefteyefvp->pNL_RightBottom=Lefteye_pNL_RightBottom;
        // //������fvObject_org��
        // FeatureVector4P* Lefteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector;
        // memcpy(Lefteyefvp_org, Lefteyefvp, sizeof(FeatureVector4P));




        // //����
        // //ptRightEye_cfm->x, ptRightEye_cfm->yΪ��������
        // //rcvRightEyeΪ�۾�����
        // FeatureVector4P* Righteyefvp = (FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector;
        // FeatureVector4P* Righteye_pNL_LeftTop = Righteyefvp + 1;
        // FeatureVector4P* Righteye_pNL_pNL_RightTop = Righteye_pNL_LeftTop + 1;
        // FeatureVector4P* Righteye_pNL_LeftBottom = Righteye_pNL_pNL_RightTop + 1;
        // FeatureVector4P* Righteye_pNL_RightBottom = Righteye_pNL_LeftBottom + 1;
        // //������������Ͻ�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left, rcvRightEye.top, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[0].x = Grayx;
        // Righteyefvp->Vector[0].y = Grayy;
        // //������������Ͻ�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left + rcvRightEye.width / 2, rcvRightEye.top, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[1].x = Grayx;
        // Righteyefvp->Vector[1].y = Grayy;
        // //������������½�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left, rcvRightEye.top + rcvRightEye.height / 2, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[2].x = Grayx;
        // Righteyefvp->Vector[2].y = Grayy;
        // //������������½�����ĻҶ�����
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left + rcvRightEye.width / 2, rcvRightEye.top + rcvRightEye.height / 2, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[3].x = Grayx;
        // Righteyefvp->Vector[3].y = Grayy;
        // //��������
        // Righteyefvp->pNL_LeftTop = Righteye_pNL_LeftTop;
        // Righteyefvp->pNL_RightTop = Righteye_pNL_pNL_RightTop;
        // Righteyefvp->pNL_LeftBottom = Righteye_pNL_LeftBottom;
        // Righteyefvp->pNL_RightBottom = Righteye_pNL_RightBottom;
        // //������fvObject_org��
        // FeatureVector4P* Righteyefvp_org = (FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector;
        // memcpy(Righteyefvp_org, Righteyefvp, sizeof(FeatureVector4P));

    }





	delete[] temp_lefteyeOpen;
    delete[] temp_righteyeOpen;
    delete[] temp_nose;
    delete[] _lefteyeOpen;
    delete[] _righteyeOpen;
    delete[] st_nose;
    delete[] ptLeftEye_cfm;
    delete[] ptRightEye_cfm;

    }
    delete[] n;

}


/*******************************************************************/

/*******************************************************************/              
DLL_EXP void ON_PLUGINEXIT()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//theApp.dlg.DestroyWindow();
}

