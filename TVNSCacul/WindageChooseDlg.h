#pragma once
#include "Resource.h"

class WindageChooseDlg : public CDialog
{
	DECLARE_DYNAMIC(WindageChooseDlg)

public:
	WindageChooseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WindageChooseDlg();

// �Ի�������
	enum { IDD = IDD_WINDAGE_CHOOSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHmWindageRadio();
	afx_msg void OnBnClickedFormulaRadio();
	int m_windageWay;
	afx_msg void OnBnClickedOk();
	BOOL OnInitDialog();
private:
	bool m_hmWindage;
	bool m_formula;
};
