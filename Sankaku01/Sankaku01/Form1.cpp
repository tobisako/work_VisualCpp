// Sankaku01.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "Form1.h"

using namespace Sankaku01;

// �{�^���P����
System::Void Form1::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
//	Graphics^	grpPicBox;
//	Bitmap^ srcbmpImage = gcnew Bitmap( 200, 100 );

	// �t�H�[���ɒ��ڕ`�悷��ۂɈ�ԊȒP�ȕ��@�B
	Graphics^ gr = this->CreateGraphics();
	gr->DrawLine( Pens::Red, 10.0f, 10.0f, 120.0f, 120.0f );
//	gr->DrawPie(

	System::String ^str = gcnew System::String( "" );

	double theta;	// ���i�V�[�^�A�p�x�j
	double ata;	// ���i�V�[�^�A�p�x�j

	ata = Math::Atan2( 100.0f, 250.0f );
	theta = Math::Tan( ata );


	str = "�l�́A" + theta + "�ł��B";

	this->label1->Text = str;

	// �O���t�B�b�N
}
