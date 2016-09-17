// Sankaku01.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "Form1.h"
#include "Field.h"

using namespace Sankaku01;


/////////////////////////////////////////////

// �R���X�g���N�^
SourceVoiceUnit::SourceVoiceUnit( SlimDX::XAudio2::XAudio2 ^xDev, String ^str )
{
	this->xDev = xDev;								// XAudio2�ێ�
	bWaveStreamReady = GenerateWaveStream( str );	// WaveStream�����i�t�@�C���ǂݍ��݁j
	this->bPlaying = false;							// �܂��Đ����Ȃ�
}

// �f�X�g���N�^
SourceVoiceUnit::~SourceVoiceUnit()
{
	delete this->xSrcVoice;
	delete this->xAudioBuf;
	delete this->xWaveStream;
}

// �\�[�X�{�C�X�ݒ�
void SourceVoiceUnit::ConfigSourceVoice( float vol, bool bLoop )
{
	this->xSrcVoice = gcnew SlimDX::XAudio2::SourceVoice(xDev, this->xWaveStream->Format);
	this->xAudioBuf = gcnew SlimDX::XAudio2::AudioBuffer();
	this->xAudioBuf->AudioBytes = (int)this->xWaveStream->Length;
	this->xAudioBuf->AudioData = this->xWaveStream;
	this->xSrcVoice->Volume = vol;
	if( bLoop ) this->xAudioBuf->LoopCount = SlimDX::XAudio2::XAudio2::LoopInfinite;
	this->xSrcVoice->SubmitSourceBuffer( xAudioBuf );
	this->xSrcVoice->BufferEnd += gcnew EventHandler<SlimDX::XAudio2::ContextEventArgs^>( this, &SourceVoiceUnit::BufferEndCallback );
}

// �Đ��J�n
void SourceVoiceUnit::Play( float vol, bool bLoop )
{
	ConfigSourceVoice( vol, bLoop );	// �\�[�X�{�C�X�ݒ�
	this->xSrcVoice->Start();			// �Đ��J�n
	this->fSrcVolume = vol;
	this->bPlaying = true;
}

// �\�[�X���ʃZ�b�g(1.0f����l�j 
bool SourceVoiceUnit::SetSrcVolume( float vol )
{
	this->fSrcVolume = vol;
	if( this->bPlaying ) this->xSrcVoice->Volume = vol;
	return true;
}

// WaveStream�����i�t�@�C�����Łj
bool SourceVoiceUnit::GenerateWaveStream( String ^str )
{
	// �t�@�C���ǂݍ���
	try {
		this->xWaveStream = gcnew SlimDX::Multimedia::WaveStream(str);
	}
	catch( System::IO::FileNotFoundException ^e )
	{	// �t�@�C�������݂��Ȃ��B
		LastException = e;	// DBG:�Ō�̃G���[�ێ�
		return false;		// SLIMAUDIO2_HANDLE_NULL;
	}
	return true;	// �n���h����ԋp����B
}

// �Đ��I���R�[���o�b�N
void SourceVoiceUnit::BufferEndCallback(System::Object^ sender, SlimDX::XAudio2::ContextEventArgs^ e)
{
	bPlaying = false;	// �Đ��t���O�����Z�b�g
}


/////////////////////////////////////////////

// �R���X�g���N�^
SlimAudio2::SlimAudio2()
{
	xDev = gcnew SlimDX::XAudio2::XAudio2();
	xMstVoice = gcnew SlimDX::XAudio2::MasteringVoice(xDev);
	voice_handle = 0;
	xMstVoice->Volume = 1.0f;
}

// �f�X�g���N�^
SlimAudio2::~SlimAudio2()
{
	delete xMstVoice;
	delete xDev;
}

// �}�X�^�[���ʂ��Z�b�g(1.0f����l)
void SlimAudio2::SetMasterVolume( float v )
{
	xMstVoice->Volume = v;
}

// �\�[�X���ʃZ�b�g(1.0f����l�j
bool SlimAudio2::SetSourceVolume( unsigned int handle, float vol )
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite;

	ite = SrcVoiceUnitMap.find( handle );
	if( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		return obj->SetSrcVolume( vol );
	}
	return false;		// �w��n���h���ɑΉ������I�u�W�F�N�g��������Ȃ�����
}

// �o�^�T�E���h�t�@�C������Ԃ��B
int SlimAudio2::GetRegistSoundFileNum(void)
{
	return SrcVoiceUnitMap.size();	// map�v�f����Ԃ�
}

// �n���h������
unsigned int SlimAudio2::GenerateHandle(void)
{
	return ++voice_handle;	// �P�����Ă���Ԃ��i���P�T�l�������j
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, float vol, bool bLoop )
{
	unsigned int handle;

	GarbageCollection();	// �Đ��σo�b�t�@�J��

	// ���j�b�g�𐶐����čĐ��A���X�g�o�^
	SourceVoiceUnit ^obj = gcnew SourceVoiceUnit( this->xDev, str );
	obj->Play( vol, bLoop );

	// Map�o�^
	handle = GenerateHandle();
	cliext::pair<unsigned int,SourceVoiceUnit^> p(handle,obj);
	SrcVoiceUnitMap.insert( p );

	return handle;
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, bool bLoop )
{
	return Play( str, 1.0f, bLoop );
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str, float v )
{
	return Play( str, v, false );
}

// �T�E���h�Đ�
unsigned int SlimAudio2::Play( String ^str )
{
	return Play( str, 1.0f, false );
}

//bool SlimAudio2::Play(unsigned int handle)
//{
//	return bool;	// �݂�������
//}

// �K�[�x�[�W�R���N�V�����I����
void SlimAudio2::GarbageCollection(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{
			delete obj;
			SrcVoiceUnitMap.erase( ite++ );		// Map����폜
		}
		else
		{
			ite++;
		}
	}
}

// �N���[�j���O����
void SlimAudio2::Cleaning(void)
{
	cliext::map<unsigned int,SourceVoiceUnit^>::iterator ite = SrcVoiceUnitMap.begin();

	while( ite != SrcVoiceUnitMap.end() )
	{
		SourceVoiceUnit ^obj = (*ite)->second;
		if( false == obj->IsPlaying() )
		{	// �폜�Ώ�
			delete obj;
			ite++;
		}
	}
	SrcVoiceUnitMap.clear();
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// Unit 

// �R���X�g���N�^
Unit::Unit()
{
	bInc = true;
}

// �t�@�C�������Z�b�g����
void Unit::SetBmpFile( System::String ^str )
{
	bmp = gcnew Bitmap( str );
}

// �|�W�V�������w�肷��
void Unit::SetPos( int x, int y )
{
	fPosx = (float)x;
	fPosy = (float)y;
}

// �r�b�g�}�b�v��Ԃ�
Bitmap ^Unit::GetBmp(void)
{
	return bmp;
}

#define PI 3.14159265

// ���j�b�g�s��
void Unit::DoUnit(void)
{
	// �p�x�ɉ�����XY�̈ړ��ʂ����߂�B
	double movementX = ( dSpeed * Math::Cos( dAngle * PI / 180 ) );
	double movementY = ( dSpeed * Math::Sin( dAngle * PI / 180 ) );

	fPosx += (float)movementX;
	fPosy += (float)movementY;

	double width = GetDeleWidth();
	double height = GetDeleHeight();

	// ���˔���i�x���j
	if( fPosy >= (height - bmp->Height) )	// ���˔���i������j
	{	// �x�����i1�`179�j
		if( dAngle < 180.0f )	// ��ʉ��֌������Ă��邩�H
		{	// ���ˊp�x��ύX�i�������ցj
			dAngle = 360.0f - dAngle;
			SokudoOmake();	// �I�}�P
			NotiHansya( 0 );
		}
	}
	else if( fPosy <= 1.0f )	// ���˔���i�と���j
	{	// �x����i181�`359�j
		if( dAngle >= 180.0f )
		{	// ���ˊp�x��ύX�i�ォ�牺�ցj
			dAngle = 360.0f - dAngle;
			SokudoOmake();	// �I�}�P
			NotiHansya( 1 );
		}
	}
	// ���˔���i�w���j
	else if( fPosx  >= (width - bmp->Width) )	// ���˔���i�E�����j
	{	// �w���E�i271�`359, 1�`89�j
		if( dAngle > 270.0f || dAngle < 90.0f )
		{
			dAngle = 180.0f - dAngle;
			if( dAngle < 0.0f ) dAngle = 360.0f + dAngle;	// �}�C�i�X�������Ȃ��΍�
			SokudoOmake();	// �I�}�P
			NotiHansya( 2 );
		}
	}
	else if( fPosx <= 1.0f )		// ���˔���i�����E�j
	{	// �w�����i90�`270�j
		if( dAngle > 90.0f && dAngle < 270.0f )
		{
			dAngle = 180.0f - dAngle;
			if( dAngle < 0.0f ) dAngle = 360.0f + dAngle;	// �}�C�i�X�������Ȃ��΍�
			SokudoOmake();	// �I�}�P
			NotiHansya( 3 );
		}
	}
}

// �I�}�P����
void Unit::SokudoOmake(void)
{
#if 0
	if( bInc )
	{	// �����Ă����܂��B
		dSpeed += 1.2f;	// �Ȃ�ƂȂ�
		if( dSpeed > 300.0f )
		{
			bInc = false;
		}
	}
	else
	{	// �����Ă����܂��B
		dSpeed -= 2.8f;	// �Ȃ�ƂȂ�
		if( dSpeed <= 5.0f )
		{
			bInc = true;
		}
	}
#endif
}

///////////////////////////////////////////////////////////
// Field

// �R���X�g���N�^

// �t�H�[���o�^
void Field::SetForm( Form1 ^f )
{
	fm = f;
}

// �t�B�[���h�̃��R�����擾
int Field::GetWidth(void)
{
	return iFieldWidth;
}

// �t�B�[���h�̃^�e�������擾
int Field::GetHeight(void)
{
	return iFieldHeight;
}

// ���˂������̒ʒm���󂯂�
void Field::NotisHansya( int udon )
{
	switch( udon )
	{
	case 0:
		sa2->Play( "sound1.wav", 1.0f, false );
		break;
	case 1:
		sa2->Play( "sound2.wav", 1.0f, false );
		break;
	case 2:
		sa2->Play( "land_combat_2.wav", 1.0f, false );
		break;
	default:
		sa2->Play( "udonken.wav", 1.0f, false );
		break;
	}
}

// �t�B�[���h�����X�^�[�g
void Field::Start(void)
{
	// �t�H�[������
	fm = gcnew Form1();

	// �t�B�[���h������
	FieldInit( 20, 20, 800, 600 );

	// �t�H�[���\���i���[�h���X����j
	fm->Show();

	// ����h���[
	fm->Refresh();
//	fm->Invalidate();

	// �X�g�b�v�E�H�b�`�N��
	StopWatchStart();

//	__int64 watch2 = 10;
//	::QueQueryPerformanceCounter( (LARGE_INTEGER *)&watch2 );

	// SlimDX XAudio2 ����
	sa2 = gcnew SlimAudio2();	// XAudio2����p�N���X�̃C���X�^���X�쐬
	master_vol = 1.0f;
	sa2->SetMasterVolume( master_vol );
	bgm_vol = 0.4f;
	bgm_handle = sa2->Play( "sugar.wav", bgm_vol, true );

	// ���C�����[�v����
	MainLoop();
}

// �t�B�[���h������
void Field::FieldInit( int posx, int posy, int width, int height )
{
	iFieldWidth = width;
	iFieldHeight = height;

	fm->SuspendLayout();

	// �t�B�[���h�p�s�N�`���{�b�N�X����
	picBox = (gcnew System::Windows::Forms::PictureBox());
	picBox->Location = System::Drawing::Point(posx, posy);
	picBox->Name = L"GameFieldPicBox";
	picBox->Size = System::Drawing::Size( width, height );
	picBox->TabIndex = 0;
	picBox->TabStop = false;
	picBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Field::picBoxPaintEvent);
	fm->Controls->Add( picBox );	// �s�N�`���{�b�N�X���t�H�[���ɓo�^����B

	// �f�o�b�O�p���x���쐬
	label = (gcnew System::Windows::Forms::Label());
	label->AutoSize = true;
	label->Location = System::Drawing::Point(80, 2);
	label->Name = L"label";
	label->Size = System::Drawing::Size(35, 12);
	label->TabIndex = 1;
	label->Text = L"�ǂ��ł��傤�H";
	fm->Controls->Add( label );	// �s�N�`���{�b�N�X���t�H�[���ɓo�^����B

	// ���ʒ����p�g���b�N�o�[�P
	this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
	this->trackBar1->Location = System::Drawing::Point(250, 615);
	this->trackBar1->Name = L"trackBar1";
	this->trackBar1->Size = System::Drawing::Size(155, 45);
	this->trackBar1->TabIndex = 0;
	this->trackBar1->Value = 100;
	this->trackBar1->Maximum = 150;
	this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &Field::trackBar1_ValueChanged);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
	fm->Controls->Add( this->trackBar1 );

	// ���ʒ����p�g���b�N�o�[�Q
	this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
	this->trackBar2->Location = System::Drawing::Point(420, 615);
	this->trackBar2->Name = L"trackBar2";
	this->trackBar2->Size = System::Drawing::Size(155, 45);
	this->trackBar2->TabIndex = 0;
	this->trackBar2->Value = 100;
	this->trackBar2->Maximum = 150;
	this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &Field::trackBar2_ValueChanged);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
	fm->Controls->Add( this->trackBar2 );

	fm->PerformLayout();

	// �t�B�[���h�p�r�b�g�}�b�v�����E�ڑ�
	picBmp = gcnew Bitmap( width, height );
	picBox->Image = picBmp;

	// �ǎ��p�r�b�g�}�b�v�����[�h���Ă���
	wallBmp = gcnew Bitmap( "wallpaper.jpg" );

	// �����炭�ŏ��̂P�񂾂��ł�����͂����B
	grph = Graphics::FromImage( picBox->Image );

#if USE_DOUBLE_BUFFER
	contex = BufferedGraphicsManager::Current;
	contex->MaximumBuffer = System::Drawing::Size( width + 1, height + 1 );
//	fm->SetStyle( static_cast<ControlStyles>(ControlStyles::AllPaintingInWmPaint | ControlStyles::UserPaint), true );
	bgrph = contex->Allocate( picBox->CreateGraphics(), Rectangle(0,0,width,height) );
	bufferingMode = 2;
	dbcnt = 0;
#endif

	// �{�b�N�X����������B
//	Brush^ brush = gcnew SolidBrush( Color::FromArgb(0, 0, 0) );
//	grph->FillRectangle( brush, 0, 0, width, height );

	// �I�u�W�F�N�g��z�u����B
	AddUnit( "takaharu.jpg", 5, 5, 160.0f, 1.0f );
	AddUnit( "2749.png", 160, 90, 35.0f, 7.5f );
	AddUnit( "icon82.jpg", 10, 50, 5.0f, 12.8f );
	AddUnit( "icon83.jpg", 200, 10 );
	AddUnit( "icon81.jpg", 120, 220, 170.0f, 0.8f );
	AddUnit( "1752.png", 0, 0 );
	AddUnit( "1752.png", 30, 0, 110.0f, 3.7f );
	AddUnit( "1752.png", 60, 0 );
	AddUnit( "1752.png", 90, 0 );
	AddUnit( "1752.png", 120, 0 );
	AddUnit( "icon82.jpg", 210, 50, 275.0f, 3.5f );
	AddUnit( "icon83.jpg", 400, 10 );
	AddUnit( "icon81.jpg", 520, 200 );
	AddUnit( "1752.png", 10, 90 );
	AddUnit( "1752.png", 130, 90 );
	AddUnit( "1752.png", 190, 90, 250.0f, 10.5f );
	AddUnit( "1752.png", 220, 90 );
	AddUnit( "icon82.jpg", 110, 150 );
	AddUnit( "icon83.jpg", 300, 110 );
	AddUnit( "icon81.jpg", 420, 300 );
	AddUnit( "1752.png", 10, 90 );
	AddUnit( "1752.png", 130, 90 );
	AddUnit( "1752.png", 160, 90 );
	AddUnit( "1752.png", 190, 90, 345.5f, 8.5f );
	AddUnit( "1752.png", 220, 90 );

	// �S�I�u�W�F�N�g��`�悵�A�r�b�g�}�b�v���X�V����B
#if USE_DOUBLE_BUFFER
	DrawToBuffer( bgrph->Graphics );
#else
	DrawToBuffer( grph );
#endif
	// �_�u���o�b�t�@�p
}

// ���j�b�g�ǉ�
void Field::AddUnit( String ^str )
{
	AddUnit( str, 0, 0 );
}

// ���j�b�g�ǉ��i���W���j
void Field::AddUnit( String ^str, int x, int y )
{
	AddUnit( str, x, y, 30.0f, 2.0f );
}

// ���j�b�g�ǉ��i���W�Ƒ��x�Ɗp�x���j
void Field::AddUnit( String ^str, int x, int y, double angle, double speed )
{
	Unit ^obj = gcnew Unit();
	obj->SetBmpFile( str );
	obj->SetPos( x, y );
	obj->dSpeed = speed;
	obj->dAngle = angle;

	// �f���Q�[�g�o�^
	obj->GetDeleWidth = gcnew GetDelegateWidth( this, &Field::GetWidth );
	obj->GetDeleHeight = gcnew GetDelegateHeight( this, &Field::GetHeight );
	obj->NotiHansya = gcnew NotifyHansya( this, &Field::NotisHansya );

	unitidx.push_back( obj );		// ���X�g�ɒǉ�
}

// �S���j�b�g�̍s���v�Z
void Field::DoAllUnit(void)
{
	cliext::list<Unit^>::iterator ite = unitidx.begin();

	while( ite != unitidx.end() )
	{
		(*ite)->DoUnit();
		ite++;
	}
}

// �S�I�u�W�F�N�g��`�悵�A�t�B�[���h�r�b�g�}�b�v���X�V����
void Field::DrawToBuffer( Graphics^ g )
{
	cliext::list<Unit^>::iterator ite = unitidx.begin();
	Bitmap ^bmp;

#if USE_DOUBLE_BUFFER
	// Clear the graphics buffer every five updates.
	if( ++dbcnt > 5 )
	{
		dbcnt = 0;
		g->FillRectangle( Brushes::Black, 0, 0, picBmp->Width, picBmp->Height );
	}
#endif

	// �ǎ���ݒ�
	RectangleF SrcRect = RectangleF( 0, 0, (float)wallBmp->Width, (float)wallBmp->Height );
	RectangleF DstRect = RectangleF( 0, 0, (float)picBmp->Width, (float)picBmp->Height );
	g->DrawImage( wallBmp, DstRect, SrcRect, GraphicsUnit::Pixel );

	// �S���j�b�g��z�u
	while( ite != unitidx.end() )
	{
		bmp = (*ite)->GetBmp();
		SrcRect = RectangleF( 0, 0, (float)bmp->Width, (float)bmp->Height );
		DstRect = RectangleF( (*ite)->fPosx, (*ite)->fPosy, (float)bmp->Width, (float)bmp->Height );
		g->DrawImage( bmp, DstRect, SrcRect, GraphicsUnit::Pixel );
		ite++;
	}
}

// �I���h�����[
void Field::picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
#if USE_DOUBLE_BUFFER
//	DrawToBuffer( bgrph->Graphics );
//	bgrph->Render( e->Graphics );
	bgrph->Render( grph );
#else
	DrawToBuffer( grph );
#endif
}

// �X�g�b�v�E�H�b�`�J�n
void Field::StopWatchStart(void)
{
	sw = gcnew Diagnostics::Stopwatch();
	sw->Start();	// �X�g�b�v�E�H�b�`�J�n
	keeptime = sw->ElapsedMilliseconds;
	bTickFlag = false;
	tickMsec = 33;			// 100msec�ɂP�񏈗����s���܂��B
}

// �X�g�b�v�E�H�b�`�̒l�`�F�b�N
bool Field::StopWatchWaitCheck(void)
{
	Int64 nowtime = sw->ElapsedMilliseconds;
	bool bRet = false;

	// �󋵔���
	if( keeptime + tickMsec < nowtime )
	{	// �e�B�b�N���Ԃ𒴂���
		bRet = true;
		keeptime = nowtime - ( nowtime - keeptime - tickMsec );
	}

	return bRet;
}

// ���C�����[�v
bool Field::MainLoop(void)
{
	bool bb = true;

	// ���b�Z�[�W���[�v�J�n
	while( bb )
	{
		Application::DoEvents();

		// ��`msec�ɂP�񏈗�����
		if( StopWatchWaitCheck() )
		{	// ���s
			DoAllUnit();

#if USE_DOUBLE_BUFFER
			DrawToBuffer( bgrph->Graphics );
//			bgrph->Render( e->Graphics );
			if( bufferingMode == 2 )
			{
				if( fm->Visible )bgrph->Render( Graphics::FromHwnd( picBox->Handle ) );
			}
			else
			{
				fm->Refresh();
			}
#else
			DrawToBuffer( grph );
			fm->Refresh();
#endif
			// ���t���b�V������
			//fm->Invalidate( false );
		}
		else
		{
			// �o�ߎ��Ԃ�\����������B
			this->label->Text = "�o�ߎ��ԁF" + sw->ElapsedMilliseconds.ToString() + "�ł��B";
		}

		// �E�B���h�E������ꂽ���𔻒肷��
		if( fm->Visible == false )
		{	// �E�B���h�E������ꂽ��A
			bb = false;		// �v���O�����I��
		}

	}

	return bb;
}

// �g���b�N�o�[�ϓ��B
System::Void Field::trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	master_vol = trackBar1->Value / 100.0f;
	sa2->SetMasterVolume( master_vol );
}

// �g���b�N�o�[�ϓ��B
System::Void Field::trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	bgm_vol = trackBar2->Value / 100.0f;
	sa2->SetSourceVolume( bgm_handle, bgm_vol );
}
