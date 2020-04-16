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
	StateWritenSnd = NO_WRITEN;				   //��������� ��������� �������
	wvFormat=0;											 //����� ���� 0 , ��� �� ���� ���� - ������� �� ������� ��� ���
}

CSound::~CSound()
{}


///////////////////////////////////////////////////////////////////////////////
//��������� ����
//////////////////////////
BOOL CSound::OpenWav(void* WavOld,void *WavNew)
{

	if (StateWritenSnd != NO_WRITEN)
	{															//���� ���� ���-�� �������, �� �� ���������, ������ �����������
		StateWritenSnd = NO_WRITEN;
		waveOutClose(wvHndl);
	}



/////////////                ��������� �������� ���������� �		  ////////////////////
////////////   ���������, ����� �� ����� ��� ����� �������������  //////////////

	 
				// � ������ wav-����� ������ ���� ��� 'WAVE', ����� ���� ���� ������ ������ ������� � �����
				// ����� ���� 'fmt ' ���� ������ ������(DWORD) � ������ �������
				// ����� ���� 'data' ���� ������ �������� ������(DWORD) � ���� �������� ������
	int SndDataSize;				//���� ������� ������ �������� ������

	BYTE* t=(BYTE*)WavOld;

//���� ��� 'WAVE'
	while (memcmp(t,"WAVE",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;

	if (t>=(BYTE*)WavOld+SndFileSize){			//������, ���� ���� ��������, � ������ ����� ���
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}


//���� ��� 'fmt ' (4�� ������ - ������ �����������).
	while (memcmp(t,"fmt ",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;

	if (t>=(BYTE*)WavOld+SndFileSize){			//������, ���� ���� ��������, � ������ ����� ���
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}
	wvFormat = (WAVEFORMATEX*)(t+8); //(��� ���� - 4 ����� � ������ ������ - 4 ����� - ��� �� �����)

	t+=*(DWORD*)(t+4)+8 ;				//���������� ��� ������ � ��������, ������ ��� ����� ������ �����
		
//���� ��� 'data' (����� ���� 4 ����� - ��� ������ ����� �������� ������, � ������ � ���� ������)
	while (memcmp(t,"data",4) && (t<(BYTE*)WavOld+SndFileSize))
		t++;
	SndDataSize = *(DWORD*)(t+4);

	if (t>=(BYTE*)WavOld+SndFileSize){			//������, ���� ���� ��������, � ������ ����� ���
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}

	t+=8;		  //������ t ��������� �� ������ �������


												//�������� � wvHndl ����� ��������� ����������, ������ ��������� 
												//������������ �� �� ����� ������ wav-�����
												//����� �������� ����� ���������� ���� ����� wvHndl
	if (waveOutOpen(&wvHndl,WAVE_MAPPER,wvFormat, NULL, NULL, CALLBACK_NULL)){			
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}
		
///////////��������� ������ � ����������� �������/////////
																		//������ ����� �������� ������(��� ������ ����������)
	wvHdrOld.dwBufferLength=SndDataSize;
	wvHdrOld.dwLoops=1;										//�������� �� ����
	wvHdrOld.dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;		//���� ���� ������ ������ � ���������
	wvHdrOld.lpData=(char*)t;	//��������� �� ���� �������� ������

	if (waveOutPrepareHeader(wvHndl,&wvHdrOld,sizeof(wvHdrOld))){		//��� ����� ��������� �������� � ����� �������
		waveOutClose(wvHndl);															 
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}
  
////////���� �����, ������ ��� ��������� �������///////////////
	wvHdrNew.dwBufferLength=SndDataSize;
	wvHdrNew.dwLoops=1;
	wvHdrNew.dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	wvHdrNew.lpData=(char*)t-(char*)WavOld+(char*)WavNew;

	if (waveOutPrepareHeader(wvHndl,&wvHdrNew,sizeof(wvHdrNew))){
		waveOutClose(wvHndl);
		AfxMessageBox("������ �������� �����. ��������, ������ �� ��������������");
		return FALSE;
	}

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////
////////// ������� ��� �������  ///////////////////////////////////////////

//////// PLAY
void CSound::PlayWave(bool bWithFilter)
{
	int MustTrack;
	WAVEHDR* MustHdr;

	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;	 //�  MustTrack ������ ��������� �������. ���� �� ���������, 
																													//�� ������������� ������ �������

  //����         ������� �� ��������    ���          �������� �� ��
	if ((StateWritenSnd == NO_WRITEN) || (StateWritenSnd != MustTrack))
	{																								  
		bWithFilter? MustHdr=&wvHdrNew : MustHdr=&wvHdrOld;

		if (StateWritenSnd != NO_WRITEN)
			waveOutReset(wvHndl);				 //���� �� ���� �����-�� ���� �������, �� �������� 
																//� ���������� ���������(����� ������ �� ������� �����������)

		if (waveOutWrite(wvHndl, MustHdr,sizeof(WAVEHDR))){
			waveOutClose(wvHndl);											//������ �� �����, ������ ��������� ����, � �������� �� ������
			AfxMessageBox("�� ������ ������������� ����");
			return;
		}
		StateWritenSnd = MustTrack;					//���������� � ��������� ����� ������� ������ ������ - ����� ��� �����
	}
	else	
	{
		waveOutRestart(wvHndl);						//���� ��� ������� ��� ��������, �� ������ �������� ������ ������
	}
}

//////// PAUSE
void CSound::PauseWave(bool bWithFilter)
{
	int MustTrack;
	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;


	//���� ���� ��� ������� �� ����� � ������� ��, ��� ������
	if ((StateWritenSnd != NO_WRITEN) && (StateWritenSnd == MustTrack))			
		waveOutPause(wvHndl);													//������ �� �����
}

//////// STOP
void CSound::StopWave(bool bWithFilter)
{
	int MustTrack;
	bWithFilter? MustTrack = WRITEN_NEW:  MustTrack = WRITEN_OLD;


	//���� ���� ��� ������������� � ������� ��, ��� ������
	if ((StateWritenSnd != NO_WRITEN) && (StateWritenSnd == MustTrack))			
	{
		waveOutReset(wvHndl);
		StateWritenSnd = NO_WRITEN;			 //����������, ��� ������� �� ������ � ���� ��������� � �������� ���������� ������
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///// ���������� ������ � ����� ������� ��� -1 (� �������������� �� ���� �� ����� ������ �� ������)
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
		return (void*)-1;										   //���� ������ �� �������, �� ����� -1
}

/////////////////////////////////////
/*double CSound::GetTimeTrack(void* CurrPosData)
{
	int SampleSize=wvFormat->nChannels*(wvFormat->wBitsPerSample/8);
	return ((BYTE*)CurrPosData-(BYTE*)wvHdrNew.lpData)/SampleSize/wvFormat->nSamplesPerSec;
}
  */

///////////////////////////////////////
//////// ���������� �������� �������
int CSound::GetStatePlayer()
{
	return StateWritenSnd;
}


short a4[8] = {0x8000,0x7fff,0x8000,0x7fff,0,0x7fff,0x8000,0x7fff};
/////////////////////////////////////////////////////////////////
//// �������� ������ ������� �� �������
///// 	 amin, amax - ���������� � ��������� - �� 0 �� 100. ���� ����� � ������ ��������, �� ������ ����� ���������������
///// 	 start, end - ��� ������������� ��������� �� ������ �����. ������������ wvHdrNew.lpData.
void CSound::ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax)
{
	BYTE* t;									//��������� �� ������. ����� ������ �� wav'�.
	int period = int(wvFormat->nSamplesPerSec/freq);	   //����� ����� ���������
	int samplPerSec = int(wvFormat->nSamplesPerSec);
	int i;
	BYTE* begData = (BYTE*)wvHdrNew.lpData; // ���������� ��� ���������� ����
	if (end > wvHdrNew.dwBufferLength)
		end = wvHdrNew.dwBufferLength;
	

	t=start+begData;

	double m;			//�� ��� ����� �������� �������� ������.


//////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
	///////// region	  �����
#define pi 3.14159265	

	float smax=0;
	float smin=0;

	 	while (t<end+begData)								  //���� ������ �� ���������
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
				*(WORD*)t = w;								  //����� �� char ������ BYTE

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
	/////////// ������� � ��� ����� � ����� ���������

																		
	if (wvFormat->wBitsPerSample==8)				//����� �������� ��� �� 2 �����: ��� ������ � 8 ��� � � 16 ���
	{
		while (t<end+begData)								  //���� ������ �� ���������
		{
			for (i=0; i<=period; i++)
			{
				m = sin(3.1416 /period*i) * (amax-amin)/100;		//�������� ������ ��� �������� i �������� �� ������ ���������
				char w =int( (*(BYTE*)t-0x80) * m +amin/100*256 );	 //�������� �� ��������� � ���������� ������ ����� ���������.
																								//BYTE �����������, ������ ��� ��������, �. �. �������� 0x80
				*(BYTE*)t = w+0x80;								  //����� �� char ������ BYTE

				if (wvFormat->nChannels==2)
				{
					t++;
					m = sin(3.1416 /period*i) * (amax-amin)/100;		//�������� ������ ��� �������� i �������� �� ������ ���������
					char w =int( (*(BYTE*)t-0x80) * m +amin/100*256 );	 //�������� �� ��������� � ���������� ������ ����� ���������
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
				m = sin(3.1416 /period*i) * (amax-amin)/100 ;			//�������� ������ ��� �������� i �������� �� ������ ���������
				short w =int( *(short*)t * m +(amin-50)/100*65536 );	 //�������� �� ��������� � ���������� ������ ����� ���������
				*(short*)t = w;

				if (wvFormat->nChannels==2)
				{
					t+=2;
					m = sin(3.1416 /period*i) * (amax-amin)/100 ;			//�������� ������ ��� �������� i �������� �� ������ ���������
					short w =int( *(short*)t * m +(amin-50)/100*65536 );	 //�������� �� ��������� � ���������� ������ ����� ���������
					*(short*)t = w;
				}

				t+=2;
				if (t>=end+begData) break;
			}
		}
	}
}



