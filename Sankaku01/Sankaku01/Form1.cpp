// Sankaku01.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"

using namespace Sankaku01;

// ボタン１押下
System::Void Form1::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
//	Graphics^	grpPicBox;
//	Bitmap^ srcbmpImage = gcnew Bitmap( 200, 100 );

	// フォームに直接描画する際に一番簡単な方法。
	Graphics^ gr = this->CreateGraphics();
	gr->DrawLine( Pens::Red, 10.0f, 10.0f, 120.0f, 120.0f );
//	gr->DrawPie(

	System::String ^str = gcnew System::String( "" );

	double theta;	// Θ（シータ、角度）
	double ata;	// Θ（シータ、角度）

	ata = Math::Atan2( 100.0f, 250.0f );
	theta = Math::Tan( ata );


	str = "値は、" + theta + "です。";

	this->label1->Text = str;

	// グラフィック
}
