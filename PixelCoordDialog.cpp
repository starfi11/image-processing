#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include "resource.h"
#include "imageProcessing.h"
#include "PixelCoordDialog.h"

// 实现动态类型支持（必须配对 DECLARE_DYNAMIC）
IMPLEMENT_DYNAMIC(CPixelCoordDialog, CDialogEx)

CPixelCoordDialog::CPixelCoordDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PIXEL_COORD_DIALOG, pParent), m_x(0), m_y(0)
{
}

void CPixelCoordDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_X, m_x);
    DDX_Text(pDX, IDC_EDIT_Y, m_y);
}

BEGIN_MESSAGE_MAP(CPixelCoordDialog, CDialogEx)
END_MESSAGE_MAP()