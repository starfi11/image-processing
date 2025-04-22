#pragma once
#include <afxwin.h>
#include <afxdialogex.h>
#include "resource.h"

enum FilterMode {
	Gaussian,
	Median,
	Bilateral
};

class CFilterParamDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterParamDialog)

public:
	CFilterParamDialog(FilterMode mode, CWnd* pParent = nullptr);

	double sigma;       // For Gaussian
	int windowSize;     // For Median
	double sigma_d;     // For Bilateral
	double sigma_r;     // For Bilateral
	FilterMode mode;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTER_PARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
