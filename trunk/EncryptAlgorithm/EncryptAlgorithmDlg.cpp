
// EncryptAlgorithmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "EncryptAlgorithmDlg.h"
#include "SymmetryDlg.h"
#include <string>

#ifndef ENCRYPTIF_H
#include "EncryptIF.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedUrlAboutboxLink();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedUrlAboutboxBloglink();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_URL_ABOUTBOX_LINK, &CAboutDlg::OnStnClickedUrlAboutboxLink)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_URL_ABOUTBOX_BLOGLINK, &CAboutDlg::OnStnClickedUrlAboutboxBloglink)
END_MESSAGE_MAP()

void CAboutDlg::OnStnClickedUrlAboutboxLink()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://www.mxfz.net",NULL,NULL,0);
}

void CAboutDlg::OnStnClickedUrlAboutboxBloglink()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://blog.csdn.net/xiaoting451292510",NULL,NULL,0);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
#ifndef IDC_HAND            
	MAKEINTRESOURCE(32649)
#endif
	::SetClassLong(GetDlgItem(IDC_URL_ABOUTBOX_LINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HAND));
	::SetClassLong(GetDlgItem(IDC_URL_ABOUTBOX_BLOGLINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HAND));
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	::SetClassLong(GetDlgItem(IDC_URL_ABOUTBOX_LINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
	::SetClassLong(GetDlgItem(IDC_URL_ABOUTBOX_BLOGLINK)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_URL_ABOUTBOX_LINK
		|| pWnd->GetDlgCtrlID()==IDC_URL_ABOUTBOX_BLOGLINK)
		pDC->SetTextColor(RGB(0,0,255));

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

// CEncryptAlgorithmDlg dialog
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_ENCRYPTARROW = _T("=>");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_DECRYPTARROW = _T("<=");

char digits[]= {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
std::string 
DataToUnsignedString(const char* dataIn, long dataInLen, int shift)
{
	std::string str;
	if (NULL == dataIn || 0x00 == dataInLen) {
		return str;
	}
	char  buf[8] = {0x00};
	char* pBuf = NULL;
	int bufLen = 0x00;
	int radix = 0x00;
	int shiftTemp = shift;
	while (shiftTemp=shiftTemp>>1)
		radix++;
	switch(shift) {
		case 16:
			bufLen = 0x02;
			break;
		case 8:
			bufLen = 0x03;
			break;
		case 4:
			bufLen = 0x04;
			break;
		case 2:
			bufLen = 0x08;
			break;
		default:
			break;
	}
	int charPos = 0x00;
	int mask = shift - 1;
	unsigned char jugde = 0x00;

	for (long lCnt=0x00; lCnt<dataInLen; lCnt++) {
		memset(buf, 0x00, sizeof(char)*8);
		pBuf = buf;
		charPos = 8;
		jugde = (unsigned char)dataIn[lCnt];
		do {
			pBuf[--charPos] = digits[jugde&mask];
			jugde = jugde>>radix;
		}while(0x00 != bufLen-(8-charPos));

		int strLen = 8 - charPos;
		pBuf = pBuf+charPos;
		while(strLen) {
			str.push_back(*pBuf);
			pBuf++;
			strLen--;
		}	
	}
	return str;
}

bool 
UnsignedStringToData(const char* dataIn, int shift, char* dataOut, long dataOutLen)
{
	// 将字符串转换成长整数，base为基数/进制
	// 如果转换成功，*endptr指向s; 否则*endptr指向第一个非法字符
	//long strtol(const char*s, char** endptr, int base);
	if (NULL == dataIn || NULL == dataOut) {
		return false;
	}
	long dataInLen = strlen(dataIn);
	int bufLen = 0x00;
	switch(shift) {
		case 16:
			bufLen = 0x02;
			break;
		case 8:
			bufLen = 0x03;
			break;
		case 4:
			bufLen = 0x04;
			break;
		case 2:
			bufLen = 0x08;
			break;
		default:
			break;
	}
	int dataOutNeed = dataInLen / bufLen;
	dataOutNeed += 0x00==dataInLen%bufLen?0x00:0x01;
	if (0x00 == dataInLen || dataOutNeed != dataOutLen) {
		return false;
	}
	char  buf[9] = {0x00};					//0xFF 11111111 一共八位字符
	char* endptr =  NULL;
	int dataOutCnt = 0x00;
	const char* pdataIn = dataIn;
	if (0x00 != dataInLen % bufLen) {
		memset(buf, 0x00, sizeof(char)*8);
		memcpy(buf, pdataIn, dataInLen%bufLen);
		pdataIn += dataInLen%bufLen;
		dataOut[dataOutCnt++] = (char)strtol(buf, &endptr, shift);
	}
	while('\0'!= (*pdataIn) && 0x00 > (pdataIn-dataIn-dataInLen) && dataOutCnt<=dataOutLen) {
		memset(buf, 0x00, sizeof(char)*8);
		memcpy(buf, pdataIn, bufLen);
		pdataIn += bufLen;
		dataOut[dataOutCnt++] = (char)strtol(buf, &endptr, shift);
	}
	return true;
}

void 
CEncryptAlgorithmDlg::UpdateCtrl()
{
	UpdateMixCtrlEnable();
}

BOOL 
CEncryptAlgorithmDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)) return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void 
CEncryptAlgorithmDlg::OnInitCtrl()
{
	((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->SetCheck(BST_CHECKED);				//默认使用文体加密

	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText("");								 
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//不可见

	((CButton* )(GetDlgItem(IDC_RADIO_MAIN_HEX)))->SetCheck(BST_CHECKED);						//默认使用十六进制
	((CButton* )(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->SetCheck(BST_CHECKED);			//默认使用保留源文件
	((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->SetCheck(BST_CHECKED);			//默认使用单一文件加密
	
	//Init TabControl
	CTabCtrl* pCTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MAIN_ENCRYPTALGORITHM);
	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY, _T("对称"));
	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY, _T("非对称"));
	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_HASH, _T("Hash"));

	CRect rect;
	m_tab.GetClientRect(&rect);//获得TAB控件的坐标
	//定位选项卡页的位置，这里可以根据情况自己调节偏移量
	rect.top +=21;
	rect.bottom -=1;
	rect.left +=1;
	rect.right -=1;
	//创建子页面
	if (NULL != m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY]) {
		m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY]->Create(IDD_DIALOG_SYMMETRY,GetDlgItem(IDC_TAB_MAIN_ENCRYPTALGORITHM));
	}

	for (int iCnt=0x00; iCnt<MAIN_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		//将子页面移动到指定的位置
		if (NULL != m_pTabDlg[iCnt]) {
			m_pTabDlg[iCnt]->MoveWindow(&rect);
			//隐藏子页面
			m_pTabDlg[iCnt]->ShowWindow(SW_HIDE);
		}
	}
	//显示当第一个子页面
	m_pTabDlg[0]->ShowWindow(SW_SHOW);
	m_eCurSelTab = static_cast<MAIN_TABLE_CONTROL_ENCRYPT_TYPE>(0x00);
}

void 
CEncryptAlgorithmDlg::UpdateMixCtrlEnable()
{
	bool bEncryptText = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()?true:false;
	
	GetDlgItem(IDC_BUTTON_MAIN_CLEARPLAINTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_BUTTON_MAIN_CLEARCIPHERTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_BUTTON_MAIN_CLEARALLTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_HEX)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_BINARY)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_OCTAL)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->EnableWindow(bEncryptText);

	bool bEncryptFile = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_REPLACESOURCEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)->EnableWindow(bEncryptFile);
	
	bool bEncryptSingleFile = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->EnableWindow(bEncryptFile&&bEncryptSingleFile);
	GetDlgItem(IDC_STATIC_MAIN_SOURCEFILE)->EnableWindow(bEncryptFile&&bEncryptSingleFile);

	bool bEncryptFolder = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->EnableWindow(bEncryptFile&&bEncryptFolder);
	GetDlgItem(IDC_STATIC_MAIN_SOURCEFOLDER)->EnableWindow(bEncryptFile&&bEncryptFolder);

	bool bKeepSourceFile = BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->EnableWindow(bEncryptFile&&bKeepSourceFile);
	GetDlgItem(IDC_STATIC_MAIN_OUTPUTFOLDER)->EnableWindow(bEncryptFile&&bKeepSourceFile);
}

void 
CEncryptAlgorithmDlg::CipherTextDisplay()
{
	if (0x00 == m_lDataOutLen || NULL == m_pchDataOut) {
		return;
	}
	char* test1 = "BCC13D18CA46463";
	char* test2 = "13314023321214244144143";
	char* test3 = "233030010331012030221012101203";
	char* test4 = "101111001100000100111101000110001100101001000110010001100011";
	
	long ltest1 = strlen(test1)/2;
	ltest1 += 0x00==strlen(test1)%2?0x00:0x01;
	char* ptest1 = new char[ltest1];
	long ltest2 = strlen(test2)/3;
	ltest2 += 0x00==strlen(test2)%3?0x00:0x01;
	char* ptest2 = new char[ltest2];
	long ltest3 = strlen(test3)/4;
	ltest3 += 0x00==strlen(test3)%4?0x00:0x01;
	char* ptest3 = new char[ltest3];
	long ltest4 = strlen(test4)/8;
	ltest4 += 0x00==strlen(test4)%8?0x00:0x01;
	char* ptest4 = new char[ltest4];
			
	bool test;
	test = UnsignedStringToData(test1, 16, ptest1, ltest1);
	test = UnsignedStringToData(test2, 8, ptest2, ltest2);
	test = UnsignedStringToData(test3, 4, ptest3, ltest3);
	test = UnsignedStringToData(test4, 2, ptest4, ltest4);

	std::string strDataOut;
	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_HEX)))->GetCheck()) {
		strDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 16);	//十六进制
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_OCTAL)))->GetCheck()) {
		strDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 8);	//八进制
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_BINARY4)))->GetCheck()) {
		strDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 4);	//四进制
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_BINARY)))->GetCheck()) {
		strDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 2);	//二进制
	}
	else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)))->GetCheck()) {
		strDataOut = m_pchDataOut;
	}
	else {
		;
	}

	GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->SetWindowText(strDataOut.c_str());	
}

bool 
CEncryptAlgorithmDlg::OnEncryptDecrypt(bool bEncryp)
{	
	long dataInLen = 0x00;
	char *dataIn = NULL;
	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()) {
		//字符串加解密
		CString cstrDataIn;
		if (true == bEncryp) {
			//字符串加密
			GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->GetWindowText(cstrDataIn);
			dataInLen = cstrDataIn.GetLength()+1;
			dataIn = new char[dataInLen];
			memset(dataIn, 0x00, sizeof(char)*(dataInLen));
			memcpy(dataIn, cstrDataIn.GetBuffer(cstrDataIn.GetLength()), dataInLen-1);
		}
		else {
			//字符串解密
			GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->GetWindowText(cstrDataIn);
			int shift = 0x00;
			int buffLen = 0x00;
			if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_HEX)))->GetCheck()) {
				shift = 0x10;	//十六进制
				buffLen = 0x02;
			}
			else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_OCTAL)))->GetCheck()) {
				shift = 0x08;	//八进制
				buffLen = 0x03;
			}
			else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_BINARY4)))->GetCheck()) {
				shift = 0x04;	//四进制
				buffLen = 0x04;
			}
			else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_BINARY)))->GetCheck()) {
				shift = 0x02;	//二进制
				buffLen = 0x08;
			}
			else if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)))->GetCheck()) {
				MessageBox(_T("使用原始字符解密可能会失败!"));
				if (NULL != dataIn) {
					delete[] dataIn;
					dataIn = NULL;
				}
				return false;
			}
			else {
				;
			}
			dataInLen = cstrDataIn.GetLength()/buffLen;
			dataInLen += 0x00==cstrDataIn.GetLength()%buffLen?0x00:0x01;
			dataIn = new char[dataInLen];
			memset(dataIn, 0x00, sizeof(char)*(dataInLen));
			if (false == UnsignedStringToData(cstrDataIn.GetBuffer(cstrDataIn.GetLength()), shift, dataIn, dataInLen)) {
				CString cstrMsg;
				cstrMsg.Format(_T("%ld进制字符串转换成数据失败!"));
				MessageBox(cstrMsg);
				if (NULL != dataIn) {
					delete[] dataIn;
					dataIn = NULL;
				}
				return false;
			}
		}
	}

	char *dataOut = NULL;
	long dataOutLen = 0x00;
	bool bSuccess = false;

	dataOutLen = (dataInLen/8+(true==bEncryp?1:0))*8;
	dataOut = new char[dataOutLen];
	memset(dataOut, 0x00, sizeof(char)*(dataOutLen));

	SetEncryptDecrypt();
	//加解密
	bSuccess = true==bEncryp?EncryptIF::Instance()->Encrypt(dataIn, dataInLen, dataOut):EncryptIF::Instance()->Decrypt(dataIn, dataInLen, dataOut);

	if (false == bSuccess) {
		true==bEncryp?MessageBox(_T("加密失败!")):MessageBox(_T("解密失败!"));
		if (NULL != dataIn) {
			delete[] dataIn;
			dataIn = NULL;
		}
		return false;
	}

	if (BST_CHECKED == ((CButton* )(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()) {
		//字符串加解密
		if (true == bEncryp) {
			//字符串加密
			if (NULL != m_pchDataOut) {
				delete[] m_pchDataOut;
				m_pchDataOut = NULL;
				m_lDataOutLen = 0x00;
			}
			m_lDataOutLen = dataOutLen;
			m_pchDataOut = new char[m_lDataOutLen];
			memset(m_pchDataOut, 0x00, sizeof(char)*m_lDataOutLen);
			memcpy(m_pchDataOut, dataOut, m_lDataOutLen);

			GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText(STATIC_CONST_CSTR_ENCRYPTARROW);								 
			GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_SHOW);

			//显示密文
			CipherTextDisplay();
		}
		else {
			//字符串解密
			GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText(STATIC_CONST_CSTR_DECRYPTARROW);								 
			GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_SHOW);

			//显示明文
			GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->SetWindowText(dataOut);
		}
	}
	if (NULL != dataIn) {
		delete[] dataIn;
		dataIn = NULL;
	}
	if (NULL != dataOut) {
		delete[] dataOut;
		dataOut = NULL;
	}
	return bSuccess;
}

void 
CEncryptAlgorithmDlg::SetEncryptDecrypt()
{
	const CDialog* pCDialog = NULL;
	switch(m_eCurSelTab) {
		case MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY:
			EncryptIF::Instance()->SetEncrypt(EncryptIF::ENCRYPT_SYMMETRY);
			if (NULL != m_pTabDlg[m_eCurSelTab]) {
				SymmetryDlg* pSymmetryDlg = dynamic_cast<SymmetryDlg*>(m_pTabDlg[m_eCurSelTab]);
				pCDialog = pSymmetryDlg->GetAlgorithmSymmetryDlg();
				SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE eSymmetryEncrypt = pSymmetryDlg->GetEncrypt();
				switch(eSymmetryEncrypt) {
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_DES:
						EncryptIF::Instance()->SetEncryptSymmetry(EncryptIF::ENCRYPTSYMMETRY_DES);
						if (NULL != pCDialog) {
							SymmetryDESDlg* pSymmetryDESDlg = dynamic_cast<SymmetryDESDlg*>(const_cast<CDialog*>(pCDialog));
							EncryptIF::Instance()->SetSymmetryKey(pSymmetryDESDlg->GetCipherKey());
							EncryptIF::Instance()->SetSymmetryDesInitValue(pSymmetryDESDlg->GetInitialValue()); 
							EncryptIF::Instance()->SetEncryptSymmetryDES(EncryptIF::ENCRYPTSYMMETRYDES_DES);
							EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_ECB);
							SetDecryptDES(pSymmetryDESDlg->GetDESType(), pSymmetryDESDlg->GetDESModeType());
						}
						break;
					default:
						break;
				}
			}
			break;
		case MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY:
			break;
		case MAIN_TABLE_CONTROL_ENCRYPT_HASH:
			break;
		default:
			break;

	}
}

void
CEncryptAlgorithmDlg::SetDecryptDES(SymmetryDESDlg::SYMMETRY_TABLE_DES_TYPE eDES, SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_TYPE eDESMode)
{
	switch(eDES) {
		case SymmetryDESDlg::SYMMETRY_TABLE_DES_DES:
			EncryptIF::Instance()->SetEncryptSymmetryDES(EncryptIF::ENCRYPTSYMMETRYDES_DES);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DES_3DES:
			EncryptIF::Instance()->SetEncryptSymmetryDES(EncryptIF::ENCRYPTSYMMETRYDES_3DES);
			break;
		default:
			EncryptIF::Instance()->SetEncryptSymmetryDES(EncryptIF::ENCRYPTSYMMETRYDES_INVALID);
			break;
	}
	switch(eDESMode) {
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_ECB:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_ECB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CBC:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_CBC);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CFB:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_CFB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_OFB:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_OFB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CTR:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_CTR);
			break;
		default:
			EncryptIF::Instance()->SetEncryptSymmetryDESMode(EncryptIF::ENCRYPTSYMMETRYDESMODE_INVALID);
			break;
	}
}


CEncryptAlgorithmDlg::CEncryptAlgorithmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEncryptAlgorithmDlg::IDD, pParent),
	m_pchDataOut(NULL),
	m_lDataOutLen(0x00),
	m_eCurSelTab(MAIN_TABLE_CONTROL_ENCRYPT_INVALID)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int iCnt=0x00; iCnt<MAIN_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		m_pTabDlg[iCnt] = NULL;
	}
	
	if (NULL == m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY]) {
		m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY] = new SymmetryDlg;
	}
}

CEncryptAlgorithmDlg::~CEncryptAlgorithmDlg()
{
	for (int iCnt=0x00; iCnt<MAIN_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		if (NULL != m_pTabDlg[iCnt]) {
			delete m_pTabDlg[iCnt];
			m_pTabDlg[iCnt] = NULL;
		}
	}

	if (NULL != m_pchDataOut) {
		delete[] m_pchDataOut;
		m_pchDataOut = NULL;
		m_lDataOutLen = 0x00;
	}	
}

void CEncryptAlgorithmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN_ENCRYPTALGORITHM, m_tab);
}

BEGIN_MESSAGE_MAP(CEncryptAlgorithmDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_MAIN_ABOUT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainAbout)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CLEARPLAINTEXT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainClearplaintext)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CLEARCIPHERTEXT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainClearciphertext)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CLEARALLTEXT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainClearalltext)
	ON_BN_CLICKED(IDC_RADIO_MAIN_HEX, &CEncryptAlgorithmDlg::OnBnClickedRadioMainHex)
	ON_BN_CLICKED(IDC_RADIO_MAIN_BINARY, &CEncryptAlgorithmDlg::OnBnClickedRadioMainBinary)
	ON_BN_CLICKED(IDC_RADIO_MAIN_ORIGINALCHARACTER, &CEncryptAlgorithmDlg::OnBnClickedRadioMainOriginalcharacter)
	ON_BN_CLICKED(IDC_RADIO_MAIN_KEEPSOURCEFILE, &CEncryptAlgorithmDlg::OnBnClickedRadioMainKeepsourcefile)
	ON_BN_CLICKED(IDC_RADIO_MAIN_REPLACESOURCEFILE, &CEncryptAlgorithmDlg::OnBnClickedRadioMainReplacesourcefile)
	ON_BN_CLICKED(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE, &CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptsinglefile)
	ON_BN_CLICKED(IDC_RADIO_MAIN_ENCRYPTFOLDER, &CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptfolder)
	ON_STN_CLICKED(IDC_STATIC_MAIN_SOURCEFILE, &CEncryptAlgorithmDlg::OnStnClickedStaticMainSourcefile)
	ON_STN_CLICKED(IDC_STATIC_MAIN_OUTPUTFOLDER, &CEncryptAlgorithmDlg::OnStnClickedStaticMainOutputfolder)
	ON_STN_CLICKED(IDC_STATIC_MAIN_SOURCEFOLDER, &CEncryptAlgorithmDlg::OnStnClickedStaticMainSourcefolder)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_ENCRYPT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_DECRYPT, &CEncryptAlgorithmDlg::OnBnClickedButtonMainDecrypt)
	ON_BN_CLICKED(IDC_RADIO_MAIN_ENCRYPTFILE, &CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptfile)
	ON_BN_CLICKED(IDC_RADIO_MAIN_ENCRYPTTEXT, &CEncryptAlgorithmDlg::OnBnClickedRadioMainEncrypttext)
	ON_BN_CLICKED(IDC_RADIO_MAIN_OCTAL, &CEncryptAlgorithmDlg::OnBnClickedRadioMainOctal)
	ON_BN_CLICKED(IDC_RADIO_MAIN_BINARY4, &CEncryptAlgorithmDlg::OnBnClickedRadioMainBinary4)
END_MESSAGE_MAP()


// CEncryptAlgorithmDlg message handlers

BOOL CEncryptAlgorithmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnInitCtrl();
	UpdateCtrl();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEncryptAlgorithmDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEncryptAlgorithmDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEncryptAlgorithmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CEncryptAlgorithmDlg::OnBnClickedButtonMainAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;	
	dlgAbout.DoModal();
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainClearplaintext()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//不可见
	
	GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->SetWindowText("");
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainClearciphertext()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//不可见

	GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->SetWindowText("");

	if (NULL != m_pchDataOut) {
		delete[] m_pchDataOut;
		m_pchDataOut = NULL;
		m_lDataOutLen = 0x00;
	}	
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainClearalltext()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButtonMainClearplaintext();
	OnBnClickedButtonMainClearciphertext();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainHex()
{
	// TODO: Add your control notification handler code here
	CipherTextDisplay();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainBinary()
{
	// TODO: Add your control notification handler code here
	CipherTextDisplay();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainOriginalcharacter()
{
	// TODO: Add your control notification handler code here
	CipherTextDisplay();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainKeepsourcefile()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainReplacesourcefile()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptsinglefile()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptfolder()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnStnClickedStaticMainSourcefile()
{
	// TODO: Add your control notification handler code here
}

void CEncryptAlgorithmDlg::OnStnClickedStaticMainOutputfolder()
{
	// TODO: Add your control notification handler code here
}

void CEncryptAlgorithmDlg::OnStnClickedStaticMainSourcefolder()
{
	// TODO: Add your control notification handler code here
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainEncrypt()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_MAIN_ENCRYPT)->EnableWindow(FALSE);
	OnBnClickedButtonMainClearciphertext();
	OnEncryptDecrypt(true);
	GetDlgItem(IDC_BUTTON_MAIN_ENCRYPT)->EnableWindow(TRUE);
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainDecrypt()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_MAIN_DECRYPT)->EnableWindow(FALSE);
	OnBnClickedButtonMainClearplaintext();
	OnEncryptDecrypt(false);
	GetDlgItem(IDC_BUTTON_MAIN_DECRYPT)->EnableWindow(TRUE);
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainEncryptfile()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainEncrypttext()
{
	// TODO: Add your control notification handler code here
	UpdateMixCtrlEnable();
}

void CEncryptAlgorithmDlg::OnBnClickedRadioMainOctal()
{
	// TODO: Add your control notification handler code here
	CipherTextDisplay();
}
void CEncryptAlgorithmDlg::OnBnClickedRadioMainBinary4()
{
	// TODO: Add your control notification handler code here
	CipherTextDisplay();
}
