#if !defined(AFX_VIBRATODLG_H__A6864287_CAEE_4E18_BBB2_C8E9F43D062C__INCLUDED_)
#define AFX_VIBRATODLG_H__A6864287_CAEE_4E18_BBB2_C8E9F43D062C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VibratoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVibratoDlg диалог. Показывает форму для заполнения данных для
//                           фильтра и заполняет переменные, которые потом можно будет почитать для применения фильтра

class CVibratoDlg : public CDialog
{
// Construction
public:
	int AmplMin,AmplMax;	  //после нажатия кнопки ОК, класс Вида (CSndVibrView) считает данные для 
	double Freq;												  //фильтра и отправит их Документу

	CVibratoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVibratoDlg)
	enum { IDD = IDD_VIBRATO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVibratoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVibratoDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIBRATODLG_H__A6864287_CAEE_4E18_BBB2_C8E9F43D062C__INCLUDED_)
