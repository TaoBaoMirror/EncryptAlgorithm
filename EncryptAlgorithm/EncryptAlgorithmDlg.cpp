
// EncryptAlgorithmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "EncryptAlgorithmDlg.h"
#include "SymmetryDlg.h"
#include "SymmetryAESDlg.h"
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
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK = _T("encrypt|decrypt");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_ENCRYPTFOLDER = _T("encrypt\\");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_DECRYPTFOLDER = _T("decrypt\\");
const long CEncryptAlgorithmDlg::STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH = 0xFF;		//��չ256��byte

/************************************************************************/
/*    �ַ���	mengxiang
/*    MD5 16λ Сд	699fcb5e21845872
/*    MD5 16λ ��д	699FCB5E21845872
/*    MD5 32λ Сд	9d439b82699fcb5e218458726b69bec1
/*    MD5 32λ ��д	9D439B82699FCB5E218458726B69BEC1                                                                  */
/************************************************************************/
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_MD5TEXT = _T("mengxiang");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_MD5LOWER16 = _T("699fcb5e21845872");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_MD5UPER16 = _T("699FCB5E21845872");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_MD5LOWER32 = _T("9d439b82699fcb5e218458726b69bec1");
const CString CEncryptAlgorithmDlg::STATIC_CONST_CSTR_MD5UPER32 = _T("9D439B82699FCB5E218458726B69BEC1");

char digits[]= {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
CString
DataToUnsignedString(const char* dataIn, long dataInLen, int shift)
{
	CString cstr;
	if (NULL == dataIn || 0x00 == dataInLen) {
		return cstr;
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
			cstr.AppendChar(*pBuf);
			pBuf++;
			strLen--;
		}	
	}
	return cstr;
}

bool 
UnsignedStringToData(const char* dataIn, int shift, char* dataOut, long dataOutLen)
{
	// ���ַ���ת���ɳ�������baseΪ����/����
	// ���ת���ɹ���*endptrָ��s; ����*endptrָ���һ���Ƿ��ַ�
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
	char  buf[9] = {0x00};					//0xFF 11111111 һ����λ�ַ�
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

void Split(char *src, const char *separator, char **dest, int *num)
{
	char *pNext;
	int count = 0;

	if (src == NULL || strlen(src) == 0) return;
	if (separator == NULL || strlen(separator) == 0) return; 

	pNext = strtok(src,separator);

	while(pNext != NULL)
	{
		*dest++ = pNext;
		++count;
		pNext = strtok(NULL,separator);
	}

	*num = count;
}

void Split(const string& src, const string& separator, vector<string>* dest)
{
	if (NULL == dest) {
		return;
	}
	dest->clear();
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest->push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest->push_back(substring);
}

void Split(const CString& src, const CString& separator, vector<CString>* dest)	//C++
{
	if (NULL == dest) {
		return;
	}
	dest->clear();
	vector<string> vStrdest;
	Split(LPCSTR(src), LPCSTR(separator), &vStrdest);
	for (int iCnt=0x00; iCnt<(int)vStrdest.size(); iCnt++) {
		dest->push_back(vStrdest[iCnt].c_str());
	}
}


BOOL CreateMultiDirectory(const CString& szPath)
{
	//���Ŀ¼������
	vector<CString> vFolder;
	Split(szPath, _T("\\"), &vFolder);

	//�������Ŀ¼������,����ÿ��Ŀ¼
	CString cstrCreatePath;
	for (int iCnt=0x00; iCnt<(int)vFolder.size(); iCnt++) {
		cstrCreatePath += vFolder[iCnt] + _T("\\") ;
		if(FALSE == ::PathIsDirectory(cstrCreatePath)) {   
			if (FALSE == ::CreateDirectory(cstrCreatePath, NULL)) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM , LPARAM lpData)   
{
	if(uMsg == BFFM_INITIALIZED) {
		CTreeCtrl	treePath;
		HTREEITEM	hItemSel;
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		treePath.SubclassWindow(::GetDlgItem(hWnd, 0x3741));
		hItemSel	= treePath.GetSelectedItem();
		treePath.Expand(hItemSel, TVE_COLLAPSE);
		treePath.UnsubclassWindow();
	} 
	return 0;  
}
vector<CString> test;
void TraverseDir(CString& cstrDir, vector<CString>* pvFilePath)
{
	if (NULL == pvFilePath || true == cstrDir.IsEmpty()) {
		return;
	}
	CFileFind cFileFind;
	if (cstrDir.Right(1) != "\\") {
		cstrDir += "\\";
	}
	cstrDir += "*.*";
	BOOL bTraverse = cFileFind.FindFile(cstrDir);
	while (bTraverse) {
		bTraverse = cFileFind.FindNextFile();
		if (TRUE == cFileFind.IsDots()) {
			// skip . and .. files;
			continue;
		}
		if (TRUE == cFileFind.IsDirectory()) {
			TraverseDir(cFileFind.GetFilePath(), pvFilePath);
		}
		else {
			pvFilePath->push_back(cFileFind.GetFilePath());
		}
	}
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
	((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->SetCheck(BST_CHECKED);				//Ĭ��ʹ���������

	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText("");								 
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//���ɼ�

	((CButton*)(GetDlgItem(IDC_RADIO_MAIN_HEX)))->SetCheck(BST_CHECKED);						//Ĭ��ʹ��ʮ������
	((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->SetCheck(BST_CHECKED);			//Ĭ��ʹ�ñ���Դ�ļ�
	((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->SetCheck(BST_CHECKED);			//Ĭ��ʹ�õ�һ�ļ�����
	
	//Init TabControl
	CTabCtrl* pCTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MAIN_ENCRYPTALGORITHM);
	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY, _T("�Գ�"));
// 	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY, _T("�ǶԳ�"));
// 	pCTabCtrl->InsertItem(MAIN_TABLE_CONTROL_ENCRYPT_HASH, _T("Hash"));

	CRect rect;
	m_tab.GetClientRect(&rect);//���TAB�ؼ�������
	//��λѡ�ҳ��λ�ã�������Ը�������Լ�����ƫ����
	rect.top +=21;
	rect.bottom -=1;
	rect.left +=1;
	rect.right -=1;
	//������ҳ��
	if (NULL != m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY]) {
		m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY]->Create(IDD_DIALOG_SYMMETRY,GetDlgItem(IDC_TAB_MAIN_ENCRYPTALGORITHM));
	}

	for (int iCnt=0x00; iCnt<MAIN_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		//����ҳ���ƶ���ָ����λ��
		if (NULL != m_pTabDlg[iCnt]) {
			m_pTabDlg[iCnt]->MoveWindow(&rect);
			//������ҳ��
			m_pTabDlg[iCnt]->ShowWindow(SW_HIDE);
		}
	}
	//��ʾ����һ����ҳ��
	m_pTabDlg[0]->ShowWindow(SW_SHOW);
	m_eCurSelTab = static_cast<MAIN_TABLE_CONTROL_ENCRYPT_TYPE>(0x00);
}

void 
CEncryptAlgorithmDlg::UpdateMixCtrlEnable()
{
	bool bEncryptText = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()?true:false;
	
	GetDlgItem(IDC_BUTTON_MAIN_CLEARPLAINTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_BUTTON_MAIN_CLEARCIPHERTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_BUTTON_MAIN_CLEARALLTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_HEX)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_BINARY)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_OCTAL)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->EnableWindow(bEncryptText);
	GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->EnableWindow(bEncryptText);

	bool bEncryptFile = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_REPLACESOURCEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)->EnableWindow(bEncryptFile);
	GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)->EnableWindow(bEncryptFile);
	
	bool bEncryptSingleFile = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->EnableWindow(bEncryptFile&&bEncryptSingleFile);
	GetDlgItem(IDC_STATIC_MAIN_SOURCEFILE)->EnableWindow(bEncryptFile&&bEncryptSingleFile);

	bool bEncryptFolder = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->EnableWindow(bEncryptFile&&bEncryptFolder);
	GetDlgItem(IDC_STATIC_MAIN_SOURCEFOLDER)->EnableWindow(bEncryptFile&&bEncryptFolder);

	bool bKeepSourceFile = BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()?true:false;
	GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->EnableWindow(bEncryptFile&&bKeepSourceFile);
	GetDlgItem(IDC_STATIC_MAIN_OUTPUTFOLDER)->EnableWindow(bEncryptFile&&bKeepSourceFile);
}

void 
CEncryptAlgorithmDlg::CipherTextDisplay()
{
	if (0x00 == m_lDataOutLen || NULL == m_pchDataOut) {
		return;
	}
	CString cstrDataOut;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_HEX)))->GetCheck()) {
		cstrDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 16);	//ʮ������
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_OCTAL)))->GetCheck()) {
		cstrDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 8);	//�˽���
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_BINARY4)))->GetCheck()) {
		cstrDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 4);	//�Ľ���
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_BINARY)))->GetCheck()) {
		cstrDataOut = DataToUnsignedString(m_pchDataOut, m_lDataOutLen, 2);	//������
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)))->GetCheck()) {
		cstrDataOut = m_pchDataOut;
	}
	else {
		;
	}

	GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->SetWindowText(cstrDataOut);	
}

bool 
CEncryptAlgorithmDlg::OnEncryptDecrypt(bool bEncryp)
{	
	bool bSuccess = false;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()) {
		CString cstrDataText;
		true==bEncryp?GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->GetWindowText(cstrDataText):GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->GetWindowText(cstrDataText);
		bSuccess = OnEncryptDecryptText(bEncryp, cstrDataText);
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFILE)))->GetCheck()) {
		//�ļ��ӽ���
		vector<CString> vFileFailed;
		vector<CString> vEncryptFilePath;
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()) {
			CString cstrSourceFile;
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->GetWindowText(cstrSourceFile);
			Split(cstrSourceFile, _T("|"), &vEncryptFilePath);
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()) {
			CString cstrSourceFolder;
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->GetWindowText(cstrSourceFolder);
			test.clear();
			TraverseDir(cstrSourceFolder, &vEncryptFilePath);
		}
		else {
			;
		}
		for (int iCnt=0x00; iCnt<(int)vEncryptFilePath.size(); iCnt++) {
			if (false == OnEncryptDecryptFile(bEncryp, vEncryptFilePath[iCnt])) {
				//��¼ʧ�ܵ��ļ�
				vFileFailed.push_back(vEncryptFilePath[iCnt]);
			}
		}
		bSuccess = vFileFailed.empty();	//ʧ�ܼ�¼Ϊ�գ���ȫ���ɹ�
	}
	else {
		;
	}
	return bSuccess;
}

bool 
CEncryptAlgorithmDlg::OnEncryptDecryptText(bool bEncryp, CString cstrDataText)
{
	long dataInLen = 0x00;
	char *dataIn = NULL;
	//�ַ����ӽ���
	if (true == bEncryp) {
		//�ַ�������
		dataInLen = cstrDataText.GetLength()+1;
		dataIn = new char[dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH];
		memset(dataIn, 0x00, sizeof(char)*(dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH));
		strcpy(dataIn, cstrDataText.GetBuffer(cstrDataText.GetLength()));
	}
	else {
		//�ַ�������
		int shift = 0x00;
		int buffLen = 0x00;
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_HEX)))->GetCheck()) {
			shift = 0x10;	//ʮ������
			buffLen = 0x02;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_OCTAL)))->GetCheck()) {
			shift = 0x08;	//�˽���
			buffLen = 0x03;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_BINARY4)))->GetCheck()) {
			shift = 0x04;	//�Ľ���
			buffLen = 0x04;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_BINARY)))->GetCheck()) {
			shift = 0x02;	//������
			buffLen = 0x08;
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ORIGINALCHARACTER)))->GetCheck()) {
			MessageBox(_T("ʹ��ԭʼ�ַ����ܿ��ܻ�ʧ��!"));
			return false;
		}
		else {
			;
		}
		dataInLen = cstrDataText.GetLength()/buffLen;
		dataInLen += 0x00==cstrDataText.GetLength()%buffLen?0x00:0x01;
		dataIn = new char[dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH];
		memset(dataIn, 0x00, sizeof(char)*(dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH));
		UnsignedStringToData(cstrDataText.GetBuffer(cstrDataText.GetLength()), shift, dataIn, dataInLen);
	}
	char *dataOut = NULL;
	long dataOutLen = 0x00;
	bool bSuccess = false;

	const CDialog* pCDialog = NULL;
	switch(m_eCurSelTab) {
		case MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY:
			if (NULL != m_pTabDlg[m_eCurSelTab]) {
				SymmetryDlg* pSymmetryDlg = dynamic_cast<SymmetryDlg*>(m_pTabDlg[m_eCurSelTab]);
				pCDialog = pSymmetryDlg->GetAlgorithmSymmetryDlg();
				switch(pSymmetryDlg->GetEncrypt()) {
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_DES:
						dataOutLen = (dataInLen/8+(0==dataInLen%8?0:1))*8;		//�����㷨����������8λ���
						break;
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_AES:
						dataOutLen = (dataInLen/16+(0==dataInLen%16?0:1))*16;		//�����㷨����������8λ���
						break;
					default:
						break;
				}
			}
		case MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY:
			break;
		case MAIN_TABLE_CONTROL_ENCRYPT_HASH:
			break;
		default:
			break;
	}
	
	dataOut = new char[dataOutLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH];
	memset(dataOut, 0x00, sizeof(char)*(dataOutLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH));

	//���üӽ��ܲ���
	SetEncryptDecrypt();
	//�ӽ���
	bSuccess = true==bEncryp?EncryptIF::Instance()->Encrypt(dataIn, dataInLen, dataOut):EncryptIF::Instance()->Decrypt(dataIn, dataInLen, dataOut);

	if (false == bSuccess) {
		true==bEncryp?MessageBox(_T("����ʧ��!")):MessageBox(_T("����ʧ��!"));
		if (NULL != dataIn) {
			delete[] dataIn;
			dataIn = NULL;
		}

		if (NULL != dataOut) {
			delete[] dataOut;
			dataOut = NULL;
		}
		return false;
	}

	//�ӽ��ܳɹ�
	if (true == bEncryp) {
		//�ַ�������
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

		//��ʾ����
		CipherTextDisplay();
	}
	else {
		//�ַ�������
		GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText(STATIC_CONST_CSTR_DECRYPTARROW);								 
		GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_SHOW);

		//��ʾ����
		GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->SetWindowText(dataOut);
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

bool 
CEncryptAlgorithmDlg::OnEncryptDecryptFile(bool bEncryp, CString cstrDataFile)
{
	ULONGLONG ullDecrytFileLen = 0x00;
	long dataInLen = 0x00;
	char *dataIn = NULL;
	FileHeader sFileHeader;
	CString cstrDataFileName;
	static CFile s_file;		//ֻ��Ҫһ���ļ�����ʵ��
	static CFileException e;
	long lFilePath = cstrDataFile.GetLength();
	if (CFile::hFileNull != s_file.m_hFile) {
		s_file.Close();
	}
	/************************************************************************/
	/* 
	CFile::modeCreate �������ļ�,����ڴ���֮ǰ����ͬ���ļ�����,��������ļ�������,�ļ����ȱ����.
	CFile::modeNoTruncate  �����CFile::modeCreate����.����ʹ���ڴ������ļ�ǰ,������ͬ���ļ�����ֱ�Ӵ򿪸��ļ�,������������ļ�.
	CFile::modeRead  ��ֻ���ļ�,Ҳ�����޷����κ�����д����ļ�.
	CFile::modeReadWrite  �򿪿ɶ���д���ļ�.
	CFile::modeWrite  ��ֻ��д����ļ�.
	CFile::modeNoInherit  ��ֹ�ӽ��̼̳�ʹ�ô��ļ�
	CFile::modeDenyNone  ���ļ���,����ִ�г��򻹿����ٴ򿪴��ļ����Ҷ�д�ļ��е�����
	CFile::modeDenyRead  ���ļ���,����ִ�г�������ٴδ򿪴��ļ�,����ֻ�ܰ�����д���ļ�
	CFile::modeDenyWrite  ���ļ���,����ִ�г�������ٴδ򿪴��ļ�,����ֻ�ܶ�ȡ�ļ��е�����
	CFile::shareExclusive  ���ļ���,��ֹ����ִ�г����ٴδ򿪴��ļ�,����ֻ�ܰ�����д���ļ�
	CFile::typeText  ���ı��ļ���,Ҳ����CR/LF�����ַ���ᱻ�����CR�����ַ�
	CFile::typeBinary  �Զ�����ģʽ��
	*/
	/************************************************************************/
	if (FALSE == s_file.Open(cstrDataFile, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone, &e)) {	//�Զ��������ļ�д
		//���ļ�ʧ��
		CString cstrFormat;
		TCHAR szError[1024] = {0x00};
		e.GetErrorMessage(szError, 1024);
		cstrFormat.Format(_T("���ļ�ʧ��!\r\n�ļ���:%s\r\n�������:%d\r\n������Ϣ:%s"),e.m_strFileName, e.m_cause, szError);
		MessageBox(cstrFormat);
		return false;
	}
	cstrDataFileName = s_file.GetFileName();
	dataInLen = (long)s_file.GetLength() - (true==bEncryp?0x00:sizeof(FileHeader));
	if (0 > dataInLen) {
		MessageBox(_T("��ѡ����ȷ�ļ�! \r\n")+cstrDataFile);
		s_file.Close();
		return false;
	}
	
	if (false == bEncryp) {
		//���� ��ȡ�ļ�ͷ
		s_file.Read(&sFileHeader, sizeof(sFileHeader));
		if (0x00 != strcmp(sFileHeader.m_MD5Verify, STATIC_CONST_CSTR_MD5UPER32)) {
			//��֤δͨ��
			MessageBox(_T("��ѡ����ȷ�ļ����ļ�!\r\n")+cstrDataFile);
			s_file.Close();
			return false;
		}
		ullDecrytFileLen = sFileHeader.m_size;
	}

	dataIn = new char[dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH];
	memset(dataIn, 0x00, sizeof(char)*(dataInLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH));
	s_file.Read(dataIn, dataInLen);
	s_file.Close();	//�ر�Դ�����ļ��ļ�
	
	char *dataOut = NULL;
	long dataOutLen = 0x00;
	bool bSuccess = false;

	const CDialog* pCDialog = NULL;
	switch(m_eCurSelTab) {
		case MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY:
			if (NULL != m_pTabDlg[m_eCurSelTab]) {
				SymmetryDlg* pSymmetryDlg = dynamic_cast<SymmetryDlg*>(m_pTabDlg[m_eCurSelTab]);
				pCDialog = pSymmetryDlg->GetAlgorithmSymmetryDlg();
				switch(pSymmetryDlg->GetEncrypt()) {
		case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_DES:
			dataOutLen = (dataInLen/8+(0==dataInLen%8?0:1))*8;		//�����㷨����������8λ���
			break;
		case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_AES:
			dataOutLen = (dataInLen/16+(0==dataInLen%16?0:1))*16;		//�����㷨����������8λ���
			break;
		default:
			break;
				}
			}
		case MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY:
			break;
		case MAIN_TABLE_CONTROL_ENCRYPT_HASH:
			break;
		default:
			break;
	}

	dataOut = new char[dataOutLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH];
	memset(dataOut, 0x00, sizeof(char)*(dataOutLen+STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH));
	//���üӽ��ܲ���
	SetEncryptDecrypt();
	//�ӽ���
	bSuccess = true==bEncryp?EncryptIF::Instance()->Encrypt(dataIn, dataInLen, dataOut):EncryptIF::Instance()->Decrypt(dataIn, dataInLen, dataOut);
	if (NULL != dataIn) {
		delete[] dataIn;
		dataIn = NULL;
	}
	if (false == bSuccess) {
		true==bEncryp?MessageBox(_T("����ʧ��!")):MessageBox(_T("����ʧ��!"));

		if (NULL != dataOut) {
			delete[] dataOut;
			dataOut = NULL;
		}
		return false;
	}

	//�ӽ��ܳɹ�
	CString cstrOutputFolder;
	CString cstrOutputFile;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()) {
		GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->GetWindowText(cstrOutputFolder);
		if (cstrOutputFolder.Right(1) != "\\") {
			cstrOutputFolder += "\\";
		}
		int nPos = cstrOutputFolder.Find(STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK);
		if (-1 != nPos) {
			//ȥ��encrypt|decrypt��ʶ
			cstrOutputFolder = cstrOutputFolder.Left(nPos);
			cstrOutputFolder += true==bEncryp?STATIC_CONST_CSTR_ENCRYPTFOLDER:STATIC_CONST_CSTR_DECRYPTFOLDER;
		}
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()) {
			//�ļ��мӽ���
			CString cstrSourceFolder;
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->GetWindowText(cstrSourceFolder);
			if (cstrSourceFolder.Right(1) != "\\") {
				cstrSourceFolder += "\\";
			}
			CString cstrOutputFolderAdd = cstrDataFile.Left(cstrDataFile.Find(cstrDataFileName));
			cstrOutputFolderAdd = cstrOutputFolderAdd.Right(cstrOutputFolderAdd.GetLength()-cstrSourceFolder.GetLength());
			cstrOutputFolder += cstrOutputFolderAdd;
		}
		if (FALSE == CreateMultiDirectory(cstrOutputFolder)) {
			if (NULL != dataOut) {
				delete[] dataOut;
				dataOut = NULL;
			}
			return false;
		}
		cstrOutputFile = cstrOutputFolder;
		cstrOutputFile += cstrDataFileName;
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_REPLACESOURCEFILE)))->GetCheck()) {
		cstrOutputFile = cstrDataFile;
	}
	else {
		;
	}

	if (CFile::hFileNull != s_file.m_hFile) {
		//�ر�֮ǰ�ļ�
		s_file.Close();
	}
	if(FALSE == s_file.Open(cstrOutputFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::shareDenyWrite)) {
		//���ļ�ʧ��
		CString cstrFormat;
		TCHAR szError[1024] = {0x00};
		e.GetErrorMessage(szError, 1024);
		cstrFormat.Format(_T("���ļ�ʧ��!\r\n�ļ���:%s\r\n�������:%d\r\n������Ϣ:%s"),e.m_strFileName, e.m_cause, szError);
		MessageBox(cstrFormat);

		if (NULL != dataOut) {
			delete[] dataOut;
			dataOut = NULL;
		}
		return false;
	}
	
	if (true == bEncryp) {
		//���� �ռ������ļ�ͷ��Ϣ
		FileHeader sFileHeaderCollect = {0x00};
		CFileStatus status;
		CFile::GetStatus(cstrDataFile, status);
		CString cstrTime;
		strcpy(sFileHeaderCollect.m_MD5Verify, STATIC_CONST_CSTR_MD5UPER32);
		cstrTime = status.m_ctime.Format("%Y-%m-%d %H:%M:%S");
		strcpy(sFileHeaderCollect.m_ctime, (LPSTR)(LPCTSTR)cstrTime);
		cstrTime = status.m_mtime.Format("%Y-%m-%d %H:%M:%S");
		strcpy(sFileHeaderCollect.m_mtime, (LPSTR)(LPCTSTR)cstrTime);
		cstrTime = status.m_atime.Format("%Y-%m-%d %H:%M:%S");
		strcpy(sFileHeaderCollect.m_atime, (LPSTR)(LPCTSTR)cstrTime);
		sFileHeaderCollect.m_size = status.m_size;
		sFileHeaderCollect.m_attribute = status.m_attribute;
		sFileHeaderCollect._m_padding = status._m_padding;
		sFileHeaderCollect.m_size = status.m_size;
		sFileHeaderCollect.m_size = status.m_size;
		sFileHeaderCollect.m_size = status.m_size;
		CString cstrFileTitle = s_file.GetFileTitle();
		CString cstrFileName = s_file.GetFileName();
		CString cstrFileType;
		int nPos = cstrFileName.Find(".");
		if (-1 != nPos) {
			cstrFileType = cstrFileName.Right(cstrFileName.GetLength()-(nPos+1));
		}
		strcpy(sFileHeaderCollect.m_szFileTitle, (LPSTR)(LPCTSTR)cstrFileTitle);
		strcpy(sFileHeaderCollect.m_szFileType, (LPSTR)(LPCTSTR)cstrFileType);
		GetEncryptLevelFromCtrl(&sFileHeaderCollect.byEncryptLevel1, &sFileHeaderCollect.byEncryptLevel2, &sFileHeaderCollect.byEncryptLevel3, &sFileHeaderCollect.byEncryptLevel4);

		s_file.Write(&sFileHeaderCollect, sizeof(sFileHeaderCollect));	//д���ļ�ͷ
		s_file.Write(dataOut, dataOutLen);					//д������
	}
	else {
		//����
		s_file.Write(dataOut, (UINT)ullDecrytFileLen);					//д���������
		//�ر��ļ���ԭ�ļ�״̬
		if (CFile::hFileNull != s_file.m_hFile) {
			s_file.Close();
		}
		CFileStatus statusRestore;
		CFile::GetStatus(cstrOutputFile, statusRestore);
		int Y,M,D,h,m,s;
		sscanf(sFileHeader.m_ctime, "%d-%d-%d %d:%d:%d",&Y,&M,&D,&h,&m,&s);
		statusRestore.m_ctime = CTime(Y,M,D,h,m,s);
		sscanf(sFileHeader.m_mtime, "%d-%d-%d %d:%d:%d",&Y,&M,&D,&h,&m,&s);
		statusRestore.m_mtime = CTime(Y,M,D,h,m,s);
		sscanf(sFileHeader.m_atime, "%d-%d-%d %d:%d:%d",&Y,&M,&D,&h,&m,&s);
		statusRestore.m_atime = CTime(Y,M,D,h,m,s);
		statusRestore.m_size = sFileHeader.m_size;
		statusRestore.m_attribute = sFileHeader.m_attribute;
		statusRestore._m_padding = sFileHeader._m_padding;
		CFile::SetStatus(cstrOutputFile, statusRestore);
	}
	if (CFile::hFileNull != s_file.m_hFile) {
		//�ر�֮ǰ�ļ�
		s_file.Close();
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
							char chSymmetryKey[EncryptIF::STATIC_CONST_LONG_SYMMETRYKEYLENGTH] = {0x00};
							char chSymmetryDESInitValue[EncryptIF::STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH] = {0x00};
							long lKeyLen = 0x00;
							long lInitValueLen = 0x00;
							pSymmetryDESDlg->GetCipherKey(chSymmetryKey, EncryptIF::STATIC_CONST_LONG_SYMMETRYKEYLENGTH, &lKeyLen);
							pSymmetryDESDlg->GetInitialValue(chSymmetryDESInitValue, EncryptIF::STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH, &lInitValueLen);
							EncryptIF::Instance()->SetSymmetryKey(chSymmetryKey, lKeyLen);
							EncryptIF::Instance()->SetSymmetryDesInitValue(chSymmetryDESInitValue, lInitValueLen); 
							SetDecryptDES(pSymmetryDESDlg->GetDESType(), pSymmetryDESDlg->GetDESModeType());
						}
						break;
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_AES:
						EncryptIF::Instance()->SetEncryptSymmetry(EncryptIF::ENCRYPTSYMMETRY_AES);
						if (NULL != pCDialog) {
							SymmetryAESDlg* pSymmetryAESDlg = dynamic_cast<SymmetryAESDlg*>(const_cast<CDialog*>(pCDialog));
							char chSymmetryKey[EncryptIF::STATIC_CONST_LONG_SYMMETRYKEYLENGTH] = {0x00};
							char chSymmetryDESInitValue[EncryptIF::STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH] = {0x00};
							long lKeyLen = 0x00;
							long lInitValueLen = 0x00;
							pSymmetryAESDlg->GetCipherKey(chSymmetryKey, EncryptIF::STATIC_CONST_LONG_SYMMETRYKEYLENGTH, &lKeyLen);
							pSymmetryAESDlg->GetInitialValue(chSymmetryDESInitValue, EncryptIF::STATIC_CONST_LONG_SYMMETRYDESINITVALUELENGTH, &lInitValueLen);
							EncryptIF::Instance()->SetSymmetryKey(chSymmetryKey, lKeyLen);
							EncryptIF::Instance()->SetSymmetryDesInitValue(chSymmetryDESInitValue, lInitValueLen); 
							SetDecryptAES(pSymmetryAESDlg->GetAESKeySizeType(), pSymmetryAESDlg->GetAESModeType());
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
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_ECB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CBC:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CBC);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CFB:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CFB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_OFB:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_OFB);
			break;
		case SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_CTR:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CTR);
			break;
		default:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_INVALID);
			break;
	}
}

void 
CEncryptAlgorithmDlg::SetDecryptAES(SymmetryAESDlg::SYMMETRY_TABLE_AESKEYLEN_TYPE eAESKeySize, SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_TYPE eAESMode)
{
	switch(eAESKeySize) {
		case SymmetryAESDlg::SYMMETRY_TABLE_AESKEYLEN_BIT128:
			EncryptIF::Instance()->SetEncryptSymmetryAESKeySize(EncryptIF::ENCRYPTSYMMETRYAESKEYSIZE_BIT128);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESKEYLEN_BIT192:
			EncryptIF::Instance()->SetEncryptSymmetryAESKeySize(EncryptIF::ENCRYPTSYMMETRYAESKEYSIZE_BIT192);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESKEYLEN_BIT256:
			EncryptIF::Instance()->SetEncryptSymmetryAESKeySize(EncryptIF::ENCRYPTSYMMETRYAESKEYSIZE_BIT256);
			break;
		default:
			EncryptIF::Instance()->SetEncryptSymmetryAESKeySize(EncryptIF::ENCRYPTSYMMETRYAESKEYSIZE_INVALID);
			break;
	}
	switch(eAESMode) {
		case SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_ECB:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_ECB);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_CBC:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CBC);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_CFB:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CFB);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_OFB:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_OFB);
			break;
		case SymmetryAESDlg::SYMMETRY_TABLE_AESMODE_CTR:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_CTR);
			break;
		default:
			EncryptIF::Instance()->SetEncryptSymmetryMode(EncryptIF::ENCRYPTSYMMETRYMODE_INVALID);
			break;
	}
}

void 
CEncryptAlgorithmDlg::GetEncryptLevelFromCtrl(BYTE* pbyEncryptLevel1, BYTE* pbyEncryptLevel2, BYTE* pbyEncryptLevel3, BYTE* pbyEncryptLevel4)
{
	const CDialog* pCDialog = NULL;
	switch(m_eCurSelTab) {
		*pbyEncryptLevel1 = m_eCurSelTab;
		case MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY:
			if (NULL != m_pTabDlg[m_eCurSelTab]) {
				SymmetryDlg* pSymmetryDlg = dynamic_cast<SymmetryDlg*>(m_pTabDlg[m_eCurSelTab]);
				pCDialog = pSymmetryDlg->GetAlgorithmSymmetryDlg();
				SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE eSymmetryEncrypt = pSymmetryDlg->GetEncrypt();
				*pbyEncryptLevel2 = eSymmetryEncrypt;
				switch(eSymmetryEncrypt) {
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_DES:
						if (NULL != pCDialog) {
							SymmetryDESDlg* pSymmetryDESDlg = dynamic_cast<SymmetryDESDlg*>(const_cast<CDialog*>(pCDialog));
							*pbyEncryptLevel3 = pSymmetryDESDlg->GetDESType();
							*pbyEncryptLevel4 = pSymmetryDESDlg->GetDESModeType();
						}
						break;
					case SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_AES:
						if (NULL != pCDialog) {
							SymmetryAESDlg* pSymmetryAESDlg = dynamic_cast<SymmetryAESDlg*>(const_cast<CDialog*>(pCDialog));
							*pbyEncryptLevel3 = pSymmetryAESDlg->GetAESKeySizeType();
							*pbyEncryptLevel4 = pSymmetryAESDlg->GetAESModeType();
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
CEncryptAlgorithmDlg::UpdateCtrlFromEncryptLevel(BYTE byEncryptLevel1, BYTE byEncryptLevel2, BYTE byEncryptLevel3, BYTE byEncryptLevel4)
{
	m_tab.SetCurSel(byEncryptLevel1);
	switch(byEncryptLevel1) {
		case MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY:
			{
				SymmetryDlg* pSymmetryDlg = dynamic_cast<SymmetryDlg*>(m_pTabDlg[byEncryptLevel1]);
				break;
			}
		case MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY:
			break;
		case MAIN_TABLE_CONTROL_ENCRYPT_HASH:
			break;
		default:
			break;
	}
	UpdateCtrl();
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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN_ENCRYPTALGORITHM, &CEncryptAlgorithmDlg::OnTcnSelchangeTabMainEncryptalgorithm)
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
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//���ɼ�
	
	GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->SetWindowText("");
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainClearciphertext()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_MAIN_ENCRYPTDECRYPT)->ShowWindow(SW_HIDE);							//���ɼ�

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
	CString cstrSourceFile;
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->GetWindowText(cstrSourceFile);
	// OFN_HIDEREADONLY:����ֻ��ѡ��
	// OFN_OVERWRITEPROMPT:���������ļ�ǰ��
	// OFN_ALLOWMULTISELECT:����ѡ�����ļ�
	// OFN_CREATEPROMPT:���������ļ��������ڣ���Ի��򷵻�ѯ���û��Ƿ���ݴ��ļ��������ļ�����Ϣ��
	// OFN_FILEMUSTEXIST:ֻ�������Ѵ��ڵ��ļ���
	// OFN_FORCESHOWHIDDEN:������ʾ���ص��ļ�
	// OFN_NOREADONLYRETURN:������ֻ���ļ�
	// OFN_OVERWRITEPROMPT:������ļ��Ѵ���ʱ����ʾ�ļ��Ѵ��ڵ���Ϣ

	//����һ������ѡ�����ļ���CFileDialog  
	CFileDialog dlgFile(TRUE, NULL, cstrSourceFile, 
		OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT, 
		_T("document files (*.txt; *.doc; *.hlp; *.wps; *.rtf; *.html; *.pdf) |*.txt; *.doc; *.hlp; *.wps; *.rtf; *.html; *.pdf |\
		   compressed files (rar; *.zip; *.tar; *.cab; *.uue; *.jar; *.iso; *.z; *.7z; *.ace; *.lzh; *.arj; *.gzip; *.bz2) |rar; *.zip; *.tar; *.cab; *.uue; *.jar; *.iso; *.z; *.7z; *.ace; *.lzh; *.arj; *.gzip; *.bz2 |\
		   image files (*.bmp; *.gif; *.jpg; *.jpge; *.pic; *.png; *.tif; *.raw) |*.bmp; *.gif; *.jpg; *.jpge; *.pic; *.png; *.tif; *.raw |\
		   audio files (*.wav; *.aif; *.au; *.mp3; *.ram; *.wma; *.mmf; *.amr; *.aac; *.flac; *.mid; *.midi; *.rm; *.rmi; *.ape; *.pcm; *.voc) |*.wav; *.aif; *.au; *.mp3; *.ram; *.wma; *.mmf; *.amr; *.aac; *.flac; *.mid; *.midi; *.rm; *.rmi; *.ape; *.pcm; *.voc |\
		   video files (*.avi; *.mkv; *.wma; *.rmvb; *.rm; *.flash; *.mp4; *.mid; *.3GP; *.mpg; *.mov; *.swf) |*.avi; *.mkv; *.wma; *.rmvb; *.rm; *.flash; *.mp4; *.mid; *.3GP; *.mpg; *.mov; *.swf |\
		   All Files (*.*)|*.*||"), NULL);
	//�����Դ�500���ļ�  
	dlgFile.m_ofn.nMaxFile = 500 * MAX_PATH;  
	TCHAR* chFile = new TCHAR[dlgFile.m_ofn.nMaxFile];  
	dlgFile.m_ofn.lpstrFile = chFile;  
	//���ڴ������  
	ZeroMemory(dlgFile.m_ofn.lpstrFile,sizeof(TCHAR) * dlgFile.m_ofn.nMaxFile);
	vector<CString> vFileName;	//��մ�ŵ��ļ���·��   
	if (IDOK == dlgFile.DoModal()) {
		CString pathName;
		CString fileName;
		CString fileTitle;
		//��ȡ��һ���ļ���λ��  
		POSITION pos_file;  
		pos_file = dlgFile.GetStartPosition();  

		//ѭ������ÿ��·���������������  
		while(pos_file != NULL){  

			//���ļ�·�������������  
			pathName = dlgFile.GetNextPathName(pos_file);  
			vFileName.push_back(pathName);  

			//��ȡ�ļ���   
			//���ַ����ĺ�����ǰ�������������'\'�����������'\'�ұߵ��ַ�����Ϊ�ļ���  
			int length = pathName.GetLength();        
			for(int i = length -1; i>0;i--)  
			{  
				if('\\' == pathName.GetAt(i))  
				{//�жϵ�ǰ�ַ��Ƿ���'\'  
					fileName = pathName.Right(length - i -1);  
					break;//����ѭ��
				}  
			}//endfor  

			//��ȡ�ļ���(��������׺)  
			//����CString��Left(int count)��ȡCString�д�����������count���ַ�  
			//fileName.GetLength()-4�е�4��ʾ".dat"�ĸ��ַ�  

			//fileTitle = fileName.Left(fileName.GetLength()-fileName.Find(_T(".")));  
			//AfxMessageBox(fileTitle);  
		}

		if (false == vFileName.empty()) {
			cstrSourceFile = vFileName[0];
			for (int iCnt = 0x01; iCnt < (int)vFileName.size(); iCnt++) {
				cstrSourceFile += (_T("|") + vFileName[iCnt]);
			}
		}
		if (true == cstrSourceFile.IsEmpty()) {
			return;
		}
		GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->SetWindowText(cstrSourceFile);

		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()) {
			if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()) {
				//��������ļ���
				CString cstrOutputFolder;
				GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->GetWindowText(cstrOutputFolder);
				if (true == cstrOutputFolder.IsEmpty() || -1 != cstrOutputFolder.Find(STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK)) {
					//û��ָ�����·������ʹ��Ĭ��·��+Encrypt/Decrypt ����Ϊ��ǰ·��+Encrypt/Decrypt
					CString cstrPathName = dlgFile.GetPathName();	//�õ��������ļ���������Ŀ¼������չ���磺c:/test/test1.txt ���ļ�ѡ��ʱ��ֻ�����ļ������ļ���·���磺c:/test/test1
					CString cstrFileName = dlgFile.GetFileName();	//�õ��������ļ�����������չ���磺test1.txt
					if (false == cstrFileName.IsEmpty()) {
						int nPos = cstrPathName.Find(cstrFileName);
						if (-1 != nPos) {
							//�ҵ��ļ���
							cstrOutputFolder = cstrPathName.Left(nPos);
						}
					}
					else {
						cstrOutputFolder = cstrPathName + _T("\\");
					}
					cstrOutputFolder += STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK;
					GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->SetWindowText(cstrOutputFolder);
				}
			}
		}
	}
	delete[] chFile;

	//ͨ����һ���ļ�ˢ�µ�ǰ����ؼ�
	if (true == vFileName.empty()) {
		return;
	}
	static CFile s_fileUpdateCrl;		//ֻ��Ҫһ���ļ�����ʵ��
	static CFileException e;
	if (CFile::hFileNull != s_fileUpdateCrl.m_hFile) {
		s_fileUpdateCrl.Close();
	}
	if (FALSE == s_fileUpdateCrl.Open(vFileName[0], CFile::modeRead|CFile::typeBinary)) {
		//���ļ�ʧ��
		CString cstrFormat;
		TCHAR szError[1024] = {0x00};
		e.GetErrorMessage(szError, 1024);
		cstrFormat.Format(_T("���ļ�ʧ��!\r\n�ļ���:%s\r\n�������:%d\r\n������Ϣ:%s"),e.m_strFileName, e.m_cause, szError);
		MessageBox(cstrFormat);
		return;
	}
	if (s_fileUpdateCrl.GetLength() > sizeof(FileHeader)) {
		FileHeader sFileHeaderJudge = {0x00};
		s_fileUpdateCrl.Read(&sFileHeaderJudge, sizeof(FileHeader));
		if (0x00 == strcmp(sFileHeaderJudge.m_MD5Verify, STATIC_CONST_CSTR_MD5UPER32)) {
			//��֤ͨ��
			UpdateCtrlFromEncryptLevel(sFileHeaderJudge.byEncryptLevel1, sFileHeaderJudge.byEncryptLevel2, sFileHeaderJudge.byEncryptLevel3, sFileHeaderJudge.byEncryptLevel4);
		}
	}
}

void CEncryptAlgorithmDlg::OnStnClickedStaticMainOutputfolder()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	char szPath[MAX_PATH] = {0x00};
	LPITEMIDLIST pList = NULL;

	//��ȡ��ǰ·��
	CString cstrOutputFolder;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER);
	pEdit->GetWindowText(cstrOutputFolder);
	strcpy(szPath, cstrOutputFolder.GetBuffer(cstrOutputFolder.GetLength()) );
	// ����·���Ի���
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pList;
	bi.pszDisplayName = szPath;
	bi.lpszTitle   =  _T("ѡ��·��");
	bi.ulFlags = BIF_EDITBOX;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)szPath;
	bi.iImage = 0;

	//����ѡ��Ŀ¼�Ի���
	if   ((pList = SHBrowseForFolder(&bi)) != NULL) {    
		if (SHGetPathFromIDList(pList, szPath)) {
			pEdit->SetWindowText(szPath);
		}
	}
}

void CEncryptAlgorithmDlg::OnStnClickedStaticMainSourcefolder()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	char szPath[MAX_PATH] = {0x00};
	LPITEMIDLIST pList = NULL;

	//��ȡ��ǰ·��
	CString cstrSourceFolder;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER);
	pEdit->GetWindowText(cstrSourceFolder);
	strcpy(szPath, cstrSourceFolder.GetBuffer(cstrSourceFolder.GetLength()) );
	// ����·���Ի���
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pList;
	bi.pszDisplayName = szPath;
	bi.lpszTitle   =  _T("ѡ��·��");
	bi.ulFlags = BIF_EDITBOX;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)szPath;
	bi.iImage = 0;

	//����ѡ��Ŀ¼�Ի���
	if   ((pList = SHBrowseForFolder(&bi)) != NULL) {    
		if (SHGetPathFromIDList(pList, szPath)) {
			pEdit->SetWindowText(szPath);
		}
	}

	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()) {
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()) {
			//��������ļ���
			CString cstrOutputFolder;
			GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->GetWindowText(cstrOutputFolder);
			if (true == cstrOutputFolder.IsEmpty() || -1 != cstrOutputFolder.Find(STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK)) {
				//û��ָ�����·������ʹ��Ĭ��·��+Encrypt/Decrypt ����Ϊ��ǰ·��+Encrypt/Decrypt
				cstrOutputFolder = szPath;
				cstrOutputFolder += (_T("\\") + STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK);
				GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->SetWindowText(cstrOutputFolder);
			}
		}
	}
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainEncrypt()
{
	// TODO: Add your control notification handler code here
	CString cstrJudge;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()) {
		GetDlgItem(IDC_EDIT_MAIN_PLAINTEXT)->GetWindowText(cstrJudge);
		if (true == cstrJudge.IsEmpty()) {
			MessageBox(_T("��������Ҫ���ܵ�����!"));
			return;
		}
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFILE)))->GetCheck()) {
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ����Ҫ���ܵ�Դ�ļ�!"));
				return;
			}
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ����Ҫ���ܵ�Դ�ļ���!"));
				return;
			}
		}
		else {
			;
		}
		
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ�����·��!"));
				return;
			}
		}
	}
	else {
		;
	}

	GetDlgItem(IDC_BUTTON_MAIN_ENCRYPT)->EnableWindow(FALSE);
	OnBnClickedButtonMainClearciphertext();
	OnEncryptDecrypt(true);
	GetDlgItem(IDC_BUTTON_MAIN_ENCRYPT)->EnableWindow(TRUE);
}

void CEncryptAlgorithmDlg::OnBnClickedButtonMainDecrypt()
{
	// TODO: Add your control notification handler code here
	CString cstrJudge;
	if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTTEXT)))->GetCheck()) {
		GetDlgItem(IDC_EDIT_MAIN_CIPHERTEXT)->GetWindowText(cstrJudge);
		if (true == cstrJudge.IsEmpty()) {
			MessageBox(_T("��������Ҫ���ܵ�����!"));
			return;
		}
	}
	else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFILE)))->GetCheck()) {
		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTSINGLEFILE)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFILE)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ����Ҫ���ܵ�Դ�ļ�!"));
				return;
			}
		}
		else if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_ENCRYPTFOLDER)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_SOURCEFOLDER)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ����Ҫ���ܵ�Դ�ļ���!"));
				return;
			}
		}
		else {
			;
		}

		if (BST_CHECKED == ((CButton*)(GetDlgItem(IDC_RADIO_MAIN_KEEPSOURCEFILE)))->GetCheck()) {
			GetDlgItem(IDC_EDIT_MAIN_OUTPUTFOLDER)->GetWindowText(cstrJudge);
			if (true == cstrJudge.IsEmpty()) {
				MessageBox(_T("��ѡ�����·��!"));
				return;
			}
		}
	}
	else {
		;
	}

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

void CEncryptAlgorithmDlg::OnTcnSelchangeTabMainEncryptalgorithm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if (m_tab.GetCurSel() >= MAIN_TABLE_CONTROL_ENCRYPT_ALL) {
		return;
	}
	//�ѵ�ǰ��ҳ����������
	m_pTabDlg[m_eCurSelTab]->ShowWindow(SW_HIDE);
	//�õ��µ�ҳ������
	m_eCurSelTab = static_cast<MAIN_TABLE_CONTROL_ENCRYPT_TYPE>(m_tab.GetCurSel());		//�����1��ʼ��
	//���µ�ҳ����ʾ����
	m_pTabDlg[m_eCurSelTab]->ShowWindow(SW_SHOW);

	//��Ҫˢ�µ�ҳ��
	if (MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY == m_eCurSelTab) {
		SymmetryDlg* pSymmetryDlg = (SymmetryDlg*)m_pTabDlg[m_eCurSelTab];
		pSymmetryDlg->UpdateCtrl();
	}
	if (MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY == m_eCurSelTab) {
		
	}
	if (MAIN_TABLE_CONTROL_ENCRYPT_HASH == m_eCurSelTab) {
		
	}

	*pResult = 0;
}
