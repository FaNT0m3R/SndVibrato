// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_)
#define AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/// значения переменной StateWritenSnd
#define NO_WRITEN 0						   //дорожка не воспроизводится
#define WRITEN_OLD 1					//первоначальная дорожка воспроизводится( или на паузе)
#define WRITEN_NEW 2				   // изменённая дорожка



/////////////////////////////////////////////////////////////////////////////////
/////// класс для работы со звуком. Использует MCI функции. Содержит в себе плейер и фильтр
class CSound  
{
public:
	CSound();
	virtual ~CSound();
	
	void ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax);		//применить фильтр
	int GetStatePlayer();									//возвращает значение StateWritenSnd
	BOOL OpenWav(void* WavOld,void *WavNew);	//инициализация звукового устройства, проверка формата wav'а
	void PlayWave(bool bWithFilter);				   //функции для управления плейером
	void PauseWave(bool bWithFilter);
	void StopWave(bool bWithFilter);
	void* GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData);

private:
	int StateWritenSnd;			//Даёт знать, какая из дорожек сейчас воспроизводится(или стоит на паузе)
										// Если NO_WRITEN, то никакая дорожка не воспроизводится
	int SndFileSize;		  //Размер звукового файла
	CString sSoundFile;	  //Имя и путь открытой дорожки

	HWAVEOUT wvHndl;	//Хэндл звукового устройства
	WAVEFORMATEX* wvFormat;		//Структура с данными о формате дорожки. Так как 
													//формат обеих дорожек одинаков, то нужна только одна структура
	WAVEHDR wvHdrOld;		//данные о воспроизведении. Так как содержит указатель на сами 
	WAVEHDR wvHdrNew;		//  звуковые  данные, то нужно 2 экземпляра - для первоначальной и для изменённой

};

#endif // !defined(AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_)
