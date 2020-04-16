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
	StartGraphOffs = 0;					//��������� �������������.
	SelectPoint=0;
	BegWavData=0;	
}

CGraph::~CGraph()
{}

void CGraph::DrawGraph(CDC *pDC, double Zoom, int LeftSel, int RightSel)
{
 	BYTE *EndWavData;			//����� ������
	BYTE* currWavData;			 //������� ���������, �� � ��� ����� ������ �� ������ � ����� ���� ������ ����� ��� �������

	int sizeSample;											//������ ������, ��� ����� ���������� � ���������

	CPoint pt;								 
	double fx;								  //��������� X �����, ������ ����� ������ �������������
	double step=1/Zoom;						//��� �� X


	if (BegWavData==0)				//���� ������ �� ����, �� �����, ������ ��������
		return;

	EndWavData = BegWavData+SizeWavData;	   


	if ((BitsPerSample==8) && (Channels==1))	  //��������� ������ ������(���� ��� ������, 
		sizeSample=1;												  //�� ����� ������� ����������, ��� ����� �������)
	if ((BitsPerSample==8) && (Channels==2))
		sizeSample=2;
	if ((BitsPerSample==16) && (Channels==1))
		sizeSample=2;
	if ((BitsPerSample==16) && (Channels==2))
		sizeSample=4;

	for (int i=0; i<Channels; i++)				//���� 2 ������, �� ���������� 2 �������, ����� ������ 1
	{
		currWavData = BegWavData+StartGraphOffs;		
		
		pDC->Rectangle(5,
								STRT_GR_Y+(256*i)-1,
								WIDTH_GR+5+2,
								STRT_GR_Y+256+(256*i));				//������� ��������������. �������� ������ ������

		fx = STRT_GR_X;													  //����� ��������� ����������
		pt.x = STRT_GR_X;
		pt.y=STRT_GR_Y+128+(256*i);
		pDC->MoveTo(pt);												  //������� �������� � ��������� �������

									//������ ��� i=0, ������ ������ �� ����� � � ��������� ������ ����� ������ ����/�����
										//������ ���(���� ������ 2), i=1 � ��������� ��������� �� ������ ����/�����
		if (BitsPerSample==8)
			currWavData+=1*i;
		if (BitsPerSample==16)
			currWavData+=2*i;
		

										  //���� ���������� �����, �� �������� ����� ������ ����� � ��������� �� �����
										//   ����� ������������ ������������ � ����������� �������� ������ � ������ 
										//����� ���� ������� ����� ������, �� ����� ��� ��� ��� ���������

		if (Zoom<16)					
		{
						/// ��������� ������(��� ����������� ����������)
			while ((currWavData<EndWavData) && (pt.x<=WIDTH_GR+STRT_GR_X))  //������ ��� ���� �� �������� ������, 
			{																										//��� ���� �� �������� ������ ��� �������
				if (BitsPerSample==8)
					pt.y=*(BYTE*)currWavData-0x80+STRT_GR_Y+128+(i<<8);	//����� � 8 ��� ����� ������� �� 0 �� 256(�. �. �� 
				if (BitsPerSample==16)																				//�����������) � ������� �������� 0�80
					pt.y=(*(short*)currWavData>>8)+STRT_GR_Y+128+(i<<8);	//����� � 16 ��� �������� =  �� -32768 �� 32767
				fx+=step;
				pt.x=int(fx);
				pDC->LineTo(pt);			   //������ ����� �� ����� ����������
				
	   			currWavData+=sizeSample;					  //��������� �����
			}
		}
		else			////////  ������� ������. ������� ���� ���������� � ���������� 
		{						 //����� � �� ����� ������ ���� ����� �� �������� �� ��������. 
			while ((currWavData<EndWavData) && (pt.x<=WIDTH_GR+STRT_GR_X))
			{
				int ymin,ymax;
																	 	//������� ���� ���������� � ���������� �����
				GetPointMinMax(currWavData,EndWavData,int(Zoom),&ymin,&ymax);	

				if (BitsPerSample==8)
					pt.y=ymin-0x80+STRT_GR_Y+128+(i<<8)-1;
				if (BitsPerSample==16)
					pt.y=ymin/256+STRT_GR_Y+128+(i<<8)-1;

				pDC->MoveTo(pt);										  //������� �������� � ���������� �������� ������
				
				if (BitsPerSample==8)
					pt.y=ymax-0x80+STRT_GR_Y+128+(i<<8);
				if (BitsPerSample==16)
					pt.y=ymax/256+STRT_GR_Y+128+(i<<8);
				
				pDC->LineTo(pt);										   // ������ ����� �� �������� ������
		

				pt.x++;
	   			currWavData+=int(sizeSample*Zoom);			  //��������� ���� �������(������ ���� ����� �������, � �� �� ������)
			}
		}
	}

												

////////////////// ������ ����� ����� - ������� ��� ������� 	////////////////
	HPEN BluePen;
	BluePen = CreatePen(PS_SOLID,2,0xFF0000);   //������ ����� ��������
	pDC->SelectObject(BluePen);						//��������� ��� � �������� ��������� ���������� ���������
		

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

	DeleteObject(BluePen);							   //������� ������ �� ������ ��������


///// ������ ������� �����, ������� ����� �������� ����� � ������ ������� ///////////////////////
	pt.x= (SelectPoint - StartGraphOffs)/int(Zoom)/sizeSample+STRT_GR_X;
	if (pt.x < STRT_GR_X+WIDTH_GR)
	{
		pt.y=STRT_GR_Y;
		HPEN RedPen;
		RedPen = CreatePen(PS_SOLID,2,0x0000FF);   //������ ������� ��������
		pDC->SelectObject(RedPen);						//��������� ��� � �������� ��������� ���������� ���������
		pDC->MoveTo(pt);
		pt.y=STRT_GR_Y+i*256-1;						   
		pDC->LineTo(pt);									 //������ ������� �����
		DeleteObject(RedPen);							   //������� ������ �� ������ ��������
	}

//��������� ������ �������� ��������. ��� ��� ���������
	bytesScrollSize = (currWavData-(BegWavData+StartGraphOffs))/2;		  
	bytesScrollSize	= bytesScrollSize & 0xFFFFFFFC;					   //��������� �� ������� �������� �����, ��� �� �������� 
																				//��������� �� ���������� �� �������� ������(���� �� �� 2� ��� 4� ����)

}


/////////////////////////////////////////////////////
///// ������� ���������� � ���������� ����� � ������� �� pData �������� � Num ������� 
/////  � ���� ��������� ����� �� pEndData, �� ������� ��� ����������� - ������-�� ���������
void CGraph::GetPointMinMax(BYTE* pData, BYTE* pEndData, int Num,int* ymin,int *ymax)
{
	BYTE bymin,bymax;
	short wymin,wymax;
	int i;
																  //� ����������� �� ������� ������ ������� ������ ��� ����������
	if (BitsPerSample==8)					//����������� ����� � 8 ���		   
	{
		bymin=*pData;
		bymax=*pData;

		for (i=0; i<=Num; i++)										  //������ <=, ����� �� ������ � ����� �� ��������� ����� , ����� ����� �������
		{
			 if (*pData>bymax) bymax=*pData;
			 if (*pData<bymin) bymin=*pData;
			 Channels==2? pData+=2: pData++;						 //� ����������� �� ���������� ������� ���
																//������������� ����� �����, ��� ������ ���������
			 if (pData>=pEndData) break;			//���� ������ ���������, �� �������
		}

		*ymin = bymin;						   	   //����������� ��������� � ��������������� ��������� �� ����������
		*ymax = bymax;
	} else if (BitsPerSample==16)			//�������� ����� � 16 ���		   
	{
  		wymin=*(short*)pData;
		wymax=*(short*)pData;

		for (i=0; i<=Num; i++)
		{
			 if (*(short*)pData>wymax) wymax=*(short*)pData;
			 if (*(short*)pData<wymin) wymin=*(short*)pData;
			 Channels==2? pData+=4: pData+=2;				 //� ����������� �� ���������� ������� ���
																//������������� ����� �����, ��� ������ ���������
			 if (pData>=pEndData) break;			//���� ������ ���������, �� �������
		}

		*ymin = wymin;						   //����������� ��������� � ��������������� ��������� �� ����������
		*ymax = wymax;
	}
}




////////////////////////////////////////////////////////////////////////////////////////////
////// ������������� ������ ��� �������
void CGraph::SetData(BYTE* pData, int SizeWavData, int Channels, int BitsPerSample)
{
	this->Channels = Channels;
	this->BitsPerSample = BitsPerSample;
	this->BegWavData = pData;
	this->SizeWavData=SizeWavData;
}

///////////////////////////////////////////////////////////////////////////////////
/////////// ��������� �������
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
//////////// ����� ������ ���� - ���� ����� ������ �������, �� �������� ������� 
//////////// ����� � ���������� true - ���� ������������ ������ ����
bool CGraph::OnLButtonUp(CPoint pt,double Zoom)
{
	int	OldSelPoint = SelectPoint;
	
	if ((pt.y>=STRT_GR_Y) && (pt.y<STRT_GR_Y+Channels*256))
	{
		pt.x-=STRT_GR_X;
		if ((pt.x>0) && (pt.x < WIDTH_GR))
			SelectPoint = StartGraphOffs +  int(Zoom)*pt.x*(BitsPerSample/8)*Channels;
		//������� ����� = ������ ���� �������� ������� + 
				//   +   ����������(��� ���������� ������� � �������)*����� X ���� * ������ ������ � ������ * ���������� �������

		if (SelectPoint>SizeWavData)
			SelectPoint=SizeWavData;
	}

	return (OldSelPoint != SelectPoint);	   //���� ����� ����������, �� ����� �����������
}
