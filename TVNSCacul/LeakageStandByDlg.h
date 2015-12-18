#pragma once
#include "Resource.h"
#include "afxwin.h"

// LeakageStandByDlg �Ի���

class LeakageStandByDlg : public CDialog
{
	DECLARE_DYNAMIC(LeakageStandByDlg)

public:
	LeakageStandByDlg(CWnd* pParent = NULL,AcDbObjectId objId = NULL,CString chimType = _T(""));   // ��׼���캯��
	virtual ~LeakageStandByDlg();

// �Ի�������
	enum { IDD = IDD_LEAKAGE_CACUL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	bool m_isFactor;
	bool m_isHM;
	CEdit m_HMEdit;
	CEdit m_factorEdit;

	AcDbObjectId m_objId;
	CString m_chimType;

public:
	afx_msg void OnBnClickedHmLeakageRadio();
	afx_msg void OnBnClickedLeakageFactorRadio();

public:
	void readPropertyData();
	void writePropertyData();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
