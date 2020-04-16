
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
		//карта сообщений. Обработывает только удаление файла, так как осталные команды сначала принимает на себя Вид
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
//открываем трек
BOOL CSndVibrDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


///////////////// Загружаем треки в память  /////////////
    CFile fSnd;
    CFileException fe;

	if (Snd.GetStatePlayer() != NO_WRITEN)
	{															//если было что-то загружено, то память освобождаем
		free(SndFileOld);
		free(SndFileNew);
	}


	if (!fSnd.Open(lpszPathName, CFile::modeRead, &fe)) {
		fe.ReportError();
		return FALSE;
    }
	TRY 
	{
		SndFileSize = fSnd.GetLength();									   //узнаём размер файла
		SndFileOld = malloc(SndFileSize);								  // выделяем память для обеих треков
		SndFileNew = malloc(SndFileSize);
		fSnd.Read(SndFileOld, SndFileSize);								 //и полностью их читаем в память
		memcpy(SndFileNew, SndFileOld, SndFileSize);			  //в память для нового трека копируем первоначальный трек
		fSnd.Close();
	}
	CATCH_ALL(e) 
	{
		fSnd.Abort();
		e->ReportError();
		return FALSE;
	}
	END_CATCH_ALL

	Snd.OpenWav( SndFileOld,SndFileNew);				 //задаём данные для класса плейера-фильтра

									//если всё отлично, то записываем путь к файлу, потом будет, что удалять :)
	sSoundFile = lpszPathName;			
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//Сохраняем трек
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
		fSnd.Write(SndFileNew, SndFileSize);			//просто записываем данные нового трека в файл
		fSnd.Close();
		sSoundFile = lpszPathName;				//записываем новый путь к файлу, удалять будем 
																		//    именно новый файл, старый уже не интересует
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
//Удаляем трек
//////////////////////////

void CSndVibrDoc::OnRemoveDocument()
{
	CFile fSnd;

	if (sSoundFile.GetLength())
	{
		if( AfxMessageBox("Вы действительно хотите удалить файл " + sSoundFile + "?", MB_YESNO)==IDYES)
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
////////// ФУНКЦИИ ДЛЯ ПЛЕЙЕРА  ///////////////////////////////////////////
//////// просто пересылают команды классу CSound. Нужны только для того, что бы Вид имел доступ к плейеру
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
//////  заполняет данные о wav'е и возвращает указатель на звуковые данные
void* CSndVibrDoc::GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData)
{
   return Snd.GetWavData(Channels, BitsPerSample, sizeWavData);
}

//////////////////////////////////////////////////////////////////
//// проверяет корректность параметров и вызывает фильтр из класса CSound
void CSndVibrDoc::ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax)
{
	if ((start<end) && (amin<amax))
		Snd.ApplyVibrato(start, end, freq, amin, amax);
}


/////////////////////////////////////////////////////////////////////////
///// отменяет все изменения
void CSndVibrDoc::ResetWav()
{
	memcpy(SndFileNew, SndFileOld, SndFileSize);			  //в память для нового трека копируем первоначальный трек
}

//////////////////////////////////////////////////////////
/// возвращает true, если wav открыт
bool CSndVibrDoc::IsLoadWav()
{
	  return (sSoundFile.GetLength()!=0);
}
