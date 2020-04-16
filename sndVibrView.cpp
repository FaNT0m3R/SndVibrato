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
 ///////// Карта сообщений. сопоставляет сообщениям функции
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
	Zoom=128;								//первоначальное уменьшение в 128 раз
	LeftSelect=0;
	RightSelect=1000;
}

CSndVibrView::~CSndVibrView()
{}

/////////////////////////////////////////////////////////////////////////////
// CSndVibrView drawing

void CSndVibrView::OnDraw(CDC* pDC)
{
	HANDLE hBMP;		   //хэндл картинки	с кнопками плейера
	CDC CComptDC;		//DC для картинки
	
	CSndVibrDoc* pDoc = GetDocument();		 //получили документ - потом у него 
									 								   //будем спрашивать данные и отсылать команды
	ASSERT_VALID(pDoc);					//проверка указателя на корректность(на всякий случай, чтоб вылетов не было)
	

	////////////рисуем два плейера
	hBMP=LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PLAYER),		//открыли картинку из ресурсов
		IMAGE_BITMAP, 0, 0,  LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if (hBMP == NULL) return;
	CComptDC.CreateCompatibleDC(pDC);							//создали совместимый контекст с экранным
	CComptDC.SelectObject(hBMP);										//поместили в новый контекст картинку
	pDC->BitBlt(10, 10, 97, 36, &CComptDC, 0, 0, SRCCOPY);				//нарисовали первую картинку(просто скопировали в наш 
																							//контекст этот новый совметстимый контекст с картинкой)
	pDC->BitBlt(WIDTH_GR+STRT_GR_X-110, 10, 97, 36, &CComptDC, 0, 0, SRCCOPY);				//нарисовали вторую картинку
	CComptDC.DeleteDC();											//освободили больше не нужный контекст с картинкой

	/////////// рисуем график
	BYTE* BegWavData;					//сюда функция запишет данные о звуке
	int Channels, BitsPerSample;	  
	int SizeWavData;

	BegWavData = (BYTE*)pDoc->GetWavData(&Channels, &BitsPerSample, &SizeWavData);
	if ((int)BegWavData == -1)									   //если данных нет, то рисовать график не надо
		return;

	SndGraph.SetData(BegWavData,SizeWavData,Channels,BitsPerSample);	//даём данные классу графика
	SndGraph.DrawGraph(pDC,Zoom, LeftSelect, RightSelect);			  									  //рисуем график

}




///////////////////////////////////////////////////////////////////////////
///////////  Нажатие левой кнопки мыши
void CSndVibrView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int right = WIDTH_GR+STRT_GR_X;							  //эта переменная только для читаемости кода

  
																	//проверяем, куда попал клик мыши
	if ((point.y>=10) && (point.y<=46))
	{														//плейер для старого файла
		if ((point.x>=10) && (point.x<=43))
			pDoc->PlayWave(false);			//PLAY
		if ((point.x>=44) && (point.x<=73))
			pDoc->PauseWave(false);			//PAUSE
		if ((point.x>=74) && (point.x<=96))
			pDoc->StopWave(false);			//STOP

																//плейер для нового файла
		if ((point.x>=right-110) && (point.x<=right-110+33))
			pDoc->PlayWave(true);			//PLAY
		if ((point.x>=right-110+33) && (point.x<=right-110+63))
			pDoc->PauseWave(true);			//PAUSE
		if ((point.x>=right-110+63) && (point.x<=right-110+86))
			pDoc->StopWave(true);		    //STOP
	}

																	//даём знать о клике классу графика и
	if (SndGraph.OnLButtonUp(point,Zoom))	   //если он вернул true, перерисовываем окно
		CSndVibrView::RedrawWindow();

	CView::OnLButtonUp(nFlags, point);			//передаём данные о клике стандартному обработчику
}



///////////////////////////////////////////////////////////////////////
//уменьшить масштаб
void CSndVibrView::OnZoomMinus()
{
	CDC *pDC;

	Zoom*=2;							//умножаем, потому что в Zoom находить коэффициент уменьшения. Чем он больше, тем картинка меньше
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
}


void CSndVibrView::OnZoomPlus()
{
	CDC *pDC;

	if (Zoom!=1)
	{
		Zoom/=2;
		pDC = GetDC();						  //перерисовываем график
		SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
		pDC->DeleteDC();
 	}
}


///////////////////////////////////////////////////////////////////////////////////
///////	 сдвиг графика
void CSndVibrView::OnLeftGraph() 
{
	CDC *pDC;
	SndGraph.MoveLeft();
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
	
  
 }

void CSndVibrView::OnRightGraph() 
{
  	CDC *pDC;
	SndGraph.MoveRight();
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();
}
 

///////////////////////////////////////////////////////////
/////  установка границ для фильтра
void CSndVibrView::OnRightSel() 
{
	CDC *pDC;
	RightSelect =  SndGraph.SelectPoint;
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}

void CSndVibrView::OnLeftSel() 
{
	CDC *pDC;
	LeftSelect =  SndGraph.SelectPoint;	
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}


//////////////////////////////////////////////////////////////
////// применить фильтр
void CSndVibrView::OnApplyFilter() 
{
	CDC *pDC;

	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	if (!pDoc->IsLoadWav())					   //если документ не открыт, то фильтр не применять
		return;

	CVibratoDlg vibDlg;
	if (vibDlg.DoModal() == IDOK)			//запускаем диалог и если была нажата кнопка ОК, то выполняем фильтр
	{
															// данные берём из публичных переменных класса диалога
		pDoc->ApplyVibrato(LeftSelect, RightSelect, vibDlg.Freq , vibDlg.AmplMin, vibDlg.AmplMax);

		pDC = GetDC();						  //перерисовываем график
		SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
		pDC->DeleteDC();
	}
}

////////////////////////////////////////////////////////////////
/// восстановить wav в первоначальное состояние
void CSndVibrView::OnReload() 
{
	CDC *pDC;
	CSndVibrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc->IsLoadWav())
		return;


	pDoc->ResetWav();				 //сброс изменений
	pDC = GetDC();						  //перерисовываем график
	SndGraph.DrawGraph(pDC, Zoom, LeftSelect, RightSelect);
	pDC->DeleteDC();

}
