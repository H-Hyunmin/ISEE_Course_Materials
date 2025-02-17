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
	//������ɫ����ʸ��ֱ��ͼ��ȫ�ֱ�������	--1024�ֽ�
	aBYTE byHistMap_U[256],byHistMap_V[256];
	aBYTE ClrLocBuf[256*2];
	//�����������õ���ȫ�ֱ�������			--16192�ֽ�
	aBYTE	FeaProcBuf[256*4*16];
	//���ٴ������õ���ȫ�ֱ�������			--1024�ֽ�
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
	aBYTE* pOtherData;									//��ʣ��ռ�ָ��
	OTHER_VARS* pOtherVars;							//������Ҫ�ı������д��������ṹ����
//	aBYTE *pImageBkgnd1d16; 							//����ͼ��
//	aBYTE *pImageBkgnd_detail1d16; 					//����ͼ��ϸ�ڲ���
//	aBYTE *pImageBkgnd_stable1d16;					//�����ŵ�ǰͼ����µı���ͼ��
//	aBYTE *pImageFrgnd1d16; 							//ǰ��ͼ��
	int nImageQueueIndex,nLastImageIndex;			//գ�ۼ����ʷͼ�����������ͼ�¼
	bool bLastEyeChecked;								//�ϴδ����Ѿ���⵽�۾��ı��
	aRect rcnFace;										//����ɫ�ȷָ�������
	int nFacePixelNum;									//�����������
	aPOINT ptTheNose,ptTheLeftEye,ptTheRightEye;	//��������λ��
	int nFVTop;											//
	bool EyeBallConfirm;								//����ɫ�ȼ������
	bool EyePosConfirm;								//����λ�ü������
	bool bNotInited;									//��ʼ�����
//	bool bBkgndInited;									//����������־
//	int  nBkgndKeepFrames;							//���������е�֡����
//	int  nBkgndLostFrames;							//��������ʧ�ܴ����������ж��Ƿ���Ҫ�ؽ�����
//	int  nUnvisibleRgnAdjust;							//��������ʱ�ԷǱ��������Ԥ�����ֵ

	int cur_allocSize,max_allocSize,allocTimes;		//�ڴ�������
	int cur_maxallocsize;								//��¼�������������ڴ���
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
