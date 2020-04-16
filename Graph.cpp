// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sndVibr.h"
#include "Graph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraph::CGraph()
{
	StartGraphOffs = 0;					//начальная инициализация.
	SelectPoint=0;
	BegWavData=0;	
}

CGraph::~CGraph()
{}

void CGraph::DrawGraph(CDC *pDC, double Zoom, int LeftSel, int RightSel)
{
 	BYTE *EndWavData;			//конец данных
	BYTE* currWavData;			 //текущий указатель, он у нас будет бегать по данным и через него читать байты для графика

	int sizeSample;											//размер сэмпла, его будем прибавлять к указателю

	CPoint pt;								 
	double fx;								  //положение X точки, только более точное представление
	double step=1/Zoom;						//шаг по X


	if (BegWavData==0)				//если данные не дали, то выход, нечего рисовать
		return;

	EndWavData = BegWavData+SizeWavData;	   


	if ((BitsPerSample==8) && (Channels==1))	  //вычисляем размер сэмпла(если два канала, 
		sizeSample=1;												  //то сэмпл двойной получается, для обеих каналов)
	if ((BitsPerSample==8) && (Channels==2))
		sizeSample=2;
	if ((BitsPerSample==16) && (Channels==1))
		sizeSample=2;
	if ((BitsPerSample==16) && (Channels==2))
		sizeSample=4;

	for (int i=0; i<Channels; i++)				//если 2 канала, то нарисуется 2 графика, иначе только 1
	{
		currWavData = BegWavData+StartGraphOffs;		
		
		pDC->Rectangle(5,
								STRT_GR_Y+(256*i)-1,
								WIDTH_GR+5+2,
								STRT_GR_Y+256+(256*i));				//фоновый прямокугольник. Затирает старый график

		fx = STRT_GR_X;													  //задаём стартовые координаты
		pt.x = STRT_GR_X;
		pt.y=STRT_GR_Y+128+(256*i);
		pDC->MoveTo(pt);												  //двигаем карандаш в начальную позицию

									//первый раз i=0, значит сдвига не будет и в указателе всегда будет первый байт/слово
										//второй раз(если канала 2), i=1 и указатель сдвинется на второй байт/слово
		if (BitsPerSample==8)
			currWavData+=1*i;
		if (BitsPerSample==16)
			currWavData+=2*i;
		

										  //если уменьшение малое, то рисовать можно каждый сэмпл и тормозить не будет
										//   иначе подсчитываем максимальное и минимальное значение сэмпла и рисуем 
										//сразу блок сэмплов одной линией, всё равно они там все сплывутся

		if (Zoom<16)					
		{
						/// Медленный способ(для небоольшого уменьшения)
			while ((currWavData<EndWavData) && (pt.x<=WIDTH_GR+STRT_GR_X))  //рисуем или пока не кончатся данные, 
			{																										//или пока не кончится облать для графика
				if (BitsPerSample==8)
					pt.y=*(BYTE*)currWavData-0x80+STRT_GR_Y+128+(i<<8);	//сэмпл в 8 бит имеет значени от 0 до 256(т. е. он 
				if (BitsPerSample==16)																				//беззнаковый) и среднее значение 0х80
					pt.y=(*(short*)currWavData>>8)+STRT_GR_Y+128+(i<<8);	//сэмпл в 16 бит знаковый =  от -32768 до 32767
				fx+=step;
				pt.x=int(fx);
				pDC->LineTo(pt);			   //рисуем линию до новой координаты
				
	   			currWavData+=sizeSample;					  //следующий сэмпл
			}
		}
		else			////////  Быстрый способ. Сначала ищем наибольший и наименьший 
		{						 //сэмпл и уж потом рисуем одну линию от большего до меньшего. 
			while ((currWavData<EndWavData) && (pt.x<=WIDTH_GR+STRT_GR_X))
			{
				int ymin,ymax;
																	 	//функция ищет наибольший и наименьший сэмпл
				GetPointMinMax(currWavData,EndWavData,int(Zoom),&ymin,&ymax);	

				if (BitsPerSample==8)
					pt.y=ymin-0x80+STRT_GR_Y+128+(i<<8)-1;
				if (BitsPerSample==16)
					pt.y=ymin/256+STRT_GR_Y+128+(i<<8)-1;

				pDC->MoveTo(pt);										  //двигаем карандаш в координату меньшего сэмпла
				
				if (BitsPerSample==8)
					pt.y=ymax-0x80+STRT_GR_Y+128+(i<<8);
				if (BitsPerSample==16)
					pt.y=ymax/256+STRT_GR_Y+128+(i<<8);
				
				pDC->LineTo(pt);										   // рисуем линию до большего сэмпла
		

				pt.x++;
	   			currWavData+=int(sizeSample*Zoom);			  //следующий блок сэмплов(рисуем ведь сразу блоками, а не по одному)
			}
		}
	}

												

////////////////// рисуем синие линии - границы для фильтра 	////////////////
	HPEN BluePen;
	BluePen = CreatePen(PS_SOLID,2,0xFF0000);   //создаём синий карандаш
	pDC->SelectObject(BluePen);						//применяем его к текущему контексту устройства рисования
		

	pt.x= (LeftSel - StartGraphOffs)/int(Zoom)/sizeSample+STRT_GR_X;
	if (pt.x < STRT_GR_X+WIDTH_GR)
	{
		pt.y=STRT_GR_Y;
		pDC->MoveTo(pt);
		pt.y=STRT_GR_Y+i*256-1;						   
		pDC->LineTo(pt);
	
	}

	pt.x= (RightSel - StartGraphOffs)/int(Zoom)/sizeSample+STRT_GR_X;
	if (pt.x < STRT_GR_X+WIDTH_GR)
	{
		pt.y=STRT_GR_Y;
		pDC->MoveTo(pt);
		pt.y=STRT_GR_Y+i*256-1;						   
		pDC->LineTo(pt);
	
	}

	DeleteObject(BluePen);							   //удаляем больше не нужный карандаш


///// рисуем красную линию, которой будем отмечать конец и начало участка ///////////////////////
	pt.x= (SelectPoint - StartGraphOffs)/int(Zoom)/sizeSample+STRT_GR_X;
	if (pt.x < STRT_GR_X+WIDTH_GR)
	{
		pt.y=STRT_GR_Y;
		HPEN RedPen;
		RedPen = CreatePen(PS_SOLID,2,0x0000FF);   //создаём красный карандаш
		pDC->SelectObject(RedPen);						//применяем его к текущему контексту устройства рисования
		pDC->MoveTo(pt);
		pt.y=STRT_GR_Y+i*256-1;						   
		pDC->LineTo(pt);									 //рисуем красную линию
		DeleteObject(RedPen);							   //удаляем больше не нужный карандаш
	}

//вычисляем размер половины страницы. Это для прокрутки
	bytesScrollSize = (currWavData-(BegWavData+StartGraphOffs))/2;		  
	bytesScrollSize	= bytesScrollSize & 0xFFFFFFFC;					   //выравняем на границу двойного слова, что бы случайно 
																				//прокрутка не закидывала на середину сэмпла(если он из 2х или 4х байт)

}


/////////////////////////////////////////////////////
///// находит наименьший и наибольший сэмпл в участке от pData размером в Num сэмплов 
/////  и если указатель дошёл до pEndData, то выходит без продолжения - данные-то кончились
void CGraph::GetPointMinMax(BYTE* pData, BYTE* pEndData, int Num,int* ymin,int *ymax)
{
	BYTE bymin,bymax;
	short wymin,wymax;
	int i;
																  //в зависимости от размера сэмпла немного разный код получается
	if (BitsPerSample==8)					//беззнаковый семпл в 8 бит		   
	{
		bymin=*pData;
		bymax=*pData;

		for (i=0; i<=Num; i++)										  //именно <=, чтобы он сверял и сэмпл из следующей части , иначе будут разрывы
		{
			 if (*pData>bymax) bymax=*pData;
			 if (*pData<bymin) bymin=*pData;
			 Channels==2? pData+=2: pData++;						 //в зависимости от количества каналов или
																//перепрыгиваем через сэмпл, или читаем следующий
			 if (pData>=pEndData) break;			//если данные кончились, то выходим
		}

		*ymin = bymin;						   	   //записывыаем результат в предоставленные указатели на переменные
		*ymax = bymax;
	} else if (BitsPerSample==16)			//знаковый семпл в 16 бит		   
	{
  		wymin=*(short*)pData;
		wymax=*(short*)pData;

		for (i=0; i<=Num; i++)
		{
			 if (*(short*)pData>wymax) wymax=*(short*)pData;
			 if (*(short*)pData<wymin) wymin=*(short*)pData;
			 Channels==2? pData+=4: pData+=2;				 //в зависимости от количества каналов или
																//перепрыгиваем через сэмпл, или читаем следующий
			 if (pData>=pEndData) break;			//если данные кончились, то выходим
		}

		*ymin = wymin;						   //записывыаем результат в предоставленные указатели на переменные
		*ymax = wymax;
	}
}




////////////////////////////////////////////////////////////////////////////////////////////
////// инициализация данных для графика
void CGraph::SetData(BYTE* pData, int SizeWavData, int Channels, int BitsPerSample)
{
	this->Channels = Channels;
	this->BitsPerSample = BitsPerSample;
	this->BegWavData = pData;
	this->SizeWavData=SizeWavData;
}

///////////////////////////////////////////////////////////////////////////////////
/////////// прокрутка графика
void CGraph::MoveLeft()
{
	if (StartGraphOffs>0)
	{
		StartGraphOffs-=bytesScrollSize;
		if (StartGraphOffs<0)
			StartGraphOffs=0;
 	}	 
	
}

void CGraph::MoveRight()
{
	if (StartGraphOffs<SizeWavData)
	{
		StartGraphOffs+=bytesScrollSize;
		if (StartGraphOffs>SizeWavData)
			StartGraphOffs=SizeWavData;
	} 
}


///////////////////////////////////////////////////////////////////////////////////////////
//////////// левай кнопка мыши - если попал внутрь графика, то сдвигаем красную 
//////////// линию и возвращаем true - если перерисовать график надо
bool CGraph::OnLButtonUp(CPoint pt,double Zoom)
{
	int	OldSelPoint = SelectPoint;
	
	if ((pt.y>=STRT_GR_Y) && (pt.y<STRT_GR_Y+Channels*256))
	{
		pt.x-=STRT_GR_X;
		if ((pt.x>0) && (pt.x < WIDTH_GR))
			SelectPoint = StartGraphOffs +  int(Zoom)*pt.x*(BitsPerSample/8)*Channels;
		//текущая точка = начало этой страницы графика + 
				//   +   уменьшение(или количество сэмплов в пикселе)*коорд X мыши * размер сэмпла в байтах * количество каналов

		if (SelectPoint>SizeWavData)
			SelectPoint=SizeWavData;
	}

	return (OldSelPoint != SelectPoint);	   //если линия сместилась, то нужна перерисовка
}
