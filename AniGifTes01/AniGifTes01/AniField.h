#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


///////////////////////////////////////////////
///////////////////////////////////////////////
// BaseImageUnitとAniGifFieldFormを結ぶインターフェース

ref class BaseImageUnit;	// 前方参照

// フィールドへのインターフェースクラス
public ref class FieldControlInterface {
public:
	FieldControlInterface();
	~FieldControlInterface();
	void SetFieldSize( int w, int h );
	int GetFieldWidth(void) { return width; }
	int GetFieldHeight(void) { return height; }
	void PlaySound( String ^str, bool bLoop );
	void DoControl(void);
	void StopWatchStart(void);						// ストップウォッチ開始
	bool StopWatchWaitCheck2Unit(void);				// ユニットへの制御実行タイミング
	bool StopWatchWaitCheck2VSync(void);			// 描画の制御実行タイミング
	Int64 GetNowTime(void);							// 現在時刻を返す
	void SetMouseX(int a) { iMouseX = a; }
	void SetMouseY(int a) { iMouseY = a; }
	void SetMousePush(bool b) { bMousePush = b; }
	int GetMouseX(void) { return iMouseX; }
	int GetMouseY(void) { return iMouseY; }
	void DoAllUnit(void);							// 全ユニットに行動指令
	void DrawAllUnit(Graphics^ g);					// 全ユニットに描画指示
	void AddBackGroundUnit( String ^str, bool b, int x, int y );
	void AddEnemyUnit( String ^str, bool b, int x, int y, double dSpd, double dAng );
	void AddFriendUnit( String ^str, bool b, int x, int y );
	void AddFireArmsUnit(String ^str, bool b, int x, int y);						// 銃弾を作成する
private:
	cliext::list<BaseImageUnit^>	unit_idx;		// ユニット全体をリスト管理
	SlimAudio2	^sa2;
	int			width;
	int			height;
	int			iMouseX;
	int			iMouseY;
	bool		bMousePush;							// ボタン押したフラグ
	Diagnostics::Stopwatch		^sw;				// ストップウォッチ
//	bool	bSWTickFlag;							// ティック時間用フラグ
//	Int64	sw_nowtime;								// 現在時間
	Int64	sw_vsync_keeptime;						// 時間保存用
	Int64	sw_vsync_ticktime;						// ティック時間管理値(33msecとか)
	Int64	sw_unit_keeptime;						// 時間保存用
	Int64	sw_unit_ticktime;						// ティック時間管理値(33msecとか)
};


///////////////////////////////////////////////

// ユニット基本クラス
public ref class BaseImageUnit abstract {
public:
	BaseImageUnit();
	BaseImageUnit( FieldControlInterface ^fc );
	void LoadImage( System::String ^str, bool b, int x, int y );	// ファイル名を指定する
	Bitmap ^GetBmp(void);
	void DoUnitBaseAction(void);					// ベースアクション
	virtual void DoUnitAction(void) abstract;		// ※純粋仮想関数
	void DrawBaseUnit( Graphics^ g );				// ベース描画
	virtual void DrawUnit( Graphics^ g ) abstract;	// ※純粋仮想関数
	void ChangeFrame(void);
	int GetWaitTime( int frame_no );
	double	GetX(void) { return dPosX; }			// アクセサ
	double	GetY(void) { return dPosY; }			// アクセサ
	void SetSpeed(double s) { dSpeed = s; }			// アクセサ
	void SetAngle(double a) { dAngle = a; }			// アクセサ
protected:
	FieldControlInterface	^fcif;			// フィールドコントロールインタフェース
	Bitmap					^bmp;			// ユニットのビットマップ
	double	dPosX;										// 座標Ｘ
	double	dPosY;										// 座標Ｙ
	double	dSpeed;										// 推進速度
	double	dAngle;										// 進行角度(0～359.999くらいまで)
	Int64	keeptime;									// 時間保存用
	Int64	nowtime;									// 現在時間
	Int64	ticktime;									// ティック時間管理値(33msecとか)
	bool JudgeReflection(double width, double height);	// 反射をジャッジする
	void MoveUnitFromAngle(void);
private:
	System::Drawing::Imaging::FrameDimension	^fd;
	System::Drawing::Imaging::PropertyItem		^pitem;
	int		frame_count;
	bool	bIsAnime;
};

// 背景ユニット
public ref class BackGroundImageUnit : BaseImageUnit {
public:
	BackGroundImageUnit() {}
	BackGroundImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};

// 敵キャラユニット
public ref class EnemyImageUnit : BaseImageUnit {
public:
	EnemyImageUnit() {}
	EnemyImageUnit( FieldControlInterface ^fc );
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
private:
	int hogecnt;
};

// 味方ユニット
public ref class FriendImageUnit : BaseImageUnit {
public:
	FriendImageUnit() {}
	FriendImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};

// 銃弾ユニット
public ref class FireArmsImageUnit : BaseImageUnit {
public:
	FireArmsImageUnit() {}
	FireArmsImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};


///////////////////////////////////////////////

// アニメGIFフィールド
public ref class AniGifFieldForm : public System::Windows::Forms::Form
{
public:
	AniGifFieldForm();
	~AniGifFieldForm();
	void CloseForm(void);
	void Start(void);
	void FieldInit(int,int);
	void UnitInit(void);
	void MainLoop(void);
private:
	FieldControlInterface				^fcif;			// フィールドコントロールインタフェース
	System::ComponentModel::Container	^components;	// コンポーネント
	System::Windows::Forms::PictureBox	^picBox;		// フィールド用PictureBox
	System::Drawing::Graphics			^grph;			// フィールド用Graphics
	Bitmap								^picBmp;		// フィールドBitmap
	bool	bIsClosing;									// フォーム終了中チェック
	bool	bIsClosed;									// フォーム終了チェック
	int		width;
	int		height;
	BufferedGraphicsContext			^b_contex;			// ダブルバッファ用
	BufferedGraphics				^b_bgrph;			// ダブルバッファ用
	Byte							b_bufferingMode;	// ダブルバッファ用
	Byte							b_dbcnt;			// ダブルバッファ用
	System::Void picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);	// ドローイベント処理？
	System::Void picBoxMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);	// マウスイベント処理
	System::Void picBoxMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);	// マウスボタン処理
	System::Void ClosingEvent(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);
	System::Void ClosedEvent(System::Object^ sender, System::EventArgs^ e);
	void DrawToBuffer( Graphics^ g );
};
