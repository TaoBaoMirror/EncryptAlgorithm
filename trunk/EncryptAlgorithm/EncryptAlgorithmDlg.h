
// EncryptAlgorithmDlg.h : header file
//

#pragma once
#include "SymmetryDESDlg.h"

// CEncryptAlgorithmDlg dialog
class CEncryptAlgorithmDlg : public CDialog
{
public:
	void UpdateCtrl();
protected:
private:
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnInitCtrl();
	void UpdateMixCtrlEnable();
	void CipherTextDisplay();

	bool OnEncryptDecrypt(bool bEncryp);
	void SetEncryptDecrypt();
	void SetDecryptDES(SymmetryDESDlg::SYMMETRY_TABLE_DES_TYPE eDES, SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_TYPE eDESMode);
	
public:
protected:
private:
	enum MAIN_TABLE_CONTROL_ENCRYPT_TYPE 
	{
		MAIN_TABLE_CONTROL_ENCRYPT_SYMMETRY = 0x00,
		MAIN_TABLE_CONTROL_ENCRYPT_UNSYMMETRY,
		MAIN_TABLE_CONTROL_ENCRYPT_HASH,

		MAIN_TABLE_CONTROL_ENCRYPT_ALL,
		MAIN_TABLE_CONTROL_ENCRYPT_INVALID = -1,
	};
	CTabCtrl m_tab;
	CDialog* m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_ALL];
	MAIN_TABLE_CONTROL_ENCRYPT_TYPE m_eCurSelTab;
	char* m_pchDataOut;										//用于保存字符串解密后的数据,画面显示(各类显示方式)需要使用
	long m_lDataOutLen;


	static const CString STATIC_CONST_CSTR_ENCRYPTARROW;
	static const CString STATIC_CONST_CSTR_DECRYPTARROW;


// Construction
public:
	CEncryptAlgorithmDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CEncryptAlgorithmDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMainAbout();
	afx_msg void OnBnClickedButtonMainClearplaintext();
	afx_msg void OnBnClickedButtonMainClearciphertext();
	afx_msg void OnBnClickedButtonMainClearalltext();
	afx_msg void OnBnClickedRadioMainHex();
	afx_msg void OnBnClickedRadioMainBinary();
	afx_msg void OnBnClickedRadioMainOriginalcharacter();
	afx_msg void OnBnClickedRadioMainKeepsourcefile();
	afx_msg void OnBnClickedRadioMainReplacesourcefile();
	afx_msg void OnBnClickedRadioMainEncryptsinglefile();
	afx_msg void OnBnClickedRadioMainEncryptfolder();
	afx_msg void OnStnClickedStaticMainSourcefile();
	afx_msg void OnStnClickedStaticMainOutputfolder();
	afx_msg void OnStnClickedStaticMainSourcefolder();
	afx_msg void OnBnClickedButtonMainEncrypt();
	afx_msg void OnBnClickedButtonMainDecrypt();
	afx_msg void OnBnClickedRadioMainEncryptfile();
	afx_msg void OnBnClickedRadioMainEncrypttext();
	afx_msg void OnBnClickedRadioMainOctal();
	afx_msg void OnBnClickedRadioMainBinary4();
};