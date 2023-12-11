#pragma once
#include "afxdialogex.h"


// CAngleinputDialog 대화 상자

class CAngleinputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAngleinputDialog)

public:
	CAngleinputDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAngleinputDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iAngle;
};
