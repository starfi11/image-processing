#pragma once

#include "afxwin.h"

// ģʽ��ʶ
enum EnhanceMode {
	Histogram,
	Sharpen,
	Canny
};

// ���ĺ���ǩ��
char* HistogramEqualization(char* pBmpBuf);
char* GradientSharpening(char* pBmpBuf, double alpha);
char* CannyEdgeDetection(char* pBmpBuf, double sigma, int lowThresh, int highThresh);
char* OtsuSegmentation(char* pBmpBuf);

