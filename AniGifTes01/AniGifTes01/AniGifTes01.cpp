// AniGifTes01.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "AniField.h"

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// メイン ウィンドウを作成して、実行します
	AniGifFieldForm	^obj = gcnew AniGifFieldForm();
	obj->Start();
	return 0;
}

/*

・課題：
フィールドとインターフェースの「疎結合」展開の無さ。
疎結合にする為には、どの様にすればよいか。

フィールド
ユニット
コントロールインターフェース

コントロールクラス。
コントロールクラスが、フィールドも、ユニットも、制御しなければ。

コントロールインタフェースは、
ユニットの為に、公開してあげなければ。

*/
