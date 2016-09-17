
#include "stdafx.h"
//#include "taiya.h"
#include "Form1.h"

using namespace StlTest02;


////////////////////////////////////////////////
// 敵親クラス

// コンストラクタ
CEnemy::CEnemy()
{
}

// デストラクタ
CEnemy::~CEnemy()
{
//	delete bmp;
	delete picBox;
	int a = 10;
}

// オブジェクト生成処理
void CEnemy::Create(int ymax, int level)
{
	int y = rand() % ymax;
	iLevel = level;
	iSpeed = 5 * ( rand() % (iLevel * 3) + 1 );

	// Ｙ座標演算
	if( iLevel < 3 )
	{
		iDirectionY = 0;
		iSpeedY = 0;
	}
	else
	{	// レベル３以上で変化をつけましょう。
		iDirectionY = rand() % 3 - 1;
		iSpeedY = rand() % iLevel;
	}

	// インスタンス生成
	picBox = (gcnew System::Windows::Forms::PictureBox());
	picBox->Location = System::Drawing::Point(0, y);
	picBox->Name = L"pictureBox1";
//	picBox->TabIndex = 0;
//	picBox->TabStop = false;

	// 画像と連結
	Bitmap^ bmp = gcnew Bitmap( sImageFileName->ToString() );
	picBox->Size = System::Drawing::Size(bmp->Width, bmp->Height);
	picBox->Image = bmp;
//	g = Graphics::FromImage( pictureBox1->Image );
}

// ファイル吐き出しについて。
System::Windows::Forms::PictureBox^	CEnemy::GetpictureBox(void)
{
	return picBox;
}

// ファイル名セット
void CEnemy::SetImageFileName(void)
{
	sImageFileName = L"でふぉると.png";
}

// ファイル名セット
void CEnemy::SetImageFileName(int i)
{
	SetImageFileName();
}

// 座標取得
System::Drawing::Point CEnemy::GetPoint(void)
{
	return picBox->Location;
}

// 座標指定１
void CEnemy::SetPoint(System::Drawing::Point po)
{
	picBox->Location = po;
}

// 座標変更２
void CEnemy::SetPoint(int x, int y)
{
	picBox->Location = System::Drawing::Point(x, y);
}

// 描画
void CEnemy::Draw(void)
{
}

// 移動
void CEnemy::Move(void)
{
	int iPlusX = 0;
	int iPlusY = 0;
	// ＶＬ４以上の時は特殊処理を入れる。
	if( iLevel >= 4 )
	{
		iPlusX = rand() % 5 - 2;
	}

	// ＶＬ５以上の時も特殊処理を。
	if( iLevel >= 5 )
	{
		iPlusY = rand() % 10;
	}

	// 移動させる。
	System::Drawing::Point po = picBox->Location;
	po.X += iSpeed + iPlusX;
	po.Y += (iDirectionY * (iSpeedY + iPlusY) );
	picBox->Location = po;
}

// 当り判定：自分と比較する。
bool CEnemy::HitCheck(System::Drawing::Point &obj1p, System::Drawing::Size &obj1s)
{
	System::Drawing::Point obj2p = picBox->Location;
	System::Drawing::Size obj2s = picBox->Size;

	int obj1_top	= obj1p.Y;
	int obj1_left	= obj1p.X;
	int obj1_bottom	= obj1_top + obj1s.Height;
	int	obj1_right	= obj1_left + obj1s.Width;
	int obj2_top	= obj2p.Y;
	int obj2_left	= obj2p.X;
	int obj2_bottom	= obj2_top + obj2s.Height;
	int	obj2_right	= obj2_left + obj2s.Width;
	bool bRet = false;

	if((obj1_left < obj2_right) && (obj1_right > obj2_left) && (obj1_top < obj2_bottom) && (obj1_bottom > obj2_top))
	{
		bRet = true;
	}
	return bRet;
}


//////////////////////////////////////////////////
// タイヤ館

// コンストラクタ
CTaiya::CTaiya()
{
	SetImageFileName();		// 子クラスでファイル名をセット
//	Create();				// 親クラスのクリエイト処理を実行
}

// ファイル名セット
void CTaiya::SetImageFileName( int i )
{
	switch( i )
	{
	case 1:
		sImageFileName = L"res/roue-icone-5136-16.png";
		break;
	case 2:
		sImageFileName = L"res/roue-icone-5136-32.png";
		break;
	case 3:
		sImageFileName = L"res/roue-icone-5136-48.png";
		break;
	case 4:
		sImageFileName = L"res/roue-icone-5136-64.png";
		break;
	case 5:
		sImageFileName = L"res/roue-icone-5136-96.png";
		break;
	case 6:
	default:
		sImageFileName = L"res/roue-icone-5136-128.png";
		break;
	}
}

// 乱数
void CTaiya::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


//////////////////////////////////////////////////
// 爆弾野郎

// コンストラクタ
CBomb::CBomb()
{
	SetImageFileName();		// 子クラスでファイル名をセット
//	Create();				// 親クラスのクリエイト処理を実行
}

// ファイル名セット
void CBomb::SetImageFileName( int i )
{
	switch( i )
	{
	case 1:
		sImageFileName = L"res/chaine-chomper-icone-9045-16.png";
		break;
	case 2:
		sImageFileName = L"res/chaine-chomper-icone-9045-32.png";
		break;
	case 3:
		sImageFileName = L"res/chaine-chomper-icone-9045-48.png";
		break;
	case 4:
		sImageFileName = L"res/chaine-chomper-icone-9045-64.png";
		break;
	case 5:
		sImageFileName = L"res/chaine-chomper-icone-9045-96.png";
		break;
	case 6:
	default:
		sImageFileName = L"res/chaine-chomper-icone-9045-128.png";
		break;
	}
}

// 乱数
void CBomb::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


//////////////////////////////////////////////////
// ブロック。

// コンストラクタ
CBlock::CBlock()
{
	SetImageFileName();		// 子クラスでファイル名をセット
}

// ファイル名セット
void CBlock::SetImageFileName( int i )
{
	switch( i )
	{
	case 1:
		sImageFileName = L"res/bloc-question-retro--icone-6383-16.png";
		break;
	case 2:
		sImageFileName = L"res/bloc-question-retro--icone-6383-32.png";
		break;
	case 3:
		sImageFileName = L"res/bloc-question-retro--icone-6383-48.png";
		break;
	case 4:
		sImageFileName = L"res/bloc-question-retro--icone-6383-64.png";
		break;
	case 5:
		sImageFileName = L"res/bloc-question-retro--icone-6383-96.png";
		break;
	case 6:
	default:
		sImageFileName = L"res/bloc-question-retro--icone-6383-128.png";
		break;
	}
}

// 乱数
void CBlock::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


