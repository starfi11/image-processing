#pragma once

#include "afxwin.h"

// 模式标识
enum EnhanceMode {
	Histogram,
	Sharpen,
	Canny
};

// 核心函数签名
char* HistogramEqualization(char* pBmpBuf);
char* GradientSharpening(char* pBmpBuf, double alpha);
char* CannyEdgeDetection(char* pBmpBuf, double sigma, int lowThresh, int highThresh);
char* OtsuSegmentation(char* pBmpBuf);

