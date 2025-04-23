#include "ImageEnhance.h"
#include "_GlobalCommon.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include"ImageFilter.h"

// Histogram Equalization
char* HistogramEqualization(char* pBmpBuf)
{
    // 只支持 8 位图
    if (!pBmpBuf || GetColorBits(pBmpBuf) != 8) return nullptr;

    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    long bytesPerRow = GetWidthBytes(pBmpBuf);
    long dataSize = bytesPerRow * height;

    char* newBuf = new char[GetDIBHEADER(pBmpBuf)->bfSize];
    memcpy(newBuf, pBmpBuf, GetDIBHEADER(pBmpBuf)->bfOffBits);
    char* newData = GetDIBImageData(newBuf);
    char* oldData = GetDIBImageData(pBmpBuf);

    int hist[256] = { 0 };
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            hist[(BYTE)oldData[i * bytesPerRow + j]]++;

    int sum = 0, total = width * height;
    BYTE cdf[256];
    for (int i = 0; i < 256; ++i) {
        sum += hist[i];
        cdf[i] = (BYTE)(255.0 * sum / total + 0.5);
    }

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            newData[i * bytesPerRow + j] = cdf[(BYTE)oldData[i * bytesPerRow + j]];

    return newBuf;
}


// Gradient Sharpening
static inline BYTE clamp(int value) {
    return (value < 0) ? 0 : ((value > 255) ? 255 : (BYTE)value);
}

char* GradientSharpening(char* pBmpBuf, double alpha)
{
    if (!pBmpBuf || GetColorBits(pBmpBuf) != 24) return nullptr;

    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    long bytesPerRow = GetWidthBytes(pBmpBuf);
    long fileSize = GetDIBHEADER(pBmpBuf)->bfSize;

    char* newBuf = new char[fileSize];
    memcpy(newBuf, pBmpBuf, GetDIBHEADER(pBmpBuf)->bfOffBits);
    char* newData = GetDIBImageData(newBuf);
    char* oldData = GetDIBImageData(pBmpBuf);

    int dx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int dy[3][3] = { {-1,-2,-1}, {0, 0, 0}, {1, 2, 1} };

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gx[3] = { 0 }, gy[3] = { 0 };
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    BYTE* p = (BYTE*)&oldData[(y + ky) * bytesPerRow + (x + kx) * 3];
                    for (int c = 0; c < 3; ++c) {
                        gx[c] += dx[ky + 1][kx + 1] * p[c];
                        gy[c] += dy[ky + 1][kx + 1] * p[c];
                    }
                }
            }

            BYTE* orig = (BYTE*)&oldData[y * bytesPerRow + x * 3];
            BYTE* dest = (BYTE*)&newData[y * bytesPerRow + x * 3];
            for (int c = 0; c < 3; ++c) {
                int grad = std::sqrt(gx[c] * gx[c] + gy[c] * gy[c]);
                dest[c] = clamp(orig[c] + int(alpha * grad));
            }
        }
    }

    return newBuf;
}


// Canny Edge Detection
char* CannyEdgeDetection(char* pBmpBuf, double sigma, int lowThresh, int highThresh)
{
    if (!pBmpBuf || GetColorBits(pBmpBuf) != 8)
        return nullptr;

    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    long bytesPerRow = GetWidthBytes(pBmpBuf);
    long bufSize = GetDIBHEADER(pBmpBuf)->bfSize;

    char* smoothBuf = GaussianSmooth(pBmpBuf, sigma);
    if (!smoothBuf) return nullptr;

    char* src = GetDIBImageData(smoothBuf);
    char* newBuf = new char[bufSize];
    memcpy(newBuf, pBmpBuf, GetDIBHEADER(pBmpBuf)->bfOffBits);
    char* dst = GetDIBImageData(newBuf);

    // Step 2: Gradient Magnitude & Direction
    int dx[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
    int dy[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

    std::vector<double> grad(width * height);
    std::vector<double> angle(width * height);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gx = 0, gy = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int val = (BYTE)src[(y + ky) * bytesPerRow + (x + kx)];
                    gx += dx[ky + 1][kx + 1] * val;
                    gy += dy[ky + 1][kx + 1] * val;
                }
            }
            grad[y * width + x] = sqrt(gx * gx + gy * gy);
            angle[y * width + x] = atan2(gy, gx);
        }
    }

    // Step 3: Non-maximum suppression
    std::vector<BYTE> nms(width * height, 0);
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            double ang = angle[y * width + x] * 180.0 / 3.1416;
            ang = ang < 0 ? ang + 180 : ang;

            double g = grad[y * width + x];
            double g1 = 0, g2 = 0;

            if ((ang >= 0 && ang < 22.5) || (ang >= 157.5 && ang <= 180)) {
                g1 = grad[y * width + x - 1];
                g2 = grad[y * width + x + 1];
            }
            else if (ang >= 22.5 && ang < 67.5) {
                g1 = grad[(y - 1) * width + x + 1];
                g2 = grad[(y + 1) * width + x - 1];
            }
            else if (ang >= 67.5 && ang < 112.5) {
                g1 = grad[(y - 1) * width + x];
                g2 = grad[(y + 1) * width + x];
            }
            else {
                g1 = grad[(y - 1) * width + x - 1];
                g2 = grad[(y + 1) * width + x + 1];
            }

            if (g >= g1 && g >= g2)
                nms[y * width + x] = (BYTE)g;
        }
    }

    // Step 4: Hysteresis thresholding
    std::vector<BYTE> edge(width * height, 0);
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            BYTE val = nms[y * width + x];
            if (val >= highThresh) {
                edge[y * width + x] = 255;
            }
            else if (val >= lowThresh) {
                for (int ky = -1; ky <= 1; ++ky)
                    for (int kx = -1; kx <= 1; ++kx)
                        if (nms[(y + ky) * width + (x + kx)] >= highThresh)
                            edge[y * width + x] = 255;
            }
        }
    }

    // Step 5: 写入输出缓冲区
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            dst[y * bytesPerRow + x] = edge[y * width + x];

    delete[] smoothBuf;
    return newBuf;
}

char* OtsuSegmentation(char* pBmpBuf)
{
    if (!pBmpBuf || GetColorBits(pBmpBuf) != 8)
        return nullptr;

    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    long bytesPerRow = GetWidthBytes(pBmpBuf);
    long bufSize = GetDIBHEADER(pBmpBuf)->bfSize;

    char* newBuf = new char[bufSize];
    memcpy(newBuf, pBmpBuf, GetDIBHEADER(pBmpBuf)->bfOffBits);
    char* dst = GetDIBImageData(newBuf);
    char* src = GetDIBImageData(pBmpBuf);

    // Step 1: 构建灰度直方图
    int hist[256] = { 0 };
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            hist[(BYTE)src[y * bytesPerRow + x]]++;

    // Step 2: 计算像素总数和总灰度
    int total = width * height;
    double sum_all = 0;
    for (int t = 0; t < 256; ++t)
        sum_all += t * hist[t];

    // Step 3: 遍历所有阈值 t，计算类间方差
    int threshold = 0;
    double sum_b = 0;
    int w_b = 0;
    double max_var = 0;

    for (int t = 0; t < 256; ++t)
    {
        w_b += hist[t];
        if (w_b == 0) continue;

        int w_f = total - w_b;
        if (w_f == 0) break;

        sum_b += t * hist[t];
        double m_b = sum_b / w_b;
        double m_f = (sum_all - sum_b) / w_f;

        double var_bf = (double)w_b * w_f * (m_b - m_f) * (m_b - m_f);
        if (var_bf > max_var)
        {
            max_var = var_bf;
            threshold = t;
        }
    }

    // Step 4: 显示最优阈值
    CString msg;
    msg.Format(_T("自动选出的最佳阈值为：%d"), threshold);
    AfxMessageBox(msg);

    // Step 5: 应用阈值分割
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            BYTE pixel = (BYTE)src[y * bytesPerRow + x];
            dst[y * bytesPerRow + x] = (pixel >= threshold ? 255 : 0);
        }

    return newBuf;
}
