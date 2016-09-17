#pragma once

#include "stdafx.h"
#include "Form1.h"
using namespace Sankaku01;

///////////////////////////////////////////////////////
// �T�E���h

// SlimDX �\�[�X���j�b�g
public ref class SourceVoiceUnit {
public:
	SourceVoiceUnit( SlimDX::XAudio2::XAudio2 ^xDev, String ^str );	// �R���X�g���N�^
	~SourceVoiceUnit();		// �f�X�g���N�^
	void Play( float vol, bool bLoop );					// �Đ��J�n
	bool IsPlaying(void) { return bPlaying;	}			// �Đ����m�F
	bool SetSrcVolume( float vol );						// �\�[�X���ʃZ�b�g(1.0f����l�j 
private:
	SlimDX::XAudio2::XAudio2		^xDev;
	SlimDX::Multimedia::WaveStream	^xWaveStream;
	SlimDX::XAudio2::AudioBuffer	^xAudioBuf;
	SlimDX::XAudio2::SourceVoice	^xSrcVoice;
	System::IO::IOException			^LastException;		// ��O
	float fSrcVolume;									// ����
	bool bWaveStreamReady;								// wav����
	bool bPlaying;										// �Đ���
	void BufferEndCallback(System::Object^ sender, SlimDX::XAudio2::ContextEventArgs^ e);
	bool GenerateWaveStream( String ^str );				// �t�@�C������wav�ǂݍ���
	void ConfigSourceVoice( float vol, bool bLoop );	// �\�[�X�{�C�X�ݒ�
};

#define SLIMAUDIO2_HANDLE_NULL	0	// �k���n���h��

// SlimDX��XAudio2
public ref class SlimAudio2 {
public:
	SlimAudio2();
	~SlimAudio2();
	int GetRegistSoundFileNum(void);		// �o�^�T�E���h�t�@�C������Ԃ��B
	unsigned int Play( String ^str, float v, bool bLoop );	// �Đ�
	unsigned int Play( String ^str, bool bLoop );			// �Đ�
	unsigned int Play( String ^str, float v );				// �Đ�	
	unsigned int Play( String ^str );						// �Đ�
	void SetMasterVolume( float v );						// �}�X�^�[���ʃZ�b�g(1.0f����l)
	bool SetSourceVolume( unsigned int handle, float v );	// �\�[�X���ʃZ�b�g(1.0f����l�j
	void Cleaning(void);	// �N���[�j���O����
private:
	SlimDX::XAudio2::XAudio2 ^xDev;
	SlimDX::XAudio2::MasteringVoice ^xMstVoice;
	cliext::map<unsigned int,SourceVoiceUnit^> SrcVoiceUnitMap;
	unsigned int voice_handle;
	void GarbageCollection(void);		// �K�x�[�W�R���N�V����
	unsigned int GenerateHandle(void);	// �n���h������
};
// �Q�l�Fhttp://www44.atwiki.jp/bokuyo/pages/25.html


///////////////////////////////////////////////////////


ref class Field;		// �O���Q��

public delegate int GetDelegateWidth(void);		// �t�B�[���h�̃��R��
public delegate int GetDelegateHeight(void);	// �t�B�[���h�̃^�e��
public delegate void NotifyHansya(int);			// ���˒ʒm

// ���j�b�g
public ref class Unit {
public:
	Bitmap ^bmp;								// ���j�b�g�̃r�b�g�}�b�v
	System::String ^str;						// ���j�b�g�̃t�@�C����
	Unit();										// �R���X�g���N�^
	void SetBmpFile( System::String ^str );		// �t�@�C�������w�肷��
	Bitmap ^GetBmp(void);						// �r�b�g�}�b�v��Ԃ�
	void SetPos( int x, int y );				// �|�W�V�������w�肷��
	void DoUnit(void);							// ���j�b�g�̍s���֐�
	float fPosx;
	float fPosy;
	double dSpeed;								// �X�s�[�h
	double dAngle;								// �A���O���i�p�x�j
	bool bInc;	// �Ȃ�ƂȂ�
	void SokudoOmake(void);	// ���܂�

	GetDelegateWidth	^GetDeleWidth;			// �t�B�[���h�̕������߂�i�f���Q�[�g�j
	GetDelegateHeight	^GetDeleHeight;			// �t�B�[���h�̍��������߂�i�f���Q�[�g�j
	NotifyHansya		^NotiHansya;			// ���˂�������ʒm�B
private:
};

// �Q�[���t�B�[���h
public ref class Field {
public:
	delegate int GetDelegateWidth2(void);			// �t�B�[���h�̃��R��

	Form1	^fm;									// �t�H�[���P
	System::Windows::Forms::PictureBox^  picBox;	// �t�B�[���h�pPictureBox
	Bitmap^ picBmp;									// �t�B�[���hBitmap
	Bitmap^ wallBmp;								// �ǎ�Bitmap
	Diagnostics::Stopwatch^ sw;						// �X�g�b�v�E�H�b�`
	Int64 keeptime;									// ���ԕۑ��p
	Int64 tickMsec;									// �e�B�b�N���ԊǗ��l�i�R�Rmsec�j
	bool bTickFlag;									// �e�B�b�N���ԗp�t���O
	String ^dbgstr;									// �f�o�b�O�p������
	System::Windows::Forms::Label^  label;			// �f�o�b�O�p���x��
	System::Drawing::Graphics^	grph;				// �t�B�[���h�pGraphics
//#if USE_DOUBLE_BUFFER
	BufferedGraphicsContext		^contex;			// �_�u���o�b�t�@�p
	BufferedGraphics			^bgrph;				// �_�u���o�b�t�@�p
	Byte bufferingMode;								// �_�u���o�b�t�@�p
	Byte dbcnt;										// �_�u���o�b�t�@�p
//#endif
	SlimAudio2	^sa2;

	int iFieldWidth;
	int iFieldHeight;
	void SetForm( Form1 ^f );						// �t�H�[���o�^
	void Start(void);								// �t�B�[���h�����X�^�[�g
	void FieldInit( int posx, int posy, int width, int height );		// �t�B�[���h������
	int GetWidth(void);								// �t�B�[���h�̃��R�����擾
	int GetHeight(void);							// �t�B�[���h�̃^�e�������擾
	void NotisHansya(int);							// ���˂������̒ʒm���󂯂�
	void AddUnit( String ^str );					// ���j�b�g�ǉ�
	void AddUnit( String ^str, int x, int y );		// ���j�b�g�ǉ�
	void AddUnit( String ^str, int x, int y, double angle, double speed );
	void DrawToBuffer(Graphics^ g );				// �t�B�[���h�r�b�g�}�b�v��`�悷��
	bool MainLoop(void);							// ���C�����[�v
	void StopWatchStart(void);						// �X�g�b�v�E�H�b�`�J�n
	bool StopWatchWaitCheck(void);					// �X�g�b�v�E�H�b�`���ԃ`�F�b�N

	void DoAllUnit(void);							// �S���j�b�g�ɍs���w�߂𔭂���B
	System::Void picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);	// �h���[�C�x���g�����H

	System::Windows::Forms::TrackBar^  trackBar1;
	System::Void trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	System::Windows::Forms::TrackBar^  trackBar2;
	System::Void trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	float	master_vol;
	float	bgm_vol;
	unsigned int bgm_handle;

private:
	cliext::list<Unit^>unitidx;						// ���j�b�g�S�̂����X�g�Ǘ��B
};
