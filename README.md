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

### 对话框怎么做

我自定义了一个类用于处理对话框获取数据。

下面描述如何为我的`CPixelCoordDialog` 设置资源。

主要的一些操作包括编写C++代码以及使用图形化界面去制作对话框。

用上的操作如下图所示，详细描述于下面的步骤中。

![image-20250422193702696](https://luweiphoto.oss-cn-wuhan-lr.aliyuncs.com/202504221937780.png)

一、打开资源视图

方法 1（推荐）：

1. 在顶部菜单栏选择：
    `视图(View)` → `其他窗口(Other Windows)` → `资源视图(Resource View)`
2. 或者快捷键：`Ctrl + Alt + R`

方法 2：

- 也可以在“解决方案资源管理器”里右键 `.rc` 文件（比如 `imageProcessing.rc`） → `打开`

二、添加对话框资源

1. 在资源视图中找到：
    `imageProcessing.rc` → `Dialog`
2. 右键 `Dialog` → **添加对话框(Add Dialog)**

此时会自动添加一个新对话框（名字默认为 `IDD_DIALOG1`）

三、设置对话框 ID 和标题

1. 选中新对话框，右侧“属性窗口”（不是弹窗）中找到：
   - `ID` → 改为：`IDD_PIXEL_COORD_DIALOG`
   - `Caption`（标题）→ 改为：`输入像素坐标`

如果你没看到属性窗口，可以按 `F4` 打开。

四、添加两个输入框控件（Edit Control）

- 添加第一个输入框：

1. 在工具箱中找到 `Edit Control`（编辑框），拖到对话框中
2. 选中它 → 右侧属性窗口中修改：
   - `ID` → `IDC_EDIT_X`
   - `Number` → 设置为 `True`（只能输入数字）
   - `Auto HScroll` → `True`

- 添加第二个输入框：

方法同上，ID 改为 `IDC_EDIT_Y`

五、添加两个标签说明文字（可选）

从工具箱拖 `Static Text`，写成 `X:`、`Y:`，放在对应输入框旁边。

六、保存并确保资源文件更新

- `Ctrl + S` 全部保存
- VS 会自动更新 `resource.h` 文件，加入这些：

```cpp
#define IDD_PIXEL_COORD_DIALOG    1001
#define IDC_EDIT_X                1002
#define IDC_EDIT_Y                1003
```

> 不要手动修改 `.rc` 文件，否则容易损坏。

七、编译验证

重新编译项目 (`Ctrl + Shift + B`)，然后你调用 `CPixelCoordDialog dlg; dlg.DoModal();` 时，**就会弹出你刚创建的输入坐标窗口啦！🎉**

补充建议

| 项目     | 建议                                                  |
| -------- | ----------------------------------------------------- |
| 控件风格 | 选中 Edit 控件，在属性中勾选 `Number`、`Auto HScroll` |
| 默认值   | 可以在对话框类的构造函数中给 `m_x`、`m_y` 赋初值      |
| 输入范围 | 后续可添加范围检查，如 `x >= 0 && x < width`          |

八、.rc文件打开异常

可能是由于曾使用vscode类编辑器打开过该文件夹，删掉.vs文件可以解决。

### 资源resource.h的ID问题

不乱改，让VS2022自动生成

### RadioButton连续ID问题

做单选按钮RadionButton时遇到的问题：

```
打开资源编辑器 → 找到 IDD_INTERPOLATION_DIALOG
双击打开你插值对话框的设计视图

点击 “最近邻插值” 的单选按钮（IDC_RADIO_NEAREST）：

在“属性窗口”中设置：

Group → True

点击 “双线性插值” 的单选按钮（IDC_RADIO_BILINEAR）：

Group → False
```

### 滤波

```cpp
char* GaussianSmooth(char* pBmpBuf, double sigma);
char* MedianFilter(char* pBmpBuf, int windowSize);
char* BilateralFilter(char* pBmpBuf, double sigma_d, double sigma_r);

```

先把三个函数写出来，然后将这三个方法的窗口整合进一个.h和.cpp，编写出对应的h/cpp，然后使用该h/cpp编写对应的view，最后完成窗口资源的创建。

### _GlobalCommon.h循环引用的问题

由于该.h文件没有携带#pragma once，出现循环引用时会出现一个函数签名被声明多次，而当函数签名中包含默认值时，多次声明会报错。

携带上#pragma once就能解决。

### 图像增强

目前实现了框架和计算方法，但计算方法还没有进行正确性测试。

