#include "EnhanceParamDialog.h"

IMPLEMENT_DYNAMIC(CEnhanceParamDialog, CDialogEx)

CEnhanceParamDialog::CEnhanceParamDialog(EnhanceMode m, CWnd* pParent)
	: CDialogEx(IDD_ENHANCE_PARAM_DIALOG, pParent),
	alpha(1.0), sigma(1.0), lowThresh(20), highThresh(50), mode(m)
{}

void CEnhanceParamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALPHA, alpha);
	DDX_Text(pDX, IDC_EDIT_SIGMA, sigma);
	DDX_Text(pDX, IDC_EDIT_LOW, lowThresh);
	DDX_Text(pDX, IDC_EDIT_HIGH, highThresh);
}

BEGIN_MESSAGE_MAP(CEnhanceParamDialog, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_ALPHA, &CEnhanceParamDialog::OnStnClickedStaticAlpha)
END_MESSAGE_MAP()

BOOL CEnhanceParamDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT_ALPHA)->ShowWindow(mode == Sharpen ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_ALPHA)->ShowWindow(mode == Sharpen ? SW_SHOW : SW_HIDE);

	BOOL showCanny = (mode == Canny);
	GetDlgItem(IDC_EDIT_SIGMA)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_SIGMA)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_EDIT_LOW)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_LOW)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_EDIT_HIGH)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_HIGH)->ShowWindow(showCanny ? SW_SHOW : SW_HIDE);

	return TRUE;
}


void CEnhanceParamDialog::OnStnClickedStaticAlpha()
{
	// TODO: 在此添加控件通知处理程序代码
}
