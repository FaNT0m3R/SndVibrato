// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__91043751_86BA_49EE_81F3_2083B7D29BA9__INCLUDED_)
#define AFX_GRAPH_H__91043751_86BA_49EE_81F3_2083B7D29BA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIDTH_GR 800
#define STRT_GR_X 5
#define STRT_GR_Y 50


class CGraph  
{
public:
	CGraph();
	virtual ~CGraph();

	bool OnLButtonUp(CPoint pt, double Zoom);	//реакция на лев кнопку мыши. Нужно для перемещения красной линии
	void MoveRight();							   //прокручивание графика в право
	void MoveLeft();						
	void SetData(BYTE* pData, int SizeWavData, int Channels, int BitsPerSample);	//инициализаци данных для графика
	void DrawGraph(CDC *pDC, double Zoom, int LeftSel, int RightSel);					//рисует график
	
	int SelectPoint;				//положение красной линии на звуковой дорожке. Потом это число будет записано в левую/правую границу для фильтра

private:
	void GetPointMinMax(BYTE* pData, BYTE* pEndData, int Num,int* ymin,int *ymax);	//находит минимальный и макс. сэмпл. Для ускорения рисования графика
	
	int bytesScrollSize;	 //столько байт надо прибавить/отнять от начала рисования графика, что бы прокрутить его на половину страницы
	int StartGraphOffs;		 //начало рисования графика относительно начала данных	
										//звука(т.е. BegWavData+StartGraphOffs - первый сэмпл для рисования графика)
	int Channels;					//количество каналов
	int BitsPerSample;			 //бит на сэмпл (8 или 16)
	BYTE* BegWavData;		 //указатель на даные звука
	int SizeWavData;			 //размер данных
	
};

#endif // !defined(AFX_GRAPH_H__91043751_86BA_49EE_81F3_2083B7D29BA9__INCLUDED_)
