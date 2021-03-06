#include "stdafx.h"
#include "AniField.h"

// #include<gdiplus.h>
const int PropertyTagFrameDelay = 20736;


///////////////////////////////////////////////
// 共通サウンド定義

#define SOUND_PLAYID_BGM01		L"snd/sugar.wav"
#define SOUND_PLAYID_BGM02		L"snd/september13.wav"
#define SOUND_PLAYID_BGM03		L"snd/funkyd.wav"

#define SOUND_PLAYID_SE01		L"snd/udonken.wav"
#define SOUND_PLAYID_SE02		L"snd/sound1.wav"
#define SOUND_PLAYID_SE03		L"snd/sound2.wav"
#define SOUND_PLAYID_SE04		L"snd/sound3.wav"
#define SOUND_PLAYID_SE05		L"snd/naval_combat_1.wav"


///////////////////////////////////////////////
///////////////////////////////////////////////
// フィールドコントローラー

FieldControlInterface::FieldControlInterface()
{
	sa2 = gcnew SlimAudio2();	// サウンド初期化
	this->StopWatchStart();		// ストップウォッチ初期化
}

FieldControlInterface::~FieldControlInterface()
{
	cliext::list<BaseImageUnit^>::iterator ite = unit_idx.begin();
	while( ite != unit_idx.end() )
	{
		delete (*ite);
		ite++;
	}
	unit_idx.clear();

	delete sa2;
	delete this->sw;
}

void FieldControlInterface::SetFieldSize( int w, int h )
{
	this->width = w;
	this->height = h;
}

void FieldControlInterface::PlaySound( String ^str, bool bLoop )
{
	sa2->Play( str, 1.0f, bLoop );
}

void FieldControlInterface::DoControl(void)
{
	if( this->bMousePush )
	{
		this->bMousePush = false;
		// 弾丸発射
		AddFireArmsUnit( "img\\explosions-02.gif", true, this->GetMouseX() - 25, this->GetMouseY() );
		// 音声再生
		this->PlaySound( SOUND_PLAYID_SE05, false );
	}
}

void FieldControlInterface::StopWatchStart(void)
{
	this->sw = gcnew Diagnostics::Stopwatch();
	this->sw->Start();			// ストップウォッチ開始
	this->sw_vsync_keeptime = this->sw_unit_keeptime = this->sw->ElapsedMilliseconds;
	this->sw_vsync_ticktime = 40;		// VSYNC:33msecに１回処理を行います
	this->sw_unit_ticktime = 30;		// Action:20msecに１回処理を行います
}

bool FieldControlInterface::StopWatchWaitCheck2Unit(void)	// ユニットへの制御実行タイミング
{
	if( this->sw_unit_keeptime + this->sw_unit_ticktime < this->sw->ElapsedMilliseconds )
	{	// ティック時間を超えた
		this->sw_unit_keeptime = this->sw->ElapsedMilliseconds - ( this->sw->ElapsedMilliseconds - this->sw_unit_keeptime - this->sw_unit_ticktime );
		return true;
	}
	return false;
}

bool FieldControlInterface::StopWatchWaitCheck2VSync(void)	// 描画の制御実行タイミング
{
	if( this->sw_vsync_keeptime + this->sw_vsync_ticktime < this->sw->ElapsedMilliseconds )
	{	// ティック時間を超えた
		this->sw_vsync_keeptime = this->sw->ElapsedMilliseconds - ( this->sw->ElapsedMilliseconds - this->sw_vsync_keeptime - this->sw_vsync_ticktime );
		return true;
	}
	return false;
}

Int64 FieldControlInterface::GetNowTime(void)
{
	return this->sw->ElapsedMilliseconds;
}

void FieldControlInterface::DoAllUnit(void)
{
	cliext::list<BaseImageUnit^>::iterator ite = this->unit_idx.begin();

	while( ite != unit_idx.end() )
	{
		(*ite)->DoUnitAction();
		ite++;
	}
}

void FieldControlInterface::DrawAllUnit(Graphics^ g)
{
	cliext::list<BaseImageUnit^>::iterator ite = unit_idx.begin();
	while( ite != unit_idx.end() )
	{
		(*ite)->DrawUnit( g );	// 各ユニット描画指示
		ite++;
	}
}

void FieldControlInterface::AddBackGroundUnit( String ^str, bool b, int x, int y )
{
	BackGroundImageUnit ^obj = gcnew BackGroundImageUnit( this );
	obj->LoadImage( str, b, x, y );
	unit_idx.push_back( (BaseImageUnit^)obj );	// アップキャスト
}

void FieldControlInterface::AddEnemyUnit( String ^str, bool b, int x, int y, double dSpd, double dAng )
{
	EnemyImageUnit ^obj = gcnew EnemyImageUnit( this );
	obj->LoadImage( str, b, x, y );
	obj->SetSpeed( dSpd );
	obj->SetAngle( dAng );
	unit_idx.push_back( (BaseImageUnit^)obj );	// アップキャスト
}

void FieldControlInterface::AddFriendUnit( String ^str, bool b, int x, int y )
{
	FriendImageUnit ^obj = gcnew FriendImageUnit( this );
	obj->LoadImage( str, b, x, y );
	unit_idx.push_back( (BaseImageUnit^)obj );	// アップキャスト
}

void FieldControlInterface::AddFireArmsUnit( String ^str, bool b, int x, int y )
{
	FireArmsImageUnit ^obj = gcnew FireArmsImageUnit( this );
	obj->LoadImage( str, b, x, y );
	unit_idx.push_back( (BaseImageUnit^)obj );	// アップキャスト
}


///////////////////////////////////////////////
////////////////////////////////////////////////
// イメージユニット

BaseImageUnit::BaseImageUnit()
{
}

BaseImageUnit::BaseImageUnit( FieldControlInterface ^fc )
{
	this->fcif = fc;
}

Bitmap ^BaseImageUnit::GetBmp(void)
{
	return bmp;
}

int BaseImageUnit::GetWaitTime( int no )
{	// GIFアニメの各コマの待機時間は、プロパティ４バイト毎に格納されている。
	return this->pitem->Value[no*4] + (this->pitem->Value[no*4+1] << 8) +
			(this->pitem->Value[no*4+2] << 16) + (this->pitem->Value[no*4+3] << 24);
}

void BaseImageUnit::LoadImage( System::String ^str, bool b, int x, int y )
{
	this->bIsAnime = b;

	if( this->bIsAnime )
	{	// アニメGIFとして処理する
		this->bmp = gcnew Bitmap( str );
		this->fd = gcnew System::Drawing::Imaging::FrameDimension( this->bmp->FrameDimensionsList[0] );
		this->frame_count = 0;
		this->pitem = this->bmp->GetPropertyItem( PropertyTagFrameDelay );
		this->ticktime = GetWaitTime( this->frame_count ) * 10;
	}
	else
	{	// 静止画として処理する
		this->bmp = gcnew Bitmap( str );
	}
	this->dPosX = (double)x;
	this->dPosY = (double)y;
}

void BaseImageUnit::DoUnitBaseAction(void)
{
	this->nowtime = this->fcif->GetNowTime();

	if( this->bIsAnime )	// GIFアニメ更新チェック
	{	// アニメGIFの時の処理
		if( this->keeptime + this->ticktime < this->nowtime )
		{	// ティック時間を超えた
			this->keeptime = this->nowtime - ( this->nowtime - this->keeptime - this->ticktime );
			this->frame_count++;
			if( frame_count >= this->bmp->GetFrameCount( this->fd ) ) this->frame_count = 0;
			ChangeFrame();
			this->ticktime = GetWaitTime( this->frame_count ) * 10;
		}
	}
}

void BaseImageUnit::DrawBaseUnit( Graphics^ g )
{
	RectangleF SrcRect, DstRect;
	SrcRect = RectangleF( 0, 0, (float)this->bmp->Width, (float)this->bmp->Height );
	DstRect = RectangleF( (float)this->dPosX, (float)this->dPosY, (float)this->bmp->Width, (float)this->bmp->Height );
	g->DrawImage( this->bmp, DstRect, SrcRect, GraphicsUnit::Pixel );
}

void BaseImageUnit::ChangeFrame(void)
{
	this->bmp->SelectActiveFrame( this->fd, this->frame_count );
}

void BaseImageUnit::MoveUnitFromAngle(void)
{	// 推力(dSpeed)と角度(dAngle)を計算して、ユニットを移動する
	double movementX = ( this->dSpeed * Math::Cos( this->dAngle * Math::PI / 180.0 ) );
	double movementY = ( this->dSpeed * Math::Sin( this->dAngle * Math::PI / 180.0 ) );
	this->dPosX += movementX;
	this->dPosY += movementY;
}

bool BaseImageUnit::JudgeReflection( double width, double height )
{	// 反射をジャッジし、必要に応じて
	bool bJudge = false;

	// 反射判定（Ｙ軸）
	if( this->dPosY >= (height - this->bmp->Height) )	// 反射判定（下→上）
	{	// Ｙ軸下（1～179）
		if( this->dAngle < 180.0 )	// 画面下へ向かっているか？
		{	// 反射角度を変更（下から上へ）
			this->dAngle = 360.0 - this->dAngle;
			bJudge = true;
		}
	}
	else if( this->dPosY <= 1.0 )	// 反射判定（上→下）
	{	// Ｙ軸上（181～359）
		if( this->dAngle >= 180.0 )
		{	// 反射角度を変更（上から下へ）
			this->dAngle = 360.0 - this->dAngle;
			bJudge = true;
		}
	}
	// 反射判定（Ｘ軸）
	else if( this->dPosX  >= (width - this->bmp->Width) )	// 反射判定（右→左）
	{	// Ｘ軸右（271～359, 1～89）
		if( this->dAngle > 270.0 || this->dAngle < 90.0 )
		{
			this->dAngle = 180.0 - this->dAngle;
			if( this->dAngle < 0.0 ) this->dAngle = 360.0 + this->dAngle;	// マイナス化させない対策
			bJudge = true;
		}
	}
	else if( this->dPosX <= 1.0 )		// 反射判定（左→右）
	{	// Ｘ軸左（90～270）
		if( this->dAngle > 90.0 && this->dAngle < 270.0 )
		{
			this->dAngle = 180.0 - this->dAngle;
			if( this->dAngle < 0.0 ) this->dAngle = 360.0 + this->dAngle;	// マイナス化させない対策
			bJudge = true;
		}
	}
	return bJudge;
}


////////////////////////////////////////////////
// 背景ユニット

//BackGroundImageUnit::BackGroundImageUnit() {}
//BackGroundImageUnit::BackGroundImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc) {}

void BackGroundImageUnit::DoUnitAction(void)
{
	// 背景スクロールパターン１：じっくり下に動いていく。
	this->dPosY += 0.12;
	if( this->dPosY > this->fcif->GetFieldHeight() ) this->dPosY = 0.0;

	BaseImageUnit::DoUnitBaseAction();		// 親クラスのベースアクション関数を呼び出す
}

void BackGroundImageUnit::DrawUnit( Graphics^ g )
{
	RectangleF SrcRect, DstRect;

	// １枚目のイラスト
	SrcRect = RectangleF( 0, 0, (float)this->bmp->Width, (float)this->bmp->Height );
	DstRect = RectangleF( (float)this->dPosX, (float)this->dPosY, (float)this->bmp->Width, (float)this->bmp->Height );
	g->DrawImage( this->bmp, DstRect, SrcRect, GraphicsUnit::Pixel );

	//　あまった部分を２枚目として表示する
	double dPosY2 = this->dPosY - this->fcif->GetFieldHeight();

	SrcRect = RectangleF( 0, 0, (float)this->bmp->Width, (float)this->bmp->Height );
	DstRect = RectangleF( (float)this->dPosX, (float)dPosY2, (float)this->bmp->Width, (float)this->bmp->Height );
	g->DrawImage( this->bmp, DstRect, SrcRect, GraphicsUnit::Pixel );
}


////////////////////////////////////////////////
// 敵ユニット

//EnemyImageUnit::EnemyImageUnit(){}
EnemyImageUnit::EnemyImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)
{
	this->dAngle = 45.0;
}

void EnemyImageUnit::DoUnitAction(void)
{
//	this->dSpeed = 0.5;

	this->MoveUnitFromAngle();

	bool bb;
	bb = this->JudgeReflection( this->fcif->GetFieldWidth(), this->fcif->GetFieldHeight() );
	if( bb )
	{
		this->fcif->PlaySound( SOUND_PLAYID_SE01, false );
	}

	hogecnt++;
	if( hogecnt > 100 )
	{
		hogecnt = 0;
		this->fcif->PlaySound( SOUND_PLAYID_SE02, false );
	}

	BaseImageUnit::DoUnitBaseAction();		// 親クラスのベースアクション関数を呼び出す
}

void EnemyImageUnit::DrawUnit( Graphics^ g )
{
	BaseImageUnit::DrawBaseUnit( g );		// 親クラスの描画関数を呼び出す
}


////////////////////////////////////////////////
// 味方ユニット

//FriendImageUnit::FriendImageUnit() {}
//FriendImageUnit::FriendImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc) {}

void FriendImageUnit::DoUnitAction(void)
{
//	this->dPosX += 0.02;
//	this->dPosY -= 0.05;
	this->dPosX = this->fcif->GetMouseX();
	this->dPosY = this->fcif->GetMouseY();

	BaseImageUnit::DoUnitBaseAction();		// 親クラスのベースアクション関数を呼び出す
}

void FriendImageUnit::DrawUnit( Graphics^ g )
{
	BaseImageUnit::DrawBaseUnit( g );		// 親クラスの描画関数を呼び出す
}


////////////////////////////////////////////////
// 銃弾ユニット

void FireArmsImageUnit::DoUnitAction(void)
{
	this->dPosX -= 2.0;
	BaseImageUnit::DoUnitBaseAction();		// 親クラスのベースアクション関数を呼び出す
}

void FireArmsImageUnit::DrawUnit( Graphics^ g )
{
	BaseImageUnit::DrawBaseUnit( g );		// 親クラスの描画関数を呼び出す
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// フォーム

AniGifFieldForm::AniGifFieldForm()
{
	// 初期化処理
	this->bIsClosing = false;
	this->bIsClosed = false;

	// フォーム生成
	this->components = gcnew System::ComponentModel::Container();
	this->Size = System::Drawing::Size(20,20);
	this->Text = L"Form1";
	this->Padding = System::Windows::Forms::Padding(0);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->Closing += gcnew System::ComponentModel::CancelEventHandler( this, &AniGifFieldForm::ClosingEvent );
	this->Closed += gcnew System::EventHandler( this, &AniGifFieldForm::ClosedEvent );
}

System::Void AniGifFieldForm::ClosingEvent(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
{	// フォームがクローズする際に呼び出されるコールバック処理
	this->bIsClosing = true;
}

System::Void AniGifFieldForm::ClosedEvent(System::Object^ sender, System::EventArgs^ e)
{
	this->bIsClosed = true;
}

AniGifFieldForm::~AniGifFieldForm()
{
	if( components ) delete this->components;
	delete this->fcif;
}

void AniGifFieldForm::CloseForm(void)
{	// ウィンドウが閉じられてから、アプリ終了する迄の処理
}

void AniGifFieldForm::Start(void)
{
	this->FieldInit( 800, 480 );
	this->UnitInit();
	this->Show();

	this->fcif->PlaySound( SOUND_PLAYID_BGM02, true );

	this->MainLoop();
//	this->CloseForm();
}

void AniGifFieldForm::FieldInit( int w, int h )
{
	// フィールド初期化
	this->width = w;
	this->height = h;
	this->fcif = gcnew FieldControlInterface();
	this->fcif->SetFieldSize( this->width, this->height );

	this->SuspendLayout();

	// フィールド用ピクチャボックス生成
	this->picBox = (gcnew System::Windows::Forms::PictureBox());
	this->picBox->Location = System::Drawing::Point(0, 0);
	this->picBox->Name = L"GameFieldPicBox";
	this->picBox->Size = System::Drawing::Size( this->width, this->height );
	this->picBox->TabIndex = 0;
	this->picBox->TabStop = false;
	this->picBox->Paint += gcnew System::Windows::Forms::PaintEventHandler( this, &AniGifFieldForm::picBoxPaintEvent );
	this->picBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler( this, &AniGifFieldForm::picBoxMouseMove );
	this->picBox->MouseDown += gcnew System::Windows::Forms::MouseEventHandler( this, &AniGifFieldForm::picBoxMouseDown );
	this->Controls->Add( picBox );	// ピクチャボックスをフォームに登録する。

	// フォームサイズ変更
	this->Size = System::Drawing::Size(this->width,this->height);

	this->PerformLayout();

	// おそらく最初の１回だけでいけるはずだ。
	// フィールド用ビットマップ生成・接続
	this->picBmp = gcnew Bitmap( this->width, this->height );
	this->picBox->Image = picBmp;
	this->grph = Graphics::FromImage( this->picBox->Image );

	// ダブルバッファ初期化
	this->b_contex = BufferedGraphicsManager::Current;
	this->b_contex->MaximumBuffer = System::Drawing::Size( width + 1, height + 1 );
	this->b_bgrph = this->b_contex->Allocate( this->picBox->CreateGraphics(), Rectangle(0,0,this->width,this->height) );
	this->b_bufferingMode = 2;
	this->b_dbcnt = 0;
}

void AniGifFieldForm::UnitInit(void)
{
	// 背景追加
	this->fcif->AddBackGroundUnit( "img\\img0004.jpg", false, 0, 0 );
//	AddBackGroundUnit( "img\\8814368.jpg", false, 0, 0 );

	// 敵ユニット追加
	this->fcif->AddEnemyUnit( "img\\ucnJO.gif", true, 120, 70, 0.0, 15.0 );	// 要請
	this->fcif->AddEnemyUnit( "img\\dora_gif.gif", true, 5, 5, 1.0, 15.0 );	// どらえもん

	this->fcif->AddEnemyUnit( "img\\19ce22edc0e8de3115dd1f89d1bafc71.gif", true, 300, 100, 1.2, 87 );	// スパイダーマソ
	this->fcif->AddEnemyUnit( "img\\robots-14.gif", true, 10, 10, 5.0, 225.0 );	// ロボがゆらゆら
	this->fcif->AddEnemyUnit( "img\\atom-25.gif", true, 200, 200, 0.5, 342 );
	this->fcif->AddEnemyUnit( "img\\monstres-12.gif", true, 60, 350, 1.5, 210.0 );	// ドアが開いてキモイ。
	this->fcif->AddEnemyUnit( "img\\swirl.gif", true, 500, 40, 0.8, 144.0 );

	// 味方ユニット追加
	this->fcif->AddFriendUnit( "img\\s_fighter-13.gif", true, 200, 220 );
}

// 全オブジェクトを描画し、フィールドビットマップを更新する
void AniGifFieldForm::DrawToBuffer( Graphics^ g )
{
	// Clear the graphics buffer every five updates.
	if( ++this->b_dbcnt > 5 )
	{
		this->b_dbcnt = 0;
		g->FillRectangle( Brushes::Black, 0, 0, this->picBox->Width, this->picBox->Height );
	}

	this->fcif->DrawAllUnit( g );	// 全ユニットを描画
}

void AniGifFieldForm::MainLoop(void)
{
	bool bLoop = true;

	while( bLoop )
	{
		Application::DoEvents();

		if( !this->bIsClosing ) this->fcif->DoControl();	// コントロールディスパッチ

		if( this->fcif->StopWatchWaitCheck2Unit() )			// ユニット制御
		{
			if( !this->bIsClosing ) this->fcif->DoAllUnit();
		}

		if( this->fcif->StopWatchWaitCheck2VSync() )	// 描画タイミング制御
		{	// 実行
			if( !this->bIsClosing )
			{
				DrawToBuffer( b_bgrph->Graphics );
				this->b_bgrph->Render( Graphics::FromHwnd( this->picBox->Handle ) );
			}
		}
	
		if( this->bIsClosing )
		{	// ウィンドウが閉じられたら、プログラム終了
			bLoop = false;
		}
	}
}

void AniGifFieldForm::picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	if( !this->bIsClosing ) this->b_bgrph->Render( this->grph );
}

System::Void AniGifFieldForm::picBoxMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^ e)
{
	this->fcif->SetMouseX( e->X );
	this->fcif->SetMouseY( e->Y );
}

System::Void AniGifFieldForm::picBoxMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	this->fcif->SetMousePush( true );
}

/////////////////////////////////////////////////////////////////////////
/*
		int a;
		String ^ss;
		for( int i = 0; i < this->bmp->GetFrameCount( this->fd ); i++ )
		{
			int vvv, xxx;
			xxx = pitem->Len;
			for( int j = 0; j < xxx; j++ )
			{
//				vvv = pitem->Value[j];
//				ss = pitem->Value[j].ToString();
//				int aaa = 20;
			}
			int yyy;
			xxx = 20;
		}
//ImageAnimator+ImageInfo.​PropertyTagFrameDelay
/*
CGdiplusInit hoge;
	LPTSTR file = FILENAME;	// ファイル名
	Image gif( file);
	PropertyItem *propertyItem;
	GUID guid;
	int frames;

	wprintf( L"%s\n", file);
	gif.GetFrameDimensionsList( &guid, 1);	// 最初のディメンションのGUIDを取得
	frames = gif.GetFrameCount( &guid);		// コマ数を取得
	int nSize = gif.GetPropertyItemSize(PropertyTagFrameDelay);
	propertyItem = (PropertyItem *)new char[nSize];
	gif.GetPropertyItem( PropertyTagFrameDelay, nSize, propertyItem);

	for( int i = 0; i < frames; i++ ) {
		// アニメGIFではコマごとの待機時間が設定できるので、それを取得する。
		int taiki_jikan = ((long*) propertyItem->value)[i] * 10;
		wprintf( L"%d ", taiki_jikan);
	}
*/
		//		this->image->GetFrameCount( 
//		this->image->SelectActiveFrame( 
//		this->image->SelectActiveFrame( System::Drawing::Imaging::FrameDimension
		//	System::Drawing::Image	^image;
//		System::Drawing::ImageAnimator ^ani = gcnew System::Drawing::ImageAnimator();
/*
button2.Enabled=false;
                    Bitmap a=new Bitmap(openFileDialog1.FileName,true);
                    System.Drawing.Imaging.FrameDimension oFDimension;
                    oFDimension = new System.Drawing.Imaging.FrameDimension(a.FrameDimensionsList[0]);
                    String s;
                    for(int i=0;i<a.GetFrameCount(oFDimension);i++)
                    {
                        s=saveFileDialog1.FileName;
                        s+="_Page";s+=i;s+=".jpg";
                        a.SelectActiveFrame(oFDimension,i);
                        a.Save(s,ImageFormat.Jpeg);
                    }
                    button2.Enabled=true;
*/
