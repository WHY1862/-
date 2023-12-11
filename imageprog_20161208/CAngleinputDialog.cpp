// CAngleinputDialog.cpp: 구현 파일
//

#include "pch.h"
#include "imageprog_20161208.h"
#include "afxdialogex.h"
#include "CAngleinputDialog.h"


// CAngleinputDialog 대화 상자

IMPLEMENT_DYNAMIC(CAngleinputDialog, CDialogEx)

CAngleinputDialog::CAngleinputDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_iAngle(0)
{

}

CAngleinputDialog::~CAngleinputDialog()
{
}

void CAngleinputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_iAngle);
}


BEGIN_MESSAGE_MAP(CAngleinputDialog, CDialogEx)
END_MESSAGE_MAP()


// CAngleinputDialog 메시지 처리기
