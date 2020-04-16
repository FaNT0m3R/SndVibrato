
// sndVibrDoc.cpp : implementation of the CSndVibrDoc class
//

#include "stdafx.h"
#include "sndVibr.h"
#include "sndVibrDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSndVibrDoc

IMPLEMENT_DYNCREATE(CSndVibrDoc, CDocument)

////////////////////////////////
		//����� ���������. ������������ ������ �������� �����, ��� ��� �������� ������� ������� ��������� �� ���� ���
BEGIN_MESSAGE_MAP(CSndVibrDoc, CDocument)					
	//{{AFX_MSG_MAP(CSndVibrDoc)
	ON_COMMAND(ID_FILE_DELETE, OnRemoveDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSndVibrDoc construction/destruction

CSndVibrDoc::CSndVibrDoc()
{}

CSndVibrDoc::~CSndVibrDoc()
{}

///////////////////////////////////////////////////////////////////////////////
//��������� ����
BOOL CSndVibrDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


///////////////// ��������� ����� � ������  /////////////
    CFile fSnd;
    CFileException fe;

	if (Snd.GetStatePlayer() != NO_WRITEN)
	{															//���� ���� ���-�� ���������, �� ������ �����������
		free(SndFileOld);
		free(SndFileNew);
	}


	if (!fSnd.Open(lpszPathName, CFile::modeRead, &fe)) {
		fe.ReportError();
		return FALSE;
    }
	TRY 
	{
		SndFileSize = fSnd.GetLength();									   //����� ������ �����
		SndFileOld = malloc(SndFileSize);								  // �������� ������ ��� ����� ������
		SndFileNew = malloc(SndFileSize);
		fSnd.Read(SndFileOld, SndFileSize);								 //� ��������� �� ������ � ������
		memcpy(SndFileNew, SndFileOld, SndFileSize);			  //� ������ ��� ������ ����� �������� �������������� ����
		fSnd.Close();
	}
	CATCH_ALL(e) 
	{
		fSnd.Abort();
		e->ReportError();
		return FALSE;
	}
	END_CATCH_ALL

	Snd.OpenWav( SndFileOld,SndFileNew);				 //����� ������ ��� ������ �������-�������

									//���� �� �������, �� ���������� ���� � �����, ����� �����, ��� ������� :)
	sSoundFile = lpszPathName;			
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//��������� ����
BOOL CSndVibrDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile fSnd;
    CFileException fe;
	
	if (!fSnd.Open(lpszPathName,  CFile::modeWrite | CFile::modeCreate, &fe)) {
		fe.ReportError();
		return FALSE;
    }
	
	TRY 
	{
		fSnd.Write(SndFileNew, SndFileSize);			//������ ���������� ������ ������ ����� � ����
		fSnd.Close();
		sSoundFile = lpszPathName;				//���������� ����� ���� � �����, ������� ����� 
																		//    ������ ����� ����, ������ ��� �� ����������
	}
	CATCH_ALL(e) 
	{
		fSnd.Abort();
		e->ReportError();
		return FALSE;
	}
	END_CATCH_ALL
	   
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//������� ����
//////////////////////////

void CSndVibrDoc::OnRemoveDocument()
{
	CFile fSnd;

	if (sSoundFile.GetLength())
	{
		if( AfxMessageBox("�� ������������� ������ ������� ���� " + sSoundFile + "?", MB_YESNO)==IDYES)
		{
			TRY{
				fSnd.Remove(sSoundFile);
				sSoundFile = "";
			}
			CATCH_ALL(e) {
				e->ReportError();
			}
			END_CATCH_ALL
		}
	}
}
	 

//////////////////////////////////////////////////////////////////////////////////
////////// ������� ��� �������  ///////////////////////////////////////////
//////// ������ ���������� ������� ������ CSound. ����� ������ ��� ����, ��� �� ��� ���� ������ � �������
//////// PLAY
void CSndVibrDoc::PlayWave(bool bWithFilter)
{
	Snd.PlayWave(bWithFilter);
}

//////// PAUSE
void CSndVibrDoc::PauseWave(bool bWithFilter)
{
	Snd.PauseWave(bWithFilter);
}

//////// STOP
void CSndVibrDoc::StopWave(bool bWithFilter)
{
	Snd.StopWave(bWithFilter);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////  ��������� ������ � wav'� � ���������� ��������� �� �������� ������
void* CSndVibrDoc::GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData)
{
   return Snd.GetWavData(Channels, BitsPerSample, sizeWavData);
}

//////////////////////////////////////////////////////////////////
//// ��������� ������������ ���������� � �������� ������ �� ������ CSound
void CSndVibrDoc::ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax)
{
	if ((start<end) && (amin<amax))
		Snd.ApplyVibrato(start, end, freq, amin, amax);
}


/////////////////////////////////////////////////////////////////////////
///// �������� ��� ���������
void CSndVibrDoc::ResetWav()
{
	memcpy(SndFileNew, SndFileOld, SndFileSize);			  //� ������ ��� ������ ����� �������� �������������� ����
}

//////////////////////////////////////////////////////////
/// ���������� true, ���� wav ������
bool CSndVibrDoc::IsLoadWav()
{
	  return (sSoundFile.GetLength()!=0);
}
