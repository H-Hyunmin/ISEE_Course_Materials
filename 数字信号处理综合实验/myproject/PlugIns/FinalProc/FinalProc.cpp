// FinalProc.cpp : Defines the initialization routines for the DLL.
//

//链接器配置
// ImageProc.lib TraceFeature.lib：指定要链接的库文件。这些库文件包含项目所需的函数和数据。

// /nologo：禁用显示编译器或链接器的标志信息。

// /subsystem:windows：指定子系统类型为 Windows，这意味着生成的可执行文件是一个 Windows 应用程序，而不是控制台应用程序。

// /dll：指示链接器生成一个动态链接库 (DLL)。

// /incremental:yes：启用增量链接，这可以加快链接过程，特别是在进行小的代码更改时。

// /pdb:"Debug/PB_FaceLocator.pdb"：指定程序数据库 (PDB) 文件的输出路径和名称。PDB 文件包含调试信息。

// /debug：生成包含调试信息的输出文件。

// /machine:I386：指定目标平台为 Intel 386 或更高版本的处理器。

// /def:".\FaceLocator.def"：指定模块定义文件 (DEF) 的路径和名称。DEF 文件定义 DLL 的导出函数和其他属性。

// /out:"../../Debug/Plugin/PB_FaceLocator.dll"：指定生成的 DLL 文件的输出路径和名称。

// /implib:"Debug/PB_FaceLocator.lib"：指定生成的导入库文件的输出路径和名称。导入库文件用于链接到 DLL。

// /pdbtype:sept：指定 PDB 文件的类型为 "separate"。这意味着调试信息将存储在一个单独的 PDB 文件中，而不是嵌入到可执行文件中。

// /libpath:"../../"：指定库文件的搜索路径。链接器将在这个路径中查找指定的库文件。




//增加头文件,在编译器中链接库文件！！！
// /libpath:"../../"：指定库文件的搜索路径。链接器将在这个路径中查找指定的库文件。


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
char sInfo[] = "瞌睡检测和报警插件";
bool bLastPlugin = false;
DLL_EXP void ON_PLUGIN_BELAST(bool bLast)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	bLastPlugin = bLast;
}

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


//自定义函数————————————————————————————————————————————————————————————————
DLL_EXP void CopyImg(aBYTE *src, aBYTE *dst, int w, int h, int x, int y, int w1, int h1) {
    // 计算源图像中Y、U、V分量的起始指针
    aBYTE *srcY = src;
    aBYTE *srcU = src + w * h;
    aBYTE *srcV = src + w * h + (w * h) / 2;

    // 计算目标图像中Y、U、V分量的起始指针
    aBYTE *dstY = dst;
    aBYTE *dstU = dst + w1 * h1;
    aBYTE *dstV = dst + w1 * h1 + (w1 * h1) / 2;

    // 复制Y分量（每像素一个Y值，大小为w*h）
    for (int j = 0; j < h1; j++) {
        for (int i = 0; i < w1; i++) {
            dstY[j * w1 + i] = srcY[(y + j) * w + (x + i)];
        }
    }

    // 复制U分量（水平方向采样率为Y的一半，垂直方向不变）
    for ( j = 0; j < h1; j++) {
        for (int i = 0; i < w1 / 2; i++) {
            dstU[j * (w1 / 2) + i] = srcU[(y + j) * (w / 2) + (x / 2) + i];
        }
    }

    // 复制V分量（同样，水平方向采样率为Y的一半，垂直方向不变）
    for ( j = 0; j < h1; j++) {
        for (int i = 0; i < w1 / 2; i++) {
            dstV[j * (w1 / 2) + i] = srcV[(y + j) * (w / 2) + (x / 2) + i];
        }
    }

}


//眼球色素校验目的是进一步确认眼睛定位的准确性，主要利用眼珠部分非肤色特点进行。
// 输入分别为向量_lefteyeOpen、_righteyeOpen中32×24大小的YUV422平面格式的双眼图片。
// 最后结果存储于point结构变量ptLeftEye_cfm、ptRightEye_cfm。
// 统计说明，眼珠为非肤色且灰度值较低，因此U空间满足式5.18或V空间满足式5.19时，
// 可认定为眼球部位。该过程采用U、V空间，由于YUV422平面格式的每两个像素取一个U、
DLL_EXP void EyeBallPixelCount (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye)
{
    int nFacePtNum=0, nEyePtNum=0;
    //1）统计左眼区域的肤色像素总数nFacePtNum。
    //2）统计左眼球像素总数nEyePtNum。
    BYTE *leftU = _lefteyeOpen+LeftEye.width*LeftEye.height;
    BYTE *leftV = _lefteyeOpen+LeftEye.width*LeftEye.height+LeftEye.width*LeftEye.height/2;
    BYTE *rightU = _righteyeOpen+RightEye.width*RightEye.height;
    BYTE *rightV = _righteyeOpen+RightEye.width*RightEye.height+RightEye.width*RightEye.height/2;

    //左眼
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

    // ShowDebugMessage("左眼眼球像素总数：%d\n", nEyePtNum);
    // ShowDebugMessage("左眼肤色像素总数：%d\n", nFacePtNum);

    //右眼
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



    // ShowDebugMessage("右眼眼球像素总数：%d\n", nEyePtNum);
    // ShowDebugMessage("右眼肤色像素总数：%d\n", nFacePtNum);

}


DLL_EXP void CalWhitePtNum (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye){
	//统计左眼区域的眼白像素总数nWhitePtNum。
	int nWhitePtNum=0;
	BYTE *leftY = _lefteyeOpen;
	BYTE *rightY = _righteyeOpen;
	//左眼
	for (int j = 0; j < LeftEye.height; j++) {
		for (int i = 0; i < LeftEye.width; i++) {
			if (leftY[j * LeftEye.width + i] >= 120) {
				nWhitePtNum++;
			}
		}
	}
	ShowDebugMessage("左眼眼白像素总数：%d\n", nWhitePtNum);
	//右眼
	nWhitePtNum = 0;
	for ( j = 0; j < RightEye.height; j++) {
		for (int i = 0; i < RightEye.width; i++) {
			if (rightY[j * RightEye.width + i] >= 120) {
				nWhitePtNum++;
			}
		}
	}
	ShowDebugMessage("右眼眼白像素总数：%d\n", nWhitePtNum);

}
DLL_EXP void CalAverageGray(BYTE* _lefteyeOpen, BYTE* _righteyeOpen, aRect LeftEye, aRect RightEye){
	//统计左眼区域的灰度值平均值nAverageGray。
	int nAverageGray=0;
	BYTE *leftY = _lefteyeOpen;
	BYTE *rightY = _righteyeOpen;
	//左眼
	for (int j = 0; j < LeftEye.height; j++) {
		for (int i = 0; i < LeftEye.width; i++) {
			nAverageGray += leftY[j * LeftEye.width + i];
		}
	}
	nAverageGray /= (LeftEye.width * LeftEye.height);
	ShowDebugMessage("左眼灰度值平均值：%d\n", nAverageGray);
	//右眼
	nAverageGray = 0;
	for ( j = 0; j < RightEye.height; j++) {
		for (int i = 0; i < RightEye.width; i++) {
			nAverageGray += rightY[j * RightEye.width + i];
		}
	}
	nAverageGray /= (RightEye.width * RightEye.height);
	ShowDebugMessage("右眼灰度值平均值：%d\n", nAverageGray);
}

DLL_EXP void gaosi_filter(aBYTE *src, aBYTE *dst, int w, int h) {
    // 3x3 单位矩阵
    float kernel[3][3] = {
        { 0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        { 0.0625, 0.125,  0.0625}
    };


    // 分配临时变量存储滤波结果
    aBYTE* tempImg = new aBYTE[w * h];
    memset(tempImg, 0, w * h * sizeof(aBYTE));

    // 对图像进行高斯滤波
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

    // 将临时变量的值拷贝给dst
    memcpy(dst, tempImg, w * h * sizeof(aBYTE));
    // 释放临时变量内存
    delete[] tempImg;
}

//差分图像
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

//计算灰度图像14,15,16,17行区域的方差
DLL_EXP double CalDeviation(BYTE* LeftEyegray){
	//计算灰度图像14,15,16,17行区域每行的方差
	double sum;
	double average;
	double deviation[4];
	//计算14行方差
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
	//计算15行方差
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
	//计算16行方差
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
	//计算17行方差
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
	//取最大值
	double deviation_max = deviation[0];
	for ( i = 1; i < 4; i++) {
		if (deviation[i] > deviation_max) {
			deviation_max = deviation[i];
		}
	}

	//归一化到0-10000
	deviation_max = deviation_max * 10000 / 255;
	return deviation_max;
}


#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// 计算两个值的平方差
double squaredDifference(double a, double b) {
    return (a - b) * (a - b);
}

// 计算数组中某个值的数量
int count(const std::vector<int>& vec, int value) {
    int count = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            count++;
        }
    }
    return count;
}

// 标准化数据
// 将数据标准化到均值为0，标准差为1的分布
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

// 计算数组元素的总和
double sum(const std::vector<double>& vec) {
    double total = 0.0;
    for (size_t i = 0; i < vec.size(); ++i) {
        total += vec[i];
    }
    return total;
}

// 使用K-means++选择初始聚类中心
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

// 二聚类函数
// 参数：存储灰度值方差的数组，数组长度
// 返回值：选择聚类结果中deviation值更小的那个结果，并计算其占总数量的比例
DLL_EXP double TwoCluster(double *deviation, int length) {
    if (length <= 1) return 0.0;

    // 标准化数据
    standardizeData(deviation, length);

    // 使用K-means++选择初始聚类中心
    double center1, center2;
    kmeansPlusPlusInit(deviation, length, center1, center2);

    std::vector<int> labels(length, 0);
    bool changed = true;

    while (changed) {
        changed = false;

        // 分配每个点到最近的聚类中心
        for (int i = 0; i < length; ++i) {
            double dist1 = squaredDifference(deviation[i], center1);
            double dist2 = squaredDifference(deviation[i], center2);
            int newLabel = (dist1 < dist2) ? 0 : 1;

            if (labels[i] != newLabel) {
                labels[i] = newLabel;
                changed = true;
            }
        }

        // 如果没有任何变化，提前终止
        if (!changed) break;

        // 更新聚类中心
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

    // 计算两个聚类中心的平均deviation值
    double avgDeviation1 = center1;
    double avgDeviation2 = center2;

    // 选择deviation值更小的那个结果
    int selectedLabel = (avgDeviation1 < avgDeviation2) ? 0 : 1;

    // 计算所选聚类结果的数量
    int selectedCount = count(labels, selectedLabel);

    // 计算所选聚类结果占总数量的比例
    double proportion = static_cast<double>(selectedCount) / length;
    return proportion;
}

// 播放MP3文件的函数
// void PlayMP3(const char* filename) {
//     // 打开MP3文件
//     mciSendString("open \"", NULL, 0, NULL);
//     mciSendString(filename, NULL, 0, NULL);
//     mciSendString("\" type mpegvideo alias mp3", NULL, 0, NULL);

//     // 播放MP3文件
//     mciSendString("play mp3", NULL, 0, NULL);
// }

// 视觉提示函数
void ShowVisualAlert() {
    MessageBox(NULL, TEXT("疲劳报警！请注意休息！"), TEXT("疲劳报警"), MB_OK | MB_ICONWARNING);
}

DLL_EXP void SleepyAlarm(){
	// 开始疲劳报警
	ShowDebugMessage("注意！疲劳报警！\n");
	ShowDebugMessage("注意！疲劳报警！\n");
	ShowDebugMessage("注意！疲劳报警！\n");
	MessageBox(NULL, TEXT("疲劳报警！请注意休息！"), TEXT("疲劳报警"), MB_OK | MB_ICONWARNING);
	
}
// // 计算两个值的平方差
// double squaredDifference(double a, double b) {
//     return (a - b) * (a - b);
// }

// // 计算数组中某个值的数量
// int count(const std::vector<int>& vec, int value) {
//     int count = 0;
//     for (size_t i = 0; i < vec.size(); ++i) {
//         if (vec[i] == value) {
//             count++;
//         }
//     }
//     return count;
// }

// // 二聚类函数
// // 参数：存储灰度值方差的数组，数组长度
// // 返回值：选择聚类结果中deviation值更小的那个结果，并计算其占总数量的比例
// DLL_EXP double TwoCluster(double *deviation, int length) {
//     if (length <= 1) return 0.0;

//     // 找到最大值和最小值
//     double minDeviation = *std::min_element(deviation, deviation + length);
//     double maxDeviation = *std::max_element(deviation, deviation + length);

//     // 初始化两个聚类中心为最大值和最小值
//     double center1 = minDeviation;
//     double center2 = maxDeviation;

//     std::vector<int> labels(length, 0);
//     bool changed = true;

//     while (changed) {
//         changed = false;

//         // 分配每个点到最近的聚类中心
//         for (int i = 0; i < length; ++i) {
//             double dist1 = squaredDifference(deviation[i], center1);
//             double dist2 = squaredDifference(deviation[i], center2);
//             int newLabel = (dist1 < dist2) ? 0 : 1;

//             if (labels[i] != newLabel) {
//                 labels[i] = newLabel;
//                 changed = true;
//             }
//         }

//         // 更新聚类中心
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

//     // 计算两个聚类中心的平均deviation值
//     double avgDeviation1 = center1;
//     double avgDeviation2 = center2;

//     // 选择deviation值更小的那个结果
//     int selectedLabel = (avgDeviation1 < avgDeviation2) ? 0 : 1;

//     // 计算所选聚类结果的数量
//     int selectedCount = count(labels, selectedLabel);

//     // 计算所选聚类结果占总数量的比例
//     double proportion = static_cast<double>(selectedCount) / length;
//     return proportion;
// }

//全局变量区————————————————————————————————————————————————————————————————
//灰度图像队列，用于计算连续帧的灰度值变化
BYTE leftImageQueue[32*24*8]={0};
BYTE rightImageQueue[32*24*8]={0};
BYTE *leftImageQueuePtr[8]={0};
BYTE *rightImageQueuePtr[8]={0}; 

//帧数
#define FrameCount 150
double leftEyeDeviation[FrameCount]={0};
int leftEyeDeviationIndex=0;//索引

int printLimit;

bool ImageQueuePtrInitFlag=false;//初始化标志
int ImageQueueIndex=-1;//图像队列索引
int Last3ImageQueueIndex=-1;//前3帧图像队列索引
int Last4ImageQueueIndex=-1;//前4帧图像队列索引
int Last5ImageQueueIndex=-1;//前5帧图像队列索引
int LeftEyeBlinkCount=0;//左眼眨眼次数
int RightEyeBlinkCount=0;//右眼眨眼次数



DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//请编写相应处理程序
	//疲劳检测，通过眨眼频率检测疲劳
	//视频频率为1秒30帧
	//将当前帧重采样成灰度图像
	//获取当前帧眼睛中心点
	//计算中心点上下4行每行的灰度分布偏差后取平均
	//睁眼时，由于瞳孔和眼白的灰度值差异较大，因此灰度分布偏差较大
	//闭眼时，灰度分布偏差较小
	//连续记录150帧的灰度图
	//进行聚类或者阈值判断，区分睁闭眼状态
	//眼睛区域已经记录在跟踪体中。
	_BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer;
	//初始化
	// if(ImageQueuePtrInitFlag==false)
	// {
	// 	ShowDebugMessage("初始化\n");
	// 	for(int i=0;i<8;i++)
	// 	{
	// 		leftImageQueuePtr[i]=leftImageQueue+i*32*24;
	// 		rightImageQueuePtr[i]=rightImageQueue+i*32*24;
	// 	}
	// 	ImageQueuePtrInitFlag=true;
	// 	leftEyeDeviationIndex=0;
	// }


	//未建立跟踪体或者跟踪体丢失，则重新初始化
	if(pBufStruct->bLastEyeChecked==false || pBufStruct->pOtherVars->objLefteye.bBrokenTrace || pBufStruct->pOtherVars->objRighteye.bBrokenTrace)
	{
		ShowDebugMessage("未建立跟踪体或者跟踪体丢失\n");
		//清空队列
		leftEyeDeviationIndex=0;
		return;
	}


	//开始处理当前帧
	// ShowDebugMessage("开始处理当前帧\n");
	//获取眼睛区域
	aRect theLeftEyeArea = pBufStruct->pOtherVars->objLefteye.rcObject;
	aRect theRightEyeArea = pBufStruct->pOtherVars->objRighteye.rcObject;
	//存储眼睛区域的颜色图像
	BYTE *LeftEyecolorimg = new BYTE[theLeftEyeArea.width*theLeftEyeArea.height*2];
	BYTE *RightEyecolorimg = new BYTE[theRightEyeArea.width*theRightEyeArea.height*2];
	BYTE *LeftEyegray = new BYTE[32*24];
	BYTE *RightEyegray = new BYTE[32*24];
	//复制眼睛区域的颜色图像
	CopyImg(pBufStruct->colorBmp,LeftEyecolorimg,w,h,theLeftEyeArea.left,theLeftEyeArea.top,theLeftEyeArea.width,theLeftEyeArea.height);
	CopyImg(pBufStruct->colorBmp,RightEyecolorimg,w,h,theRightEyeArea.left,theRightEyeArea.top,theRightEyeArea.width,theRightEyeArea.height);
	//重采样成灰度图像
	ReSample(LeftEyecolorimg,theLeftEyeArea.width,theLeftEyeArea.height,32,24,false,true,LeftEyegray);
	ReSample(RightEyecolorimg,theRightEyeArea.width,theRightEyeArea.height,32,24,false,true,RightEyegray);



	leftEyeDeviation[leftEyeDeviationIndex]=CalDeviation(LeftEyegray);
	ShowDebugMessage("第%d帧灰度值方差：%f\n", leftEyeDeviationIndex, leftEyeDeviation[leftEyeDeviationIndex]);
	if (leftEyeDeviationIndex == FrameCount-1) {
		double blinkRate = TwoCluster(leftEyeDeviation, FrameCount);
		ShowDebugMessage("眨眼频率：%f\n", blinkRate);
		if (blinkRate > 0.3) {
			SleepyAlarm(); // 眨眼频率超过阈值，触发报警
		}
	}
	leftEyeDeviationIndex=(leftEyeDeviationIndex+1)%FrameCount;



	





	// //高斯滤波
	// gaosi_filter(LeftEyegray,LeftEyegray,32,24);
	// gaosi_filter(RightEyegray,RightEyegray,32,24);

	// //将当前帧的灰度图像存入队列
	// if(ImageQueueIndex==-1)
	// {
	// 	//初始化
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
	// 	//将当前帧的灰度图像存入队列
	// 	ImageQueueIndex=(ImageQueueIndex+1)%8;
	// 	Last3ImageQueueIndex=(Last3ImageQueueIndex+1)%8;
	// 	Last4ImageQueueIndex=(Last4ImageQueueIndex+1)%8;
	// 	Last5ImageQueueIndex=(Last5ImageQueueIndex+1)%8;
	// 	memcpy(leftImageQueuePtr[ImageQueueIndex],LeftEyegray,32*24);
	// 	memcpy(rightImageQueuePtr[ImageQueueIndex],RightEyegray,32*24);
	// }

	// //分配临时变量存储差分图像
	// BYTE *leftDiffImg3 = new BYTE[32*24];
	// BYTE *rightDiffImg3 = new BYTE[32*24];
	// BYTE *leftDiffImg4 = new BYTE[32*24];
	// BYTE *rightDiffImg4 = new BYTE[32*24];
	// BYTE *leftDiffImg5 = new BYTE[32*24];
	// BYTE *rightDiffImg5 = new BYTE[32*24];

	// //计算前3帧、前4帧、前5帧的差分图像
	// diff_img(leftImageQueuePtr[Last3ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg3,32,24);
	// diff_img(rightImageQueuePtr[Last3ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg3,32,24);
	// diff_img(leftImageQueuePtr[Last4ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg4,32,24);
	// diff_img(rightImageQueuePtr[Last4ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg4,32,24);
	// diff_img(leftImageQueuePtr[Last5ImageQueueIndex],leftImageQueuePtr[ImageQueueIndex],leftDiffImg5,32,24);
	// diff_img(rightImageQueuePtr[Last5ImageQueueIndex],rightImageQueuePtr[ImageQueueIndex],rightDiffImg5,32,24);

	// //采用直方图方法来确定阈值方法判断眨眼
	// int *n = new int[256];
	// memset(n,0,256*sizeof(int));
	// int N0=40,N1=8;
	// //先处理leftDiffImg3
	// int graysum=0;
	// for( int i=0;i<32*24;i++)
	// {
	// 	graysum+=leftDiffImg3[i];
	// }
	// //统计直方图
	// for( i=0;i<32*24;i++)
	// {
	// 	n[leftDiffImg3[i]]++;
	// }
	// //计算阈值
	// int threshold=255,sum=0;
	// while(sum<N0 && threshold>0)
	// {
	// 	sum+=n[threshold];
	// 	threshold--;
	// }
	// //判断是否眨眼
	// if(graysum>=1800 && threshold>=10)
	// {
	// 	LeftEyeBlinkCount++;
	// }


	// //debug
	// ShowDebugMessage("阈值：%d\n,灰度值总和：%d\n",threshold,graysum);
	// ShowDebugMessage("左眼眨眼次数：%d\n",LeftEyeBlinkCount);




	//打印眼睛区域的灰度图像debug
	CopyToRect(LeftEyegray,pYBits,32,24,w,h,0,0,true);
	// CopyToRect(RightEyegray,pYBits,32,24,w,h,32,0,true);

	// // 打印差分图像debug
	// CopyToRect(leftDiffImg3,pYBits,32,24,w,h,0,24,true);
	// CopyToRect(rightDiffImg3,pYBits,32,24,w,h,32,24,true);
	// CopyToRect(leftDiffImg4,pYBits,32,24,w,h,0,48,true);
	// CopyToRect(rightDiffImg4,pYBits,32,24,w,h,32,48,true);
	// CopyToRect(leftDiffImg5,pYBits,32,24,w,h,0,72,true);
	// CopyToRect(rightDiffImg5,pYBits,32,24,w,h,32,72,true);




	// //打印眼睛区域的颜色图像debug
	// CopyToRect(LeftEyecolorimg,pYBits,theLeftEyeArea.width,theLeftEyeArea.height,w,h,0,0,false);
	// CopyToRect(RightEyecolorimg,pYBits,theRightEyeArea.width,theRightEyeArea.height,w,h,0,theLeftEyeArea.height+10,false);

	// //打印眼睛区域的灰度图像debug
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
   AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.DestroyWindow();
}

