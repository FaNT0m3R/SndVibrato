// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sndVibr.h"
#include "Sound.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSound::CSound()
{
	StateWritenSnd = NO_WRITEN;				   //начальное состояние плейера
	wvFormat=0;											 //пишем сюда 0 , что бы было ясно - открыта ли мелодия или нет
}

CSound::~CSound()
{}


///////////////////////////////////////////////////////////////////////////////
//открываем трек
//////////////////////////
BOOL CSound::OpenWav(void* WavOld,void *WavNew)
{

	if (StateWritenSnd != NO_WRITEN)
	{															//если было что-то открыто, то всё закрываем, память освобождаем
		StateWritenSnd = NO_WRITEN;
		waveOutClose(wvHndl);
	}



/////////////                Открываем звуковое устройство и		  ////////////////////
////////////   проверяем, может ли Винда эти треки воспроизвести  //////////////

	 
				// в начале wav-файла должен быть тег 'WAVE', после него идут нужные данные формата и звука
				// после тега 'fmt ' идут размер секции(DWORD) и данные формата
				// после тега 'data' идут размер звуковых данных(DWORD) и сами звуковые данные
	int SndDataSize;				//сюда запишем размер звуковых данных

	BYTE* t=(BYTE*)WavOld;

//Ищем тег 'WAVE'
	while (memcmp(t,"WAVE",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;

	if (t>=(BYTE*)WavOld+SndFileSize){			//ошибка, если файл кончился, а нужных тегов нет
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}


//Ищем тег 'fmt ' (4ый символ - пробел обязательно).
	while (memcmp(t,"fmt ",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;

	if (t>=(BYTE*)WavOld+SndFileSize){			//ошибка, если файл кончился, а нужных тегов нет
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}
	wvFormat = (WAVEFORMATEX*)(t+8); //(имя тега - 4 байта и размер секции - 4 байта - нам не нужен)

	t+=*(DWORD*)(t+4)+8 ;				//пропускаем всю секцию с форматом, теперь нам нужны данные звука
		
//Ищем тег 'data' (после него 4 байта - это размер самих звуковых данных, а дальше и сами данные)
	while (memcmp(t,"data",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;
	SndDataSize = *(DWORD*)(t+4);

	if (t>=(BYTE*)WavOld+SndFileSize){			//ошибка, если файл кончился, а нужных тегов нет
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}

	t+=8;		  //теперь t указывает на начало мелодии


												//получаем в wvHndl Хэндл звукового устройства, заодно проверяем 
												//поддерживает ли он такой формат wav-файла
												//Потом функциям будем передавать этот самый wvHndl
	if (waveOutOpen(&wvHndl,WAVE_MAPPER,wvFormat, NULL, NULL, CALLBACK_NULL)){			
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}
		
///////////заполняем данный о неизменённой дорожке/////////
																		//размер самих звуковых данных(без всяких заголовков)
	wvHdrOld.dwBufferLength=SndDataSize;
	wvHdrOld.dwLoops=1;										//повторов не надо
	wvHdrOld.dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;		//этот блок данных первый и последний
	wvHdrOld.lpData=(char*)t;	//указатель на сами звуковые данные

	if (waveOutPrepareHeader(wvHndl,&wvHdrOld,sizeof(wvHdrOld))){		//даём знать звуковому драйверу о нашей дорожке
		waveOutClose(wvHndl);															 
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}
  
////////тоже самое, только для изменённой дорожки///////////////
	wvHdrNew.dwBufferLength=SndDataSize;
	wvHdrNew.dwLoops=1;
	wvHdrNew.dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	wvHdrNew.lpData=(char*)t-(char*)WavOld+(char*)WavNew;

	if (waveOutPrepareHeader(wvHndl,&wvHdrNew,sizeof(wvHdrNew))){
		waveOutClose(wvHndl);
		AfxMessageBox("Ошибка открытия файла. Возможно, формат не поддерживается");
		return FALSE;
	}

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////
////////// ФУНКЦИИ ДЛЯ ПЛЕЙЕРА  ///////////////////////////////////////////

//////// PLAY
void CSound::PlayWave(bool bWithFilter)
{
	int MustTrack;
	WAVEHDR* MustHdr;

	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;	 //в  MustTrack нужное состояние плейера. если не совпадает, 
																													//то переоткрываем другую дорожку

  //если         дорожка не запущена    или          запущена не та
	if ((StateWritenSnd == NO_WRITEN) || (StateWritenSnd != MustTrack))
	{																								  
		bWithFilter? MustHdr=&wvHdrNew : MustHdr=&wvHdrOld;

		if (StateWritenSnd != NO_WRITEN)
			waveOutReset(wvHndl);				 //если всё таки какой-то трек запущен, то сбросить 
																//к начальному состоянию(иначе другой не захочет открываться)

		if (waveOutWrite(wvHndl, MustHdr,sizeof(WAVEHDR))){
			waveOutClose(wvHndl);											//ничего не вышло, потому закрываем трек, и сообщаем об ошибке
			AfxMessageBox("Не удаётся воспроизвести файл");
			return;
		}
		StateWritenSnd = MustTrack;					//записываем в состояние какая дорожка сейчас играет - страя или новая
	}
	else	
	{
		waveOutRestart(wvHndl);						//если эта дорожка уже запущена, то просто начинаем играть заново
	}
}

//////// PAUSE
void CSound::PauseWave(bool bWithFilter)
{
	int MustTrack;
	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;


	//если есть что ставить на паузу и дорожка та, что играет
	if ((StateWritenSnd != NO_WRITEN) && (StateWritenSnd == MustTrack))			
		waveOutPause(wvHndl);													//ставим на паузу
}

//////// STOP
void CSound::StopWave(bool bWithFilter)
{
	int MustTrack;
	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;


	//если есть что останавливать и дорожка та, что играет
	if ((StateWritenSnd != NO_WRITEN) && (StateWritenSnd == MustTrack))			
	{
		waveOutReset(wvHndl);
		StateWritenSnd = NO_WRITEN;			 //записываем, что дорожка не играет и надо загружать в звуковое устройство заново
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///// возвращает данные о новой дорожке или -1 (с первоначальной мы ведь всё равно ничего не делаем)
void* CSound::GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData)
{
	if (wvFormat)
	{
		*Channels = wvFormat->nChannels;
		*BitsPerSample = wvFormat->wBitsPerSample;
		*sizeWavData = wvHdrNew.dwBufferLength;
		return wvHdrNew.lpData;
	}
	else
		return (void*)-1;										   //если ничего не открыто, то вернём -1
}

/////////////////////////////////////
/*double CSound::GetTimeTrack(void* CurrPosData)
{
	int SampleSize=wvFormat->nChannels*(wvFormat->wBitsPerSample/8);
	return ((BYTE*)CurrPosData-(BYTE*)wvHdrNew.lpData)/SampleSize/wvFormat->nSamplesPerSec;
}
  */

///////////////////////////////////////
//////// возвращает состояни плейера
int CSound::GetStatePlayer()
{
	return StateWritenSnd;
}


short a4[8] = {0x8000,0x7fff,0x8000,0x7fff,0,0x7fff,0x8000,0x7fff};
/////////////////////////////////////////////////////////////////
//// наложить эффект вибрато на дорожку
///// 	 amin, amax - измеряется в процентах - от 0 до 100. Хотя можно и другие значения, но эффект будет непредсказуемый
///// 	 start, end - это относительные указатели на данные звуко. Относительно wvHdrNew.lpData.
void CSound::ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax)
{
	BYTE* t;									//указатель на данные. Будет бегать по wav'у.
	int period = int(wvFormat->nSamplesPerSec/freq);	   //длина волны синусоиды
	int samplPerSec = int(wvFormat->nSamplesPerSec);
	int i;
	BYTE* begData = (BYTE*)wvHdrNew.lpData; // переменная для читаемости кода
	if (end > wvHdrNew.dwBufferLength)
		end = wvHdrNew.dwBufferLength;
	

	t=start+begData;

	double m;			//На это будем умножать величину сэмпла.


//////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
	///////// region	  ПРОБЫ
#define pi 3.14159265	

	float smax=0;
	float smin=0;

	 	while (t<end+begData)								  //пока данные не кончились
		{
			//for (i=0; i<period; i++)
			for (i=0; i<5;i++)
			{
				//int k2 =    (i %2);

				//float s =  sin(i*pi*2 * freq /samplPerSec);
				float s =  sin(i*pi*2/1.7);
				s<smin?smin=s:s=s;
				s>smax?smax=s:s=s;
				short w = s*0x7FFF;		//k2*0xFFFF-0x8000;	//k*0xFFFF-0x8000;   //sin(i*pi /period)*0x7FFF;    
				

				//m = sin(i*2*pi/amin);
				*(WORD*)t = w;								  //вновь из char делаем BYTE

				t+=2;
				if (t>=end+begData) break;
			}
			
			/*i = 0;
			for (; i<period /4*2; i++)
			{
				int k4 =    (i %8);
				short w = a4[k4];
				*(WORD*)t = w;
				t+=2;
				if (t>=end+begData) break;
			}
			 
			   */

		}

	
	return;

	///////// end region
//////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
	/////////// вибарто у нас будет в форме синусоиды

																		
	if (wvFormat->wBitsPerSample==8)				//опять разобьём код на 2 части: для сэмпла в 8 бит и в 16 бит
	{
		while (t<end+begData)								  //пока данные не кончились
		{
			for (i=0; i<=period; i++)
			{
				m = sin(3.1416 /period*i) * (amax-amin)/100;		//значение синуса для текущего i умножаем на ширину амплитуды
				char w =int( (*(BYTE*)t-0x80) * m +amin/100*256 );	 //умножаем на синусоиду и прибавляем нижний порог амплитуды.
																								//BYTE беззнаковый, делаем его знаковым, т. е. отнимаем 0x80
				*(BYTE*)t = w+0x80;								  //вновь из char делаем BYTE

				if (wvFormat->nChannels==2)
				{
					t++;
					m = sin(3.1416 /period*i) * (amax-amin)/100;		//значение синуса для текущего i умножаем на ширину амплитуды
					char w =int( (*(BYTE*)t-0x80) * m +amin/100*256 );	 //умножаем на синусоиду и прибавляем нижний порог амплитуды
					*(BYTE*)t = w+0x80;
				}

				t++;
				if (t>=end+begData) break;
			}
		}
	} else if (wvFormat->wBitsPerSample==16)
	{
		while (t<end+begData)
		{
			for (i=0; i<=period; i++)
			{
				m = sin(3.1416 /period*i) * (amax-amin)/100 ;			//значение синуса для текущего i умножаем на ширину амплитуды
				short w =int( *(short*)t * m +(amin-50)/100*65536 );	 //умножаем на синусоиду и прибавляем нижний порог амплитуды
				*(short*)t = w;

				if (wvFormat->nChannels==2)
				{
					t+=2;
					m = sin(3.1416 /period*i) * (amax-amin)/100 ;			//значение синуса для текущего i умножаем на ширину амплитуды
					short w =int( *(short*)t * m +(amin-50)/100*65536 );	 //умножаем на синусоиду и прибавляем нижний порог амплитуды
					*(short*)t = w;
				}

				t+=2;
				if (t>=end+begData) break;
			}
		}
	}
}



