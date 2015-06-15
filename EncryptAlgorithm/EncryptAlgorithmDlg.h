
// EncryptAlgorithmDlg.h : header file
//

#pragma once
#include "SymmetryDESDlg.h"
#include <vector>
using namespace std;

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
	bool OnEncryptDecryptText(bool bEncryp, CString cstrDataText);
	bool OnEncryptDecryptFile(bool bEncryp, CString cstrDataFile);
	void SetEncryptDecrypt();
	void SetDecryptDES(SymmetryDESDlg::SYMMETRY_TABLE_DES_TYPE eDES, SymmetryDESDlg::SYMMETRY_TABLE_DESMODE_TYPE eDESMode);
	void GetEncryptLevelFromCtrl(BYTE* pbyEncryptLevel1, BYTE* pbyEncryptLevel2, BYTE* pbyEncryptLevel3, BYTE* pbyEncryptLevel4);
	void UpdateCtrlFromEncryptLevel(BYTE byEncryptLevel1, BYTE byEncryptLevel2, BYTE byEncryptLevel3, BYTE byEncryptLevel4);
	
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

	struct FileHeader{
		/************************************************************************/
		/*    字符串	mengxiang
		/*    MD5 16位 小写	699fcb5e21845872
		/*    MD5 16位 大写	699FCB5E21845872
		/*    MD5 32位 小写	9d439b82699fcb5e218458726b69bec1
		/*    MD5 32位 大写	9D439B82699FCB5E218458726B69BEC1                                                                  */
		/************************************************************************/
		TCHAR m_MD5Verify[36];	// 使用MD5验证文件是否是本软件加密文件 (MD5 32位 大写)
		TCHAR m_ctime[24];		// creation date/time of file 2003-10-27 08:09:10 方便转换
		TCHAR m_mtime[24];		// creation date/time of file
		TCHAR m_atime[24];		// last access date/time of file
		ULONGLONG m_size;       // logical size of file in bytes
		BYTE m_attribute;       // logical OR of CFile::Attribute enum values
		BYTE _m_padding;        // pad the structure to a WORD
		BYTE m_byReserve[2];
		TCHAR m_szFileTitle[128];// file title
		TCHAR m_szFileType[16];	// file type
		BYTE byEncryptLevel1;	// symmetry, unsymmetry, hash...s
		BYTE byEncryptLevel2;	// des, aes...
		BYTE byEncryptLevel3;	// des,3des...
		BYTE byEncryptLevel4;	// ecb,cbc...
		BYTE m_byReserve1[256];
	};

	CTabCtrl m_tab;
	CDialog* m_pTabDlg[MAIN_TABLE_CONTROL_ENCRYPT_ALL];
	MAIN_TABLE_CONTROL_ENCRYPT_TYPE m_eCurSelTab;
	char* m_pchDataOut;										//用于保存字符串解密后的数据,画面显示(各类显示方式)需要使用
	long m_lDataOutLen;
	
	static const CString STATIC_CONST_CSTR_ENCRYPTARROW;
	static const CString STATIC_CONST_CSTR_DECRYPTARROW;
	static const CString STATIC_CONST_CSTR_ENCRYPTDECRYPTFOLDERMARK;//加解密文件夹标识，输出文件夹默认逻辑使用。
	static const CString STATIC_CONST_CSTR_ENCRYPTFOLDER;
	static const CString STATIC_CONST_CSTR_DECRYPTFOLDER;
	static const long STATIC_CONST_LONG_DESBUFFEREXPANDLENGTH;		//DES加密时，扩展内存长度，防止加密算法内存越界

	static const CString STATIC_CONST_CSTR_MD5TEXT;
	static const CString STATIC_CONST_CSTR_MD5LOWER16;
	static const CString STATIC_CONST_CSTR_MD5UPER16;
	static const CString STATIC_CONST_CSTR_MD5LOWER32;
	static const CString STATIC_CONST_CSTR_MD5UPER32;
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
	afx_msg void OnTcnSelchangeTabMainEncryptalgorithm(NMHDR *pNMHDR, LRESULT *pResult);
};