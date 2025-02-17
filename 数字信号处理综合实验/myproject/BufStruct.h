#ifndef __ANTSLP_BUFSTRUCT_H__
#define __ANTSLP_BUFSTRUCT_H__

#define DLL_EXP extern "C" _declspec(dllexport) 
#define DLL_INP extern "C" _declspec(dllimport) 
#define DLL_IMP _declspec(dllimport) 

#define MAX_FEA_SIZE 512

#define YUV2 844715353
#define VUY2 353517448
#define YUY2 842094158
#define NV12 842094159
#define RGB24 0


#define OFBYTE(R) (((R)>255)?255:(((R)<0)?0:(R)))
#define RECT_RIGHT(a) ((a).left+(a).width-1)
#define RECT_BOTTOM(a) ((a).top+(a).height-1)

typedef unsigned char aBYTE;
typedef struct _arect{
	int left;
	int top;
	int width;
	int height;
} aRect;
typedef struct _apoint {
	int x;
	int y;
} aPOINT;
typedef struct _feavect {
	int size; 
	aBYTE Vector[MAX_FEA_SIZE];
} FeatureVector;

struct LINECELL{
    LONG x;
    LONG y;
    LONG g;
};

struct TRACE_OBJECT
{
    //aBYTE object_bmp[MAX_EYE_SIZE*MAX_EYE_SIZE*2];
    //aBYTE object_orgbmp[MAX_EYE_SIZE*MAX_EYE_SIZE*2];
    aRect rcObject;
    FeatureVector fvObject;
    FeatureVector fvObject_org;
    int spdxObj;
    int spdyObj;
    int nMinDist;
    bool bBrokenTrace;
    bool bSaveit;
    int nBrokenTimes;
	char sName[8];
};

struct OTHER_VARS
{
	TRACE_OBJECT 	objNose,
					objLefteye,
					objRighteye,
					objLefteyeClose,
					objRighteyeClose;
	//脸部颜色过滤矢量直方图及全局变量区域	--1024字节
	aBYTE byHistMap_U[256],byHistMap_V[256];
	aBYTE ClrLocBuf[256*2];
	//特征处理中用到的全局变量区域			--16192字节
	aBYTE	FeaProcBuf[256*4*16];
	//跟踪处理中用到的全局变量区域			--1024字节
//	
	aBYTE   TraceProcBuf[256*4-sizeof(bool)];
};

struct _BUF_STRUCT
{
	int W,H;
	aBYTE* displayImage;
	aBYTE *colorBmp; 
	aBYTE *grayBmp;
	aBYTE *LastGrayImage;
	aBYTE *clrBmp_1d8;
	aBYTE *grayBmp_1d16;
	aBYTE *TempImage1d8;
	aBYTE *lastImageQueue1d16m8;
	aBYTE *pImageQueue[8];
	aBYTE* pOtherData;									//后方剩余空间指针
	OTHER_VARS* pOtherVars;							//其他必要的变量集中存放在这个结构体中
//	aBYTE *pImageBkgnd1d16; 							//背景图像
//	aBYTE *pImageBkgnd_detail1d16; 					//背景图像细节部分
//	aBYTE *pImageBkgnd_stable1d16;					//不随着当前图像更新的背景图像
//	aBYTE *pImageFrgnd1d16; 							//前景图像
	int nImageQueueIndex,nLastImageIndex;			//眨眼检测历史图像序列索引和记录
	bool bLastEyeChecked;								//上次处理已经检测到眼睛的标记
	aRect rcnFace;										//脸部色度分割结果区域
	int nFacePixelNum;									//脸部区域点数
	aPOINT ptTheNose,ptTheLeftEye,ptTheRightEye;	//脸部器官位置
	int nFVTop;											//
	bool EyeBallConfirm;								//眼珠色度检验控制
	bool EyePosConfirm;								//眼珠位置检验控制
	bool bNotInited;									//初始化标记
//	bool bBkgndInited;									//背景建立标志
//	int  nBkgndKeepFrames;							//背景建立中的帧计数
//	int  nBkgndLostFrames;							//背景更新失败次数，用于判断是否需要重建背景
//	int  nUnvisibleRgnAdjust;							//背景更新时对非背景区域的预测调整值

	int cur_allocSize,max_allocSize,allocTimes;		//内存分配参数
	int cur_maxallocsize;								//记录运行中最大分配内存数
	aBYTE byReserved[256-17*sizeof(aBYTE*)-4*sizeof(bool)-10*sizeof(int)-sizeof(aRect)-3*sizeof(aPOINT)-sizeof(OTHER_VARS*)];
	//                     68                16             40            16            24               4
};
struct FeatureVector4P{
	int nLevels;
	aPOINT Vector[4];
	aPOINT faceColor_WeightCenter;
	FeatureVector4P* pNL_LeftTop;
	FeatureVector4P* pNL_RightTop;
	FeatureVector4P* pNL_LeftBottom;
	FeatureVector4P* pNL_RightBottom;
};
typedef struct _BUF_STRUCT BUF_STRUCT;

#endif
