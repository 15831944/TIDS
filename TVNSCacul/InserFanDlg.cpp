#include "stdafx.h"
#include "InserFanDlg.h"
#include "../Sqlite/Sqlite.h"

// InserFanDlg �Ի���

IMPLEMENT_DYNAMIC(InserFanDlg, CDialog)

InserFanDlg::InserFanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InserFanDlg::IDD, pParent)
	, m_fanName(_T(""))
	, m_minQ(_T(""))
	, m_maxQ(_T(""))
	, m_minH(_T(""))
	, m_maxH(_T(""))
	, m_power(_T(""))
{

}

InserFanDlg::~InserFanDlg()
{
}

void InserFanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FANNAME_EDIT, m_fanName);
	DDX_Text(pDX, IDC_NEWMINQ_EDIT, m_minQ);
	DDX_Text(pDX, IDC_NEWMAXQ_EDIT, m_maxQ);
	DDX_Text(pDX, IDC_NEWMINH_EDIT, m_minH);
	DDX_Text(pDX, IDC_NEWMAXH_EDIT, m_maxH);
	DDX_Text(pDX, IDC_NEWPOWER_EDIT, m_power);
	DDX_Control(pDX, IDC_NEWMETHOD_COMBO, m_methodCmBox);
}


BEGIN_MESSAGE_MAP(InserFanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &InserFanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// InserFanDlg ��Ϣ�������
static char* UnicodeToUtf8(CString unicode);

void InserFanDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	Sqlite *sqlite = new Sqlite;
	if (m_fanName.IsEmpty() || m_minQ.IsEmpty() || m_maxQ.IsEmpty() || m_minH.IsEmpty() || m_maxH.IsEmpty() || m_power.IsEmpty())
	{
		AfxMessageBox(_T("�����������Ϊ��"),MB_ICONSTOP | MB_OK);
		return;
	}
	else
	{
		int minQ = _ttoi(m_minQ);
		int maxQ = _ttoi(m_maxQ);
		int minH = _ttoi(m_minH);
		int maxH = _ttoi(m_maxH);
		CString mehtod;
		GetDlgItem(IDC_NEWMETHOD_COMBO)->GetWindowText(mehtod);
		char *name = UnicodeToUtf8(m_fanName);
		char *power = UnicodeToUtf8(m_power);
		if(sqlite->insertFan(mehtod,name,minQ,maxQ,minH,maxH,power))
		{
			AfxMessageBox(_T("���ݳɹ�����"),MB_ICONWARNING | MB_OK);
		}
	}
	OnOK();
}

BOOL InserFanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_methodCmBox.AddString(_T("ѹ��ʽ"));
	m_methodCmBox.AddString(_T("���ʽ"));
	m_methodCmBox.SetCurSel(0);
	return TRUE;
}

static char* UnicodeToUtf8(CString unicode)
{
	wchar_t* wszString = unicode.AllocSysString();
	//Ԥת�����õ�����ռ�Ĵ�С������õĺ��������������෴
	int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	//ͬ�ϣ�����ռ�Ҫ��'\0'�����ռ�
	//UTF8��Ȼ��Unicode��ѹ����ʽ����Ҳ�Ƕ��ֽ��ַ��������Կ�����char����ʽ����
	char* szU8 = new char[u8Len + 1];
	//ת��
	//unicode���Ӧ��strlen��wcslen
	::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);
	//������'\0'
	szU8[u8Len] = '\0';
	return szU8;
}
