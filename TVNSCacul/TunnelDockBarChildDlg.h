#pragma once


#include "PropertyData_DockBarChildDlg.h"
#include "afxwin.h"

class TunnelDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(TunnelDockBarChildDlg)

public:
	TunnelDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TunnelDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_TUNNEL_PARAM_DLG };

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
	CString m_velocity;
	CString m_Q;
};
