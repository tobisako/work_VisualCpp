// Sankaku01.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"

using namespace Sankaku01;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// メイン ウィンドウを作成して、実行します
//	Application::Run(gcnew Form1());

	Field	^obj = gcnew Field();
	obj->Start();	// フィールド処理スタート

	return 0;
}
