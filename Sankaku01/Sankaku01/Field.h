#pragma once

#include "stdafx.h"
#include "Form1.h"
using namespace Sankaku01;

///////////////////////////////////////////////////////
// サウンド

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
	unsigned int voice_handle;
	void GarbageCollection(void);		// ガベージコレクション
	unsigned int GenerateHandle(void);	// ハンドル生成
};
// 参考：http://www44.atwiki.jp/bokuyo/pages/25.html


///////////////////////////////////////////////////////


ref class Field;		// 前方参照

public delegate int GetDelegateWidth(void);		// フィールドのヨコ幅
public delegate int GetDelegateHeight(void);	// フィールドのタテ幅
public delegate void NotifyHansya(int);			// 反射通知

// ユニット
public ref class Unit {
public:
	Bitmap ^bmp;								// ユニットのビットマップ
	System::String ^str;						// ユニットのファイル名
	Unit();										// コンストラクタ
	void SetBmpFile( System::String ^str );		// ファイル名を指定する
	Bitmap ^GetBmp(void);						// ビットマップを返す
	void SetPos( int x, int y );				// ポジションを指定する
	void DoUnit(void);							// ユニットの行動関数
	float fPosx;
	float fPosy;
	double dSpeed;								// スピード
	double dAngle;								// アングル（角度）
	bool bInc;	// なんとなく
	void SokudoOmake(void);	// おまけ

	GetDelegateWidth	^GetDeleWidth;			// フィールドの幅を求める（デリゲート）
	GetDelegateHeight	^GetDeleHeight;			// フィールドの高さを求める（デリゲート）
	NotifyHansya		^NotiHansya;			// 反射した事を通知。
private:
};

// ゲームフィールド
public ref class Field {
public:
	delegate int GetDelegateWidth2(void);			// フィールドのヨコ幅

	Form1	^fm;									// フォーム１
	System::Windows::Forms::PictureBox^  picBox;	// フィールド用PictureBox
	Bitmap^ picBmp;									// フィールドBitmap
	Bitmap^ wallBmp;								// 壁紙Bitmap
	Diagnostics::Stopwatch^ sw;						// ストップウォッチ
	Int64 keeptime;									// 時間保存用
	Int64 tickMsec;									// ティック時間管理値（３３msec）
	bool bTickFlag;									// ティック時間用フラグ
	String ^dbgstr;									// デバッグ用文字列
	System::Windows::Forms::Label^  label;			// デバッグ用ラベル
	System::Drawing::Graphics^	grph;				// フィールド用Graphics
//#if USE_DOUBLE_BUFFER
	BufferedGraphicsContext		^contex;			// ダブルバッファ用
	BufferedGraphics			^bgrph;				// ダブルバッファ用
	Byte bufferingMode;								// ダブルバッファ用
	Byte dbcnt;										// ダブルバッファ用
//#endif
	SlimAudio2	^sa2;

	int iFieldWidth;
	int iFieldHeight;
	void SetForm( Form1 ^f );						// フォーム登録
	void Start(void);								// フィールド処理スタート
	void FieldInit( int posx, int posy, int width, int height );		// フィールド初期化
	int GetWidth(void);								// フィールドのヨコ幅を取得
	int GetHeight(void);							// フィールドのタテ高さを取得
	void NotisHansya(int);							// 反射した事の通知を受ける
	void AddUnit( String ^str );					// ユニット追加
	void AddUnit( String ^str, int x, int y );		// ユニット追加
	void AddUnit( String ^str, int x, int y, double angle, double speed );
	void DrawToBuffer(Graphics^ g );				// フィールドビットマップを描画する
	bool MainLoop(void);							// メインループ
	void StopWatchStart(void);						// ストップウォッチ開始
	bool StopWatchWaitCheck(void);					// ストップウォッチ時間チェック

	void DoAllUnit(void);							// 全ユニットに行動指令を発する。
	System::Void picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);	// ドローイベント処理？

	System::Windows::Forms::TrackBar^  trackBar1;
	System::Void trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	System::Windows::Forms::TrackBar^  trackBar2;
	System::Void trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	float	master_vol;
	float	bgm_vol;
	unsigned int bgm_handle;

private:
	cliext::list<Unit^>unitidx;						// ユニット全体をリスト管理。
};
