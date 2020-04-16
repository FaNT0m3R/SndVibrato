// sndVibrDoc.h : interface of the CSndVibrDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNDVIBRDOC_H__CD385E88_3E67_44ED_B095_74353E943DAB__INCLUDED_)
#define AFX_SNDVIBRDOC_H__CD385E88_3E67_44ED_B095_74353E943DAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sound.h"



//////////////////////////////////////////////////////////
///// Класс Документа. Отвечает за открыттие, закрытие, удаление и обработку документа
class CSndVibrDoc : public CDocument
{
protected:
	CSndVibrDoc();
	DECLARE_DYNCREATE(CSndVibrDoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSndVibrDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);			//обработчик открытия документа
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);			//обработчик сохранения документа
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsLoadWav();
	void ResetWav();								  //сбрасывает все изменения. Просто копирует данные из первоначального звука в наш подопытный
	void* GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData);	   //записывает характеристики wav'а и возвращает 
																															// указатель на звуковые даные
	void ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax);		//вызывает функцию фильтра класса CSound

	//функции для управления воспроизведением.
	//фактически они только вызывают соответствующие функции класса CSound. Нужны для доступа CSndVibrView к плейеру
	void PlayWave(bool bWithFilter);	//в bWithFilter определяется - какуй из дорожек воспроизводить, если true - то подопытную, иначе первоначальную
	void PauseWave(bool bWithFilter);
	void StopWave(bool bWithFilter);						
	
	virtual ~CSndVibrDoc();


// Generated message map functions
protected:
	//{{AFX_MSG(CSndVibrDoc)
	afx_msg void OnRemoveDocument();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void* SndFileNew;	  //указатель на данные профильтрованной дорожки
	void* SndFileOld;		//указатель на данные первоначальной дорожки
	CString sSoundFile;	  //Имя и путь открытой дорожки
	int SndFileSize;		  //Размер звукового файла

	CSound Snd;				//класс для работы со звуком(на самом деле в нём плейер и 
								//  фильтр, функции в этом классе только пересылают данные этому классу)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNDVIBRDOC_H__CD385E88_3E67_44ED_B095_74353E943DAB__INCLUDED_)
