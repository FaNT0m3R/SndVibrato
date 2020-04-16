// sndVibrView.h : interface of the CSndVibrView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNDVIBRVIEW_H__410C1A86_BACD_4923_9F23_E47B368E6C11__INCLUDED_)
#define AFX_SNDVIBRVIEW_H__410C1A86_BACD_4923_9F23_E47B368E6C11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Graph.h"


/////////////////////////////////////////////////////////////////////////
////// отвечает за представление документа и взаимодеёствие с пользователем
////// рисует графики, кнопки для плейера
class CSndVibrView : public CView
{
protected: // create from serialization only
	CSndVibrView();
	DECLARE_DYNCREATE(CSndVibrView)

// Attributes
public:
	CSndVibrDoc* GetDocument();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSndVibrView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CSndVibrView();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSndVibrView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	//обработчики нажатий в ToolBar'е
	afx_msg void OnZoomPlus();
	afx_msg void OnZoomMinus();
	afx_msg void OnLeftGraph();
	afx_msg void OnRightGraph();
	afx_msg void OnLeftSel();
	afx_msg void OnRightSel();
	afx_msg void OnApplyFilter();
	afx_msg void OnReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	int LeftSelect;						 //левая граница для фильтра. Это смещение данных для фильтрации относительно их начала 
	int RightSelect;				
	double Zoom;						//во столько раз уменьшаем график
	CGraph SndGraph;			 //класс графика. Занимается рисованием графика и реакцией на клик по нему
};


///////// возвращает указатель на класс документа. Надо же как-то Виду узнавать о рисуемых данных
inline CSndVibrDoc* CSndVibrView::GetDocument()
   { return (CSndVibrDoc*)m_pDocument; }

   
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNDVIBRVIEW_H__410C1A86_BACD_4923_9F23_E47B368E6C11__INCLUDED_)
