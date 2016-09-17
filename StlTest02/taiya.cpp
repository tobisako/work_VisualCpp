
#include "stdafx.h"
//#include "taiya.h"
#include "Form1.h"

using namespace StlTest02;


////////////////////////////////////////////////
// �G�e�N���X

// �R���X�g���N�^
CEnemy::CEnemy()
{
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
//	delete bmp;
	delete picBox;
	int a = 10;
}

// �I�u�W�F�N�g��������
void CEnemy::Create(int ymax, int level)
{
	int y = rand() % ymax;
	iLevel = level;
	iSpeed = 5 * ( rand() % (iLevel * 3) + 1 );

	// �x���W���Z
	if( iLevel < 3 )
	{
		iDirectionY = 0;
		iSpeedY = 0;
	}
	else
	{	// ���x���R�ȏ�ŕω������܂��傤�B
		iDirectionY = rand() % 3 - 1;
		iSpeedY = rand() % iLevel;
	}

	// �C���X�^���X����
	picBox = (gcnew System::Windows::Forms::PictureBox());
	picBox->Location = System::Drawing::Point(0, y);
	picBox->Name = L"pictureBox1";
//	picBox->TabIndex = 0;
//	picBox->TabStop = false;

	// �摜�ƘA��
	Bitmap^ bmp = gcnew Bitmap( sImageFileName->ToString() );
	picBox->Size = System::Drawing::Size(bmp->Width, bmp->Height);
	picBox->Image = bmp;
//	g = Graphics::FromImage( pictureBox1->Image );
}

// �t�@�C���f���o���ɂ��āB
System::Windows::Forms::PictureBox^	CEnemy::GetpictureBox(void)
{
	return picBox;
}

// �t�@�C�����Z�b�g
void CEnemy::SetImageFileName(void)
{
	sImageFileName = L"�łӂ����.png";
}

// �t�@�C�����Z�b�g
void CEnemy::SetImageFileName(int i)
{
	SetImageFileName();
}

// ���W�擾
System::Drawing::Point CEnemy::GetPoint(void)
{
	return picBox->Location;
}

// ���W�w��P
void CEnemy::SetPoint(System::Drawing::Point po)
{
	picBox->Location = po;
}

// ���W�ύX�Q
void CEnemy::SetPoint(int x, int y)
{
	picBox->Location = System::Drawing::Point(x, y);
}

// �`��
void CEnemy::Draw(void)
{
}

// �ړ�
void CEnemy::Move(void)
{
	int iPlusX = 0;
	int iPlusY = 0;
	// �u�k�S�ȏ�̎��͓��ꏈ��������B
	if( iLevel >= 4 )
	{
		iPlusX = rand() % 5 - 2;
	}

	// �u�k�T�ȏ�̎������ꏈ�����B
	if( iLevel >= 5 )
	{
		iPlusY = rand() % 10;
	}

	// �ړ�������B
	System::Drawing::Point po = picBox->Location;
	po.X += iSpeed + iPlusX;
	po.Y += (iDirectionY * (iSpeedY + iPlusY) );
	picBox->Location = po;
}

// ���蔻��F�����Ɣ�r����B
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
// �^�C����

// �R���X�g���N�^
CTaiya::CTaiya()
{
	SetImageFileName();		// �q�N���X�Ńt�@�C�������Z�b�g
//	Create();				// �e�N���X�̃N���G�C�g���������s
}

// �t�@�C�����Z�b�g
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

// ����
void CTaiya::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


//////////////////////////////////////////////////
// ���e��Y

// �R���X�g���N�^
CBomb::CBomb()
{
	SetImageFileName();		// �q�N���X�Ńt�@�C�������Z�b�g
//	Create();				// �e�N���X�̃N���G�C�g���������s
}

// �t�@�C�����Z�b�g
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

// ����
void CBomb::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


//////////////////////////////////////////////////
// �u���b�N�B

// �R���X�g���N�^
CBlock::CBlock()
{
	SetImageFileName();		// �q�N���X�Ńt�@�C�������Z�b�g
}

// �t�@�C�����Z�b�g
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

// ����
void CBlock::SetImageFileName(void)
{
	SetImageFileName( rand() % 6 + 1 );
}


