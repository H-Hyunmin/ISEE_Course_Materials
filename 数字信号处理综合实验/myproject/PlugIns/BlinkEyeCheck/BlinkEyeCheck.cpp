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
这些变量只在PC版中存在，方便调试
在DSP版本中不用引入
***************************************************************/
#ifdef PC_MODE
aBYTE open_eye_left[32*24*2];
aBYTE open_eye_right[32*24*2];
aBYTE close_eye_left[32*24];
aBYTE close_eye_right[32*24];
aBYTE st_nose[32*48*2];
#endif


//插件定义变量


tagPOINT ptLeftEye, ptRightEye; // 左右眼坐标


bool bret = false; // 是否检测到眼睛

char sInfo[] = "人脸跟踪-眨眼检测人脸定位处理插件";



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


/*******************************************************************/
//							My Function
/*******************************************************************/

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
    // 分配临时空间并初始化
    aBYTE* tempImage = new aBYTE[w * h];
    memset(tempImage, 0, w * h);

    // 3x1结构元素的腐蚀操作
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
    
    // 3x1结构元素的膨胀操作
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

    // 释放临时空间
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

    int nextLabel = 1; //下一个标记
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int index = y * w + x; // 当前像素索引
            if (src[index] == 0) {
                tempImage[index] = 0; 
                continue;
            }

            // 获取邻居像素的标记
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
                // 两个邻居像素都有标记，取左边
				tempImage[index] = leftLabel;

				// 更新等价表
				int Lmax = leftLabel>topLabel?leftLabel:topLabel;
				int Lmin = leftLabel<topLabel?leftLabel:topLabel;
                
                //
                if (LK[Lmax] != Lmax) {//追踪
                    int j = Lmax;
                    while (LK[j] != j) {
                        j = LK[j];
                    }
                    if (j>Lmin) LK[j] = Lmin;
                    else LK[Lmin] = j;
                }
                else{//向上查找，直到找到最小的标记
                    while (LK[Lmin] != Lmin) {
                        Lmin = LK[Lmin];
                    }
                    //记录等价关系
                    LK[Lmax] = Lmin;
                }

            }
        }
    }


    // 整理等价表
	for ( i = 0; i < nextLabel; i++) {
        int j = i;
        while (LK[j] != j) {
            j = LK[j];
        }
        LK[i] =j;
	}

    //连通区域重新编号
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
    //连通区域总数
    int nconnetedcomponent = newi - 1;
    // ShowDebugMessage("连通区域总数：%d\n", nconnetedcomponent);
    //图片替换
    for ( i = 0; i < w * h; i++) {
        tempImage[i] = LK1[tempImage[i]];
    }

        // 统计各个连通区域的大小和重心
    int *pixelCount = new int[nconnetedcomponent + 1]; // pixelCount[i]记录标号i的连通区域的像素个数，pixelCount[0]不用
    int *size = new int[nconnetedcomponent + 1]; // 记录每个连通区域的大小,size=x^2+y^2, x和y分别是连通区域的宽和高
    float *centerX = new float[nconnetedcomponent + 1]; // 记录每个连通区域的重心x坐标,centerX[i] = sigma(x)/pixelCount[i]
    float *centerY = new float[nconnetedcomponent + 1]; // 记录每个连通区域的重心y坐标
    
    // 初始化统计数组
    for (i = 1; i <= nconnetedcomponent; i++) {
        pixelCount[i] = 0;
        size[i] = 0;
        centerX[i] = 0.0f;
        centerY[i] = 0.0f;
    }
    
    // 统计每个连通区域的像素个数和重心
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
    
    // 计算每个连通区域的重心
    for (i = 1; i <= nconnetedcomponent; i++) {
        if (pixelCount[i] > 0) {
            centerX[i] /= pixelCount[i];
            centerY[i] /= pixelCount[i];
        }
    }
    
    // 找到框定每个标号的连通区域的矩形
    int *minX = new int[nconnetedcomponent + 1];
    int *minY = new int[nconnetedcomponent + 1];
    int *maxX = new int[nconnetedcomponent + 1];
    int *maxY = new int[nconnetedcomponent + 1];
    
    // 初始化矩形边界
    for (i = 1; i <= nconnetedcomponent; i++) {
        minX[i] = w;
        minY[i] = h;
        maxX[i] = 0;
        maxY[i] = 0;
    }
    
    // 计算每个连通区域的矩形边界
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
    
    // 计算每个连通区域的大小
    for ( i = 1; i <= nconnetedcomponent; i++) {
        int width = maxX[i] - minX[i] + 1;
        int height = maxY[i] - minY[i] + 1;
        size[i] = width * width + height * height;
    }
    
    //从上述连通域定位双眼，假设连通域 i、j 符合下列几个条件，可初步认为是双眼。
    //1）两个连通域基本上在同一水平线上：|CenterY[i]- CenterY[j]|<4；
    //2) 两个连通域间距符合双眼间距：15<|CenterX[i]- CenterX[j]| <30；
    // 3）两个连通域符合眼睛大小，即不能太大：size[i]<200 和 size[j]<200
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
        // 找到符合条件的第一对连通域
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
        bret = true; // 找到双眼
        //拓展到大图片
        ptLeftEye.x *= 4;
        ptLeftEye.y *= 4;
        ptRightEye.x *= 4;
        ptRightEye.y *= 4;
    }



    // 释放内存
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

// 从源图像的(x,y)从左上角开始复制w1*h1大小的图像到目标图像中
// src,宽w,高h的YUV422彩色图像，src指针指向的内存块中存放的图像,图像大小为2*w*h,前w*h为Y分量，之后w*h/2为U分量，再之后w*h/2为V分量
// dst目标图像指针，w1,h1为目标图像的宽高
// x,y为源图像的某个左上角坐标

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
// V分量，因此U、V大小为16×24像素，共384个像素。
DLL_EXP void EyeBallCheck (BYTE* _lefteyeOpen, BYTE* _righteyeOpen, POINT* ptLeftEye_cfm, POINT* ptRightEye_cfm)
{
    int nFacePtNum=0, nEyePtNum=0;
    //1）统计左眼区域的肤色像素总数nFacePtNum。
    //2）统计左眼球像素总数nEyePtNum。
    BYTE *leftU = _lefteyeOpen+32*24;
    BYTE *leftV = _lefteyeOpen+32*24+32*24/2;
    BYTE *rightU = _righteyeOpen+32*24;
    BYTE *rightV = _righteyeOpen+32*24+32*24/2;

    //左眼
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
    // ShowDebugMessage("左眼眼球像素总数：%d\n", nEyePtNum);
    // ShowDebugMessage("左眼肤色像素总数：%d\n", nFacePtNum);

    //右眼
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


    // ShowDebugMessage("右眼眼球像素总数：%d\n", nEyePtNum);
    // ShowDebugMessage("右眼肤色像素总数：%d\n", nFacePtNum);

}

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



/*******************************************************************/
//眨眼检测与眼睛定位插件
/*******************************************************************/
DLL_EXP void ON_PLUGINRUN(int w,int h,BYTE* pYBits,BYTE* pUBits,BYTE* pVBits,BYTE* pBuffer)
{
//pYBits 大小为w*h
//pUBits 和 pVBits 的大小为 w*h/2
//pBuffer 的大小为 w*h*4
//下面算法都基于一个假设， 即w是16的倍数
    AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
    _BUF_STRUCT *pBufStruct = (BUF_STRUCT*)pBuffer; //pBuffer指向的内存块中存放了图像处理过程中需要用到的数据结构



    //初始化
    if(bret==false)
    {
        ptLeftEye.x = -1;
        ptLeftEye.y = -1;
        ptRightEye.x = -1;
        ptRightEye.y = -1;
    }

	 //请编写相应处理程序
    //(1)高斯滤波
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, 0, 0, true);
    gaosi_filter(pBufStruct->grayBmp_1d16, pBufStruct->grayBmp_1d16, w/4, h/4);
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, 0, h/4, true);

    //(2)nImageQueueIndex标记前4帧
    if(pBufStruct->nImageQueueIndex == -1)
    {   //初始化
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
    //(3)差分图像
    diff_img(pBufStruct->pImageQueue[pBufStruct->nLastImageIndex], pBufStruct->pImageQueue[pBufStruct->nImageQueueIndex], pBufStruct->TempImage1d8, w/4, h/4);
    //debug 
    // CopyToRect(pBufStruct->TempImage1d8, pYBits, w/4, h/4, w, h, 0, 0, true);
    
    
    //(4)二值化 采用直方图方法来确定阈值ret
    int *n = new int[256];
    //调参——————————————————
    int N0 = 40, N1=8; 
    //调参——————————————————
    memset(n, 0, 256 * sizeof(int)); 
    //统计每个灰度值出现的次数
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
    //阈值N1，ret<N1时，认为没有眨眼
    int blinkflag = 1;
    if(ret < N1)
    {
        blinkflag = 0;
    }
    //二值化
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
    //(5)形态学处理
    //开运算
    open_3x1(pBufStruct->TempImage1d8, pBufStruct->TempImage1d8, w/4, h/4);
    // CopyToRect(pBufStruct->TempImage1d8, pYBits, w/4, h/4, w, h, 0, h/4, true);
    //区域标记，
    //TODO:需要两个区域
	Morphological_Processing(pBufStruct->TempImage1d8, pBufStruct->TempImage1d8, w/4, h/4);
    
    // CopyToRect(pBufStruct->grayBmp_1d16, pYBits, w/4, h/4, w, h, w/4, h/4, true);
	
    //(6)初步定位双眼及显示
    //绘制十字
    if(bret)
    {
        DrawCross(pBufStruct->displayImage, w, h, ptLeftEye.x, ptLeftEye.y, 20, TYUV1(0, 191, 255), true);
        DrawCross(pBufStruct->displayImage, w, h, ptRightEye.x, ptRightEye.y, 20, TYUV1(0, 191, 255), true);
    }

    //打印debug信息
    // if(bret)
    // {
    //     ShowDebugMessage("LeftEye:(%d,%d) RightEye:(%d,%d)\n", ptLeftEye.x, ptLeftEye.y, ptRightEye.x, ptRightEye.y);
    // }
    // else
    // {
    //     ShowDebugMessage("No Eye Detected\n");
    // }
    //2.常识性验证双眼
    bool eyecheck = false; //常识性检查标识
    //计算两眼距离、眼睛宽度和高度
    int nEyeDist=-1, nEyeWidth=-1, nEyeHeight=-1;
    int NoseWidth = -1, NoseHeight = -1;
    if(bret)
    {
        nEyeDist = ptRightEye.x - ptLeftEye.x;
        nEyeWidth = nEyeDist*2/3;
        nEyeHeight = nEyeDist/2;
    }
    //检测眼睛大小是否合适
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
    //检测眼睛是否在图像中
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
    //3.复制并重采样眼睛鼻部位图片
    //  满足下面两个条件后进行复制并重采样眼睛鼻部位图片：
    // 1）常识性验证双眼通过，也就是基本上验证帧差法定位双眼的正确性。
    // 2）bLastEyeChecked为false时或眼鼻三个跟踪体有一个跟踪失败，这表示还没建立跟
    // 踪体模型或跟踪体模型已过时。
    aRect rcvLeftEye, rcvRightEye, rcvNose;
    if(eyecheck &&  (!pBufStruct->bLastEyeChecked ||    
        pBufStruct->pOtherVars->objLefteye.bBrokenTrace || 
        pBufStruct->pOtherVars->objRighteye.bBrokenTrace || 
        pBufStruct->pOtherVars->objNose.bBrokenTrace)
    )
    {
    //复制眼睛鼻部位图片
    //计算左眼、右眼及鼻子在W*H图片时区域范围，并分别存入结构体rcvLeftEye、rcvRightEye和rcvNose中
    // 计算跟踪体特征要求双眼的宽度和高度、鼻子的宽度和高度均应为4的倍数。
    NoseWidth = nEyeDist*3/4;
    NoseHeight = nEyeDist;

    //设置双眼的宽度和高度、鼻子的宽度和高度均应为4的倍数。
    nEyeWidth = (nEyeWidth/4)*4;
    nEyeHeight = (nEyeHeight/4)*4;
    NoseWidth = (NoseWidth/4)*4;
    NoseHeight = (NoseHeight/4)*4;

    //设置左眼区域
    rcvLeftEye.left = ptLeftEye.x - nEyeWidth/2;
    rcvLeftEye.top = ptLeftEye.y - nEyeHeight/2;
    rcvLeftEye.width = nEyeWidth;
    rcvLeftEye.height = nEyeHeight;
    //设置右眼区域
    rcvRightEye.left = ptRightEye.x - nEyeWidth/2;
    rcvRightEye.top = ptRightEye.y - nEyeHeight/2;
    rcvRightEye.width = nEyeWidth;
    rcvRightEye.height = nEyeHeight;
    //设置鼻子区域
    rcvNose.left = ptLeftEye.x + nEyeDist/8;
    rcvNose.top = ptLeftEye.y ;
    rcvNose.width = NoseWidth;
    rcvNose.height = NoseHeight;
    //分配内存
    BYTE *temp_lefteyeOpen = new BYTE[nEyeWidth*nEyeHeight*2];
    BYTE *temp_righteyeOpen = new BYTE[nEyeWidth*nEyeHeight*2];
    BYTE *temp_nose = new BYTE[NoseWidth*NoseHeight*2];

    BYTE *_lefteyeOpen = new BYTE[24*32*2];
    BYTE *_righteyeOpen = new BYTE[24*32*2];
    BYTE *st_nose = new BYTE[48*32*2];

    //复制图片从彩色图片colorBmp中分别复制左、右眼部位彩色图片，并重采样为32×24大小的
    //YUV422平面格式，最后存于向量_lefteyeOpen，_righteyeOpen、用于眼球色素校验。

    //测试CopyImg函数
    // BYTE *temimg=new BYTE[w*h/2];
    // CopyImg(pBufStruct->colorBmp, temimg, w, h, w/2, h/2, w/2, h/2);
    // CopyToRect(temimg, pYBits,w/2,h/2,w,h,0,0,false);
    // CopyToRect(temimg, pYBits,w/2,h/2,w,h,w/2,0,true);
//————————————————————————————————————————————————————————————————————————————————————————————————————

    //复制
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

//————————————————————————————————————————————————————————————————————————————————————————————————————
    //眼球色素校验目的是进一步确认眼睛定位的准确性，主要利用眼珠部分非肤色特点进行。
    // 输入分别为向量_lefteyeOpen、_righteyeOpen中32×24大小的YUV422平面格式的双眼图片。
    // 最后结果存储于point结构变量ptLeftEye_cfm、ptRightEye_cfm。
    POINT *ptLeftEye_cfm = new POINT;
    POINT *ptRightEye_cfm = new POINT;
    ptLeftEye_cfm->x = -1;
    ptLeftEye_cfm->y = -1;
    ptRightEye_cfm->x = -1;
    ptRightEye_cfm->y = -1;
    bool EyeballCheck_false = false;
    //眼球色素校验
    EyeBallCheck(_lefteyeOpen, _righteyeOpen, ptLeftEye_cfm, ptRightEye_cfm);
    //判断是否通过眼球色素校验
    if(ptLeftEye_cfm->x == -1 || ptLeftEye_cfm->y == -1 || ptRightEye_cfm->x == -1 || ptRightEye_cfm->y == -1)
    {
        EyeballCheck_false = false;
    }
    else
    {
        EyeballCheck_false = true;
    }
    // ShowDebugMessage("眼球色素检验:%d\n", EyeballCheck_false);

    //眼球位置校验
    // 眼球位置校验有下面几项内容：
    // 1）左、右眼中心是否全在脸部内。只要有一个在脸外，校验失败。
    // 2）眼睛水平方向校验：
    // 先在眼球中点位置水平搜索左右的脸部边界，结果存入left， right。若符合下列情况一
    // 种，校验失败。
    bool EyeballPositionCheck_false = false;
    if(EyeballCheck_false){//眼球色素校验通过
        //眼球位置校验
        EyeballPositionCheck_false = true;
        //左眼
        int fleft = pBufStruct->rcnFace.left*2;
        int ftop = pBufStruct->rcnFace.top*4;
        int fwidth = pBufStruct->rcnFace.width*2;
        int fheight = pBufStruct->rcnFace.height*4;
        int left = fleft;
        int right = fleft + fwidth;
        //检测眼球中心在脸部矩形框中
        if(ptLeftEye_cfm->x < left || ptLeftEye_cfm->x > right || 
        ptLeftEye_cfm->y < ftop || ptLeftEye_cfm->y > ftop + fheight)
        {
            EyeballPositionCheck_false = false;
        }
        //右眼
        if(ptRightEye_cfm->x < left || ptRightEye_cfm->x > right || 
        ptRightEye_cfm->y < ftop || ptRightEye_cfm->y > ftop + fheight)
        {
            EyeballPositionCheck_false = false;
        }
    }
    // ShowDebugMessage("眼球位置检验:%d\n", EyeballPositionCheck_false);

    if(EyeballPositionCheck_false && EyeballCheck_false){
        pBufStruct->pOtherVars->objLefteye.rcObject=rcvLeftEye;
        pBufStruct->pOtherVars->objLefteyeClose.rcObject=rcvLeftEye;
        pBufStruct->pOtherVars->objRighteye.rcObject=rcvRightEye;
        pBufStruct->pOtherVars->objRighteyeClose.rcObject=rcvRightEye;
        pBufStruct->pOtherVars->objNose.rcObject=rcvNose;

        //计算特征值并存储
        int Grayx=0,Grayy=0;//临时变量存放灰度重心

        //函数测试
        FeatureVector4P* Lefteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvLeftEye.left,rcvLeftEye.top,rcvLeftEye.width,rcvLeftEye.height,Lefteyefvp);

        FeatureVector4P* Righteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvRightEye.left,rcvRightEye.top,rcvRightEye.width,rcvRightEye.height,Righteyefvp);

        FeatureVector4P* Nosefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject.Vector;
        CalEigen(pBufStruct,w,h,rcvNose.left,rcvNose.top,rcvNose.width,rcvNose.height,Nosefvp);

        //特征值存入fvObject_org中
        FeatureVector4P* Lefteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvLeftEye.left,rcvLeftEye.top,rcvLeftEye.width,rcvLeftEye.height,Lefteyefvp_org);

        FeatureVector4P* Righteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvRightEye.left,rcvRightEye.top,rcvRightEye.width,rcvRightEye.height,Righteyefvp_org);

        FeatureVector4P* Nosefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objNose.fvObject_org.Vector;
        CalEigen(pBufStruct,w,h,rcvNose.left,rcvNose.top,rcvNose.width,rcvNose.height,Nosefvp_org);
        //设置bLastEyeChecked为TRUE
        pBufStruct->bLastEyeChecked = true;
        // //左眼
        // //ptLeftEye_cfm->x, ptLeftEye_cfm->y为眼球中心
        // //rcvLeftEye为眼睛区域
        // FeatureVector4P* Lefteyefvp=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject.Vector;
        // FeatureVector4P* Lefteye_pNL_LeftTop=Lefteyefvp + 1;
        // FeatureVector4P* Lefteye_pNL_pNL_RightTop=Lefteye_pNL_LeftTop+1;
        // FeatureVector4P* Lefteye_pNL_LeftBottom=Lefteye_pNL_pNL_RightTop+1;
        // FeatureVector4P* Lefteye_pNL_RightBottom=Lefteye_pNL_LeftBottom+1;


        // //计算跟踪体左上角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left, rcvLeftEye.top, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[0].x = Grayx;
        // Lefteyefvp->Vector[0].y = Grayy;
        // //计算跟踪体右上角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left+rcvLeftEye.width/2, rcvLeftEye.top, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[1].x = Grayx;
        // Lefteyefvp->Vector[1].y = Grayy;
        // //计算跟踪体左下角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left, rcvLeftEye.top+rcvLeftEye.height/2, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[2].x = Grayx;
        // Lefteyefvp->Vector[2].y = Grayy;
        // //计算跟踪体右下角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptLeftEye_cfm->x, ptLeftEye_cfm->y, rcvLeftEye.left+rcvLeftEye.width/2, rcvLeftEye.top+rcvLeftEye.height/2, rcvLeftEye.width/2, rcvLeftEye.height/2, &Grayx, &Grayy,pBufStruct);
        // Lefteyefvp->Vector[3].x = Grayx;
        // Lefteyefvp->Vector[3].y = Grayy;
        // //其它参数
        // Lefteyefvp->nLevels=2;
        // Lefteyefvp->pNL_LeftTop=Lefteye_pNL_LeftTop;
        // Lefteyefvp->pNL_RightTop=Lefteye_pNL_pNL_RightTop;
        // Lefteyefvp->pNL_LeftBottom=Lefteye_pNL_LeftBottom;
        // Lefteyefvp->pNL_RightBottom=Lefteye_pNL_RightBottom;
        // //拷贝到fvObject_org中
        // FeatureVector4P* Lefteyefvp_org=(FeatureVector4P*)pBufStruct->pOtherVars->objLefteye.fvObject_org.Vector;
        // memcpy(Lefteyefvp_org, Lefteyefvp, sizeof(FeatureVector4P));




        // //右眼
        // //ptRightEye_cfm->x, ptRightEye_cfm->y为眼球中心
        // //rcvRightEye为眼睛区域
        // FeatureVector4P* Righteyefvp = (FeatureVector4P*)pBufStruct->pOtherVars->objRighteye.fvObject.Vector;
        // FeatureVector4P* Righteye_pNL_LeftTop = Righteyefvp + 1;
        // FeatureVector4P* Righteye_pNL_pNL_RightTop = Righteye_pNL_LeftTop + 1;
        // FeatureVector4P* Righteye_pNL_LeftBottom = Righteye_pNL_pNL_RightTop + 1;
        // FeatureVector4P* Righteye_pNL_RightBottom = Righteye_pNL_LeftBottom + 1;
        // //计算跟踪体左上角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left, rcvRightEye.top, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[0].x = Grayx;
        // Righteyefvp->Vector[0].y = Grayy;
        // //计算跟踪体右上角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left + rcvRightEye.width / 2, rcvRightEye.top, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[1].x = Grayx;
        // Righteyefvp->Vector[1].y = Grayy;
        // //计算跟踪体左下角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left, rcvRightEye.top + rcvRightEye.height / 2, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[2].x = Grayx;
        // Righteyefvp->Vector[2].y = Grayy;
        // //计算跟踪体右下角区域的灰度重心
        // CalGrayGravity(pBufStruct->grayBmp, w, h, ptRightEye_cfm->x, ptRightEye_cfm->y, rcvRightEye.left + rcvRightEye.width / 2, rcvRightEye.top + rcvRightEye.height / 2, rcvRightEye.width / 2, rcvRightEye.height / 2, &Grayx, &Grayy, pBufStruct);
        // Righteyefvp->Vector[3].x = Grayx;
        // Righteyefvp->Vector[3].y = Grayy;
        // //其它参数
        // Righteyefvp->pNL_LeftTop = Righteye_pNL_LeftTop;
        // Righteyefvp->pNL_RightTop = Righteye_pNL_pNL_RightTop;
        // Righteyefvp->pNL_LeftBottom = Righteye_pNL_LeftBottom;
        // Righteyefvp->pNL_RightBottom = Righteye_pNL_RightBottom;
        // //拷贝到fvObject_org中
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
   AFX_MANAGE_STATE(AfxGetStaticModuleState());//模块状态切换
	//theApp.dlg.DestroyWindow();
}

