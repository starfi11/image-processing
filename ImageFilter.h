#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>

#include "afxwin.h"           // for BYTE, RGBQUAD
#include "_GlobalCommon.h"    // for image functions

char* GaussianSmooth(char* pBmpBuf, double sigma);
char* MedianFilter(char* pBmpBuf, int windowSize);
char* BilateralFilter(char* pBmpBuf, double sigma_d, double sigma_r);
