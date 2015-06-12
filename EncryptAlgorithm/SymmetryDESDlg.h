#pragma once


// SymmetryDESDlg dialog

class SymmetryDESDlg : public CDialog
{
public:
	enum SYMMETRY_TABLE_DES_TYPE 
	{
		SYMMETRY_TABLE_DES_NON = 0x00,
		SYMMETRY_TABLE_DES_DES,
		SYMMETRY_TABLE_DES_3DES,

		SYMMETRY_TABLE_DES_ALL,
		SYMMETRY_TABLE_DES_INVALID = -1,
	};

	enum SYMMETRY_TABLE_DESMODE_TYPE 
	{
		SYMMETRY_TABLE_DESMODE_NON = 0x00,
		SYMMETRY_TABLE_DESMODE_ECB,
		SYMMETRY_TABLE_DESMODE_CBC,
		SYMMETRY_TABLE_DESMODE_CFB,
		SYMMETRY_TABLE_DESMODE_OFB,
		SYMMETRY_TABLE_DESMODE_CTR,

		SYMMETRY_TABLE_DESMODE_ALL,
		SYMMETRY_TABLE_DESMODE_INVALID = -1,
	};
	SYMMETRY_TABLE_DES_TYPE GetDESType();
	SYMMETRY_TABLE_DESMODE_TYPE GetDESModeType();
	
	CString GetCipherKey();
	CString GetInitialValue();

	void UpdateCtrl();
protected:
private:
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnInitCtrl();
	void UpdateMixCtrl();
	void UpdateMixCtrlValue();
	void UpdateMixCtrlEnable();
public:
protected:
private:
	static const CString STATIC_CONST_CSTR_INITIALVECTORVALUE;
	static const CString STATIC_CONST_CSTR_INITIALCOUNTVALUE;

	DECLARE_DYNAMIC(SymmetryDESDlg)

public:
	SymmetryDESDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SymmetryDESDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYMMETRYDES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSymmetrydesDes();
	afx_msg void OnBnClickedRadioSymmetrydes3des();
	afx_msg void OnBnClickedRadioSymmetrydesDesecb();
	afx_msg void OnBnClickedRadioSymmetrydesDescbc();
	afx_msg void OnBnClickedRadioSymmetrydesDescfb();
	afx_msg void OnBnClickedRadioSymmetrydesDesofb();
	afx_msg void OnBnClickedRadioSymmetrydesDesctr();
};
