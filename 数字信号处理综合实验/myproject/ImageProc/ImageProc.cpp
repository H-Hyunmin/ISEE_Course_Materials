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


//����һ���ṹ������˫�����ڲ巨�ز���
struct LookupCell{
    UINT x;
    UINT i;
    UINT k;
    UINT ksx;
    UINT xsi;
};
//����һ��ȫ�ֱ���
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
//��ʼ��ȫ�ֱ���
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
//�ڶ�̬�ڴ��д�����С Ϊsize�ֽڵ�����
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
		ShowDebugMessage("���̬�˴�����ﵽ%d�ֽ�",pBS->cur_maxallocsize);
		}
	return pBS->pOtherData+pBS->cur_allocSize-size;
}
/*******************************************************************/
//�ͷŶ�̬�ڴ��е�����
/*******************************************************************/
DLL_EXP void myHeapFree(aBYTE* ptr)
{
	ASSERT(pBS->allocTimes>0);
	ASSERT(((pBS->pOtherData+pBS->cur_allocSize)-ptr)==*((int*)(ptr-sizeof(int))) );
	pBS->cur_allocSize -= *((int*)(ptr-sizeof(int)))+sizeof(int);
	pBS->allocTimes--;
}

/*******************************************************************/
//����
/*******************************************************************/
DLL_EXP void PutPixel(
	aBYTE* ThisImage,int W,int H,//��ʾͼƬָ�뼰��С
	int x,int y,//�������
	COLORREF Color,//��ɫ��YUV��ʽ
	bool bGray)    //�Ҷȱ�ǣ�TRUE-�Ҷȣ�FALSE-��ɫ
{
    ASSERT(ThisImage);
    int R,G,B;
    aBYTE  * lpBits = ThisImage;
    if(x<0 || x>=W) return;
    if(y<0 || y>=H) return;

    R = GetRValue(Color);//ȡ���ֽڣ�Y��R
    G = GetGValue(Color);//ȡ�м��ֽ�,U��G
    B = GetBValue(Color);//ȡ���ֽ�,V��B

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
//��ˮƽֱ��
/*******************************************************************/
DLL_EXP void DrawHLine(
		aBYTE* ThisImage,int W,int H,//��ʾͼƬָ�뼰��С
		int nvSX,int nvEX,           //ֱ��ˮƽ��㡢�յ�
		int nvY,                     //ֱ�ߴ�ֱλ��
		COLORREF Color,//�߿���ɫ��YUV��ʽ
		bool bGray//�Ҷȱ�ǣ�TRUE-�Ҷȣ�FALSE-��ɫ
		)
{
    int i;
	ASSERT(ThisImage);
    if( nvEX > nvSX )
        for(i=nvSX;i<=nvEX;i++)
			//������(i,nvY)������
             PutPixel(ThisImage,W,H, i,nvY,Color,bGray);
    else
        for(i=nvEX;i<=nvSX;i++)
            //������(i,nvY)������
             PutPixel(ThisImage,W,H, i,nvY,Color,bGray);
}
/*******************************************************************/
//����ֱֱ��
/*******************************************************************/
DLL_EXP void DrawVLine(
			aBYTE* ThisImage,int W,int H,//��ʾͼƬָ�뼰��С
			int nvX,    //ֱ��ˮƽλ��
			int nvSY,int nvEY,//ֱ�ߴ�ֱ��㡢�յ�
			COLORREF Color,
			bool bGray)//�Ҷȱ�ǣ�TRUE-�Ҷȣ�FALSE-��ɫ
{
    int i;
	ASSERT(ThisImage);
    if( nvEY > nvSY )
         for(i=nvSY;i<=nvEY;i++)
         //������(nvX,i)������
          PutPixel(ThisImage,W,H, nvX,i,Color,bGray);
    else
         for(i=nvEY;i<=nvSY;i++)
        //������(nvX,i)������
         PutPixel(ThisImage,W,H, nvX,i,Color,bGray);
}
/*******************************************************************/
//�����α߿�
/*******************************************************************/
DLL_EXP void DrawRectangle(
	aBYTE* ThisImage,int W,int H,//��ʾͼƬָ�뼰��С
	aRect rc,//��������
	COLORREF Color,//�߿���ɫ��YUV��ʽ
	bool bGray//�Ҷȱ�ǣ�TRUE-�Ҷȣ�FALSE-��ɫ
	)
{
	ASSERT(ThisImage);
   
	DrawHLine(ThisImage,W,H,rc.left,RECT_RIGHT(rc),rc.top,Color,bGray); //�ϱ�
	DrawVLine(ThisImage,W,H,rc.left,rc.top,RECT_BOTTOM(rc),Color,bGray); //���
	DrawHLine(ThisImage,W,H,rc.left,RECT_RIGHT(rc),RECT_BOTTOM(rc),Color,bGray); //�±�
	DrawVLine(ThisImage,W,H,RECT_RIGHT(rc),rc.top,RECT_BOTTOM(rc),Color,bGray); //�ұ�
}
/*******************************************************************/
//��"ʮ��"
/*******************************************************************/
DLL_EXP void DrawCross(
	aBYTE* ThisImage,int W,int H,//��ʾͼƬָ�뼰��С
	int nvX,int nvY,//��ʮ�֡����ĵ�����
	int nSize,      //��ʮ�֡���С
	COLORREF Color, //�߿���ɫ��YUV��ʽ
	bool bGray)
{    
	ASSERT(ThisImage);
	DrawHLine(ThisImage,W,H,nvX-nSize,nvX+nSize,nvY,Color,bGray);//ˮƽֱ��
	DrawVLine(ThisImage,W,H,nvX,nvY-nSize,nvY+nSize,Color,bGray);//��ֱֱ��
}

/*******************************************************************/
//��ͼƬ���Ƶ�Ŀ��ͼƬ�е�ĳһ����
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
	         aBYTE* ThisImage,//����ͼƬָ��
			 aBYTE* anImage,//Ŀ��ͼƬָ��
			 int W,int H,//����ͼƬ��С
		     int DestW,int DestH,//Ŀ��ͼƬ��С
			 int nvLeft,int nvTop,//����λ�ã�ͼƬ���Ͻ���Ŀ��ͼƬ������
			 bool bGray//�Ҷ�ͼƬ��ǣ�true���Ҷȣ�false����ɫ��
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
//�Ҷ�ͼ��˫�����ڲ巨�ز���
/*******************************************************************/
bool GrayLinearIns(
				   aBYTE* ThisImage,int Width,int Height,//ԴͼƬָ�뼰��С
				   int ResWidth,int ResHeight,//Ŀ��ͼƬ��С
				   aBYTE* result)//Ŀ��ͼƬָ��
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
        RowLookup[v].i = RowLookup[v].x/100;//��ߵ�
        if( RowLookup[v].i>=Height-1 )	RowLookup[v].i = Height-2;
        RowLookup[v].k = RowLookup[v].i+1;//�ұߵ�
        //���ұߵ���루�����������������ؾ���Ϊ100
		RowLookup[v].ksx = RowLookup[v].k*100-RowLookup[v].x;
        //����ߵ���룬
        RowLookup[v].xsi = RowLookup[v].x-RowLookup[v].i*100;
    }
    for(u=0;u<ResWidth;u++)
	{   ColLookup[u].x = u*100*(Width-1)/(ResWidth-1);
        ColLookup[u].i = ColLookup[u].x/100;//�ϱߵ�
        if( ColLookup[u].i>=Width-1 )	ColLookup[u].i = Width-2;
        ColLookup[u].k = ColLookup[u].i+1;//�±ߵ�
        ColLookup[u].ksx = ColLookup[u].k*100-ColLookup[u].x;//���±ߵ����
        ColLookup[u].xsi = ColLookup[u].x-ColLookup[u].i*100;//���ϱߵ����
    }
    //˫�����ڲ巨
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
//��ɫͼ��˫�����ڲ巨�ز���
/*******************************************************************/
bool ColorLinearIns(
					aBYTE* ThisImage,int Width,int Height,//ԴͼƬָ�뼰��С
					int ResWidth,int ResHeight,//Ŀ��ͼƬ��С
					aBYTE* result//Ŀ��ͼƬָ��
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
//�ز���
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
DLL_EXP bool ReSample(aBYTE* ThisImage,int Width,int Height,//ԴͼƬָ�뼰��С
					  int newWidth,int newHeight,//Ŀ��ͼƬ��С
					  bool InsMode,//true��˫�����ڲ巨��false���ڽ��������
					  bool bGray,//true���Ҷ�ͼƬ������false����ɫͼƬ����
					  aBYTE *result//Ŀ��ͼƬָ��
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
	{   //�ڽ��������
        HSampleTable = (int*)myHeapAlloc(newWidth*sizeof(int));
       	VSampleTable = (int*)myHeapAlloc(newHeight*sizeof(int));
        //�ڽ���X��������
		for(i=0;i<newWidth;i++)
		{ HSampleTable[i] = int(float(i)*rsWRatio);
          if( HSampleTable[i]>Width-1 ) HSampleTable[i] = Width-1;
		}
        //�ڽ���Y��������
        for(i=0;i<newHeight;i++)
		{   VSampleTable[i] = int(float(i)*rsHRatio);
            if( VSampleTable[i]>Height-1 ) VSampleTable[i] = Height-1;
        }
		//Y����
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
		//V����
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
	{   //˫�����ڲ巨
        if( bGray )	GrayLinearIns(ThisImage,Width,Height,newWidth,newHeight,result);
		else		ColorLinearIns(ThisImage,Width,Height,newWidth,newHeight,result);
    }
    return true;
}


