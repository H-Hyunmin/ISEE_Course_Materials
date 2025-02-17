// ImageProc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ImageProc.h"
#include "bufstruct.h"
#define DLL_EXP extern "C" _declspec(dllexport)
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
// CImageProcApp

BEGIN_MESSAGE_MAP(CImageProcApp, CWinApp)
	//{{AFX_MSG_MAP(CImageProcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcApp construction


CImageProcApp::CImageProcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageProcApp object

CImageProcApp theApp;


//定义一个结构，用于双线性内插法重采样
struct LookupCell{
    UINT x;
    UINT i;
    UINT k;
    UINT ksx;
    UINT xsi;
};
//定义一个全局变量
BUF_STRUCT * pBS = NULL;
//
void (*AddMessageToList)(char * message)=NULL;

DLL_EXP void InitMessageOpFunction(void (*AMTL)(char *))
{
	AddMessageToList = AMTL;
}

DLL_EXP void ShowDebugMessage(char* format,...)
{
	va_list ap;
	char sBuffer[1024];
	va_start(ap, format);
	vsprintf(sBuffer,format,ap);
	va_end(ap);
	if( AddMessageToList )
		(*AddMessageToList)(sBuffer);
}
/*******************************************************************/
//初始化全局变量
/*******************************************************************/
DLL_EXP void myHeapAllocInit(BUF_STRUCT* pBufStruct)
{
	ASSERT(pBufStruct);
	pBS = pBufStruct;
	pBS->cur_allocSize = 0;
	pBS->allocTimes = 0;
	pBS->cur_maxallocsize = 0;
}
/*******************************************************************/
//在动态内存中创建大小 为size字节的向量
/*******************************************************************/
DLL_EXP aBYTE* myHeapAlloc(int size)
{
	ASSERT(pBS->cur_allocSize+size<pBS->max_allocSize);
	ASSERT(size>0);
	if( (size%4)!=0 )	size = (size+3)/4*4;
	*((int*)(pBS->pOtherData+pBS->cur_allocSize)) = size;
	pBS->cur_allocSize+=size+sizeof(int);
	pBS->allocTimes++;
	if( pBS->cur_allocSize>pBS->cur_maxallocsize )
		{
		pBS->cur_maxallocsize = pBS->cur_allocSize;
		ShowDebugMessage("最大动态运存需求达到%d字节",pBS->cur_maxallocsize);
		}
	return pBS->pOtherData+pBS->cur_allocSize-size;
}
/*******************************************************************/
//释放动态内存中的向量
/*******************************************************************/
DLL_EXP void myHeapFree(aBYTE* ptr)
{
	ASSERT(pBS->allocTimes>0);
	ASSERT(((pBS->pOtherData+pBS->cur_allocSize)-ptr)==*((int*)(ptr-sizeof(int))) );
	pBS->cur_allocSize -= *((int*)(ptr-sizeof(int)))+sizeof(int);
	pBS->allocTimes--;
}

/*******************************************************************/
//画点
/*******************************************************************/
DLL_EXP void PutPixel(
	aBYTE* ThisImage,int W,int H,//显示图片指针及大小
	int x,int y,//点的坐标
	COLORREF Color,//颜色，YUV格式
	bool bGray)    //灰度标记，TRUE-灰度，FALSE-彩色
{
    ASSERT(ThisImage);
    int R,G,B;
    aBYTE  * lpBits = ThisImage;
    if(x<0 || x>=W) return;
    if(y<0 || y>=H) return;

    R = GetRValue(Color);//取低字节，Y或R
    G = GetGValue(Color);//取中间字节,U或G
    B = GetBValue(Color);//取高字节,V或B

    if( bGray )
		lpBits[W*y+x] = (aBYTE)G;
	else
		{
        lpBits[W*y+x] = R;//Y
        lpBits[W*H+W*y/2+x/2] = G;//U
        lpBits[W*H+W*H/2+W*y/2+x/2] = B;//V
		}
}
/*******************************************************************/
//画水平直线
/*******************************************************************/
DLL_EXP void DrawHLine(
		aBYTE* ThisImage,int W,int H,//显示图片指针及大小
		int nvSX,int nvEX,           //直线水平起点、终点
		int nvY,                     //直线垂直位置
		COLORREF Color,//边框颜色，YUV格式
		bool bGray//灰度标记，TRUE-灰度，FALSE-彩色
		)
{
    int i;
	ASSERT(ThisImage);
    if( nvEX > nvSX )
        for(i=nvSX;i<=nvEX;i++)
			//在坐标(i,nvY)处画点
             PutPixel(ThisImage,W,H, i,nvY,Color,bGray);
    else
        for(i=nvEX;i<=nvSX;i++)
            //在坐标(i,nvY)处画点
             PutPixel(ThisImage,W,H, i,nvY,Color,bGray);
}
/*******************************************************************/
//画垂直直线
/*******************************************************************/
DLL_EXP void DrawVLine(
			aBYTE* ThisImage,int W,int H,//显示图片指针及大小
			int nvX,    //直线水平位置
			int nvSY,int nvEY,//直线垂直起点、终点
			COLORREF Color,
			bool bGray)//灰度标记，TRUE-灰度，FALSE-彩色
{
    int i;
	ASSERT(ThisImage);
    if( nvEY > nvSY )
         for(i=nvSY;i<=nvEY;i++)
         //在坐标(nvX,i)处画点
          PutPixel(ThisImage,W,H, nvX,i,Color,bGray);
    else
         for(i=nvEY;i<=nvSY;i++)
        //在坐标(nvX,i)处画点
         PutPixel(ThisImage,W,H, nvX,i,Color,bGray);
}
/*******************************************************************/
//画矩形边框
/*******************************************************************/
DLL_EXP void DrawRectangle(
	aBYTE* ThisImage,int W,int H,//显示图片指针及大小
	aRect rc,//矩形区域
	COLORREF Color,//边框颜色，YUV格式
	bool bGray//灰度标记，TRUE-灰度，FALSE-彩色
	)
{
	ASSERT(ThisImage);
   
	DrawHLine(ThisImage,W,H,rc.left,RECT_RIGHT(rc),rc.top,Color,bGray); //上边
	DrawVLine(ThisImage,W,H,rc.left,rc.top,RECT_BOTTOM(rc),Color,bGray); //左边
	DrawHLine(ThisImage,W,H,rc.left,RECT_RIGHT(rc),RECT_BOTTOM(rc),Color,bGray); //下边
	DrawVLine(ThisImage,W,H,RECT_RIGHT(rc),rc.top,RECT_BOTTOM(rc),Color,bGray); //右边
}
/*******************************************************************/
//画"十字"
/*******************************************************************/
DLL_EXP void DrawCross(
	aBYTE* ThisImage,int W,int H,//显示图片指针及大小
	int nvX,int nvY,//“十字”中心点坐标
	int nSize,      //“十字”大小
	COLORREF Color, //边框颜色，YUV格式
	bool bGray)
{    
	ASSERT(ThisImage);
	DrawHLine(ThisImage,W,H,nvX-nSize,nvX+nSize,nvY,Color,bGray);//水平直线
	DrawVLine(ThisImage,W,H,nvX,nvY-nSize,nvY+nSize,Color,bGray);//垂直直线
}

/*******************************************************************/
//将图片复制到目标图片中的某一区域
/*******************************************************************/




/**
 * @brief 
 * 
 * @param ThisImage 
 * @param anImage 
 * @param W 
 * @param H 
 * @param DestW 
 * @param DestH 
 * @param nvLeft 
 * @param nvTop 
 * @param bGray 
 * @return DLL_EXP 
 */
DLL_EXP void CopyToRect(
	         aBYTE* ThisImage,//区域图片指针
			 aBYTE* anImage,//目标图片指针
			 int W,int H,//区域图片大小
		     int DestW,int DestH,//目标图片大小
			 int nvLeft,int nvTop,//区域位置：图片左上角在目标图片的坐标
			 bool bGray//灰度图片标记，true：灰度；false：彩色。
					  )
{
    aBYTE  * lpSrc = ThisImage;
    aBYTE  * lpDes = anImage;
    aBYTE  * lps, * lpd;
    int h;
	ASSERT(ThisImage && anImage);
    ASSERT( nvLeft+W<=DestW && nvTop+H<=DestH && nvLeft>=0 && nvTop>=0 );
	//Y
    for(h=0;h<H;h++){
        lpd = lpDes+(nvTop+h)*DestW+nvLeft;
        lps = lpSrc+(DWORD)h*W;
		memcpy(lpd,lps,W);
    }
	if( bGray )	return;
	//U
	lpSrc += W*H;
	lpDes += DestW*DestH;
    for(h=0;h<H;h++){
        lpd = lpDes+(nvTop+h)*(DestW/2)+nvLeft/2;
        lps = lpSrc+(DWORD)h*W/2;
		memcpy(lpd,lps,W/2);
    }
	//V
	lpSrc += W*H/2;
	lpDes += DestW*DestH/2;
    for(h=0;h<H;h++){
        lpd = lpDes+(nvTop+h)*(DestW/2)+nvLeft/2;
        lps = lpSrc+(DWORD)h*W/2;
		memcpy(lpd,lps,W/2);
    }
}

/*******************************************************************/
//灰度图像双线性内插法重采样
/*******************************************************************/
bool GrayLinearIns(
				   aBYTE* ThisImage,int Width,int Height,//源图片指针及大小
				   int ResWidth,int ResHeight,//目标图片大小
				   aBYTE* result)//目标图片指针
{
    UINT i,j,u,v,k,l,gray=0;
    UINT x=0,y=0,tempx=0,tempy=0;
    UINT ksx,xsi,lsy,ysj;
    aBYTE* pOringinImage;
    aBYTE* pScaleImage;
    struct LookupCell *RowLookup,*ColLookup;
	ASSERT(result);
    pOringinImage=ThisImage;
    pScaleImage=result;
    RowLookup = (LookupCell*)myHeapAlloc(ResHeight*sizeof(LookupCell));
    ColLookup = (LookupCell*)myHeapAlloc(ResWidth*sizeof(LookupCell));
    for(v=0;v<ResHeight;v++)
	{   RowLookup[v].x = v*100*(Height-1)/(ResHeight-1);
        RowLookup[v].i = RowLookup[v].x/100;//左边点
        if( RowLookup[v].i>=Height-1 )	RowLookup[v].i = Height-2;
        RowLookup[v].k = RowLookup[v].i+1;//右边点
        //跟右边点距离（比例），相邻两像素距离为100
		RowLookup[v].ksx = RowLookup[v].k*100-RowLookup[v].x;
        //跟左边点距离，
        RowLookup[v].xsi = RowLookup[v].x-RowLookup[v].i*100;
    }
    for(u=0;u<ResWidth;u++)
	{   ColLookup[u].x = u*100*(Width-1)/(ResWidth-1);
        ColLookup[u].i = ColLookup[u].x/100;//上边点
        if( ColLookup[u].i>=Width-1 )	ColLookup[u].i = Width-2;
        ColLookup[u].k = ColLookup[u].i+1;//下边点
        ColLookup[u].ksx = ColLookup[u].k*100-ColLookup[u].x;//跟下边点距离
        ColLookup[u].xsi = ColLookup[u].x-ColLookup[u].i*100;//跟上边点距离
    }
    //双线性内插法
    for(v=0;v<ResHeight;v++){
        for(u=0;u<ResWidth;u++){
            i = ColLookup[u].i;
            j = RowLookup[v].i;
            k = ColLookup[u].k;
            l = RowLookup[v].k;
            ksx = ColLookup[u].ksx;
            lsy = RowLookup[v].ksx;
            xsi = ColLookup[u].xsi;
            ysj = RowLookup[v].xsi;
         
            gray =  pOringinImage[l*Width+i]*ksx*ysj
                    +pOringinImage[l*Width+k]*xsi*ysj
                    +pOringinImage[j*Width+k]*xsi*lsy
                    +pOringinImage[j*Width+i]*ksx*lsy;
            pScaleImage[v*ResWidth+u] = (aBYTE)(gray/10000);
        }
    }
	myHeapFree((aBYTE*)ColLookup);
	myHeapFree((aBYTE*)RowLookup);
    return true;
}
/*******************************************************************/
//彩色图像双线性内插法重采样
/*******************************************************************/
bool ColorLinearIns(
					aBYTE* ThisImage,int Width,int Height,//源图片指针及大小
					int ResWidth,int ResHeight,//目标图片大小
					aBYTE* result//目标图片指针
					)
{
	ASSERT(result);
	GrayLinearIns(ThisImage,Width,Height,ResWidth,ResHeight,result);
	GrayLinearIns(ThisImage+Width*Height,Width/2,Height,ResWidth/2,ResHeight,result+ResWidth*ResHeight);
	GrayLinearIns(ThisImage+Width*Height+Width*Height/2,Width/2,Height,
		          ResWidth/2,ResHeight,result+ResWidth*ResHeight+ResWidth*ResHeight/2);
	return true;
}
/*******************************************************************/
//重采样
/*******************************************************************/

/**
 * @brief 
 * 
 * @param ThisImage 
 * @param Width 
 * @param Height 
 * @param newWidth 
 * @param newHeight 
 * @param InsMode 
 * @param bGray 
 * @param result 
 * @return DLL_EXP 
 */
DLL_EXP bool ReSample(aBYTE* ThisImage,int Width,int Height,//源图片指针及大小
					  int newWidth,int newHeight,//目标图片大小
					  bool InsMode,//true：双线性内插法，false：邻近点采样法
					  bool bGray,//true：灰度图片采样，false：彩色图片采样
					  aBYTE *result//目标图片指针
					  )
{
    int *HSampleTable;
    int *VSampleTable;
    int i,j;
    float rsWRatio = float(Width)/float(newWidth);
    float rsHRatio = float(Height)/float(newHeight);
    aBYTE *lpDes;
    aBYTE *lpSrc;
    aBYTE *lpLineSrc,*lpLineDes;
    if( (Width==newWidth) && (Height==newHeight) )
        return true;
	ASSERT(result);
    if( !InsMode )
	{   //邻近点采样法
        HSampleTable = (int*)myHeapAlloc(newWidth*sizeof(int));
       	VSampleTable = (int*)myHeapAlloc(newHeight*sizeof(int));
        //邻近点X坐标向量
		for(i=0;i<newWidth;i++)
		{ HSampleTable[i] = int(float(i)*rsWRatio);
          if( HSampleTable[i]>Width-1 ) HSampleTable[i] = Width-1;
		}
        //邻近点Y坐标向量
        for(i=0;i<newHeight;i++)
		{   VSampleTable[i] = int(float(i)*rsHRatio);
            if( VSampleTable[i]>Height-1 ) VSampleTable[i] = Height-1;
        }
		//Y分量
        lpDes = result;
        lpSrc = ThisImage;
        for(j=0;j<newHeight;j++)
		{   lpLineSrc = lpSrc+VSampleTable[j]*Width;
            lpLineDes = lpDes+j*newWidth;
            for(i=0;i<newWidth;i++) lpLineDes[i] = lpLineSrc[HSampleTable[i]];
        }
		if( bGray )
		{ myHeapFree((aBYTE*)VSampleTable);
	      myHeapFree((aBYTE*)HSampleTable);
		  return true;
		}
		//U
        lpDes += newWidth*newHeight;
        lpSrc += Width*Height;
        for(j=0;j<newHeight;j++)
		{   lpLineSrc = lpSrc+VSampleTable[j]*Width/2;
            lpLineDes = lpDes+j*newWidth/2;
            for(i=0;i<newWidth/2;i++) lpLineDes[i] = lpLineSrc[HSampleTable[i*2]/2];
        }
		//V分量
        lpDes += newWidth*newHeight/2;
        lpSrc += Width*Height/2;
        for(j=0;j<newHeight;j++)
		{   lpLineSrc = lpSrc+VSampleTable[j]*Width/2;
            lpLineDes = lpDes+j*newWidth/2;
            for(i=0;i<newWidth/2;i++) lpLineDes[i] = lpLineSrc[HSampleTable[i*2]/2];
        }
        myHeapFree((aBYTE*)VSampleTable);
        myHeapFree((aBYTE*)HSampleTable);
    }
    else
	{   //双线性内插法
        if( bGray )	GrayLinearIns(ThisImage,Width,Height,newWidth,newHeight,result);
		else		ColorLinearIns(ThisImage,Width,Height,newWidth,newHeight,result);
    }
    return true;
}


