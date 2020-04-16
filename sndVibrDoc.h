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
///// ����� ���������. �������� �� ���������, ��������, �������� � ��������� ���������
class CSndVibrDoc : public CDocument
{
protected:
	CSndVibrDoc();
	DECLARE_DYNCREATE(CSndVibrDoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSndVibrDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);			//���������� �������� ���������
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);			//���������� ���������� ���������
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsLoadWav();
	void ResetWav();								  //���������� ��� ���������. ������ �������� ������ �� ��������������� ����� � ��� ����������
	void* GetWavData(int* Channels, int* BitsPerSample, int* sizeWavData);	   //���������� �������������� wav'� � ���������� 
																															// ��������� �� �������� �����
	void ApplyVibrato(DWORD start, DWORD end, double freq, int amin, int amax);		//�������� ������� ������� ������ CSound

	//������� ��� ���������� ����������������.
	//���������� ��� ������ �������� ��������������� ������� ������ CSound. ����� ��� ������� CSndVibrView � �������
	void PlayWave(bool bWithFilter);	//� bWithFilter ������������ - ����� �� ������� ��������������, ���� true - �� ����������, ����� ��������������
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
	void* SndFileNew;	  //��������� �� ������ ���������������� �������
	void* SndFileOld;		//��������� �� ������ �������������� �������
	CString sSoundFile;	  //��� � ���� �������� �������
	int SndFileSize;		  //������ ��������� �����

	CSound Snd;				//����� ��� ������ �� ������(�� ����� ���� � �� ������ � 
								//  ������, ������� � ���� ������ ������ ���������� ������ ����� ������)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNDVIBRDOC_H__CD385E88_3E67_44ED_B095_74353E943DAB__INCLUDED_)
