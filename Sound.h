// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_)
#define AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/// �������� ���������� StateWritenSnd
#define NO_WRITEN 0						   //������� �� ���������������
#define WRITEN_OLD 1					//�������������� ������� ���������������( ��� �� �����)
#define WRITEN_NEW 2				   // ��������� �������



/////////////////////////////////////////////////////////////////////////////////
/////// ����� ��� ������ �� ������. ���������� MCI �������. �������� � ���� ������ � ������
class CSound  
{
public:
	CSound();
	virtual ~CSound();
	
	void ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax);		//��������� ������
	int GetStatePlayer();									//���������� �������� StateWritenSnd
	BOOL OpenWav(void* WavOld,void *WavNew);	//������������� ��������� ����������, �������� ������� wav'�
	void PlayWave(bool bWithFilter);				   //������� ��� ���������� ��������
	void PauseWave(bool bWithFilter);
	void StopWave(bool bWithFilter);
	void* GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData);

private:
	int StateWritenSnd;			//��� �����, ����� �� ������� ������ ���������������(��� ����� �� �����)
										// ���� NO_WRITEN, �� ������� ������� �� ���������������
	int SndFileSize;		  //������ ��������� �����
	CString sSoundFile;	  //��� � ���� �������� �������

	HWAVEOUT wvHndl;	//����� ��������� ����������
	WAVEFORMATEX* wvFormat;		//��������� � ������� � ������� �������. ��� ��� 
													//������ ����� ������� ��������, �� ����� ������ ���� ���������
	WAVEHDR wvHdrOld;		//������ � ���������������. ��� ��� �������� ��������� �� ���� 
	WAVEHDR wvHdrNew;		//  ��������  ������, �� ����� 2 ���������� - ��� �������������� � ��� ���������

};

#endif // !defined(AFX_SOUND_H__46A9B01D_4649_4D92_9B2B_E6D37F46655A__INCLUDED_)
