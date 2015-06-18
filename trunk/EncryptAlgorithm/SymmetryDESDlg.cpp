// SymmetryDESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "SymmetryDESDlg.h"


extern CString DataToUnsignedString(const char* dataIn, long dataInLen, int shift);
extern bool UnsignedStringToData(const char* dataIn, int shift, char* dataOut, long dataOutLen);

// SymmetryDESDlg dialog
const CString SymmetryDESDlg::STATIC_CONST_CSTR_INITIALVECTORVALUE = _T("初始向量值:");
const CString SymmetryDESDlg::STATIC_CONST_CSTR_INITIALCOUNTVALUE = _T("初始计数值:");

SymmetryDESDlg::SYMMETRY_TABLE_DES_TYPE 
SymmetryDESDlg::GetDESType()
{
	SYMMETRY_TABLE_DES_TYPE eDES = SYMMETRY_TABLE_DES_DES;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DES)))->GetCheck()) {
		eDES = SYMMETRY_TABLE_DES_DES;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->GetCheck()) {
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
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_ECB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCBC)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CBC;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCFB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CFB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESOFB)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_OFB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCTR)))->GetCheck()) {
		eDESMode = SYMMETRY_TABLE_DESMODE_CTR;
	}
	else {
		eDESMode = SYMMETRY_TABLE_DESMODE_INVALID;
	}
	return eDESMode;
}

bool 
SymmetryDESDlg::GetCipherKey(char* pKeyOut, long KeyOutLen, long* pKeyLen)
{
	CString cstrCipherKey;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_CIPHERKEY)->GetWindowText(cstrCipherKey);
	bool bRet = ConvertCStringToData(cstrCipherKey, pKeyOut, KeyOutLen, pKeyLen);
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DES)))->GetCheck()) {
		if (*pKeyLen > 0x08) {
			*pKeyLen = 0x08;
		}
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->GetCheck()) {
		if (*pKeyLen > 0x10) {
			*pKeyLen = 0x10;
		}
	}
	else {
		
	}
	return bRet;
}

bool  
SymmetryDESDlg::GetInitialValue(char* pKeyOut, long KeyOutLen, long* pKeyLen)
{
	CString cstrInitialValue;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->GetWindowText(cstrInitialValue);
	return  ConvertCStringToData(cstrInitialValue, pKeyOut, KeyOutLen, pKeyLen);
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
	
	strncpy(m_chKey, _T("mxEncryptDecrypt"), STATIC_CONST_LONG_KEYLENGTHMAX-1);
	m_lKeyLength = strlen(m_chKey);
	strncpy(m_chInitValue, _T("mxEncryp"), STATIC_CONST_LONG_INITVALUELENGTHMAX-1);
	m_lInitValueLength = strlen(m_chInitValue);
	
	((CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE))->SetLimitText(STATIC_CONST_LONG_INITVALUELENGTHMAX-1);
	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_TEXT)))->SetCheck(BST_CHECKED);	//默认使用文本
	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->SetCheck(BST_CHECKED);	//默认使用3DES加密
	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->SetCheck(BST_CHECKED);	//默认使用ECB模式
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
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESCTR)))->GetCheck()) {
		GetDlgItem(IDC_STATIC_SYMMETRYDES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALCOUNTVALUE);	
	}
	else {
		GetDlgItem(IDC_STATIC_SYMMETRYDES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALVECTORVALUE);
	}

	//3DES加密模式支持16个字符, DES加密模式支持8个字符
	long lKeyLen = (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_3DES)))->GetCheck())?(STATIC_CONST_LONG_KEYLENGTHMAX-1):((STATIC_CONST_LONG_KEYLENGTHMAX-1)/2);
	long lInitValueLen = STATIC_CONST_LONG_INITVALUELENGTHMAX-1;

	//刷新
	CString cstrCipherKey;
	CString cstrInitValue;
	int bufLen = 0x00;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_TEXT)))->GetCheck()) {
		cstrCipherKey = m_chKey;
		cstrInitValue = m_chInitValue;
		bufLen = 0x01;
	}
	else {
		int shift = 0x00;
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_HEX)))->GetCheck()) {
			shift = 0x10;
			bufLen = 0x02;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_OCTAL)))->GetCheck()) {
			shift = 0x08;
			bufLen = 0x03;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_BINARTY4)))->GetCheck()) {
			shift = 0x04;
			bufLen = 0x04;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_BINARY)))->GetCheck()) {
			shift = 0x02;
			bufLen = 0x08;
		}
		else {
			;
		}
		cstrCipherKey = DataToUnsignedString(m_chKey, m_lKeyLength, shift);
		cstrInitValue = DataToUnsignedString(m_chInitValue, m_lInitValueLength, shift);
	}
	lKeyLen *= bufLen;
	lInitValueLen *= bufLen;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYDES_CIPHERKEY);
	pEdit->SetLimitText(lKeyLen);
	pEdit->SetWindowText(cstrCipherKey.Left(lKeyLen));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE);
	pEdit->SetLimitText(lInitValueLen);
	pEdit->SetWindowText(cstrInitValue.Left(lInitValueLen));
	
}

void 
SymmetryDESDlg::UpdateMixCtrlEnable()
{	
	bool bECB = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_DESECB)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->EnableWindow(!bECB);
}

bool 
SymmetryDESDlg::ConvertCStringToData(CString cstrDataIn, char* pDataOut, long DataOutLen, long* pDataLen)
{
	if (NULL == pDataOut || NULL == pDataLen) {
		return false;
	}
	memset(pDataOut, 0x00, DataOutLen);

	char* pchCipherKey = cstrDataIn.GetBuffer(cstrDataIn.GetLength());
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_TEXT)))->GetCheck()) {
		*pDataLen = strlen(pchCipherKey);
		if (*pDataLen > DataOutLen) {
			return false;
		}
		memcpy(pDataOut, pchCipherKey, *pDataLen);
	}
	else {
		long keyLen = 0x00;
		long buffLen = 0x00;
		int shift = 0x00;
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_HEX)))->GetCheck()) {
			buffLen = 0x02;
			shift = 0x10;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_OCTAL)))->GetCheck()) {
			buffLen = 0x03;
			shift = 0x08;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_BINARTY4)))->GetCheck()) {
			buffLen = 0x04;
			shift = 0x04;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYDES_BINARY)))->GetCheck()) {
			buffLen = 0x08;
			shift = 0x02;
		}
		else {
			;
		}
		keyLen = strlen(pchCipherKey)/buffLen;
		keyLen += 0x00==strlen(pchCipherKey)%buffLen?0x00:0x01;
		*pDataLen = keyLen;
		if (*pDataLen > DataOutLen) {
			return false;
		}
		return UnsignedStringToData(pchCipherKey, shift, pDataOut, *pDataLen);
	}	
	return true;
}

IMPLEMENT_DYNAMIC(SymmetryDESDlg, CDialog)

SymmetryDESDlg::SymmetryDESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SymmetryDESDlg::IDD, pParent),
	m_lKeyLength(0x00),
	m_lInitValueLength(0x00)
{
	memset(m_chKey, 0x00, sizeof(char)*STATIC_CONST_LONG_KEYLENGTHMAX);
	memset(m_chInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_INITVALUELENGTHMAX);
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
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_TEXT, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesText)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_HEX, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesHex)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_OCTAL, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesOctal)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_BINARTY4, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesBinarty4)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYDES_BINARY, &SymmetryDESDlg::OnBnClickedRadioSymmetrydesBinary)
	ON_EN_KILLFOCUS(IDC_EDIT_SYMMETRYDES_CIPHERKEY, &SymmetryDESDlg::OnEnKillfocusEditSymmetrydesCipherkey)
	ON_EN_KILLFOCUS(IDC_EDIT_SYMMETRYDES_INITIALVALUE, &SymmetryDESDlg::OnEnKillfocusEditSymmetrydesInitialvalue)
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
	UpdateMixCtrl();
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

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesText()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesHex()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesOctal()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesBinarty4()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnBnClickedRadioSymmetrydesBinary()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrl();
}

void SymmetryDESDlg::OnEnKillfocusEditSymmetrydesCipherkey()
{
	// TODO: Add your control notification handler code here
	CString cstrCipherKey;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_CIPHERKEY)->GetWindowText(cstrCipherKey);
	ConvertCStringToData(cstrCipherKey, m_chKey, STATIC_CONST_LONG_KEYLENGTHMAX, &m_lKeyLength);
}

void SymmetryDESDlg::OnEnKillfocusEditSymmetrydesInitialvalue()
{
	// TODO: Add your control notification handler code here
	CString cstrInitialValue;
	GetDlgItem(IDC_EDIT_SYMMETRYDES_INITIALVALUE)->GetWindowText(cstrInitialValue);
	ConvertCStringToData(cstrInitialValue, m_chInitValue, STATIC_CONST_LONG_INITVALUELENGTHMAX, &m_lInitValueLength);
}
