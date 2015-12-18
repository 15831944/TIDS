#pragma once


#include "PropertyData_DockBarChildDlg.h"

class TTunnelDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(TTunnelDockBarChildDlg)

public:
	TTunnelDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TTunnelDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_TTUNNEL_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

private:
	CString m_name;
	CString m_lenth;
	CString m_area;
};
