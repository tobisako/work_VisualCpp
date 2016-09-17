#include "stdafx.h"
//#include "taiya.h"
#include "Form1.h"


using namespace StlTest02;

// �Q�[�������������i�R���X�g���N�^���j
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
	this->timer1->Interval = 200;		// �P�b�ɂP�O��B
	this->timer1->Enabled = true;
	this->timer1->Start();
	this->label1->Text = "";

	// �������b�Z�[�W�E�B���h�E�̐���
//	Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
//	pictureBox1->Image = bmp;
//	this->g = Graphics::FromImage( pictureBox1->Image );
}

// ���v���C�{�^����������
System::Void Form1::Replay_Click(System::Object^  sender, System::EventArgs^  e)
{
//	// �e�X�g�p�����F���߂��ɐ������Ă݂�B
//	CTaiya	^obj = gcnew CTaiya();
//	obj->Create(50);
//	obj->SetPoint( 10, 10 );
//	this->Controls->Add( obj->GetpictureBox() );	// �t�H�[���֘A������
//	this->Refresh();
//	lisidx.push_back( obj );

	// �Q�[���ĊJ����B
	GameInit();
}

// �}�E�X���[�u����
System::Void Form1::Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if( bMouseIcon )
	{
		// ��l�����}�E�X�œ�����
		System::Drawing::Point curpoint;
		curpoint = this->pictureBox2->Location;
		curpoint.X = e->X - (this->pictureBox2->Size.Width / 2);
		curpoint.Y = e->Y - (this->pictureBox2->Size.Height / 2);
		this->pictureBox2->Location = curpoint;
	}
}
// �}�E�X���[�u����
System::Void Form1::pictureBox2_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if( bMouseIcon )
	{
		// PictureBox�̍��W�ύX(PictureBox)
		System::Drawing::Point curpoint;
		curpoint = this->pictureBox2->Location;
		curpoint.X = e->X - (this->pictureBox2->Size.Width / 2) + this->pictureBox2->Location.X;
		curpoint.Y = e->Y - (this->pictureBox2->Size.Height / 2) + this->pictureBox2->Location.Y;
		this->pictureBox2->Location = curpoint;
	}
}

// �t�H�[���̃y�C���g����
System::Void Form1::Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
}

// �Q�[���N���b�N����
System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	// �^�C�}���ɃC�x���g���������؂蕪����
	if( bChangeEventStatus )
	{	// �X�e�[�^�X�ύX����
		GameEventStatusChange();
	}
	else
	{	// �ʏ폈��
		GameLoopProc();
	}

	// �^�C�}�����`�F�b�N����
	if( iTimeCount > iExpireTime )
	{
		bChangeEventStatus = true;
	}
	iTimeCount ++;
//	this->Refresh();
}

// �C�x���g�����i�����ɂ����̂͗ǂ��Ȃ��B�j
typedef enum {
	eve_init,
	eve_start01,
	eve_start02,
	eve_playing,
	eve_nextstage,
	eve_gameover
};

// �C�x���g�X�e�[�^�X�ύX����
void Form1::GameEventStatusChange(void)
{
	// ���̃X�e�[�^�X�֐؂�ւ��A���̃X�e�[�^�X�̏����֐������s����B
	// �i�Q�[���I�[�o�[���̂݁A��O�ɂȂ��Ă���B�݌v���_���B�j
	switch( iEventStatusID )
	{
	case eve_init:	// ������
		GamePrepare_Start();				// ��������
		iEventStatusID = eve_start01;		// ��ԕύX
		iAppearCount = 10;					// �ŏ��̂P�C�������߂ɏo���i���o�j
		iExpireTime = 8;
		break;
	case eve_start01:	// �X�^�[�g�P
		GamePrepare_Start();				// ��������
		iEventStatusID = eve_start02;		// ��ԕύX
		iExpireTime = 8;
		break;
	case eve_start02:	// �X�^�[�g�Q
		GamePrepare_Playing();				// ��������
		iEventStatusID = eve_playing;		// ��ԕύX
		iExpireTime = 150;
		break;
	case eve_playing:	// �v���C��
		GamePrepare_NextStage();			// ��������
		iEventStatusID = eve_nextstage;		// ��ԕύX
		iExpireTime = 50;
		break;
	case eve_nextstage:	// ���̃X�e�[�W�ցB
		GamePrepare_Playing();				// ��������
		iEventStatusID = eve_playing;		// ��ԕύX
		iExpireTime = 150;
		break;
	case eve_gameover:	// �Q�[���I�[�o�[
		GamePrepare_GameOver();				// ��������
		break;
	}
	iTimeCount = 0;
	bChangeEventStatus = false;
}

// ���������F�X�^�[�g��
void Form1::GamePrepare_Start(void)
{
	// ���b�Z�[�W�E�B���h�E
	if( eve_init == iEventStatusID )
	{
		// �������b�Z�[�W�E�B���h�E�̐���
		Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
		pictureBox1->Image = bmp;
		this->g = Graphics::FromImage( pictureBox1->Image );

		// �������`��
		Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
		this->g->Clear( Color::FromArgb(0, 0, 0) );
		this->g->DrawString( "Game", gcnew System::Drawing::Font("Arial", 16 ), brush, 0, 0 );
	}
	else
	{
		// �������`��
		Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
		this->g->Clear( Color::FromArgb(0, 0, 0) );
		this->g->DrawString( "Start!!", gcnew System::Drawing::Font("Arial", 18 ), brush, 50, 30 );
		this->Refresh();
	}

	// ��l���̏���
	// �摜�ƘA��
	Bitmap^ bmp2 = gcnew Bitmap( "res/icon81.jpg" );
	pictureBox2->Size = System::Drawing::Size(bmp2->Width, bmp2->Height);
	pictureBox2->Image = bmp2;
}

// ���������F�X�^�[�g��
void Form1::GamePrepare_Playing(void)
{
	// �X�^�[�g�E�B���h�E�����B
	// �����Ȃ�����Ă��������ǁA�������Ƃ��Ă������B
}

// ���������F�X�e�[�W�N���A��
void Form1::GamePrepare_NextStage(void)
{
	this->pictureBox1->Size = System::Drawing::Size(370, 108);

	// ���b�Z�[�W��\������B
	System::String ^sCleraMess = L"LEVEL " + iLevel + L" CLEAR!!\n";

	// �������`��
	Brush^ brush = gcnew SolidBrush( Color::FromArgb(255, 20, 255) );
	this->g->Clear( Color::FromArgb(0, 0, 0) );
	this->g->DrawString( sCleraMess, gcnew System::Drawing::Font("Arial", 18 ), brush, 50, 30 );
	this->Refresh();

	// ���x����i�߂�B
	iLevel++;
}

// ���������F�Q�[���I�[�o�[��
void Form1::GamePrepare_GameOver(void)
{
	// �Q�[���I�[�o�[�p�摜�ɍ����ւ���B
	Bitmap^ bmp3 = gcnew Bitmap( "res/koee.jpg" );
	pictureBox1->Size = System::Drawing::Size(bmp3->Width, bmp3->Height);
	pictureBox1->Image = bmp3;

	// ��l���̊���c�O�ɂ���B
	Bitmap^ bmp2 = gcnew Bitmap( "res/icon83.jpg" );
	pictureBox2->Size = System::Drawing::Size(bmp2->Width, bmp2->Height);
	pictureBox2->Image = bmp2;

	// ���b�Z�[�W��\������B
	System::String ^sGameOver = "GAME OVER!!!!(Level=" + iLevel + ")";
	this->label1->Text = sGameOver->ToString();

	// �}�E�X�Ǐ]����
	bMouseIcon = false;
}

// �Q�[�����[�v
void Form1::GameLoopProc(void)
{
	// �C�x���g�ʏ���
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

	// ���ʏ���
	ite = lisidx.begin();
	while( ite != lisidx.end() )
	{
		// �G���[�u����
		(*ite)->Move();

		// ���蔻����s��
		bool bAtari = (*ite)->HitCheck( pictureBox2->Location, pictureBox2->Size );
		if( bAtari )
		{	// �Q�[���I�[�o�[�ɑJ�ڂ��Ȃ���΂Ȃ�Ȃ��B
			bChangeEventStatus = true;
			iEventStatusID = eve_gameover;
		}

		// ��ʂ̒[�����ɂ�����A�����Ă��B
		System::Drawing::Point po = (*ite)->GetPoint();
		if( po.X > (this->ClientSize.Width - 30) )	// �蔲�������B
		{
			CEnemy ^obj = (*ite);
			lisidx.erase( ite++ );	// ���X�g�������
			delete obj;				// �C���X�^���X�J��
		}
		else
		{
			ite++;
		}
	}

	iKao++;
	if( iKao > 8 && iEventStatusID != eve_gameover )
	{
		// ��l���̊��ς���B
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

// �X�^�[�g���̃��[�v�����A���o�B
void Form1::GameLoop_Start( void )
{
}

// �v���C���̃��[�v����
void Form1::GameLoop_Playing( void )
{
	// �Q�[���J�n�Ɠ����ɁA�E�B���h�E���u�������v���܂��傤�B
	System::Drawing::Size ss = pictureBox1->Size;
	ss.Width -= 20;
	if( ss.Width < 0 ) ss.Width = 0;
	ss.Height -= 10;
	if( ss.Height < 0 ) ss.Height = 0;
	pictureBox1->Size = ss;

	// �G���������o�������܂��傤�B
	iAppearCount++;

	if( iAppearCount > (15 + ( rand() % 5 - 2 )) - (iLevel*2) )
	{	// �G���P���҂��o��������B
		int num = rand() % 3;
		CEnemy ^obj;
		switch( rand() % iLevel )
		{
		case 0:		// �^�C��
			{
				CTaiya ^tai = gcnew CTaiya();		// �^�C���𐶐�
				obj = (CEnemy^)tai;					// �A�b�v�L���X�g����B
			}
			break;
		case 1:		// ���e
			{
				CBomb ^bom = gcnew CBomb();			// ���e�𐶐�
				obj = (CEnemy^)bom;					// �A�b�v�L���X�g����B
			}
			break;
		case 2:		// �u���b�N
		default:
			CBlock ^blk = gcnew CBlock();			// �u���b�N�𐶐�
			obj = (CEnemy^)blk;						// �A�b�v�L���X�g����B
			break;

		}
		obj->Create( this->ClientSize.Height, iLevel );
		this->Controls->Add( obj->GetpictureBox() );	// �t�H�[���֘A������
		this->Refresh();
		lisidx.push_back( obj );

		iAppearCount = 0;
	}
}


// �C�x���g�P
void Form1::GameEvent_startdisp( void )
{
}
