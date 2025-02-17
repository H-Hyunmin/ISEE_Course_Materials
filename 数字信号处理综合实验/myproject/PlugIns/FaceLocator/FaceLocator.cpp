// FaceLocator.cpp : Defines the initialization routines for the DLL.
#include "stdafx.h"
#include "FaceLocator.h"
#include "BufStruct.h"
#include "ImageProc.h"

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
// CFaceLocatorApp
BEGIN_MESSAGE_MAP(CFaceLocatorApp, CWinApp)
	//{{AFX_MSG_MAP(CFaceLocatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CFaceLocatorApp construction
CFaceLocatorApp::CFaceLocatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CFaceLocatorApp object
CFaceLocatorApp theApp;
char sInfo[] = "��������-���ڲ�ɫ��Ϣ�������ָ����";
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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

/****************************************************************************************/
/*                               myfunction                                             */
/****************************************************************************************/


// // ����ֱ��ͼ
// // ������src - ����ͼ�����ݣ�histogram - ���ֱ��ͼ���飬size - ͼ�����ݴ�С
// void calculateHistogram(const BYTE* src, int* histogram, int size) {
//     std::memset(histogram, 0, 256 * sizeof(int));
//     for (int i = 0; i < size; ++i) {
//         histogram[src[i]]++;
//     }
// }

// // �����ۻ�ֱ��ͼ
// // ������histogram - ����ֱ��ͼ���飬cumulativeHistogram - ����ۻ�ֱ��ͼ����
// void calculateCumulativeHistogram(const int* histogram, int* cumulativeHistogram) {
//     cumulativeHistogram[0] = histogram[0];
//     for (int i = 1; i < 256; ++i) {
//         cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
//     }
// }

// /**
//  * @brief  ʹ��ֱ��ͼ���⻯���й��ղ���
//  * 
//  * @param src ����ͼ������
//  * @param dst  ���ͼ������
//  * @param width  ͼ����
//  * @param height  ͼ��߶�
//  */
// void illuminationCompensation(BYTE* src, BYTE* dst, int width, int height) {
//     int size = width * height;
//     int histogram[256];
//     int cumulativeHistogram[256];

//     // ����ֱ��ͼ
//     calculateHistogram(src, histogram, size);

//     // �����ۻ�ֱ��ͼ
//     calculateCumulativeHistogram(histogram, cumulativeHistogram);

//     // ������⻯�������ֵ
//     BYTE lut[256];
//     for (int i = 0; i < 256; ++i) {
//         lut[i] = static_cast<BYTE>(255.0 * cumulativeHistogram[i] / size + 0.5);
//     }

//     // Ӧ�þ��⻯�������ֵ
//     for (int i = 0; i < size; ++i) {
//         dst[i] = lut[src[i]];
//     }
// }




//��ʴ
//NxN����NΪ����
//src:����ͼ��
//dst:���ͼ��
//w:ͼ����
//h:ͼ��߶�
DLL_EXP void erode(aBYTE *src, aBYTE *dst, int w, int h, int N) {
	//������ʱ�ռ䲢��ʼ��
	BYTE* tempImage1 = myHeapAlloc(w*h);
	BYTE* tempImage2 = myHeapAlloc(w*h);
	memset(tempImage1, 0, w*h);
	memset(tempImage2, 0, w*h);

	//1xN��������ʴ
	for (int i = 0; i < h; i++)
	{
		for (int j = N / 2; j < w - N / 2; j++)
		{
			bool flag = true;
			for (int k = 0; k < N; k++)
			{
				if (src[i*w + j - N / 2 + k] == 0)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				tempImage1[i*w + j] = 255;
			}
		}
	}
	//��tempImage1Nx1��������ʴ���������tempImage2
	for (i = N / 2; i < h - N / 2; i++)
	{
		for (int j = 0; j < w; j++)
		{
			bool flag = true;
			for (int k = 0; k < N; k++)
			{
				if (tempImage1[(i - N / 2 + k)*w + j] == 0)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				tempImage2[i*w + j] = 255;
			}
		}
	}
	memcpy(dst, tempImage2, w*h);
	myHeapFree(tempImage2);
	myHeapFree(tempImage1);
}


DLL_EXP void dilate(aBYTE *src, aBYTE *dst, int w, int h, int N) {
	//������ʱ�ռ䲢��ʼ��
	BYTE* tempImg1 = myHeapAlloc(w * h);
	BYTE* tempImg2 = myHeapAlloc(w * h);
	memset(tempImg1, 0, w*h);
	memset(tempImg2, 0, w*h);

	//NxN��������
	//1xN����������
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			bool flag = false;
			for (int k = -N / 2; k <= N / 2; k++)
			{
				int x = j + k;
				if (x >= 0 && x < w && src[i * w + x] == 255)
				{
					flag = true;
					break;
				}
			}
			if (flag)
			{
				tempImg1[i * w + j] = 255;
			}
		}
	}
	//��tempImg1Nx1���������ͣ��������tempImg2
	for (i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			bool flag = false;
			for (int k = -N / 2; k <= N / 2; k++)
			{
				int y = i + k;
				if (y >= 0 && y < h && tempImg1[y * w + j] == 255)
				{
					flag = true;
					break;
				}
			}
			if (flag)
			{
				tempImg2[i * w + j] = 255;
			}
		}
	}
	memcpy(dst, tempImg2, w * h);
	myHeapFree(tempImg2);
	myHeapFree(tempImg1);
}

DLL_EXP void open(aBYTE *src, aBYTE *dst, int w, int h, int N) {
	//��ʴ
	erode(src, dst, w, h, N);
	//����
	dilate(src, dst, w, h, N);
}
DLL_EXP void close(aBYTE *src, aBYTE *dst, int w, int h, int N) {
	//����
	dilate(src, dst, w, h, N);
	//��ʴ
	erode(src, dst, w, h, N);
}

//��ͨ���Ǻ���
DLL_EXP void connectedcomponentlabelling(aBYTE *src, aBYTE *dst, int w, int h) {
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


    //ͼƬ�滻
    for ( i = 0; i < w * h; i++) {
        tempImage[i] = LK1[tempImage[i]];
    }

    //�ҵ����ı��
    int* labelCount = new int[nextLabel];
    memset(labelCount, 0, nextLabel * sizeof(int));
    for ( i = 0; i < w * h; i++) {
        if (tempImage[i] != 0)
        labelCount[tempImage[i]]++;
    }
    int maxLabel = 0;
    for ( i = 0; i < nextLabel; i++) {
        if (labelCount[i] > labelCount[maxLabel]) {
            maxLabel = i;
        }
    }


    //�����ı����Ϊ255 ������Ϊ0
    for ( i = 0; i < w * h; i++) {
        if (tempImage[i] == maxLabel) {
            dst[i] = 255;
        } else {
            dst[i] = 0;
        }
    }
    
    delete[] tempImage;
    delete[] LK;
    delete[] LK1;
    delete[] labelCount;
}


DLL_EXP void faceLoc(aBYTE *src, aRect *face, int w, int h, BYTE* pBuffer) {
	int left, top, width, height;
	int flag = 0;
	int i, j;

	for (i = 0; i < w*h; i += w) {
		for (j = 0; j < w; j++) {
			if (src[i + j] == 255) {
				if (flag == 0) {
					top = i/w;
					flag = 1;
				}
				break;
			}
		}
		if ((j == w && flag == 1) || (i == (w*(h-1)))) {
			height = i/w - top;
			break;
		}
	}
	flag = 0;
	for (i = 0; i < w; i++) {
		for (j = 0; j < w*h; j += w) {
			if (src[i + j] == 255) {
				if (flag == 0) {
					left = i;
					flag = 1;
				}
				break;
			}
		}
		if ((j == w*h && flag == 1) || (i == w - 1)) {
			width = i - left;
			break;
		}
	}
	// ShowDebugMessage("left=%d, width=%d, top=%d, height=%d", left, width, top, height);
	((BUF_STRUCT*)pBuffer)->rcnFace.left = left * 2;
	((BUF_STRUCT*)pBuffer)->rcnFace.top = top;
	((BUF_STRUCT*)pBuffer)->rcnFace.width = width * 2;
	((BUF_STRUCT*)pBuffer)->rcnFace.height = height;
	((BUF_STRUCT*)pBuffer)->nFacePixelNum = width * height * 2;

	for (i = 0; i < w*h; i += w) {
		for (j = 0; j < w; j++) {
			((BUF_STRUCT*)pBuffer)->clrBmp_1d8[(i+j)*2] = src[i + j];
			((BUF_STRUCT*)pBuffer)->clrBmp_1d8[(i+j)*2 + 1] = src[i + j];
		}
	}
	face->left = ((BUF_STRUCT*)pBuffer)->rcnFace.left;
	face->width = ((BUF_STRUCT*)pBuffer)->rcnFace.width * 2;
	face->top = ((BUF_STRUCT*)pBuffer)->rcnFace.top * 4;
	face->height = ((BUF_STRUCT*)pBuffer)->rcnFace.height * 4;
}


/****************************************************************************************/
/*                             ��������붨λ                                           */
/****************************************************************************************/


DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
//pYBits ��СΪw*h
//pUBits �� pVBits �Ĵ�СΪ w*h/2
//pBuffer �Ĵ�СΪ w*h*4
//�����㷨������һ�����裬��w��16�ı���

	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	
    //���д��Ӧ�������
	//1���ղ���
	// illuminationCompensation (pYBits, pYBits, w, h);


	//2.��ɫ��ģ
	//���YUV����ָ��
	BYTE* pY = (aBYTE*)(((BUF_STRUCT*)pBuffer)->clrBmp_1d8);
	BYTE* pU = pY + w*h/8;
	BYTE* pV = pU + w*h/16;

	//��ֵ������
	BYTE* tempImage = myHeapAlloc(w*h/16); //��ʱͼ��
	for ( int i = 0; i < w*h/16; i++)
	{
		tempImage[i] = (pU[i] >=85 && pU[i] <= 126 && pV[i] >= 130 && pV[i] <= 165) ? 255 : 0;
	}
	// CopyToRect(tempImage, pYBits, w/4, h/4, w, h, 0, 0, true);
	// // ��ֵ������
	// BYTE* tempImage = myHeapAlloc(w * h / 16); // ��ʱͼ��
	// for (int i = 0; i < w * h / 16; i++) {
	// 	tempImage[i] = 255 * (((BUF_STRUCT*)pBuffer)->pOtherVars->byHistMap_U[pU[i]] & ((BUF_STRUCT*)pBuffer)->pOtherVars->byHistMap_V[pV[i]]);
	// }

	//3.��̬ѧ����
	//������
	open(tempImage, tempImage, w/4, h/4, 3);
	//������
	close(tempImage, tempImage, w/4, h/4, 7);

	// CopyToRect(tempImage, pYBits, w/4, h/4, w, h, 0, h/4, true);
	//4.�������
	connectedcomponentlabelling(tempImage, tempImage, w/4, h/4);

	//5.��������붨λ
	// aRect face;
	// faceLoc(tempImage, &face, w/4, h/4, pBuffer);
	// DrawRectangle(((BUF_STRUCT*)pBuffer)->displayImage, w, h, face, TYUV1(0, 191, 255), false);

	aRect faceRect;//���ڻ�ͼ
	//�ҵ���ֵͼ255Ԫ�ؾ��ο��
	int minX = w / 4, minY = h / 4, maxX = 0, maxY = 0;
	bool found = false;

	for (int y = 0; y < h / 4; ++y) {
		for (int x = 0; x < w / 4; ++x) {
			if (tempImage[y * (w / 4) + x] == 255) {
				if (!found) {
					found = true;
				}
				minX = (x < minX) ? x : minX;
				minY = (y < minY) ? y : minY;
				maxX = (x > maxX) ? x : maxX;
				maxY = (y > maxY) ? y : maxY;
			}
		}
	}
	if (found){
	((BUF_STRUCT*)pBuffer)->rcnFace.left = minX * 2;
	((BUF_STRUCT*)pBuffer)->rcnFace.top = minY;
	((BUF_STRUCT*)pBuffer)->rcnFace.width = (maxX - minX + 1) * 2;
	((BUF_STRUCT*)pBuffer)->rcnFace.height = maxY - minY + 1;
	((BUF_STRUCT*)pBuffer)->nFacePixelNum = ((maxX - minX + 1) * 2) * (maxY - minY + 1);
	}
	else{
	((BUF_STRUCT*)pBuffer)->rcnFace.left = 0;
	((BUF_STRUCT*)pBuffer)->rcnFace.top = 0;
	((BUF_STRUCT*)pBuffer)->rcnFace.width = 0;
	((BUF_STRUCT*)pBuffer)->rcnFace.height = 0;
	((BUF_STRUCT*)pBuffer)->nFacePixelNum = 0;
	}

	//����ֵͼ����չ��clrBmp_1d8
	for(i=0;i<h/4;i++){
		for(int j=0;j<w/4;j++){
			((BUF_STRUCT*)pBuffer)->clrBmp_1d8[(i*w/4+j)*2] = tempImage[i*w/4+j];
			((BUF_STRUCT*)pBuffer)->clrBmp_1d8[(i*w/4+j)*2+1] = tempImage[i*w/4+j];
		}
	}


	faceRect.left = minX * 4;
	faceRect.top = minY * 4;
	faceRect.width = (maxX - minX + 1) * 4;
	faceRect.height = (maxY - minY + 1) * 4;

	DrawRectangle(((BUF_STRUCT*)pBuffer)->displayImage, w, h, faceRect, TYUV1(0, 191, 255), false);

	//CopyToRect(((BUF_STRUCT*)pBuffer)->clrBmp_1d8,  pYBits, w/2, h/4, w, h, w/2, 0, false);

	myHeapFree(tempImage);
}

DLL_EXP void ON_PLUGINEXIT()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//theApp.dlg.DestroyWindow();
}



