#pragma once

#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include "resource.h"

class CPixelCoordDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CPixelCoordDialog)

public:
    CPixelCoordDialog(CWnd* pParent = nullptr);

    int m_x;
    int m_y;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PIXEL_COORD_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};