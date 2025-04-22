#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include "resource.h"

class CInterpolationDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CInterpolationDialog)
    DECLARE_MESSAGE_MAP()

public:
    CInterpolationDialog(CWnd* pParent = nullptr);

    double m_scaleX;
    double m_scaleY;
    int m_method;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_INTERPOLATION_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
};
