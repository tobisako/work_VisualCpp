// Sankaku01.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"
#include "Field.h"

using namespace Sankaku01;


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
///////////////////////////////////////////////////////////
// Unit 

// コンストラクタ
Unit::Unit()
{
	bInc = true;
}

// ファイル名をセットする
void Unit::SetBmpFile( System::String ^str )
{
	bmp = gcnew Bitmap( str );
}

// ポジションを指定する
void Unit::SetPos( int x, int y )
{
	fPosx = (float)x;
	fPosy = (float)y;
}

// ビットマップを返す
Bitmap ^Unit::GetBmp(void)
{
	return bmp;
}

#define PI 3.14159265

// ユニット行動
void Unit::DoUnit(void)
{
	// 角度に応じたXYの移動量を求める。
	double movementX = ( dSpeed * Math::Cos( dAngle * PI / 180 ) );
	double movementY = ( dSpeed * Math::Sin( dAngle * PI / 180 ) );

	fPosx += (float)movementX;
	fPosy += (float)movementY;

	double width = GetDeleWidth();
	double height = GetDeleHeight();

	// 反射判定（Ｙ軸）
	if( fPosy >= (height - bmp->Height) )	// 反射判定（下→上）
	{	// Ｙ軸下（1～179）
		if( dAngle < 180.0f )	// 画面下へ向かっているか？
		{	// 反射角度を変更（下から上へ）
			dAngle = 360.0f - dAngle;
			SokudoOmake();	// オマケ
			NotiHansya( 0 );
		}
	}
	else if( fPosy <= 1.0f )	// 反射判定（上→下）
	{	// Ｙ軸上（181～359）
		if( dAngle >= 180.0f )
		{	// 反射角度を変更（上から下へ）
			dAngle = 360.0f - dAngle;
			SokudoOmake();	// オマケ
			NotiHansya( 1 );
		}
	}
	// 反射判定（Ｘ軸）
	else if( fPosx  >= (width - bmp->Width) )	// 反射判定（右→左）
	{	// Ｘ軸右（271～359, 1～89）
		if( dAngle > 270.0f || dAngle < 90.0f )
		{
			dAngle = 180.0f - dAngle;
			if( dAngle < 0.0f ) dAngle = 360.0f + dAngle;	// マイナス化させない対策
			SokudoOmake();	// オマケ
			NotiHansya( 2 );
		}
	}
	else if( fPosx <= 1.0f )		// 反射判定（左→右）
	{	// Ｘ軸左（90～270）
		if( dAngle > 90.0f && dAngle < 270.0f )
		{
			dAngle = 180.0f - dAngle;
			if( dAngle < 0.0f ) dAngle = 360.0f + dAngle;	// マイナス化させない対策
			SokudoOmake();	// オマケ
			NotiHansya( 3 );
		}
	}
}

// オマケ処理
void Unit::SokudoOmake(void)
{
#if 0
	if( bInc )
	{	// 足していきます。
		dSpeed += 1.2f;	// なんとなく
		if( dSpeed > 300.0f )
		{
			bInc = false;
		}
	}
	else
	{	// 引いていきます。
		dSpeed -= 2.8f;	// なんとなく
		if( dSpeed <= 5.0f )
		{
			bInc = true;
		}
	}
#endif
}

///////////////////////////////////////////////////////////
// Field

// コンストラクタ

// フォーム登録
void Field::SetForm( Form1 ^f )
{
	fm = f;
}

// フィールドのヨコ幅を取得
int Field::GetWidth(void)
{
	return iFieldWidth;
}

// フィールドのタテ高さを取得
int Field::GetHeight(void)
{
	return iFieldHeight;
}

// 反射した事の通知を受ける
void Field::NotisHansya( int udon )
{
	switch( udon )
	{
	case 0:
		sa2->Play( "sound1.wav", 1.0f, false );
		break;
	case 1:
		sa2->Play( "sound2.wav", 1.0f, false );
		break;
	case 2:
		sa2->Play( "land_combat_2.wav", 1.0f, false );
		break;
	default:
		sa2->Play( "udonken.wav", 1.0f, false );
		break;
	}
}

// フィールド処理スタート
void Field::Start(void)
{
	// フォーム生成
	fm = gcnew Form1();

	// フィールド初期化
	FieldInit( 20, 20, 800, 600 );

	// フォーム表示（モードレス制御）
	fm->Show();

	// 初回ドロー
	fm->Refresh();
//	fm->Invalidate();

	// ストップウォッチ起動
	StopWatchStart();

//	__int64 watch2 = 10;
//	::QueQueryPerformanceCounter( (LARGE_INTEGER *)&watch2 );

	// SlimDX XAudio2 生成
	sa2 = gcnew SlimAudio2();	// XAudio2制御用クラスのインスタンス作成
	master_vol = 1.0f;
	sa2->SetMasterVolume( master_vol );
	bgm_vol = 0.4f;
	bgm_handle = sa2->Play( "sugar.wav", bgm_vol, true );

	// メインループ処理
	MainLoop();
}

// フィールド初期化
void Field::FieldInit( int posx, int posy, int width, int height )
{
	iFieldWidth = width;
	iFieldHeight = height;

	fm->SuspendLayout();

	// フィールド用ピクチャボックス生成
	picBox = (gcnew System::Windows::Forms::PictureBox());
	picBox->Location = System::Drawing::Point(posx, posy);
	picBox->Name = L"GameFieldPicBox";
	picBox->Size = System::Drawing::Size( width, height );
	picBox->TabIndex = 0;
	picBox->TabStop = false;
	picBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Field::picBoxPaintEvent);
	fm->Controls->Add( picBox );	// ピクチャボックスをフォームに登録する。

	// デバッグ用ラベル作成
	label = (gcnew System::Windows::Forms::Label());
	label->AutoSize = true;
	label->Location = System::Drawing::Point(80, 2);
	label->Name = L"label";
	label->Size = System::Drawing::Size(35, 12);
	label->TabIndex = 1;
	label->Text = L"どうでしょう？";
	fm->Controls->Add( label );	// ピクチャボックスをフォームに登録する。

	// 音量調整用トラックバー１
	this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
	this->trackBar1->Location = System::Drawing::Point(250, 615);
	this->trackBar1->Name = L"trackBar1";
	this->trackBar1->Size = System::Drawing::Size(155, 45);
	this->trackBar1->TabIndex = 0;
	this->trackBar1->Value = 100;
	this->trackBar1->Maximum = 150;
	this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &Field::trackBar1_ValueChanged);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
	fm->Controls->Add( this->trackBar1 );

	// 音量調整用トラックバー２
	this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
	this->trackBar2->Location = System::Drawing::Point(420, 615);
	this->trackBar2->Name = L"trackBar2";
	this->trackBar2->Size = System::Drawing::Size(155, 45);
	this->trackBar2->TabIndex = 0;
	this->trackBar2->Value = 100;
	this->trackBar2->Maximum = 150;
	this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &Field::trackBar2_ValueChanged);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
	fm->Controls->Add( this->trackBar2 );

	fm->PerformLayout();

	// フィールド用ビットマップ生成・接続
	picBmp = gcnew Bitmap( width, height );
	picBox->Image = picBmp;

	// 壁紙用ビットマップをロードしておく
	wallBmp = gcnew Bitmap( "wallpaper.jpg" );

	// おそらく最初の１回だけでいけるはずだ。
	grph = Graphics::FromImage( picBox->Image );

#if USE_DOUBLE_BUFFER
	contex = BufferedGraphicsManager::Current;
	contex->MaximumBuffer = System::Drawing::Size( width + 1, height + 1 );
//	fm->SetStyle( static_cast<ControlStyles>(ControlStyles::AllPaintingInWmPaint | ControlStyles::UserPaint), true );
	bgrph = contex->Allocate( picBox->CreateGraphics(), Rectangle(0,0,width,height) );
	bufferingMode = 2;
	dbcnt = 0;
#endif

	// ボックスを黒くする。
//	Brush^ brush = gcnew SolidBrush( Color::FromArgb(0, 0, 0) );
//	grph->FillRectangle( brush, 0, 0, width, height );

	// オブジェクトを配置する。
	AddUnit( "takaharu.jpg", 5, 5, 160.0f, 1.0f );
	AddUnit( "2749.png", 160, 90, 35.0f, 7.5f );
	AddUnit( "icon82.jpg", 10, 50, 5.0f, 12.8f );
	AddUnit( "icon83.jpg", 200, 10 );
	AddUnit( "icon81.jpg", 120, 220, 170.0f, 0.8f );
	AddUnit( "1752.png", 0, 0 );
	AddUnit( "1752.png", 30, 0, 110.0f, 3.7f );
	AddUnit( "1752.png", 60, 0 );
	AddUnit( "1752.png", 90, 0 );
	AddUnit( "1752.png", 120, 0 );
	AddUnit( "icon82.jpg", 210, 50, 275.0f, 3.5f );
	AddUnit( "icon83.jpg", 400, 10 );
	AddUnit( "icon81.jpg", 520, 200 );
	AddUnit( "1752.png", 10, 90 );
	AddUnit( "1752.png", 130, 90 );
	AddUnit( "1752.png", 190, 90, 250.0f, 10.5f );
	AddUnit( "1752.png", 220, 90 );
	AddUnit( "icon82.jpg", 110, 150 );
	AddUnit( "icon83.jpg", 300, 110 );
	AddUnit( "icon81.jpg", 420, 300 );
	AddUnit( "1752.png", 10, 90 );
	AddUnit( "1752.png", 130, 90 );
	AddUnit( "1752.png", 160, 90 );
	AddUnit( "1752.png", 190, 90, 345.5f, 8.5f );
	AddUnit( "1752.png", 220, 90 );

	// 全オブジェクトを描画し、ビットマップを更新する。
#if USE_DOUBLE_BUFFER
	DrawToBuffer( bgrph->Graphics );
#else
	DrawToBuffer( grph );
#endif
	// ダブルバッファ用
}

// ユニット追加
void Field::AddUnit( String ^str )
{
	AddUnit( str, 0, 0 );
}

// ユニット追加（座標つき）
void Field::AddUnit( String ^str, int x, int y )
{
	AddUnit( str, x, y, 30.0f, 2.0f );
}

// ユニット追加（座標と速度と角度つき）
void Field::AddUnit( String ^str, int x, int y, double angle, double speed )
{
	Unit ^obj = gcnew Unit();
	obj->SetBmpFile( str );
	obj->SetPos( x, y );
	obj->dSpeed = speed;
	obj->dAngle = angle;

	// デリゲート登録
	obj->GetDeleWidth = gcnew GetDelegateWidth( this, &Field::GetWidth );
	obj->GetDeleHeight = gcnew GetDelegateHeight( this, &Field::GetHeight );
	obj->NotiHansya = gcnew NotifyHansya( this, &Field::NotisHansya );

	unitidx.push_back( obj );		// リストに追加
}

// 全ユニットの行動計算
void Field::DoAllUnit(void)
{
	cliext::list<Unit^>::iterator ite = unitidx.begin();

	while( ite != unitidx.end() )
	{
		(*ite)->DoUnit();
		ite++;
	}
}

// 全オブジェクトを描画し、フィールドビットマップを更新する
void Field::DrawToBuffer( Graphics^ g )
{
	cliext::list<Unit^>::iterator ite = unitidx.begin();
	Bitmap ^bmp;

#if USE_DOUBLE_BUFFER
	// Clear the graphics buffer every five updates.
	if( ++dbcnt > 5 )
	{
		dbcnt = 0;
		g->FillRectangle( Brushes::Black, 0, 0, picBmp->Width, picBmp->Height );
	}
#endif

	// 壁紙を設定
	RectangleF SrcRect = RectangleF( 0, 0, (float)wallBmp->Width, (float)wallBmp->Height );
	RectangleF DstRect = RectangleF( 0, 0, (float)picBmp->Width, (float)picBmp->Height );
	g->DrawImage( wallBmp, DstRect, SrcRect, GraphicsUnit::Pixel );

	// 全ユニットを配置
	while( ite != unitidx.end() )
	{
		bmp = (*ite)->GetBmp();
		SrcRect = RectangleF( 0, 0, (float)bmp->Width, (float)bmp->Height );
		DstRect = RectangleF( (*ite)->fPosx, (*ite)->fPosy, (float)bmp->Width, (float)bmp->Height );
		g->DrawImage( bmp, DstRect, SrcRect, GraphicsUnit::Pixel );
		ite++;
	}
}

// オンドｩロｧー
void Field::picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
#if USE_DOUBLE_BUFFER
//	DrawToBuffer( bgrph->Graphics );
//	bgrph->Render( e->Graphics );
	bgrph->Render( grph );
#else
	DrawToBuffer( grph );
#endif
}

// ストップウォッチ開始
void Field::StopWatchStart(void)
{
	sw = gcnew Diagnostics::Stopwatch();
	sw->Start();	// ストップウォッチ開始
	keeptime = sw->ElapsedMilliseconds;
	bTickFlag = false;
	tickMsec = 33;			// 100msecに１回処理を行います。
}

// ストップウォッチの値チェック
bool Field::StopWatchWaitCheck(void)
{
	Int64 nowtime = sw->ElapsedMilliseconds;
	bool bRet = false;

	// 状況判定
	if( keeptime + tickMsec < nowtime )
	{	// ティック時間を超えた
		bRet = true;
		keeptime = nowtime - ( nowtime - keeptime - tickMsec );
	}

	return bRet;
}

// メインループ
bool Field::MainLoop(void)
{
	bool bb = true;

	// メッセージループ開始
	while( bb )
	{
		Application::DoEvents();

		// ん～msecに１回処理する
		if( StopWatchWaitCheck() )
		{	// 実行
			DoAllUnit();

#if USE_DOUBLE_BUFFER
			DrawToBuffer( bgrph->Graphics );
//			bgrph->Render( e->Graphics );
			if( bufferingMode == 2 )
			{
				if( fm->Visible )bgrph->Render( Graphics::FromHwnd( picBox->Handle ) );
			}
			else
			{
				fm->Refresh();
			}
#else
			DrawToBuffer( grph );
			fm->Refresh();
#endif
			// リフレッシュ処理
			//fm->Invalidate( false );
		}
		else
		{
			// 経過時間を表示し続ける。
			this->label->Text = "経過時間：" + sw->ElapsedMilliseconds.ToString() + "です。";
		}

		// ウィンドウが閉じられたかを判定する
		if( fm->Visible == false )
		{	// ウィンドウが閉じられたら、
			bb = false;		// プログラム終了
		}

	}

	return bb;
}

// トラックバー変動。
System::Void Field::trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	master_vol = trackBar1->Value / 100.0f;
	sa2->SetMasterVolume( master_vol );
}

// トラックバー変動。
System::Void Field::trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	bgm_vol = trackBar2->Value / 100.0f;
	sa2->SetSourceVolume( bgm_handle, bgm_vol );
}
