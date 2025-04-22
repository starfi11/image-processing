#include "FilterParamDialog.h"

IMPLEMENT_DYNAMIC(CFilterParamDialog, CDialogEx)

CFilterParamDialog::CFilterParamDialog(FilterMode m, CWnd* pParent)
	: CDialogEx(IDD_FILTER_PARAM_DIALOG, pParent),
	sigma(1.0), windowSize(3), sigma_d(2.0), sigma_r(25.0), mode(m)
{}

void CFilterParamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGMA, sigma);
	DDX_Text(pDX, IDC_EDIT_WINDOW, windowSize);
	DDX_Text(pDX, IDC_EDIT_SIGMA_D, sigma_d);
	DDX_Text(pDX, IDC_EDIT_SIGMA_R, sigma_r);
}

BEGIN_MESSAGE_MAP(CFilterParamDialog, CDialogEx)
END_MESSAGE_MAP()

BOOL CFilterParamDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT_SIGMA)->ShowWindow(mode == Gaussian ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_SIGMA)->ShowWindow(mode == Gaussian ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_EDIT_WINDOW)->ShowWindow(mode == Median ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_WINDOW)->ShowWindow(mode == Median ? SW_SHOW : SW_HIDE);

	BOOL isBilateral = (mode == Bilateral);
	GetDlgItem(IDC_EDIT_SIGMA_D)->ShowWindow(isBilateral ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_EDIT_SIGMA_R)->ShowWindow(isBilateral ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_SIGMA_D)->ShowWindow(isBilateral ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_SIGMA_R)->ShowWindow(isBilateral ? SW_SHOW : SW_HIDE);

	return TRUE;
}
