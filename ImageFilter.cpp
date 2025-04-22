#include"ImageFilter.h"

char* GaussianSmooth(char* pBmpBuf, double sigma) {
    if (!pBmpBuf || sigma <= 0.0) return NULL;

    int kernelRadius = (int)ceil(3 * sigma);
    int kernelSize = kernelRadius * 2 + 1;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;

    // 构造高斯核
    for (int y = -kernelRadius; y <= kernelRadius; y++) {
        for (int x = -kernelRadius; x <= kernelRadius; x++) {
            double weight = exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel[y + kernelRadius][x + kernelRadius] = weight;
            sum += weight;
        }
    }
    for (auto& row : kernel) for (auto& v : row) v /= sum;

    // 创建新图像
    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    int depth = GetColorBits(pBmpBuf);
    long newSize = ((BITMAPFILEHEADER*)pBmpBuf)->bfSize;
    char* newBuf = new char[newSize];
    memcpy(newBuf, pBmpBuf, newSize);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBQUAD acc = { 0, 0, 0, 0 };
            double weightSum = 0.0;
            for (int j = -kernelRadius; j <= kernelRadius; j++) {
                for (int i = -kernelRadius; i <= kernelRadius; i++) {
                    int xx = min(max(x + i, 0), width - 1);
                    int yy = min(max(y + j, 0), height - 1);
                    RGBQUAD rgb;
                    GetPixel(pBmpBuf, xx, yy, &rgb);
                    double w = kernel[j + kernelRadius][i + kernelRadius];
                    acc.rgbRed += (BYTE)(rgb.rgbRed * w);
                    acc.rgbGreen += (BYTE)(rgb.rgbGreen * w);
                    acc.rgbBlue += (BYTE)(rgb.rgbBlue * w);
                }
            }
            SetPixel(newBuf, x, y, acc);
        }
    }
    return newBuf;
}

char* MedianFilter(char* pBmpBuf, int windowSize) {
    if (!pBmpBuf || windowSize < 3 || windowSize % 2 == 0) return NULL;

    int r = windowSize / 2;
    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    long newSize = ((BITMAPFILEHEADER*)pBmpBuf)->bfSize;
    char* newBuf = new char[newSize];
    memcpy(newBuf, pBmpBuf, newSize);

    std::vector<BYTE> rvec, gvec, bvec;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            rvec.clear(); gvec.clear(); bvec.clear();
            for (int j = -r; j <= r; j++) {
                for (int i = -r; i <= r; i++) {
                    int xx = min(max(x + i, 0), width - 1);
                    int yy = min(max(y + j, 0), height - 1);
                    RGBQUAD rgb;
                    GetPixel(pBmpBuf, xx, yy, &rgb);
                    rvec.push_back(rgb.rgbRed);
                    gvec.push_back(rgb.rgbGreen);
                    bvec.push_back(rgb.rgbBlue);
                }
            }
            std::sort(rvec.begin(), rvec.end());
            std::sort(gvec.begin(), gvec.end());
            std::sort(bvec.begin(), bvec.end());
            RGBQUAD rgb = {
                bvec[bvec.size() / 2],
                gvec[gvec.size() / 2],
                rvec[rvec.size() / 2],
                0
            };
            SetPixel(newBuf, x, y, rgb);
        }
    }
    return newBuf;
}

char* BilateralFilter(char* pBmpBuf, double sigma_d, double sigma_r) {
    if (!pBmpBuf || sigma_d <= 0.0 || sigma_r <= 0.0) return NULL;

    int width = GetImageWidth(pBmpBuf);
    int height = GetImageHeight(pBmpBuf);
    int radius = (int)ceil(3 * sigma_d);
    long newSize = ((BITMAPFILEHEADER*)pBmpBuf)->bfSize;
    char* newBuf = new char[newSize];
    memcpy(newBuf, pBmpBuf, newSize);

    auto gauss = [](double x, double sigma) {
        return exp(-(x * x) / (2 * sigma * sigma));
        };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBQUAD center;
            GetPixel(pBmpBuf, x, y, &center);

            double sumR = 0, sumG = 0, sumB = 0, wSum = 0;
            for (int j = -radius; j <= radius; j++) {
                for (int i = -radius; i <= radius; i++) {
                    int xx = min(max(x + i, 0), width - 1);
                    int yy = min(max(y + j, 0), height - 1);
                    RGBQUAD neighbor;
                    GetPixel(pBmpBuf, xx, yy, &neighbor);

                    double wd = gauss(sqrt(i * i + j * j), sigma_d);
                    double wr = gauss(
                        (center.rgbRed - neighbor.rgbRed) * (center.rgbRed - neighbor.rgbRed) +
                        (center.rgbGreen - neighbor.rgbGreen) * (center.rgbGreen - neighbor.rgbGreen) +
                        (center.rgbBlue - neighbor.rgbBlue) * (center.rgbBlue - neighbor.rgbBlue),
                        sigma_r
                    );

                    double w = wd * wr;
                    sumR += neighbor.rgbRed * w;
                    sumG += neighbor.rgbGreen * w;
                    sumB += neighbor.rgbBlue * w;
                    wSum += w;
                }
            }
            RGBQUAD result;
            result.rgbRed = (BYTE)(sumR / wSum);
            result.rgbGreen = (BYTE)(sumG / wSum);
            result.rgbBlue = (BYTE)(sumB / wSum);
            SetPixel(newBuf, x, y, result);
        }
    }
    return newBuf;
}
