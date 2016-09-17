#pragma once
// SlimDXを使ったXAudio2を便利に使う為のクラス
// シングルトン構成にしようかなぁ。

#include <cliext/map>		// map用
using namespace System;

// SlimDX ソースユニット
public ref class SourceVoiceUnit {
public:
	SourceVoiceUnit( SlimDX::XAudio2::XAudio2 ^xDev, String ^str );	// コンストラクタ
	~SourceVoiceUnit();		// デストラクタ
	void Play( float vol, bool bLoop );					// 再生開始
	bool IsPlaying(void) { return bPlaying;	}			// 再生中確認
	bool SetSrcVolume( float vol );						// ソース音量セット(1.0fが基準値） 
private:
	SlimDX::XAudio2::XAudio2		^xDev;
	SlimDX::Multimedia::WaveStream	^xWaveStream;
	SlimDX::XAudio2::AudioBuffer	^xAudioBuf;
	SlimDX::XAudio2::SourceVoice	^xSrcVoice;
	System::IO::IOException			^LastException;		// 例外
	float fSrcVolume;									// 音量
	bool bWaveStreamReady;								// wav準備
	bool bPlaying;										// 再生中
	void BufferEndCallback(System::Object^ sender, SlimDX::XAudio2::ContextEventArgs^ e);
	bool GenerateWaveStream( String ^str );				// ファイルからwav読み込み
	void ConfigSourceVoice( float vol, bool bLoop );	// ソースボイス設定
};

#define SLIMAUDIO2_HANDLE_NULL	0	// ヌルハンドル

// SlimDXでXAudio2
public ref class SlimAudio2 {
public:
	SlimAudio2();
	~SlimAudio2();
	int GetRegistSoundFileNum(void);		// 登録サウンドファイル数を返す。
	unsigned int Play( String ^str, float v, bool bLoop );	// 再生
	unsigned int Play( String ^str, bool bLoop );			// 再生
	unsigned int Play( String ^str, float v );				// 再生	
	unsigned int Play( String ^str );						// 再生
	void SetMasterVolume( float v );						// マスター音量セット(1.0fが基準値)
	bool SetSourceVolume( unsigned int handle, float v );	// ソース音量セット(1.0fが基準値）
	void Cleaning(void);	// クリーニングする
private:
	SlimDX::XAudio2::XAudio2 ^xDev;
	SlimDX::XAudio2::MasteringVoice ^xMstVoice;
	cliext::map<unsigned int,SourceVoiceUnit^> SrcVoiceUnitMap;
	UInt32 voice_handle;
	void GarbageCollection(void);		// ガベージコレクション
	unsigned int GenerateHandle(void);	// ハンドル生成
};
// 参考：http://www44.atwiki.jp/bokuyo/pages/25.html
