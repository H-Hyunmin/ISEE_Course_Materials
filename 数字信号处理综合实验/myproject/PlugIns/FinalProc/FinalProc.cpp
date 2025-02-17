// FinalProc.cpp : Defines the initialization routines for the DLL.
//

//����������
// ImageProc.lib TraceFeature.lib��ָ��Ҫ���ӵĿ��ļ�����Щ���ļ�������Ŀ����ĺ��������ݡ�

// /nologo��������ʾ���������������ı�־��Ϣ��

// /subsystem:windows��ָ����ϵͳ����Ϊ Windows������ζ�����ɵĿ�ִ���ļ���һ�� Windows Ӧ�ó��򣬶����ǿ���̨Ӧ�ó���

// /dll��ָʾ����������һ����̬���ӿ� (DLL)��

// /incremental:yes�������������ӣ�����Լӿ����ӹ��̣��ر����ڽ���С�Ĵ������ʱ��

// /pdb:"Debug/PB_FaceLocator.pdb"��ָ���������ݿ� (PDB) �ļ������·�������ơ�PDB �ļ�����������Ϣ��

// /debug�����ɰ���������Ϣ������ļ���

// /machine:I386��ָ��Ŀ��ƽ̨Ϊ Intel 386 ����߰汾�Ĵ�������

// /def:".\FaceLocator.def"��ָ��ģ�鶨���ļ� (DEF) ��·�������ơ�DEF �ļ����� DLL �ĵ����������������ԡ�

// /out:"../../Debug/Plugin/PB_FaceLocator.dll"��ָ�����ɵ� DLL �ļ������·�������ơ�

// /implib:"Debug/PB_FaceLocator.lib"��ָ�����ɵĵ�����ļ������·�������ơ�������ļ��������ӵ� DLL��

// /pdbtype:sept��ָ�� PDB �ļ�������Ϊ "separate"������ζ�ŵ�����Ϣ���洢��һ�������� PDB �ļ��У�������Ƕ�뵽��ִ���ļ��С�

// /libpath:"../../"��ָ�����ļ�������·�����������������·���в���ָ���Ŀ��ļ���




//����ͷ�ļ�,�ڱ����������ӿ��ļ�������
// /libpath:"../../"��ָ�����ļ�������·�����������������·���в���ָ���Ŀ��ļ���


#include "stdafx.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <windows.h>
#include <mmsystem.h>

#include "FinalProc.h"
#include "BufStruct.h"
#include "ImageProc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// #pragma comment(lib, "winmm.lib")
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
// CFinalProcApp

BEGIN_MESSAGE_MAP(CFinalProcApp, CWinApp)
	//{{AFX_MSG_MAP(CFinalProcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinalProcApp construction

CFinalProcApp::CFinalProcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFinalProcApp object

CFinalProcApp theApp;
char sInfo[] = "�˯���ͱ������";
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


//�Զ��庯����������������������������������������������������������������������������������������������������������������������������������
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
DLL_EXP void EyeBallPixelCount (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye)
{
    int nFacePtNum=0, nEyePtNum=0;
    //1��ͳ����������ķ�ɫ��������nFacePtNum��
    //2��ͳ����������������nEyePtNum��
    BYTE *leftU = _lefteyeOpen+LeftEye.width*LeftEye.height;
    BYTE *leftV = _lefteyeOpen+LeftEye.width*LeftEye.height+LeftEye.width*LeftEye.height/2;
    BYTE *rightU = _righteyeOpen+RightEye.width*RightEye.height;
    BYTE *rightV = _righteyeOpen+RightEye.width*RightEye.height+RightEye.width*RightEye.height/2;

    //����
    for (int j = 0; j < LeftEye.height; j++) {
		for (int i = 0; i < LeftEye.width/2; i++) {
			if ((leftU[j * LeftEye.width/2 + i] <= 131 && leftU[j * LeftEye.width/2 + i] >= 124) && (leftV[j * LeftEye.width/2 + i] <= 134 && leftV[j * LeftEye.width/2 + i] >= 121)) {
				nEyePtNum++;
			}
			if (leftU[j * LeftEye.width/2 + i] <= 126 && leftU[j * LeftEye.width/2 + i] >= 85 && leftV[j * LeftEye.width/2 + i] <= 165 && leftV[j * LeftEye.width/2 + i] >= 130) {
				nFacePtNum++;
			}
		}
	}

    // ShowDebugMessage("������������������%d\n", nEyePtNum);
    // ShowDebugMessage("���۷�ɫ����������%d\n", nFacePtNum);

    //����
    nFacePtNum = 0;
    nEyePtNum = 0;
    for ( j = 0; j < RightEye.height; j++) {
		for (int i = 0; i < RightEye.width/2; i++) {
			if ((rightU[j * RightEye.width/2 + i] <= 131 && rightU[j * RightEye.width/2 + i] >= 124) && (rightV[j * RightEye.width/2 + i] <= 134 && rightV[j * RightEye.width/2 + i] >= 121)) {
				nEyePtNum++;
			}
			if (rightU[j * RightEye.width/2 + i] <= 126 && rightU[j * RightEye.width/2 + i] >= 85 && rightV[j * RightEye.width/2 + i] <= 165 && rightV[j * RightEye.width/2 + i] >= 130) {
				nFacePtNum++;
			}
		}
    }



    // ShowDebugMessage("������������������%d\n", nEyePtNum);
    // ShowDebugMessage("���۷�ɫ����������%d\n", nFacePtNum);

}


DLL_EXP void CalWhitePtNum (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye){
	//ͳ������������۰���������nWhitePtNum��
	int nWhitePtNum=0;
	BYTE *leftY = _lefteyeOpen;
	BYTE *rightY = _righteyeOpen;
	//����
	for (int j = 0; j < LeftEye.height; j++) {
		for (int i = 0; i < LeftEye.width; i++) {
			if (leftY[j * LeftEye.width + i] >= 120) {
				nWhitePtNum++;
			}
		}
	}
	ShowDebugMessage("�����۰�����������%d\n", nWhitePtNum);
	//����
	nWhitePtNum = 0;
	for ( j = 0; j < RightEye.height; j++) {
		for (int i = 0; i < RightEye.width; i++) {
			if (rightY[j * RightEye.width + i] >= 120) {
				nWhitePtNum++;
			}
		}
	}
	ShowDebugMessage("�����۰�����������%d\n", nWhitePtNum);

}
DLL_EXP void CalAverageGray(BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye){
	//ͳ����������ĻҶ�ֵƽ��ֵnAverageGray��
	int nAverageGray=0;
	BYTE *leftY = _lefteyeOpen;
	BYTE *rightY = _righteyeOpen;
	//����
	for (int j = 0; j < LeftEye.height; j++) {
		for (int i = 0; i < LeftEye.width; i++) {
			nAverageGray += leftY[j * LeftEye.width + i];
		}
	}
	nAverageGray /= (LeftEye.width * LeftEye.height);
	ShowDebugMessage("���ۻҶ�ֵƽ��ֵ��%d\n", nAverageGray);
	//����
	nAverageGray = 0;
	for ( j = 0; j < RightEye.height; j++) {
		for (int i = 0; i < RightEye.width; i++) {
			nAverageGray += rightY[j * RightEye.width + i];
		}
	}
	nAverageGray /= (RightEye.width * RightEye.height);
	ShowDebugMessage("���ۻҶ�ֵƽ��ֵ��%d\n", nAverageGray);
}

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

//���ͼ��
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

//����Ҷ�ͼ��14,15,16,17������ķ���
DLL_EXP double CalDeviation(BYTE* LeftEyegray){
	//����Ҷ�ͼ��14,15,16,17������ÿ�еķ���
	double sum;
	double average;
	double deviation[4];
	//����14�з���
	sum = 0;
	for (int i = 0; i < 32; i++) {
		sum += LeftEyegray[14 * 32 + i];
	}
	average = sum / 32;
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += (LeftEyegray[14 * 32 + i] - average) * (LeftEyegray[14 * 32 + i] - average);
	}
	deviation[0] = sqrt(sum / 32);
	//����15�з���
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += LeftEyegray[15 * 32 + i];
	}
	average = sum / 32;
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += (LeftEyegray[15 * 32 + i] - average) * (LeftEyegray[15 * 32 + i] - average);
	}
	deviation[1] = sqrt(sum / 32);
	//����16�з���
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += LeftEyegray[16 * 32 + i];
	}
	average = sum / 32;
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += (LeftEyegray[16 * 32 + i] - average) * (LeftEyegray[16 * 32 + i] - average);
	}
	deviation[2] = sqrt(sum / 32);
	//����17�з���
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += LeftEyegray[17 * 32 + i];
	}
	average = sum / 32;
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += (LeftEyegray[17 * 32 + i] - average) * (LeftEyegray[17 * 32 + i] - average);
	}
	deviation[3] = sqrt(sum / 32);
	//ȡ���ֵ
	double deviation_max = deviation[0];
	for ( i = 1; i < 4; i++) {
		if (deviation[i] > deviation_max) {
			deviation_max = deviation[i];
		}
	}

	//��һ����0-10000
	deviation_max = deviation_max * 10000 / 255;
	return deviation_max;
}


#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// ��������ֵ��ƽ����
double squaredDifference(double a, double b) {
    return (a - b) * (a - b);
}

// ����������ĳ��ֵ������
int count(const std::vector<int>& vec, int value) {
    int count = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            count++;
        }
    }
    return count;
}

// ��׼������
// �����ݱ�׼������ֵΪ0����׼��Ϊ1�ķֲ�
void standardizeData(double* data, int length) {
    double mean = 0.0;
    for (int i = 0; i < length; ++i) {
        mean += data[i];
    }
    mean /= length;

    double stddev = 0.0;
    for ( i = 0; i < length; ++i) {
        stddev += (data[i] - mean) * (data[i] - mean);
    }
    stddev = sqrt(stddev / length);

    for ( i = 0; i < length; ++i) {
        data[i] = (data[i] - mean) / stddev;
    }
}

// ��������Ԫ�ص��ܺ�
double sum(const std::vector<double>& vec) {
    double total = 0.0;
    for (size_t i = 0; i < vec.size(); ++i) {
        total += vec[i];
    }
    return total;
}

// ʹ��K-means++ѡ���ʼ��������
void kmeansPlusPlusInit(double* data, int length, double& center1, double& center2) {
    srand(time(0));
    center1 = data[rand() % length];

    std::vector<double> distances(length, 0.0);
    for (int i = 0; i < length; ++i) {
        distances[i] = squaredDifference(data[i], center1);
    }

    double totalDistance = sum(distances);
    double randomValue = (rand() / (double)RAND_MAX) * totalDistance;

    for ( i = 0; i < length; ++i) {
        randomValue -= distances[i];
        if (randomValue <= 0) {
            center2 = data[i];
            break;
        }
    }
}

// �����ຯ��
// �������洢�Ҷ�ֵ��������飬���鳤��
// ����ֵ��ѡ���������deviationֵ��С���Ǹ��������������ռ�������ı���
DLL_EXP double TwoCluster(double *deviation, int length) {
    if (length <= 1) return 0.0;

    // ��׼������
    standardizeData(deviation, length);

    // ʹ��K-means++ѡ���ʼ��������
    double center1, center2;
    kmeansPlusPlusInit(deviation, length, center1, center2);

    std::vector<int> labels(length, 0);
    bool changed = true;

    while (changed) {
        changed = false;

        // ����ÿ���㵽����ľ�������
        for (int i = 0; i < length; ++i) {
            double dist1 = squaredDifference(deviation[i], center1);
            double dist2 = squaredDifference(deviation[i], center2);
            int newLabel = (dist1 < dist2) ? 0 : 1;

            if (labels[i] != newLabel) {
                labels[i] = newLabel;
                changed = true;
            }
        }

        // ���û���κα仯����ǰ��ֹ
        if (!changed) break;

        // ���¾�������
        double sum1 = 0.0, sum2 = 0.0;
        int count1 = 0, count2 = 0;

        for ( i = 0; i < length; ++i) {
            if (labels[i] == 0) {
                sum1 += deviation[i];
                count1++;
            } else {
                sum2 += deviation[i];
                count2++;
            }
        }

        if (count1 > 0) center1 = sum1 / count1;
        if (count2 > 0) center2 = sum2 / count2;
    }

    // ���������������ĵ�ƽ��deviationֵ
    double avgDeviation1 = center1;
    double avgDeviation2 = center2;

    // ѡ��deviationֵ��С���Ǹ����
    int selectedLabel = (avgDeviation1 < avgDeviation2) ? 0 : 1;

    // ������ѡ������������
    int selectedCount = count(labels, selectedLabel);

    // ������ѡ������ռ�������ı���
    double proportion = static_cast<double>(selectedCount) / length;
    return proportion;
}

// ����MP3�ļ��ĺ���
// void PlayMP3(const char* filename) {
//     // ��MP3�ļ�
//     mciSendString("open \"", NULL, 0, NULL);
//     mciSendString(filename, NULL, 0, NULL);
//     mciSendString("\" type mpegvideo alias mp3", NULL, 0, NULL);

//     // ����MP3�ļ�
//     mciSendString("play mp3", NULL, 0, NULL);
// }

// �Ӿ���ʾ����
void ShowVisualAlert() {
    MessageBox(NULL, TEXT("ƣ�ͱ�������ע����Ϣ��"), TEXT("ƣ�ͱ���"), MB_OK | MB_ICONWARNING);
}

DLL_EXP void SleepyAlarm(){
	// ��ʼƣ�ͱ���
	ShowDebugMessage("ע�⣡ƣ�ͱ�����\n");
	ShowDebugMessage("ע�⣡ƣ�ͱ�����\n");
	ShowDebugMessage("ע�⣡ƣ�ͱ�����\n");
	MessageBox(NULL, TEXT("ƣ�ͱ�������ע����Ϣ��"), TEXT("ƣ�ͱ���"), MB_OK | MB_ICONWARNING);
	
}
// // ��������ֵ��ƽ����
// double squaredDifference(double a, double b) {
//     return (a - b) * (a - b);
// }

// // ����������ĳ��ֵ������
// int count(const std::vector<int>& vec, int value) {
//     int count = 0;
//     for (size_t i = 0; i < vec.size(); ++i) {
//         if (vec[i] == value) {
//             count++;
//         }
//     }
//     return count;
// }

// // �����ຯ��
// // �������洢�Ҷ�ֵ��������飬���鳤��
// // ����ֵ��ѡ���������deviationֵ��С���Ǹ��������������ռ�������ı���
// DLL_EXP double TwoCluster(double *deviation, int length) {
//     if (length <= 1) return 0.0;

//     // �ҵ����ֵ����Сֵ
//     double minDeviation = *std::min_element(deviation, deviation + length);
//     double maxDeviation = *std::max_element(deviation, deviation + length);

//     // ��ʼ��������������Ϊ���ֵ����Сֵ
//     double center1 = minDeviation;
//     double center2 = maxDeviation;

//     std::vector<int> labels(length, 0);
//     bool changed = true;

//     while (changed) {
//         changed = false;

//         // ����ÿ���㵽����ľ�������
//         for (int i = 0; i < length; ++i) {
//             double dist1 = squaredDifference(deviation[i], center1);
//             double dist2 = squaredDifference(deviation[i], center2);
//             int newLabel = (dist1 < dist2) ? 0 : 1;

//             if (labels[i] != newLabel) {
//                 labels[i] = newLabel;
//                 changed = true;
//             }
//         }

//         // ���¾�������
//         double sum1 = 0.0, sum2 = 0.0;
//         int count1 = 0, count2 = 0;

//         for ( i = 0; i < length; ++i) {
//             if (labels[i] == 0) {
//                 sum1 += deviation[i];
//                 count1++;
//             } else {
//                 sum2 += deviation[i];
//                 count2++;
//             }
//         }

//         if (count1 > 0) center1 = sum1 / count1;
//         if (count2 > 0) center2 = sum2 / count2;
//     }

//     // ���������������ĵ�ƽ��deviationֵ
//     double avgDeviation1 = center1;
//     double avgDeviation2 = center2;

//     // ѡ��deviationֵ��С���Ǹ����
//     int selectedLabel = (avgDeviation1 < avgDeviation2) ? 0 : 1;

//     // ������ѡ������������
//     int selectedCount = count(labels, selectedLabel);

//     // ������ѡ������ռ�������ı���
//     double proportion = static_cast<double>(selectedCount) / length;
//     return proportion;
// }

//ȫ�ֱ�������������������������������������������������������������������������������������������������������������������������������������
//�Ҷ�ͼ����У����ڼ�������֡�ĻҶ�ֵ�仯
BYTE leftImageQueue[32*24*8]={0};
BYTE rightImageQueue[32*24*8]={0};
BYTE *leftImageQueuePtr[8]={0};
BYTE *rightImageQueuePtr[8]={0}; 

//֡��
#define FrameCount 150
double leftEyeDeviation[FrameCount]={0};
int leftEyeDeviationIndex=0;//����

int printLimit;

bool ImageQueuePtrInitFlag=false;//��ʼ����־
int ImageQueueIndex=-1;//ͼ���������
int Last3ImageQueueIndex=-1;//ǰ3֡ͼ���������
int Last4ImageQueueIndex=-1;//ǰ4֡ͼ���������
int Last5ImageQueueIndex=-1;//ǰ5֡ͼ���������
int LeftEyeBlinkCount=0;//����գ�۴���
int RightEyeBlinkCount=0;//����գ�۴���



DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//���д��Ӧ�������
	//ƣ�ͼ�⣬ͨ��գ��Ƶ�ʼ��ƣ��
	//��ƵƵ��Ϊ1��30֡
	//����ǰ֡�ز����ɻҶ�ͼ��
	//��ȡ��ǰ֡�۾����ĵ�
	//�������ĵ�����4��ÿ�еĻҶȷֲ�ƫ���ȡƽ��
	//����ʱ������ͫ�׺��۰׵ĻҶ�ֵ����ϴ���˻Ҷȷֲ�ƫ��ϴ�
	//����ʱ���Ҷȷֲ�ƫ���С
	//������¼150֡�ĻҶ�ͼ
	//���о��������ֵ�жϣ�����������״̬
	//�۾������Ѿ���¼�ڸ������С�
	_BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer;
	//��ʼ��
	// if(ImageQueuePtrInitFlag==false)
	// {
	// 	ShowDebugMessage("��ʼ��\n");
	// 	for(int i=0;i<8;i++)
	// 	{
	// 		leftImageQueuePtr[i]=leftImageQueue+i*32*24;
	// 		rightImageQueuePtr[i]=rightImageQueue+i*32*24;
	// 	}
	// 	ImageQueuePtrInitFlag=true;
	// 	leftEyeDeviationIndex=0;
	// }


	//δ������������߸����嶪ʧ�������³�ʼ��
	if(pBufStruct->bLastEyeChecked==false || pBufStruct->pOtherVars->objLefteye.bBrokenTrace || pBufStruct->pOtherVars->objRighteye.bBrokenTrace)
	{
		ShowDebugMessage("δ������������߸����嶪ʧ\n");
		//��ն���
		leftEyeDeviationIndex=0;
		return;
	}


	//��ʼ����ǰ֡
	// ShowDebugMessage("��ʼ����ǰ֡\n");
	//��ȡ�۾�����
	aRect theLeftEyeArea = pBufStruct->pOtherVars->objLefteye.rcObject;
	aRect theRightEyeArea = pBufStruct->pOtherVars->objRighteye.rcObject;
	//�洢�۾��������ɫͼ��
	BYTE *LeftEyecolorimg = new BYTE[theLeftEyeArea.width*theLeftEyeArea.height*2];
	BYTE *RightEyecolorimg = new BYTE[theRightEyeArea.width*theRightEyeArea.height*2];
	BYTE *LeftEyegray = new BYTE[32*24];
	BYTE *RightEyegray = new BYTE[32*24];
	//�����۾��������ɫͼ��
	CopyImg(pBufStruct->colorBmp,LeftEyecolorimg,w,h,theLeftEyeArea.left,theLeftEyeArea.top,theLeftEyeArea.width,theLeftEyeArea.height);
	CopyImg(pBufStruct->colorBmp,RightEyecolorimg,w,h,theRightEyeArea.left,theRightEyeArea.top,theRightEyeArea.width,theRightEyeArea.height);
	//�ز����ɻҶ�ͼ��
	ReSample(LeftEyecolorimg,theLeftEyeArea.width,theLeftEyeArea.height,32,24,false,true,LeftEyegray);
	ReSample(RightEyecolorimg,theRightEyeArea.width,theRightEyeArea.height,32,24,false,true,RightEyegray);



	leftEyeDeviation[leftEyeDeviationIndex]=CalDeviation(LeftEyegray);
	ShowDebugMessage("��%d֡�Ҷ�ֵ���%f\n", leftEyeDeviationIndex, leftEyeDeviation[leftEyeDeviationIndex]);
	if (leftEyeDeviationIndex == FrameCount-1) {
		double blinkRate = TwoCluster(leftEyeDeviation, FrameCount);
		ShowDebugMessage("գ��Ƶ�ʣ�%f\n", blinkRate);
		if (blinkRate > 0.3) {
			SleepyAlarm(); // գ��Ƶ�ʳ�����ֵ����������
		}
	}
	leftEyeDeviationIndex=(leftEyeDeviationIndex+1)%FrameCount;



	





	// //��˹�˲�
	// gaosi_filter(LeftEyegray,LeftEyegray,32,24);
	// gaosi_filter(RightEyegray,RightEyegray,32,24);

	// //����ǰ֡�ĻҶ�ͼ��������
	// if(ImageQueueIndex==-1)
	// {
	// 	//��ʼ��
	// 	for(int i=0;i<8;i++)
	// 	{
	// 		memcpy(leftImageQueuePtr[i],LeftEyegray,32*24);
	// 		memcpy(rightImageQueuePtr[i],RightEyegray,32*24);
	// 	}
	// 	ImageQueueIndex=5;
	// 	Last3ImageQueueIndex=2;
	// 	Last4ImageQueueIndex=1;
	// 	Last5ImageQueueIndex=0;
	// }
	// else
	// {
	// 	//����ǰ֡�ĻҶ�ͼ��������
	// 	ImageQueueIndex=(ImageQueueIndex+1)%8;
	// 	Last3ImageQueueIndex=(Last3ImageQueueIndex+1)%8;
	// 	Last4ImageQueueIndex=(Last4ImageQueueIndex+1)%8;
	// 	Last5ImageQueueIndex=(Last5ImageQueueIndex+1)%8;
	// 	memcpy(leftImageQueuePtr[ImageQueueIndex],LeftEyegray,32*24);
	// 	memcpy(rightImageQueuePtr[ImageQueueIndex],RightEyegray,32*24);
	// }

	// //������ʱ�����洢���ͼ��
	// BYTE *leftDiffImg3 = new BYTE[32*24];
	// BYTE *rightDiffImg3 = new BYTE[32*24];
	// BYTE *leftDiffImg4 = new BYTE[32*24];
	// BYTE *rightDiffImg4 = new BYTE[32*24];
	// BYTE *leftDiffImg5 = new BYTE[32*24];
	// BYTE *rightDiffImg5 = new BYTE[32*24];

	// //����ǰ3֡��ǰ4֡��ǰ5֡�Ĳ��ͼ��
	// diff_img(leftImageQueuePtr[Last3ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg3,32,24);
	// diff_img(rightImageQueuePtr[Last3ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg3,32,24);
	// diff_img(leftImageQueuePtr[Last4ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg4,32,24);
	// diff_img(rightImageQueuePtr[Last4ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg4,32,24);
	// diff_img(leftImageQueuePtr[Last5ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg5,32,24);
	// diff_img(rightImageQueuePtr[Last5ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg5,32,24);

	// //����ֱ��ͼ������ȷ����ֵ�����ж�գ��
	// int *n = new int[256];
	// memset(n,0,256*sizeof(int));
	// int N0=40,N1=8;
	// //�ȴ���leftDiffImg3
	// int graysum=0;
	// for( int i=0;i<32*24;i++)
	// {
	// 	graysum+=leftDiffImg3[i];
	// }
	// //ͳ��ֱ��ͼ
	// for( i=0;i<32*24;i++)
	// {
	// 	n[leftDiffImg3[i]]++;
	// }
	// //������ֵ
	// int threshold=255,sum=0;
	// while(sum<N0 && threshold>0)
	// {
	// 	sum+=n[threshold];
	// 	threshold--;
	// }
	// //�ж��Ƿ�գ��
	// if(graysum>=1800 && threshold>=10)
	// {
	// 	LeftEyeBlinkCount++;
	// }


	// //debug
	// ShowDebugMessage("��ֵ��%d\n,�Ҷ�ֵ�ܺͣ�%d\n",threshold,graysum);
	// ShowDebugMessage("����գ�۴�����%d\n",LeftEyeBlinkCount);




	//��ӡ�۾�����ĻҶ�ͼ��debug
	CopyToRect(LeftEyegray,pYBits,32,24,w,h,0,0,true);
	// CopyToRect(RightEyegray,pYBits,32,24,w,h,32,0,true);

	// // ��ӡ���ͼ��debug
	// CopyToRect(leftDiffImg3,pYBits,32,24,w,h,0,24,true);
	// CopyToRect(rightDiffImg3,pYBits,32,24,w,h,32,24,true);
	// CopyToRect(leftDiffImg4,pYBits,32,24,w,h,0,48,true);
	// CopyToRect(rightDiffImg4,pYBits,32,24,w,h,32,48,true);
	// CopyToRect(leftDiffImg5,pYBits,32,24,w,h,0,72,true);
	// CopyToRect(rightDiffImg5,pYBits,32,24,w,h,32,72,true);




	// //��ӡ�۾��������ɫͼ��debug
	// CopyToRect(LeftEyecolorimg,pYBits,theLeftEyeArea.width,theLeftEyeArea.height,w,h,0,0,false);
	// CopyToRect(RightEyecolorimg,pYBits,theRightEyeArea.width,theRightEyeArea.height,w,h,0,theLeftEyeArea.height+10,false);

	// //��ӡ�۾�����ĻҶ�ͼ��debug
	// CopyToRect(LeftEyegray,pYBits,32,24,w,h,theLeftEyeArea.height+30,0,true);
	// CopyToRect(RightEyegray,pYBits,32,24,w,h,theLeftEyeArea.height+30,theLeftEyeArea.height+10,true);




	delete [] LeftEyecolorimg;
	delete [] RightEyecolorimg;
	delete [] LeftEyegray;
	delete [] RightEyegray;
	//delete [] n;
	// delete [] leftDiffImg3;
	// delete [] rightDiffImg3;
	// delete [] leftDiffImg4;
	// delete [] rightDiffImg4;
	// delete [] leftDiffImg5;
	// delete [] rightDiffImg5;


}

DLL_EXP void ON_PLUGINEXIT()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());//ģ��״̬�л�
	//theApp.dlg.DestroyWindow();
}

