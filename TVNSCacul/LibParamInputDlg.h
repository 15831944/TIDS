#pragma once

#include "PropertyData_DockBarChildDlg.h"
#include "afxwin.h"

class LibParamInputDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(LibParamInputDlg)

public:
	LibParamInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LibParamInputDlg();

// �Ի�������
	enum { IDD = IDD_LIB_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_rand;
	CString m_leakage;

protected:
	void readPropertyData();
	void writePropertyData();
	void initDatas();

public:
	virtual BOOL OnInitDialog();

};
