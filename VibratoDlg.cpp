// VibratoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sndVibr.h"
#include "VibratoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVibratoDlg dialog


CVibratoDlg::CVibratoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVibratoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVibratoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVibratoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVibratoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVibratoDlg, CDialog)
	//{{AFX_MSG_MAP(CVibratoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Обработка нажатия ОК
void CVibratoDlg::OnOK() 
{
	CString str;												//записываем в публичные переменные значения с формы
	GetDlgItemText(EDT_FREQ,str);
	Freq = atof(str.GetBuffer(30));
	GetDlgItemText(EDT_AMP_MIN,str);
	AmplMin = atoi(str.GetBuffer(30));
	GetDlgItemText(EDT_AMP_MAX,str);
	AmplMax = atoi(str.GetBuffer(30));

	CDialog::OnOK();
}																																															

BOOL CVibratoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(EDT_FREQ,"11");			//выставляем значения по умолчанию. Удобней так
	SetDlgItemText(EDT_AMP_MIN,"0");
	SetDlgItemText(EDT_AMP_MAX,"100");
	
	return TRUE;  
}
