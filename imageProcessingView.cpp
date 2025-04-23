// imageProcessingView.cpp: CimageProcessingView 类的实现
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "imageProcessing.h"
#include "imageProcessingDoc.h"
#include "imageProcessingView.h"
#include "_GlobalCommon.h"
#include "PixelCoordDialog.h"
#include "InterpolationDialog.h"
#include "ImageFilter.h"
#include "FilterParamDialog.h"
#include "ImageEnhance.h"
#include "EnhanceParamDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CimageProcessingView, CView)

BEGIN_MESSAGE_MAP(CimageProcessingView, CView)
	ON_COMMAND(ID_IMAGEPROCESS_OPENBMPFILE, &CimageProcessingView::OnImageprocessOpenbmpfile)
	ON_COMMAND(ID_IMAGEPROCESS_SAVETOFILE, &CimageProcessingView::OnImageprocessSavetofile)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYFILEHEADER, &CimageProcessingView::OnImageprocessDisplayfileheader)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYPALETTE, &CimageProcessingView::OnImageprocessDisplaypalette)
	ON_COMMAND(ID_IMAGEPROCESS_GETPIXELVALUE, &CimageProcessingView::OnImageprocessGetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_SETPIXELVALUE, &CimageProcessingView::OnImageprocessSetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_INERPOLATION, &CimageProcessingView::OnImageprocessInerpolation)
	ON_COMMAND(ID_IMAGEPROCESS_GAUSSSMOOTH, &CimageProcessingView::OnImageprocessGausssmooth)
	ON_COMMAND(ID_IMAGEPROCESS_MEDIANFILTER, &CimageProcessingView::OnImageprocessMedianfilter)
	ON_COMMAND(ID_IMAGEPROCESS_BILATERALFILTER, &CimageProcessingView::OnImageprocessBilateralfilter)
	ON_COMMAND(ID_IMAGEPROCESS_HISTOEQUALIZATION, &CimageProcessingView::OnImageprocessHistoequalization)
	ON_COMMAND(ID_IMAGEPROCESS_SHARPENGRAD, &CimageProcessingView::OnImageprocessSharpengrad)
	ON_COMMAND(ID_IMAGEPROCESS_CANNYEDGE, &CimageProcessingView::OnImageprocessCannyedge)
	ON_COMMAND(ID_IMAGEPROCESS_OTSUSEGMENT, &CimageProcessingView::OnImageprocessOtsusegment)
END_MESSAGE_MAP()

CimageProcessingView::CimageProcessingView() noexcept
{
	pFileBuf = NULL;
}

CimageProcessingView::~CimageProcessingView()
{
	if( pFileBuf ) 
	{
		delete [] pFileBuf;
		pFileBuf = 0;
	}
}

BOOL CimageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CimageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CimageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimageProcessingDoc *CimageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageProcessingDoc)));
	return (CimageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// CimageProcessingView 绘图
void CimageProcessingView::OnDraw(CDC *pDC)
{
	CimageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if( pFileBuf != NULL )
	{
		DisplayImage(pDC,pFileBuf,10,10,0,0,0);
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Here are the functions to be programmed by you!

//Open a BMP file
void CimageProcessingView::OnImageprocessOpenbmpfile()
{
	LPCTSTR lpszFilter = "BMP Files (*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_NOCHANGEDIR,lpszFilter,NULL);
	if( dlg.DoModal() != IDOK ) return;
	if( pFileBuf != NULL )
	{
		delete [] pFileBuf;
	}
	pFileBuf = OpenBMPfile( dlg.GetPathName() );
	Invalidate();
	UpdateWindow();
}

//Save to a new BMP file
void CimageProcessingView::OnImageprocessSavetofile()
{
	if (pFileBuf == NULL)
		return;

	LPCTSTR lpszFilter = _T("BMP Files (*.bmp)|*.bmp||");
	CFileDialog dlg(FALSE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, this);

	if (dlg.DoModal() != IDOK)
		return;

	CString strBmpFile = dlg.GetPathName();
	if (!SaveDIB(pFileBuf, strBmpFile))
	{
		AfxMessageBox(_T("Save failed."));
	}
	else
	{
		AfxMessageBox(_T("Saved successfully."));
	}
}

//Display BMP file header
void CimageProcessingView::OnImageprocessDisplayfileheader()
{
	if(pFileBuf == NULL) return;
	/**/
	DisplayHeaderMessage(pFileBuf);
}

// DrawPalette：将绘制调色板逻辑封装
void CimageProcessingView::DrawPalette(CDC* pDC, RGBQUAD* palette, int numColors, int originX, int originY)
{
	int blockSize = 20;      // 每个颜色块的大小
	int textOffset = 22;     // 块与文字之间的距离
	int cols = 8;            // 每行几个颜色块

	for (int i = 0; i < numColors; ++i)
	{
		int row = i / cols;
		int col = i % cols;

		RGBQUAD color = palette[i];
		COLORREF rgb = RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);

		// 颜色块位置
		CRect rect(originX + col * 150,
			originY + row * (blockSize + textOffset),
			originX + col * 150 + blockSize,
			originY + row * (blockSize + textOffset) + blockSize);

		CBrush brush(rgb);
		pDC->FillRect(&rect, &brush);

		// RGB数值文本
		CString text;
		text.Format(_T("(%3d, %3d, %3d)"), color.rgbRed, color.rgbGreen, color.rgbBlue);
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(rect.right + 5, rect.top, text);
	}
}

//Display Pallete
void CimageProcessingView::OnImageprocessDisplaypalette()
{
	if(pFileBuf == NULL) return;
	/**/
	int num = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pFileBuf,&num);
	if( pallete == NULL )
	{
		AfxMessageBox("No palette");
	}
	else
	{
		CDC* pDC = GetDC();  // 获取窗口设备上下文

		// 图像高度作为调色板起始位置向下偏移
		BITMAPINFOHEADER* pInfoHeader = GetDIBINFO(pFileBuf);
		int imageHeight = pInfoHeader ? pInfoHeader->biHeight : 256;
		int paletteStartY = imageHeight + 30;

		// 调用绘制函数
		DrawPalette(pDC, pallete, num, 10, paletteStartY);

		ReleaseDC(pDC);
	}

}

//Get pixel value
void CimageProcessingView::OnImageprocessGetpixelvalue()
{
	if (pFileBuf == NULL)
	{
		AfxMessageBox(_T("请先打开 BMP 文件。"));
		return;
	}

	// 调用坐标输入对话框
	CPixelCoordDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	int x = dlg.m_x;
	int y = dlg.m_y;

	RGBQUAD rgb;
	bool bGray;
	GetPixel(pFileBuf, x, y, &rgb, &bGray);

	CString msg;
	if (bGray)
		msg.Format(_T("像素 (%d, %d) = 灰度 %d"), x, y, rgb.rgbReserved);
	else
		msg.Format(_T("像素 (%d, %d) = RGB(%d, %d, %d)"), x, y, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);

	AfxMessageBox(msg);
}

//Set pixel value
void CimageProcessingView::OnImageprocessSetpixelvalue()
{
	if (pFileBuf == NULL)
	{
		AfxMessageBox(_T("请先打开 BMP 文件。"));
		return;
	}

	// 调用坐标输入对话框
	CPixelCoordDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	int x = dlg.m_x;
	int y = dlg.m_y;

	// 弹出颜色选择器
	CColorDialog colorDlg;
	if (colorDlg.DoModal() != IDOK)
		return;

	COLORREF c = colorDlg.GetColor();
	RGBQUAD rgb;
	rgb.rgbRed = GetRValue(c);
	rgb.rgbGreen = GetGValue(c);
	rgb.rgbBlue = GetBValue(c);
	rgb.rgbReserved = 0;

	SetPixel(pFileBuf, x, y, rgb);

	Invalidate();
	UpdateWindow();

	CString msg;
	msg.Format(_T("已设置像素 (%d, %d) 为 RGB(%d, %d, %d)"), x, y, rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
	AfxMessageBox(msg);
}

//Image interpolaion
void CimageProcessingView::OnImageprocessInerpolation()
{
	if (pFileBuf == NULL)
	{
		AfxMessageBox(_T("请先打开一张BMP图像。"));
		return;
	}

	// 弹出插值参数输入对话框
	CInterpolationDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	double scaleX = dlg.m_scaleX;
	double scaleY = dlg.m_scaleY;
	int method = dlg.m_method;  // 0-最近邻，1-双线性

	// 获取原图宽高
	BITMAPINFOHEADER* pInfo = (BITMAPINFOHEADER*)(pFileBuf + sizeof(BITMAPFILEHEADER));
	int orgWidth = pInfo->biWidth;
	int orgHeight = abs(pInfo->biHeight); // 可能为负

	// 计算缩放后新尺寸
	int newWidth = (int)(orgWidth * scaleX + 0.5);
	int newHeight = (int)(orgHeight * scaleY + 0.5);

	if (newWidth <= 0 || newHeight <= 0)
	{
		AfxMessageBox(_T("缩放后的尺寸非法。"));
		return;
	}

	// 调用图像缩放核心函数
	char* pNewBuf = ImageInterpolation(pFileBuf, newWidth, newHeight, method);
	if (!pNewBuf)
	{
		AfxMessageBox(_T("图像缩放失败，参数或内存分配错误！"));
		return;
	}

	// 替换缓冲区并刷新显示
	delete[] pFileBuf;
	pFileBuf = pNewBuf;
	Invalidate();
	UpdateWindow();

	AfxMessageBox(_T("图像缩放完成。"));
}


//Gaussian smoothing
void CimageProcessingView::OnImageprocessGausssmooth()
{
	if (!pFileBuf) return;
	CFilterParamDialog dlg(FilterMode::Gaussian);
	if (dlg.DoModal() == IDOK) {
		char* newBuf = GaussianSmooth(pFileBuf, dlg.sigma);
		if (newBuf) {
			delete[] pFileBuf;
			pFileBuf = newBuf;
			Invalidate(); UpdateWindow();
			AfxMessageBox(_T("高斯滤波完成。"));
		}
		else {
			AfxMessageBox(_T("高斯滤波失败。"));
		}
	}
}

//Median filtering
void CimageProcessingView::OnImageprocessMedianfilter()
{
	if (!pFileBuf) return;
	CFilterParamDialog dlg(FilterMode::Median);
	if (dlg.DoModal() == IDOK) {
		char* newBuf = MedianFilter(pFileBuf, dlg.windowSize);
		if (newBuf) {
			delete[] pFileBuf;
			pFileBuf = newBuf;
			Invalidate(); UpdateWindow();
			AfxMessageBox(_T("中值滤波完成。"));
		}
		else {
			AfxMessageBox(_T("中值滤波失败。"));
		}
	}
}

//Bilateral filtering
void CimageProcessingView::OnImageprocessBilateralfilter()
{
	if (!pFileBuf) return;
	CFilterParamDialog dlg(FilterMode::Bilateral);
	if (dlg.DoModal() == IDOK) {
		char* newBuf = BilateralFilter(pFileBuf, dlg.sigma_d, dlg.sigma_r);
		if (newBuf) {
			delete[] pFileBuf;
			pFileBuf = newBuf;
			Invalidate(); UpdateWindow();
			AfxMessageBox(_T("双边滤波完成。"));
		}
		else {
			AfxMessageBox(_T("双边滤波失败。"));
		}
	}
}

//Histogram equalization
void CimageProcessingView::OnImageprocessHistoequalization()
{
	if (!pFileBuf) {
		AfxMessageBox(_T("请先打开一张BMP图像。"));
		return;
	}

	char* newBuf = HistogramEqualization(pFileBuf);
	if (newBuf) {
		delete[] pFileBuf;
		pFileBuf = newBuf;
		Invalidate(); UpdateWindow();
		AfxMessageBox(_T("直方图均衡化完成。"));
	}
	else {
		AfxMessageBox(_T("直方图均衡化失败。"));
	}
}

//Sharpening by gradient
void CimageProcessingView::OnImageprocessSharpengrad()
{
	if (!pFileBuf) {
		AfxMessageBox(_T("请先打开一张BMP图像。"));
		return;
	}

	CEnhanceParamDialog dlg(EnhanceMode::Sharpen);
	if (dlg.DoModal() == IDOK) {
		char* newBuf = GradientSharpening(pFileBuf, dlg.alpha);
		if (newBuf) {
			delete[] pFileBuf;
			pFileBuf = newBuf;
			Invalidate(); UpdateWindow();
			AfxMessageBox(_T("图像锐化完成。"));
		}
		else {
			AfxMessageBox(_T("图像锐化失败。"));
		}
	}
}

//Cany edge detection
void CimageProcessingView::OnImageprocessCannyedge()
{
	if (!pFileBuf) {
		AfxMessageBox(_T("请先打开一张BMP图像。"));
		return;
	}

	CEnhanceParamDialog dlg(EnhanceMode::Canny);
	if (dlg.DoModal() == IDOK) {
		char* newBuf = CannyEdgeDetection(pFileBuf, dlg.sigma, dlg.lowThresh, dlg.highThresh);
		if (newBuf) {
			delete[] pFileBuf;
			pFileBuf = newBuf;
			Invalidate(); UpdateWindow();
			AfxMessageBox(_T("Canny边缘检测完成。"));
		}
		else {
			AfxMessageBox(_T("Canny边缘检测失败。"));
		}
	}
}

//Otsu segmentation
void CimageProcessingView::OnImageprocessOtsusegment()
{
}
