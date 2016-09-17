#pragma once
// SlimDX���g����XAudio2��֗��Ɏg���ׂ̃N���X
// �V���O���g���\���ɂ��悤���Ȃ��B

#include <cliext/map>		// map�p
using namespace System;

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
	UInt32 voice_handle;
	void GarbageCollection(void);		// �K�x�[�W�R���N�V����
	unsigned int GenerateHandle(void);	// �n���h������
};
// �Q�l�Fhttp://www44.atwiki.jp/bokuyo/pages/25.html
