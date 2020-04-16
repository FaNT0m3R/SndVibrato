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

	bool OnLButtonUp(CPoint pt, double Zoom);	//������� �� ��� ������ ����. ����� ��� ����������� ������� �����
	void MoveRight();							   //������������� ������� � �����
	void MoveLeft();						
	void SetData(BYTE* pData, int SizeWavData, int Channels, int BitsPerSample);	//������������ ������ ��� �������
	void DrawGraph(CDC *pDC, double Zoom, int LeftSel, int RightSel);					//������ ������
	
	int SelectPoint;				//��������� ������� ����� �� �������� �������. ����� ��� ����� ����� �������� � �����/������ ������� ��� �������

private:
	void GetPointMinMax(BYTE* pData, BYTE* pEndData, int Num,int* ymin,int *ymax);	//������� ����������� � ����. �����. ��� ��������� ��������� �������
	
	int bytesScrollSize;	 //������� ���� ���� ���������/������ �� ������ ��������� �������, ��� �� ���������� ��� �� �������� ��������
	int StartGraphOffs;		 //������ ��������� ������� ������������ ������ ������	
										//�����(�.�. BegWavData+StartGraphOffs - ������ ����� ��� ��������� �������)
	int Channels;					//���������� �������
	int BitsPerSample;			 //��� �� ����� (8 ��� 16)
	BYTE* BegWavData;		 //��������� �� ����� �����
	int SizeWavData;			 //������ ������
	
};

#endif // !defined(AFX_GRAPH_H__91043751_86BA_49EE_81F3_2083B7D29BA9__INCLUDED_)
