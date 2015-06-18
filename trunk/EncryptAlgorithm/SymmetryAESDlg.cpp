// SymmetryAESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "SymmetryAESDlg.h"


extern CString DataToUnsignedString(const char* dataIn, long dataInLen, int shift);
extern bool UnsignedStringToData(const char* dataIn, int shift, char* dataOut, long dataOutLen);

// SymmetryAESDlg dialog
const CString SymmetryAESDlg::STATIC_CONST_CSTR_INITIALVECTORVALUE = _T("初始向量值:");
const CString SymmetryAESDlg::STATIC_CONST_CSTR_INITIALCOUNTVALUE = _T("初始计数值:");


SymmetryAESDlg::SYMMETRY_TABLE_AESKEYLEN_TYPE 
SymmetryAESDlg::GetAESKeySizeType()
{
	SYMMETRY_TABLE_AESKEYLEN_TYPE eAESKeySize = SYMMETRY_TABLE_AESKEYLEN_NON;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT128)))->GetCheck()) {
		eAESKeySize = SYMMETRY_TABLE_AESKEYLEN_BIT128;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT192)))->GetCheck()) {
		eAESKeySize = SYMMETRY_TABLE_AESKEYLEN_BIT192;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT256)))->GetCheck()) {
		eAESKeySize = SYMMETRY_TABLE_AESKEYLEN_BIT256;
	}
	else {
		eAESKeySize = SYMMETRY_TABLE_AESKEYLEN_INVALID;
	}
	return eAESKeySize;
}

SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_TYPE 
SymmetryAESDlg::GetAESModeType()
{
	SYMMETRY_TABLE_AESMODE_TYPE eAESMode = SYMMETRY_TABLE_AESMODE_NON;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESECB)))->GetCheck()) {
		eAESMode = SYMMETRY_TABLE_AESMODE_ECB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESCBC)))->GetCheck()) {
		eAESMode = SYMMETRY_TABLE_AESMODE_CBC;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESCFB)))->GetCheck()) {
		eAESMode = SYMMETRY_TABLE_AESMODE_CFB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESOFB)))->GetCheck()) {
		eAESMode = SYMMETRY_TABLE_AESMODE_OFB;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESCTR)))->GetCheck()) {
		eAESMode = SYMMETRY_TABLE_AESMODE_CTR;
	}
	else {
		eAESMode = SYMMETRY_TABLE_AESMODE_INVALID;
	}
	return eAESMode;
}


bool 
SymmetryAESDlg::GetCipherKey(char* pKeyOut, long KeyOutLen, long* pKeyLen)
{
	CString cstrCipherKey;
	GetDlgItem(IDC_EDIT_SYMMETRYAES_CIPHERKEY)->GetWindowText(cstrCipherKey);
	bool bRet = ConvertCStringToData(cstrCipherKey, pKeyOut, KeyOutLen, pKeyLen);
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT128)))->GetCheck()) {
		if (*pKeyLen > 0x10) {
			*pKeyLen = 0x10;
		}
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT192)))->GetCheck()) {
		if (*pKeyLen > 0x18) {
			*pKeyLen = 0x18;
		}
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT256)))->GetCheck()) {
		if (*pKeyLen > 0x20) {
			*pKeyLen = 0x20;
		}
	}
	else {

	}
	return bRet;
}

bool  
SymmetryAESDlg::GetInitialValue(char* pKeyOut, long KeyOutLen, long* pKeyLen)
{
	CString cstrInitialValue;
	GetDlgItem(IDC_EDIT_SYMMETRYAES_INITIALVALUE)->GetWindowText(cstrInitialValue);
	return ConvertCStringToData(cstrInitialValue, pKeyOut, KeyOutLen, pKeyLen);
}

void 
SymmetryAESDlg::UpdateCtrl()
{
	UpdateMixCtrl();
}

BOOL 
SymmetryAESDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)) return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void 
SymmetryAESDlg::OnInitCtrl()
{
	GetDlgItem(IDC_STATIC_SYMMETRYAES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALVECTORVALUE);

	strncpy(m_chKey, _T("mengxiangEncryptDecrypt"), STATIC_CONST_LONG_KEYLENGTHMAX-1);
	m_lKeyLength = strlen(m_chKey);
	strncpy(m_chInitValue, _T("mengxiangEncryp"), STATIC_CONST_LONG_INITVALUELENGTHMAX-1);
	m_lInitValueLength = strlen(m_chInitValue);

	((CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYAES_INITIALVALUE))->SetLimitText(STATIC_CONST_LONG_INITVALUELENGTHMAX-1);
	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_TEXT)))->SetCheck(BST_CHECKED);	//默认使用文本

	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT256)))->SetCheck(BST_CHECKED);		//仅一种AES加解密
	((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESECB)))->SetCheck(BST_CHECKED);	//默认使用ECB模式
}

void 
SymmetryAESDlg::UpdateMixCtrl()
{
	UpdateMixCtrlValue();
	UpdateMixCtrlEnable();
}

void 
SymmetryAESDlg::UpdateMixCtrlValue()
{
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESCTR)))->GetCheck()) {
		GetDlgItem(IDC_STATIC_SYMMETRYAES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALCOUNTVALUE);	
	}
	else {
		GetDlgItem(IDC_STATIC_SYMMETRYAES_INITIALVALUE)->SetWindowText(STATIC_CONST_CSTR_INITIALVECTORVALUE);
	}

	//AES加密模式支持16 24 32个字符
	long lKeyLen = 0x00;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT128)))->GetCheck()) {
		lKeyLen = 16;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT192)))->GetCheck()) {
		lKeyLen = 24;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BIT256)))->GetCheck()) {
		lKeyLen = STATIC_CONST_LONG_KEYLENGTHMAX-1;
	}
	
	long lInitValueLen = STATIC_CONST_LONG_INITVALUELENGTHMAX-1;

	//刷新
	CString cstrCipherKey;
	CString cstrInitValue;
	int bufLen = 0x00;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_TEXT)))->GetCheck()) {
		cstrCipherKey = m_chKey;
		cstrInitValue = m_chInitValue;
		bufLen = 0x01;
	}
	else {
		int shift = 0x00;
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_HEX)))->GetCheck()) {
			shift = 0x10;
			bufLen = 0x02;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_OCTAL)))->GetCheck()) {
			shift = 0x08;
			bufLen = 0x03;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BINARTY4)))->GetCheck()) {
			shift = 0x04;
			bufLen = 0x04;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BINARY)))->GetCheck()) {
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

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYAES_CIPHERKEY);
	pEdit->SetLimitText(lKeyLen);
	pEdit->SetWindowText(cstrCipherKey.Left(lKeyLen));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SYMMETRYAES_INITIALVALUE);
	pEdit->SetLimitText(lInitValueLen);
	pEdit->SetWindowText(cstrInitValue.Left(lInitValueLen));
}

void 
SymmetryAESDlg::UpdateMixCtrlEnable()
{	
	bool bECB = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_AESECB)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_SYMMETRYAES_INITIALVALUE)->EnableWindow(!bECB);
}

bool 
SymmetryAESDlg::ConvertCStringToData(CString cstrDataIn, char* pDataOut, long DataOutLen, long* pDataLen)
{
	if (NULL == pDataOut || NULL == pDataLen) {
		return false;
	}
	memset(pDataOut, 0x00, DataOutLen);

	char* pchCipherKey = cstrDataIn.GetBuffer(cstrDataIn.GetLength());
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_TEXT)))->GetCheck()) {
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
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_HEX)))->GetCheck()) {
			buffLen = 0x02;
			shift = 0x10;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_OCTAL)))->GetCheck()) {
			buffLen = 0x03;
			shift = 0x08;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BINARTY4)))->GetCheck()) {
			buffLen = 0x04;
			shift = 0x04;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_SYMMETRYAES_BINARY)))->GetCheck()) {
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

IMPLEMENT_DYNAMIC(SymmetryAESDlg, CDialog)

SymmetryAESDlg::SymmetryAESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SymmetryAESDlg::IDD, pParent),
	m_lKeyLength(0x00),
	m_lInitValueLength(0x00)
{
	memset(m_chKey, 0x00, sizeof(char)*STATIC_CONST_LONG_KEYLENGTHMAX);
	memset(m_chInitValue, 0x00, sizeof(char)*STATIC_CONST_LONG_INITVALUELENGTHMAX);
}

SymmetryAESDlg::~SymmetryAESDlg()
{
}

void SymmetryAESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SymmetryAESDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_TEXT, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesText)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_HEX, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesHex)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_OCTAL, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesOctal)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_BINARTY4, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesBinarty4)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_BINARY, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesBinary)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_AESECB, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesecb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_AESCBC, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesAescbc)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_AESCFB, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesAescfb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_AESOFB, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesofb)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_AESCTR, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesctr)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_BIT128, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit128)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_BIT192, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit192)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRYAES_BIT256, &SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit256)
	ON_EN_KILLFOCUS(IDC_EDIT_SYMMETRYAES_INITIALVALUE, &SymmetryAESDlg::OnEnKillfocusEditSymmetryaesInitialvalue)
	ON_EN_KILLFOCUS(IDC_EDIT_SYMMETRYAES_CIPHERKEY, &SymmetryAESDlg::OnEnKillfocusEditSymmetryaesCipherkey)
END_MESSAGE_MAP()


// SymmetryAESDlg message handlers

BOOL SymmetryAESDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnInitCtrl();
	UpdateCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesText()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesHex()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesOctal()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesBinarty4()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesBinary()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesecb()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesAescbc()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesAescfb()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesofb()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesAesctr()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit128()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit192()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnBnClickedRadioSymmetryaesbit256()
{
	// TODO: Add your control notification handler code here
	UpdateCtrl();
}

void SymmetryAESDlg::OnEnKillfocusEditSymmetryaesInitialvalue()
{
	// TODO: Add your control notification handler code here
	CString cstrInitialValue;
	GetDlgItem(IDC_EDIT_SYMMETRYAES_INITIALVALUE)->GetWindowText(cstrInitialValue);
	ConvertCStringToData(cstrInitialValue, m_chInitValue, STATIC_CONST_LONG_INITVALUELENGTHMAX, &m_lInitValueLength);
}

void SymmetryAESDlg::OnEnKillfocusEditSymmetryaesCipherkey()
{
	// TODO: Add your control notification handler code here
	CString cstrCipherKey;
	GetDlgItem(IDC_EDIT_SYMMETRYAES_CIPHERKEY)->GetWindowText(cstrCipherKey);
	ConvertCStringToData(cstrCipherKey, m_chKey, STATIC_CONST_LONG_KEYLENGTHMAX, &m_lKeyLength);
}
