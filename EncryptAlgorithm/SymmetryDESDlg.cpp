// SymmetryDESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "SymmetryDESDlg.h"


// SymmetryDESDlg dialog

const CString SymmetryDESDlg::STATIC_CONST_CSTR_INITIALVECTORVALUE = _T("初始向量值:");
const CString SymmetryDESDlg::STATIC_CONST_CSTR_INITIALCOUNTVALUE = _T("初始计数值:");

SymmetryDESDlg::SYMMETRY_TABLE_DES_TYPE 
SymmetryDESDlg::GetDESType()
{
	SYMMETRY_TABLE_DES_TYPE eDES = SYMMETRY_TABLE_DES_DES;
	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DES)))->GetCheck()) {
		eDES = SYMMETRY_TABLE_DES_DES;
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->GetCheck()) {
		eDES = SYMMETRY_TABLE_DES_3DES;
	}
	else {
		eDES = SYMMETRY_TABLE_DES_INVALID;
	}
	return eDES;
}

SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_TYPE 
SymmetryDESDlg::GetDESModeType()
{
	SYMMETRY_TABLE_DESMODE_TYPE eDESMode = SYMMETRY_TABLE_DESMODE_NON;
	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_ECB;
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCBC)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CBC;
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCFB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CFB;
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESOFB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_OFB;
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCTR)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CTR;
	}
	else {
		eDESMode = SYMMETRY_TABLE_DESMODE_INVALID;
	}
	return eDESMode;
}

CString 
SymmetryDESDlg::GetCipherKey()
{
	CString cstrCipherKey;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_CIPHERKEY)->GetWindowText(cstrCipherKey);
	return cstrCipherKey;
}

CString 
SymmetryDESDlg::GetInitialValue()
{
	CString cstrInitialValue;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->GetWindowText(cstrInitialValue);
	return cstrInitialValue;
}

void 
SymmetryDESDlg::UpdateCtrl()
{
	UpdateMixCtrl();
}

BOOL 
SymmetryDESDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)) return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void 
SymmetryDESDlg::OnInitCtrl()
{
	GetDlgItem(IDC_STATIC_SYMMETRYDES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALVECTORVALUE);
	
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->SetWindowText("mx111111");
	GetDlgItem(IDC_EDIT_SYMMETRYDES_CIPHERKEY)->SetWindowText("mx123456");

	((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->SetCheck(BST_CHECKED);	//默认使用3DES加密
	((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->SetCheck(BST_CHECKED);	//默认使用ECB模式
}

void 
SymmetryDESDlg::UpdateMixCtrl()
{
	UpdateMixCtrlValue();
	UpdateMixCtrlEnable();
}

void 
SymmetryDESDlg::UpdateMixCtrlValue()
{
	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCTR)))->GetCheck()) {
		GetDlgItem(IDC_STATIC_SYMMETRYDES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALCOUNTVALUE);	
	}
	else {
		GetDlgItem(IDC_STATIC_SYMMETRYDES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALVECTORVALUE);
	}
}

void 
SymmetryDESDlg::UpdateMixCtrlEnable()
{
	bool b3DES = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->GetCheck()?true:false;
	//3DES加密模式支持16个字符, DES加密模式支持8个字符
	
	bool bECB = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->EnableWindow(!bECB);
}

IMPLEMENT_DYNAMIC(SymmetryDESDlg, CDialog)

SymmetryDESDlg::SymmetryDESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SymmetryDESDlg::IDD, pParent)
{

}

SymmetryDESDlg::~SymmetryDESDlg()
{
}

void SymmetryDESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SymmetryDESDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DES, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDes)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_3DES, &SymmetryDESDlg::OnBnClickedRadioSymmetrydes3des)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DESECB, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesecb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DESCBC, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDescbc)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DESCFB, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDescfb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DESOFB, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesofb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_DESCTR, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesctr)
END_MESSAGE_MAP()


// SymmetryDESDlg message handlers

BOOL SymmetryDESDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnInitCtrl();
	UpdateCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDes()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydes3des()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesecb()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDescbc()
{
	// TODO: Add your control notification handler code here
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDescfb()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesofb()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesDesctr()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}
