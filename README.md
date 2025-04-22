# README

这是HUST数字图像处理课程的大作业，本仓库仅用于记录实践过程。

## 过程记录

### 安装环境：

![image-20250420183711867](https://luweiphoto.oss-cn-wuhan-lr.aliyuncs.com/202504201837953.png)

### 任务

| 实验任务描述            | 菜单命令ID                          | 响应函数名                                                | 所在文件                  |
| ----------------------- | ----------------------------------- | --------------------------------------------------------- | ------------------------- |
| Open a BMP file         | `ID_IMAGEPROCESS_OPENBMPFILE`       | `CimageProcessingView::OnImageprocessOpenbmpfile()`       | `imageProcessingView.cpp` |
| Save to a new BMP file  | `ID_IMAGEPROCESS_SAVETOFILE`        | `CimageProcessingView::OnImageprocessSavetofile()`        | `imageProcessingView.cpp` |
| Display BMP file header | `ID_IMAGEPROCESS_DISPLAYFILEHEADER` | `CimageProcessingView::OnImageprocessDisplayfileheader()` | `imageProcessingView.cpp` |
| Display Palette         | `ID_IMAGEPROCESS_DISPLAYPALETTE`    | `CimageProcessingView::OnImageprocessDisplaypalette()`    | `imageProcessingView.cpp` |
| Get pixel value         | `ID_IMAGEPROCESS_GETPIXELVALUE`     | `CimageProcessingView::OnImageprocessGetpixelvalue()`     | `imageProcessingView.cpp` |
| Set pixel value         | `ID_IMAGEPROCESS_SETPIXELVALUE`     | `CimageProcessingView::OnImageprocessSetpixelvalue()`     | `imageProcessingView.cpp` |
| Image interpolation     | `ID_IMAGEPROCESS_INERPOLATION`      | `CimageProcessingView::OnImageprocessInerpolation()`      | `imageProcessingView.cpp` |
| Gaussian smoothing      | `ID_IMAGEPROCESS_GAUSSSMOOTH`       | `CimageProcessingView::OnImageprocessGausssmooth()`       | `imageProcessingView.cpp` |
| Median filtering        | `ID_IMAGEPROCESS_MEDIANFILTER`      | `CimageProcessingView::OnImageprocessMedianfilter()`      | `imageProcessingView.cpp` |
| Bilateral filtering     | `ID_IMAGEPROCESS_BILATERALFILTER`   | `CimageProcessingView::OnImageprocessBilateralfilter()`   | `imageProcessingView.cpp` |
| Histogram equalization  | `ID_IMAGEPROCESS_HISTOEQUALIZATION` | `CimageProcessingView::OnImageprocessHistoequalization()` | `imageProcessingView.cpp` |
| Sharpening by gradient  | `ID_IMAGEPROCESS_SHARPENGRAD`       | `CimageProcessingView::OnImageprocessSharpengrad()`       | `imageProcessingView.cpp` |
| Canny edge detection    | `ID_IMAGEPROCESS_CANNYEDGE`         | `CimageProcessingView::OnImageprocessCannyedge()`         | `imageProcessingView.cpp` |
| Otsu segmentation       | `ID_IMAGEPROCESS_OTSUSEGMENT`       | `CimageProcessingView::OnImageprocessOtsusegment()`       | `imageProcessingView.cpp` |

### 项目架构与接口

#### 项目架构

```cpp
CimageProcessingApp
     ↓ 注册文档模板
 ┌────────────────────┐
 │ CMainFrame（主窗口）│
 └────┬───────────────┘
      ↓
┌────────────┐      ┌─────────────┐
│ CDocument  │◄───►│  CView      │
│ 保存状态   │       │ 图像交互和绘制 │
└────────────┘      └─────────────┘
                          ↓
                   `_GlobalCommon.cpp`
                   （实际图像处理函数）

```

#### MFC架构

| 类名                   | 类型     | 继承自      | 作用                                       | 文件位置                    |
| ---------------------- | -------- | ----------- | ------------------------------------------ | --------------------------- |
| `CimageProcessingApp`  | 应用类   | `CWinApp`   | MFC程序的入口，负责初始化、注册文档模板    | `imageProcessing.cpp/h`     |
| `CMainFrame`           | 主窗口类 | `CFrameWnd` | 主窗口框架，负责创建菜单、工具栏等外观     | `MainFrm.cpp/h`             |
| `CimageProcessingDoc`  | 文档类   | `CDocument` | 负责数据的管理与存储逻辑（可扩展）         | `imageProcessingDoc.cpp/h`  |
| `CimageProcessingView` | 视图类   | `CView`     | 用户图像的操作界面，处理图像显示和命令响应 | `imageProcessingView.cpp/h` |
| `CAboutDlg`            | 对话框类 | `CDialogEx` | “关于”窗口                                 | `imageProcessing.cpp`       |

#### 详细组成与接口功能

##### CimagePorcessingAPP

- 继承CWinApp
- 职责：启动应用程序、注册doc-view架构、处理命令行、显示主窗口
- 核心方法：
  - InitInstance()：项目启动的真正入口
  - OnAppAbout()：显示关于对话框

##### CMainFrame

- 继承CFrameWnd
- 职责：界面样式的容器、包含菜单栏、工具栏
- 核心方法：
  - PreCreateWindow() 修改窗口属性
  - AssertValid()
  - Dump()

##### CimageProcessingDoc

- 继承CDocument
- 职责：存储和管理数据
- 核心方法
  - OnNewDocument() 初始化文档数据结构
  - Serialize(CArchive&) 存储/加载数据接口
  - AssertValid() / Dump() 调试辅助

##### CimageProcessingView (视图类)

- 继承CView
- 职责：响应用户命令
- 重要方法：命令响应函数
  - 所有的任务函数

### 路径规划

#### ✅ 阶段一：基础文件操作（不依赖图像处理）

1. `Open a BMP file`
2. `Display BMP file header`
3. `Display Palette`
4. `Get pixel value`
5. `Set pixel value`
6. `Save to a new BMP file`

👉 熟悉图像文件结构、调色板和数据缓冲区。

------

#### ✅ 阶段二：几何变换（需要理解像素映射）

1. `Image interpolation`
    （最邻近、双线性，理解图像放缩原理）

------

#### ✅ 阶段三：基础滤波（先易后难）

1. `Gaussian smoothing`
2. `Median filtering`
3. `Bilateral filtering`

👉 注意空间滤波器、边缘保持能力的比较。

------

#### ✅ 阶段四：图像增强与边缘

1. `Histogram equalization`
2. `Sharpening by gradient`
3. `Canny edge detection`

👉 注意图像对比度增强与边缘检测的组合意义。

------

#### ✅ 阶段五：图像分割

1. `Otsu segmentation`
    👉 适合作为最后一个任务，整合对图像直方图、阈值、内存操作的综合理解。