#include "stdafx.h"

/////////////////////////////////////////////

// �R���X�g���N�^
SourceVoiceUnit::SourceVoiceUnit( SlimDX::XAudio2::XAudio2 ^xDev, String ^str )
{
	this->xDev = xDev;								// XAudio2�ێ�
	bWaveStreamReady = GenerateWaveStream( str );	// WaveStream�����i�t�@�C���ǂݍ��݁j
	this->bPlaying = false;							// �܂��Đ����Ȃ�
}

// �f�X�g���N�^
SourceVoiceUnit::~SourceVoiceUnit()
{
	delete this->xSrcVoice;
	delete this->xAudioBuf;
	delete this->xWaveStream;
}

// �\�[�X�{�C�X�ݒ�
void SourceVoiceUnit::ConfigSourceVoice( float vol, bool bLoop )
{
	this->xSrcVoice = gcnew SlimDX::XAudio2::SourceVoice(xDev, this->xWaveStream->Format);
	this->xAudioBuf = gcnew SlimDX::XAudio2::AudioBuffer();
	this->xAudioBuf->AudioBytes = (int)this->xWaveStream->Length;
	this->xAudioBuf->AudioData = this->xWaveStream;
	this->xSrcVoice->Volume = vol;
	if( bLoop ) this->xAudioBuf->LoopCount = SlimDX::XAudio2::XAudio2::LoopInfinite;
	this->xSrcVoice->SubmitSourceBuffer( xAudioBuf );
	this->xSrcVoice->BufferEnd += gcnew EventHandler<SlimDX::XAudio2::ContextEventArgs^>( this, &SourceVoiceUnit::BufferEndCallback );
}

// �Đ��J�n
void SourceVoiceUnit::Play( float vol, bool bLoop )
{
	ConfigSourceVoice( vol, bLoop );	// �\�[�X�{�C�X�ݒ�
	this->xSrcVoice->Start();			// �Đ��J�n
	this->fSrcVolume = vol;
	this->bPlaying = true;
}

// �\�[�X���ʃZ�b�g(1.0f����l�j 
bool SourceVoiceUnit::SetSrcVolume( float vol )
{
	this->fSrcVolume = vol;
	if( this->bPlaying ) this->xSrcVoice->Volume = vol;
	return true;
}

// WaveStream�����i�t�@�C�����Łj
bool SourceVoiceUnit::GenerateWaveStream( String ^str )
{
	// �t�@�C���ǂݍ���
	try {
		this->xWaveStream = gcnew SlimDX::Multimedia::WaveStream(str);
	}
	catch( System::IO::FileNotFoundException ^e )
	{	// �t�@�C�������݂��Ȃ��B
		LastException = e;	// DBG:�Ō�̃G���[�ێ�
		return false;		// SLIMAUDIO2_HANDLE_NULL;
	}
	return true;	// �n���h����ԋp����B
}

// �Đ��I���R�[���o�b�N
void SourceVoiceUnit::BufferEndCallback(System::Object^ sender, SlimDX::XAudio2::ContextEventArgs^ e)
{
	bPlaying = false;	// �Đ��t���O�����Z�b�g
}


/////////////////////////////////////////////

// �R���X�g���N�^
SlimAudio2::SlimAudio2()
{
	xDev = gcnew SlimDX::XAudio2::XAudio2();
	xMstVoice = gcnew SlimDX::XAudio2::MasteringVoice(xDev);
	voice_handle = 0;
	xMstVoice->Volume = 1.0f;
}

// �f�X�g���N�^
SlimAudio2::~SlimAudio2()
{
	delete xMstVoice;
	delete xDev;
}

// �}�X�^�[���ʂ��Z�b�g(1.0f����l)
void SlimAudio2::SetMasterVolume( float v )
{
	xMstVoice->Volume = v;
}

// �\�[�X���ʃZ�b�g(1.0f����l�j
bool SlimAudio2::SetSourceVolume( unsigned int handle, float vol )
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite;

	ite = SrcVoiceUnitMap.find( handle );
	if( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		return obj->SetSrcVolume( vol );
	}
	return false;		// �w��n���h���ɑΉ������I�u�W�F�N�g��������Ȃ�����
}

// �o�^�T�E���h�t�@�C������Ԃ��B
int SlimAudio2::GetRegistSoundFileNum(void)
{
	return SrcVoiceUnitMap.size();	// map�v�f����Ԃ�
}

// �n���h������
unsigned int SlimAudio2::GenerateHandle(void)
{
	return ++voice_handle;	// �P�����Ă���Ԃ��i���P�T�l�������j
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, float vol, bool bLoop )
{
	unsigned int handle;

	GarbageCollection();	// �Đ��σo�b�t�@�J��

	// ���j�b�g�𐶐����čĐ��A���X�g�o�^
	SourceVoiceUnit ^obj = gcnew SourceVoiceUnit( this->xDev, str );
	obj->Play( vol, bLoop );

	// Map�o�^
	handle = GenerateHandle();
	cliext::pair<unsigned int,SourceVoiceUnit^> p(handle,obj);
	SrcVoiceUnitMap.insert( p );

	return handle;
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, bool bLoop )
{
	return Play( str, 1.0f, bLoop );
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, float v )
{
	return Play( str, v, false );
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str )
{
	return Play( str, 1.0f, false );
}

//bool SlimAudio2::Play(unsigned int handle)
//{
//	return bool;	// �݂�������
//}

// �K�[�x�[�W�R���N�V�����I����
void SlimAudio2::GarbageCollection(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{
			delete obj;
			SrcVoiceUnitMap.erase( ite++ );		// Map����폜
		}
		else
		{
			ite++;
		}
	}
}

// �N���[�j���O����
void SlimAudio2::Cleaning(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{	// �폜�Ώ�
			delete obj;
			ite++;
		}
	}
	SrcVoiceUnitMap.clear();
}


///////////////////////////////////////////////////////////