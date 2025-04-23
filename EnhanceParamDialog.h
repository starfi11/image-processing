#pragma once

#include "afxwin.h"
#include <afxext.h>
#include <afxdialogex.h>
#include "resource.h"
#include "ImageEnhance.h"

class CEnhanceParamDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEnhanceParamDialog)

public:
	CEnhanceParamDialog(EnhanceMode mode, CWnd* pParent = nullptr);

	// ²ÎÊý×Ö¶Î
	double alpha;       // Gradient
	double sigma;       // Canny
	int lowThresh;
	int highThresh;
	EnhanceMode mode;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENHANCE_PARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticAlpha();
};
