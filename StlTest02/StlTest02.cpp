// StlTest02.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"

using namespace StlTest02;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	System::String ^abc;

	abc = "fdfe";

	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// メイン ウィンドウを作成して、実行します
	Application::Run(gcnew Form1());
	return 0;
}
