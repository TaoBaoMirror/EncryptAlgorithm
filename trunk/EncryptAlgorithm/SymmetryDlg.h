#pragma once


// SymmetryDlg dialog

class SymmetryDlg : public CDialog
{
public:
	enum SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE 
	{
		SYMMETRY_TABLE_CONTROL_ENCRYPT_DES = 0x00,

		SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL,
		SYMMETRY_TABLE_CONTROL_ENCRYPT_INVALID = -1,
	};
	SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE GetEncrypt();

	const CDialog* GetAlgorithmSymmetryDlg();
	void UpdateCtrl();
protected:
private:
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnInitCtrl();

public:
	
protected:
private:
	CTabCtrl m_tab;
	CDialog* m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL];
	SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE m_eCurSelTab;


	DECLARE_DYNAMIC(SymmetryDlg)

public:
	SymmetryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SymmetryDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYMMETRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
