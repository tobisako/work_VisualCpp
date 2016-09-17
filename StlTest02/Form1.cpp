#include "stdafx.h"
//#include "taiya.h"
#include "Form1.h"


using namespace StlTest02;

// ゲーム初期化処理（コンストラクタ時）
void Form1::GameInit(void)
{
	srand((unsigned int)time(NULL));

	bChangeEventStatus = true;
	iEventStatusID = 0;					// eve_init
	iTimeCount = 0;
	iAppearCount = 0;
	iLevel = 1;
	bMouseIcon = true;
	iKao = 0;
	bKaoFlg = true;
	this->timer1->Interval = 200;		// １秒に１０回。
	this->timer1->Enabled = true;
	this->timer1->Start();
	this->label1->Text = "";

	// 中央メッセージウィンドウの生成
//	Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
//	pictureBox1->Image = bmp;
//	this->g = Graphics::FromImage( pictureBox1->Image );
}

// リプレイボタンを押した
System::Void Form1::Replay_Click(System::Object^  sender, System::EventArgs^  e)
{
//	// テスト用処理：ためしに生成してみる。
//	CTaiya	^obj = gcnew CTaiya();
//	obj->Create(50);
//	obj->SetPoint( 10, 10 );
//	this->Controls->Add( obj->GetpictureBox() );	// フォームへ連結する
//	this->Refresh();
//	lisidx.push_back( obj );

	// ゲーム再開する。
	GameInit();
}

// マウスムーブ動作
System::Void Form1::Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if( bMouseIcon )
	{
		// 主人公をマウスで動かす
		System::Drawing::Point curpoint;
		curpoint = this->pictureBox2->Location;
		curpoint.X = e->X - (this->pictureBox2->Size.Width / 2);
		curpoint.Y = e->Y - (this->pictureBox2->Size.Height / 2);
		this->pictureBox2->Location = curpoint;
	}
}
// マウスムーブ処理
System::Void Form1::pictureBox2_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if( bMouseIcon )
	{
		// PictureBoxの座標変更(PictureBox)
		System::Drawing::Point curpoint;
		curpoint = this->pictureBox2->Location;
		curpoint.X = e->X - (this->pictureBox2->Size.Width / 2) + this->pictureBox2->Location.X;
		curpoint.Y = e->Y - (this->pictureBox2->Size.Height / 2) + this->pictureBox2->Location.Y;
		this->pictureBox2->Location = curpoint;
	}
}

// フォームのペイント処理
System::Void Form1::Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
}

// ゲームクロック処理
System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	// タイマ毎にイベント処理動作を切り分ける
	if( bChangeEventStatus )
	{	// ステータス変更処理
		GameEventStatusChange();
	}
	else
	{	// 通常処理
		GameLoopProc();
	}

	// タイマ満了チェック処理
	if( iTimeCount > iExpireTime )
	{
		bChangeEventStatus = true;
	}
	iTimeCount ++;
//	this->Refresh();
}

// イベント処理（ここにおくのは良くない。）
typedef enum {
	eve_init,
	eve_start01,
	eve_start02,
	eve_playing,
	eve_nextstage,
	eve_gameover
};

// イベントステータス変更処理
void Form1::GameEventStatusChange(void)
{
	// 次のステータスへ切り替え、次のステータスの準備関数を実行する。
	// （ゲームオーバー時のみ、例外になっている。設計がダメ。）
	switch( iEventStatusID )
	{
	case eve_init:	// 初期化
		GamePrepare_Start();				// 準備処理
		iEventStatusID = eve_start01;		// 状態変更
		iAppearCount = 10;					// 最初の１匹だけ早めに出す（演出）
		iExpireTime = 8;
		break;
	case eve_start01:	// スタート１
		GamePrepare_Start();				// 準備処理
		iEventStatusID = eve_start02;		// 状態変更
		iExpireTime = 8;
		break;
	case eve_start02:	// スタート２
		GamePrepare_Playing();				// 準備処理
		iEventStatusID = eve_playing;		// 状態変更
		iExpireTime = 150;
		break;
	case eve_playing:	// プレイ中
		GamePrepare_NextStage();			// 準備処理
		iEventStatusID = eve_nextstage;		// 状態変更
		iExpireTime = 50;
		break;
	case eve_nextstage:	// 次のステージへ。
		GamePrepare_Playing();				// 準備処理
		iEventStatusID = eve_playing;		// 状態変更
		iExpireTime = 150;
		break;
	case eve_gameover:	// ゲームオーバー
		GamePrepare_GameOver();				// 準備処理
		break;
	}
	iTimeCount = 0;
	bChangeEventStatus = false;
}

// 準備処理：スタート時
void Form1::GamePrepare_Start(void)
{
	// メッセージウィンドウ
	if( eve_init == iEventStatusID )
	{
		// 中央メッセージウィンドウの生成
		Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
		pictureBox1->Image = bmp;
		this->g = Graphics::FromImage( pictureBox1->Image );

		// 文字列を描画
		Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
		this->g->Clear( Color::FromArgb(0, 0, 0) );
		this->g->DrawString( "Game", gcnew System::Drawing::Font("Arial", 16 ), brush, 0, 0 );
	}
	else
	{
		// 文字列を描画
		Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
		this->g->Clear( Color::FromArgb(0, 0, 0) );
		this->g->DrawString( "Start!!", gcnew System::Drawing::Font("Arial", 18 ), brush, 50, 30 );
		this->Refresh();
	}

	// 主人公の準備
	// 画像と連結
	Bitmap^ bmp2 = gcnew Bitmap( "res/icon81.jpg" );
	pictureBox2->Size = System::Drawing::Size(bmp2->Width, bmp2->Height);
	pictureBox2->Image = bmp2;
}

// 準備処理：スタート時
void Form1::GamePrepare_Playing(void)
{
	// スタートウィンドウを閉じる。
	// いきなり消してもいいけど、ゆっくりとじておこう。
}

// 準備処理：ステージクリア時
void Form1::GamePrepare_NextStage(void)
{
	this->pictureBox1->Size = System::Drawing::Size(370, 108);

	// メッセージを表示する。
	System::String ^sCleraMess = L"LEVEL " + iLevel + L" CLEAR!!\n";

	// 文字列を描画
	Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
	this->g->Clear( Color::FromArgb(0, 0, 0) );
	this->g->DrawString( sCleraMess, gcnew System::Drawing::Font("Arial", 18 ), brush, 50, 30 );
	this->Refresh();

	// レベルを進める。
	iLevel++;
}

// 準備処理：ゲームオーバー時
void Form1::GamePrepare_GameOver(void)
{
	// ゲームオーバー用画像に差し替える。
	Bitmap^ bmp3 = gcnew Bitmap( "res/koee.jpg" );
	pictureBox1->Size = System::Drawing::Size(bmp3->Width, bmp3->Height);
	pictureBox1->Image = bmp3;

	// 主人公の顔も残念にする。
	Bitmap^ bmp2 = gcnew Bitmap( "res/icon83.jpg" );
	pictureBox2->Size = System::Drawing::Size(bmp2->Width, bmp2->Height);
	pictureBox2->Image = bmp2;

	// メッセージを表示する。
	System::String ^sGameOver = "GAME OVER!!!!(Level=" + iLevel + ")";
	this->label1->Text = sGameOver->ToString();

	// マウス追従無効
	bMouseIcon = false;
}

// ゲームループ
void Form1::GameLoopProc(void)
{
	// イベント別処理
	switch( iEventStatusID )
	{
	case eve_start01:
	case eve_start02:
		GameLoop_Start();
		break;
	case eve_playing:
		GameLoop_Playing();
		break;
	}

	// 共通処理
	ite = lisidx.begin();
	while( ite != lisidx.end() )
	{
		// 敵ムーブ処理
		(*ite)->Move();

		// 当り判定を行う
		bool bAtari = (*ite)->HitCheck( pictureBox2->Location, pictureBox2->Size );
		if( bAtari )
		{	// ゲームオーバーに遷移しなければならない。
			bChangeEventStatus = true;
			iEventStatusID = eve_gameover;
		}

		// 画面の端っこにきたら、消してやる。
		System::Drawing::Point po = (*ite)->GetPoint();
		if( po.X > (this->ClientSize.Width - 30) )	// 手抜き処理。
		{
			CEnemy ^obj = (*ite);
			lisidx.erase( ite++ );	// リストから消去
			delete obj;				// インスタンス開放
		}
		else
		{
			ite++;
		}
	}

	iKao++;
	if( iKao > 8 && iEventStatusID != eve_gameover )
	{
		// 主人公の顔を変える。
		Bitmap^ bmp2;
		if( bKaoFlg )
		{
			bmp2 = gcnew Bitmap( "res/icon81.jpg" );
			bKaoFlg = false;
		}
		else
		{
			bmp2 = gcnew Bitmap( "res/icon82.jpg" );
			bKaoFlg = true;
		}
		pictureBox2->Size = System::Drawing::Size(bmp2->Width, bmp2->Height);
		pictureBox2->Image = bmp2;
		iKao = 0;
	}
}

// スタート時のループ処理、演出。
void Form1::GameLoop_Start( void )
{
}

// プレイ中のループ処理
void Form1::GameLoop_Playing( void )
{
	// ゲーム開始と同時に、ウィンドウを「ゆっくり」閉じましょう。
	System::Drawing::Size ss = pictureBox1->Size;
	ss.Width -= 20;
	if( ss.Width < 0 ) ss.Width = 0;
	ss.Height -= 10;
	if( ss.Height < 0 ) ss.Height = 0;
	pictureBox1->Size = ss;

	// 敵をゆっくり出現させましょう。
	iAppearCount++;

	if( iAppearCount > (15 + ( rand() % 5 - 2 )) - (iLevel*2) )
	{	// 敵を１っぴき出現させる。
		int num = rand() % 3;
		CEnemy ^obj;
		switch( rand() % iLevel )
		{
		case 0:		// タイヤ
			{
				CTaiya ^tai = gcnew CTaiya();		// タイヤを生成
				obj = (CEnemy^)tai;					// アップキャストする。
			}
			break;
		case 1:		// 爆弾
			{
				CBomb ^bom = gcnew CBomb();			// 爆弾を生成
				obj = (CEnemy^)bom;					// アップキャストする。
			}
			break;
		case 2:		// ブロック
		default:
			CBlock ^blk = gcnew CBlock();			// ブロックを生成
			obj = (CEnemy^)blk;						// アップキャストする。
			break;

		}
		obj->Create( this->ClientSize.Height, iLevel );
		this->Controls->Add( obj->GetpictureBox() );	// フォームへ連結する
		this->Refresh();
		lisidx.push_back( obj );

		iAppearCount = 0;
	}
}


// イベント１
void Form1::GameEvent_startdisp( void )
{
}
