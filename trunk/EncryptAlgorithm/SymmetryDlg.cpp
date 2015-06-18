// SymmetryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptAlgorithm.h"
#include "SymmetryDlg.h"
#include "SymmetryDESDlg.h"
#include "SymmetryAESDlg.h"



// SymmetryDlg dialog
SymmetryDlg::SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE 
SymmetryDlg::GetEncrypt()
{
	return m_eCurSelTab;
}

const CDialog* 
SymmetryDlg::GetAlgorithmSymmetryDlg()
{
	return m_pTabDlg[m_eCurSelTab];
}

void 
SymmetryDlg::UpdateCtrl()
{

}

BOOL 
SymmetryDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)) return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void 
SymmetryDlg::OnInitCtrl()
{
	//Init TabControl
	CTabCtrl* pCTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_SYMMETRY_ENCRYPTALGORITHM);
	pCTabCtrl->InsertItem(SYMMETRY_TABLE_CONTROL_ENCRYPT_DES, _T("DES"));
	pCTabCtrl->InsertItem(SYMMETRY_TABLE_CONTROL_ENCRYPT_AES, _T("AES"));

	CRect rect;
	m_tab.GetClientRect(&rect);//获得TAB控件的坐标
	//定位选项卡页的位置，这里可以根据情况自己调节偏移量
	rect.top +=21;
	rect.bottom -=1;
	rect.left +=1;
	rect.right -=1;
	//创建子页面
	if (NULL != m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_DES]) {
		m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_DES]->Create(IDD_DIALOG_SYMMETRYDES,GetDlgItem(IDC_TAB_SYMMETRY_ENCRYPTALGORITHM));
	}
	if (NULL != m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_AES]) {
		m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_AES]->Create(IDD_DIALOG_SYMMETRYAES,GetDlgItem(IDC_TAB_SYMMETRY_ENCRYPTALGORITHM));
	}

	for (int iCnt=0x00; iCnt<SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		//将子页面移动到指定的位置
		if (NULL != m_pTabDlg[iCnt]) {
			m_pTabDlg[iCnt]->MoveWindow(&rect);
			//隐藏子页面
			m_pTabDlg[iCnt]->ShowWindow(SW_HIDE);
		}
	}
	//显示当第一个子页面
	m_pTabDlg[0]->ShowWindow(SW_SHOW);
	m_eCurSelTab = static_cast<SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE>(0x00);
}

IMPLEMENT_DYNAMIC(SymmetryDlg, CDialog)

SymmetryDlg::SymmetryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SymmetryDlg::IDD, pParent),
	m_eCurSelTab(SYMMETRY_TABLE_CONTROL_ENCRYPT_INVALID)
{
	for (int iCnt=0x00; iCnt<SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		m_pTabDlg[iCnt] = NULL;
	}

	if (NULL == m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_DES]) {
		m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_DES] = new SymmetryDESDlg;
	}
	if (NULL == m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_AES]) {
		m_pTabDlg[SYMMETRY_TABLE_CONTROL_ENCRYPT_AES] = new SymmetryAESDlg;
	}
}

SymmetryDlg::~SymmetryDlg()
{
	for (int iCnt=0x00; iCnt<SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL; iCnt++) {
		if (NULL != m_pTabDlg[iCnt]) {
			delete m_pTabDlg[iCnt];
			m_pTabDlg[iCnt] = NULL;
		}
	}
}

void SymmetryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SYMMETRY_ENCRYPTALGORITHM, m_tab);
}


BEGIN_MESSAGE_MAP(SymmetryDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SYMMETRY_ENCRYPTALGORITHM, &SymmetryDlg::OnTcnSelchangeTabSymmetryEncryptalgorithm)
END_MESSAGE_MAP()


// SymmetryDlg message handlers

BOOL SymmetryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnInitCtrl();
	UpdateCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SymmetryDlg::OnTcnSelchangeTabSymmetryEncryptalgorithm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if (m_tab.GetCurSel() >= SYMMETRY_TABLE_CONTROL_ENCRYPT_ALL) {
		return;
	}
	//把当前的页面隐藏起来
	m_pTabDlg[m_eCurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_eCurSelTab = static_cast<SYMMETRY_TABLE_CONTROL_ENCRYPT_TYPE>(m_tab.GetCurSel());		//数组从1开始的
	//把新的页面显示出来
	m_pTabDlg[m_eCurSelTab]->ShowWindow(SW_SHOW);

	//需要刷新的页面
	if (SYMMETRY_TABLE_CONTROL_ENCRYPT_DES == m_eCurSelTab){
		SymmetryDESDlg* pSymmetryDESDlg = dynamic_cast<SymmetryDESDlg*>(m_pTabDlg[m_eCurSelTab]);
		pSymmetryDESDlg->UpdateCtrl();
	}
	if (SYMMETRY_TABLE_CONTROL_ENCRYPT_AES == m_eCurSelTab){
		SymmetryAESDlg* pSymmetryAESDlg = dynamic_cast<SymmetryAESDlg*>(m_pTabDlg[m_eCurSelTab]);
		pSymmetryAESDlg->UpdateCtrl();
	}

	*pResult = 0;
}
