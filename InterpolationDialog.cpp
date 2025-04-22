#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include "resource.h"
#include "imageProcessing.h"
#include "InterpolationDialog.h"

IMPLEMENT_DYNAMIC(CInterpolationDialog, CDialogEx)

CInterpolationDialog::CInterpolationDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_INTERPOLATION_DIALOG, pParent),
    m_scaleX(1.0), m_scaleY(1.0), m_method(0)
{
}

void CInterpolationDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCALE_X, m_scaleX);
    DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_scaleY);
    DDX_Radio(pDX, IDC_RADIO_NEAREST, m_method);
}

BEGIN_MESSAGE_MAP(CInterpolationDialog, CDialogEx)
END_MESSAGE_MAP()
