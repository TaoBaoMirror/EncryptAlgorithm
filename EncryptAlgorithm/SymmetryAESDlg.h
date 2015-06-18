#pragma once
#pragma warning (disable:4996)	//È¡Ïû¾¯¸æ

// SymmetryAESDlg dialog

class SymmetryAESDlg : public CDialog
{
public:

	enum SYMMETRY_TABLE_AESKEYLEN_TYPE 
	{
		SYMMETRY_TABLE_AESKEYLEN_NON = 0x00,
		SYMMETRY_TABLE_AESKEYLEN_BIT128,
		SYMMETRY_TABLE_AESKEYLEN_BIT192,
		SYMMETRY_TABLE_AESKEYLEN_BIT256,

		SYMMETRY_TABLE_AESKEYLEN__ALL,
		SYMMETRY_TABLE_AESKEYLEN_INVALID = -1,
	};
	SYMMETRY_TABLE_AESKEYLEN_TYPE GetAESKeySizeType();

	enum SYMMETRY_TABLE_AESMODE_TYPE 
	{
		SYMMETRY_TABLE_AESMODE_NON = 0x00,
		SYMMETRY_TABLE_AESMODE_ECB,
		SYMMETRY_TABLE_AESMODE_CBC,
		SYMMETRY_TABLE_AESMODE_CFB,
		SYMMETRY_TABLE_AESMODE_OFB,
		SYMMETRY_TABLE_AESMODE_CTR,

		SYMMETRY_TABLE_AESMODE_ALL,
		SYMMETRY_TABLE_AESMODE_INVALID = -1,
	};
	SYMMETRY_TABLE_AESMODE_TYPE GetAESModeType();

	bool GetCipherKey(char* pKeyOut, long KeyOutLen, long* pKeyLen);
	bool GetInitialValue(char* pKeyOut, long KeyOutLen, long* pKeyLen);

	void UpdateCtrl();
protected:
private:
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnInitCtrl();
	void UpdateMixCtrl();
	void UpdateMixCtrlValue();
	void UpdateMixCtrlEnable();

	bool ConvertCStringToData(CString cstrKeyIn, char* pKeyOut, long KeyOutLen, long* pKeyLen);

public:
protected:
private:
	static const long STATIC_CONST_LONG_KEYLENGTHMAX = 32+1;
	char m_chKey[STATIC_CONST_LONG_KEYLENGTHMAX];
	long m_lKeyLength;
	static const long STATIC_CONST_LONG_INITVALUELENGTHMAX = 16+1;
	char m_chInitValue[STATIC_CONST_LONG_INITVALUELENGTHMAX];
	long m_lInitValueLength;

	static const CString STATIC_CONST_CSTR_INITIALVECTORVALUE;
	static const CString STATIC_CONST_CSTR_INITIALCOUNTVALUE;

	DECLARE_DYNAMIC(SymmetryAESDlg)

public:
	SymmetryAESDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SymmetryAESDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYMMETRYAES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSymmetryaesText();
	afx_msg void OnBnClickedRadioSymmetryaesHex();
	afx_msg void OnBnClickedRadioSymmetryaesOctal();
	afx_msg void OnBnClickedRadioSymmetryaesBinarty4();
	afx_msg void OnBnClickedRadioSymmetryaesBinary();
	afx_msg void OnBnClickedRadioSymmetryaesAesecb();
	afx_msg void OnBnClickedRadioSymmetryaesAescbc();
	afx_msg void OnBnClickedRadioSymmetryaesAescfb();
	afx_msg void OnBnClickedRadioSymmetryaesAesofb();
	afx_msg void OnBnClickedRadioSymmetryaesAesctr();
	afx_msg void OnBnClickedRadioSymmetryaesbit128();
	afx_msg void OnBnClickedRadioSymmetryaesbit192();
	afx_msg void OnBnClickedRadioSymmetryaesbit256();
	afx_msg void OnEnKillfocusEditSymmetryaesInitialvalue();
	afx_msg void OnEnKillfocusEditSymmetryaesCipherkey();
};
