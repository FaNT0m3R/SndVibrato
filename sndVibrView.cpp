// sndVibrView.cpp : implementation of the CSndVibrView class
//

#include "stdafx.h"
#include "sndVibr.h"

#include "sndVibrDoc.h"
#include "sndVibrView.h"
#include "VibratoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSndVibrView

IMPLEMENT_DYNCREATE(CSndVibrView, CView)

//////////////////////////////////////////////
 ///////// ����� ���������. ������������ ���������� �������
BEGIN_MESSAGE_MAP(CSndVibrView, CView)
	//{{AFX_MSG_MAP(CSndVibrView)
	ON_WM_LBUTTONUP()											
	ON_COMMAND(ID_PLUS, OnZoomPlus)
	ON_COMMAND(ID_MINUS, OnZoomMinus)
	ON_COMMAND(ID_LEFT, OnLeftGraph)
	ON_COMMAND(ID_RIGHT, OnRightGraph)
	ON_COMMAND(ID_RIGHTSEL, OnRightSel)
	ON_COMMAND(ID_LEFTSEL, OnLeftSel)
	ON_COMMAND(ID_APPLYFILTER, OnApplyFilter)
	ON_COMMAND(ID_RELOAD, OnReload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSndVibrView construction/destruction

CSndVibrView::CSndVibrView()
{
	Zoom=128;								//�������������� ���������� � 128 ���
	LeftSelect=0;
	RightSelect=1000;
}

CSndVibrView::~CSndVibrView()
{}

/////////////////////////////////////////////////////////////////////////////
// CSndVibrView drawing

void CSndVibrView::OnDraw(CDC* pDC)
{
	HANDLE hBMP;		   //����� ��������	� �������� �������
	CDC CComptDC;		//DC ��� ��������
	
	CSndVibrDoc* pDoc = GetDocument();		 //�������� �������� - ����� � ���� 
									 								   //����� ���������� ������ � �������� �������
	ASSERT_VALID(pDoc);					//�������� ��������� �� ������������(�� ������ ������, ���� ������� �� ����)
	

	////////////������ ��� �������
	hBMP=LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PLAYER),		//������� �������� �� ��������
		IMAGE_BITMAP, 0, 0,  LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if (hBMP == NULL) return;
	CComptDC.CreateCompatibleDC(pDC);							//������� ����������� �������� � ��������
	CComptDC.SelectObject(hBMP);										//��������� � ����� �������� ��������
	pDC->BitBlt(10, 10, 97, 36, &CComptDC, 0, 0, SRCCOPY);				//���������� ������ ��������(������ ����������� � ��� 
																							//�������� ���� ����� ������������ �������� � ���������)
	pDC->BitBlt(WIDTH_GR+STRT_GR_X-110, 10, 97, 36, &CComptDC, 0, 0, SRCCOPY);				//���������� ������ ��������
	CComptDC.DeleteDC();											//���������� ������ �� ������ �������� � ���������

	/////////// ������ ������
	BYTE* BegWavData;					//���� ������� ������� ������ � �����
	int Channels, BitsPerSample;	  
	int SizeWavData;

	BegWavData = (BYTE*)pDoc->GetWavData(&Channels, &BitsPerSample, &SizeWavData);
	if ((int)BegWavData == -1)									   //���� ������ ���, �� �������� ������ �� ����
		return;

	SndGraph.SetData(BegWavData,SizeWavData,Channels,BitsPerSample);	//��� ������ ������ �������
	SndGraph.DrawGraph(pDC,Zoom, LeftSelect, RightSelect);			  									  //������ ������

}




///////////////////////////////////////////////////////////////////////////
///////////  ������� ����� ������ ����
void CSndVibrView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int right = WIDTH_GR+STRT_GR_X;							  //��� ���������� ������ ��� ���������� ����

  
																	//���������, ���� ����� ���� ����
	if ((point.y>=10) && (point.y<=46))
	{														//������ ��� ������� �����
		if ((point.x>=10) && (point.x<=43))
			pDoc->PlayWave(false);			//PLAY
		if ((point.x>=44) && (point.x<=73))
			pDoc->PauseWave(false);			//PAUSE
		if ((point.x>=74) && (point.x<=96))
			pDoc->StopWave(false);			//STOP

																//������ ��� ������ �����
		if ((point.x>=right-110) && (point.x<=right-110+33))
			pDoc->PlayWave(true);			//PLAY
		if ((point.x>=right-110+33) && (point.x<=right-110+63))
			pDoc->PauseWave(true);			//PAUSE
		if ((point.x>=right-110+63) && (point.x<=right-110+86))
			pDoc->StopWave(true);		    //STOP
	}

																	//��� ����� � ����� ������ ������� �
	if (SndGraph.OnLButtonUp(point,Zoom))	   //���� �� ������ true, �������������� ����
		CSndVibrView::RedrawWindow();

	CView::OnLButtonUp(nFlags, point);			//������� ������ � ����� ������������ �����������
}



///////////////////////////////////////////////////////////////////////
//��������� �������
void CSndVibrView::OnZoomMinus()
{
	CDC *pDC;

	Zoom*=2;							//��������, ������ ��� � Zoom �������� ����������� ����������. ��� �� ������, ��� �������� ������
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
}


void CSndVibrView::OnZoomPlus()
{
	CDC *pDC;

	if (Zoom!=1)
	{
		Zoom/=2;
		pDC = GetDC();						  //�������������� ������
		SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
		pDC->DeleteDC();
 	}
}


///////////////////////////////////////////////////////////////////////////////////
///////	 ����� �������
void CSndVibrView::OnLeftGraph() 
{
	CDC *pDC;
	SndGraph.MoveLeft();
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
	
  
 }

void CSndVibrView::OnRightGraph() 
{
  	CDC *pDC;
	SndGraph.MoveRight();
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
}
 

///////////////////////////////////////////////////////////
/////  ��������� ������ ��� �������
void CSndVibrView::OnRightSel() 
{
	CDC *pDC;
	RightSelect =  SndGraph.SelectPoint;
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}

void CSndVibrView::OnLeftSel() 
{
	CDC *pDC;
	LeftSelect =  SndGraph.SelectPoint;	
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}


//////////////////////////////////////////////////////////////
////// ��������� ������
void CSndVibrView::OnApplyFilter() 
{
	CDC *pDC;

	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	if (!pDoc->IsLoadWav())					   //���� �������� �� ������, �� ������ �� ���������
		return;

	CVibratoDlg vibDlg;
	if (vibDlg.DoModal() == IDOK)			//��������� ������ � ���� ���� ������ ������ ��, �� ��������� ������
	{
															// ������ ���� �� ��������� ���������� ������ �������
		pDoc->ApplyVibrato(LeftSelect, RightSelect, vibDlg.Freq , vibDlg.AmplMin, vibDlg.AmplMax);

		pDC = GetDC();						  //�������������� ������
		SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
		pDC->DeleteDC();
	}
}

////////////////////////////////////////////////////////////////
/// ������������ wav � �������������� ���������
void CSndVibrView::OnReload() 
{
	CDC *pDC;
	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc->IsLoadWav())
		return;


	pDoc->ResetWav();				 //����� ���������
	pDC = GetDC();						  //�������������� ������
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}
