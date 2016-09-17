#include "stdafx.h"

/////////////////////////////////////////////

// コンストラクタ
SourceVoiceUnit::SourceVoiceUnit( SlimDX::XAudio2::XAudio2 ^xDev, String ^str )
{
	this->xDev = xDev;								// XAudio2保持
	bWaveStreamReady = GenerateWaveStream( str );	// WaveStream生成（ファイル読み込み）
	this->bPlaying = false;							// まだ再生しない
}

// デストラクタ
SourceVoiceUnit::~SourceVoiceUnit()
{
	delete this->xSrcVoice;
	delete this->xAudioBuf;
	delete this->xWaveStream;
}

// ソースボイス設定
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

// 再生開始
void SourceVoiceUnit::Play( float vol, bool bLoop )
{
	ConfigSourceVoice( vol, bLoop );	// ソースボイス設定
	this->xSrcVoice->Start();			// 再生開始
	this->fSrcVolume = vol;
	this->bPlaying = true;
}

// ソース音量セット(1.0fが基準値） 
bool SourceVoiceUnit::SetSrcVolume( float vol )
{
	this->fSrcVolume = vol;
	if( this->bPlaying ) this->xSrcVoice->Volume = vol;
	return true;
}

// WaveStream生成（ファイル名で）
bool SourceVoiceUnit::GenerateWaveStream( String ^str )
{
	// ファイル読み込み
	try {
		this->xWaveStream = gcnew SlimDX::Multimedia::WaveStream(str);
	}
	catch( System::IO::FileNotFoundException ^e )
	{	// ファイルが存在しない。
		LastException = e;	// DBG:最後のエラー保持
		return false;		// SLIMAUDIO2_HANDLE_NULL;
	}
	return true;	// ハンドルを返却する。
}

// 再生終了コールバック
void SourceVoiceUnit::BufferEndCallback(System::Object^ sender, SlimDX::XAudio2::ContextEventArgs^ e)
{
	bPlaying = false;	// 再生フラグをリセット
}


/////////////////////////////////////////////

// コンストラクタ
SlimAudio2::SlimAudio2()
{
	xDev = gcnew SlimDX::XAudio2::XAudio2();
	xMstVoice = gcnew SlimDX::XAudio2::MasteringVoice(xDev);
	voice_handle = 0;
	xMstVoice->Volume = 1.0f;
}

// デストラクタ
SlimAudio2::~SlimAudio2()
{
	delete xMstVoice;
	delete xDev;
}

// マスター音量をセット(1.0fが基準値)
void SlimAudio2::SetMasterVolume( float v )
{
	xMstVoice->Volume = v;
}

// ソース音量セット(1.0fが基準値）
bool SlimAudio2::SetSourceVolume( unsigned int handle, float vol )
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite;

	ite = SrcVoiceUnitMap.find( handle );
	if( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		return obj->SetSrcVolume( vol );
	}
	return false;		// 指定ハンドルに対応したオブジェクトが見つからなかった
}

// 登録サウンドファイル数を返す。
int SlimAudio2::GetRegistSoundFileNum(void)
{
	return SrcVoiceUnitMap.size();	// map要素数を返す
}

// ハンドル生成
unsigned int SlimAudio2::GenerateHandle(void)
{
	return ++voice_handle;	// １つ足してから返す（※１週考慮無し）
}

// サウンド再生
unsigned int SlimAudio2::Play( String ^str, float vol, bool bLoop )
{
	unsigned int handle;

	GarbageCollection();	// 再生済バッファ開放

	// ユニットを生成して再生、リスト登録
	SourceVoiceUnit ^obj = gcnew SourceVoiceUnit( this->xDev, str );
	obj->Play( vol, bLoop );

	// Map登録
	handle = GenerateHandle();
	cliext::pair<unsigned int,SourceVoiceUnit^> p(handle,obj);
	SrcVoiceUnitMap.insert( p );

	return handle;
}

// サウンド再生
unsigned int SlimAudio2::Play( String ^str, bool bLoop )
{
	return Play( str, 1.0f, bLoop );
}

// サウンド再生
unsigned int SlimAudio2::Play( String ^str, float v )
{
	return Play( str, v, false );
}

// サウンド再生
unsigned int SlimAudio2::Play( String ^str )
{
	return Play( str, 1.0f, false );
}

//bool SlimAudio2::Play(unsigned int handle)
//{
//	return bool;	// みじっそう
//}

// ガーベージコレクション的処理
void SlimAudio2::GarbageCollection(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{
			delete obj;
			SrcVoiceUnitMap.erase( ite++ );		// Mapから削除
		}
		else
		{
			ite++;
		}
	}
}

// クリーニングする
void SlimAudio2::Cleaning(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{	// 削除対象
			delete obj;
			ite++;
		}
	}
	SrcVoiceUnitMap.clear();
}


///////////////////////////////////////////////////////////