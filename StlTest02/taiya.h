#pragma once

// タイヤ。

#include "stdafx.h"
//#include "Form1.h"

//using namespace StlTest02;

// エネミー
public ref class CEnemy {
public:
	System::Windows::Forms::PictureBox^	picBox;
	CEnemy();										// コンストラクタ
	~CEnemy();										// デストラクタ
	virtual void SetImageFileName(void);			// イメージファイル名をセットする
	virtual void SetImageFileName(int);				// イメージファイル名をセットする
	void Create(int ymax, int level);				// クリエイト
	System::Drawing::Point CEnemy::GetPoint(void);	// 座標取得
	void SetPoint( System::Drawing::Point );		// 座標指定１
	void SetPoint(int x, int y);					// 座標指定２
	void Draw(void);								// 描画処理
	void Move(void);								// 移動処理
	System::Windows::Forms::PictureBox^	GetpictureBox(void);	// Form連結用
	// 当り判定関数
	bool HitCheck(System::Drawing::Point &obj1p, System::Drawing::Size &obj1s);
protected:
	System::String ^sImageFileName;
private:
	int iLevel;										// 敵レベル
	int iSpeed;										// 敵スピード
	int iDirectionY;								// Ｙ座標の移動方向。
	int iSpeedY;									// Ｙ座標の移動速度。
	cliext::list<CEnemy^> lisidx;
};

// タイヤ
public ref class CTaiya : public CEnemy {
public:
	CTaiya();
	void SetImageFileName(void) new;	// 仮想関数を継承した場合「new」と書いて明示しなければwarningが出てウザイ。
	void SetImageFileName(int) new;
};

// 爆弾
public ref class CBomb : public CEnemy {
public:
	CBomb();
	void SetImageFileName(void) new;
	void SetImageFileName(int) new;
};

// ブロック
public ref class CBlock : public CEnemy {
public:
	CBlock();
	void SetImageFileName(void) new;
	void SetImageFileName(int) new;
};

public ref class CField {
public:
	int hoge;
private:
//	cliext::list<CEnemy^> lisidx3;
};
