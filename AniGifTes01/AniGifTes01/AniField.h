#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


///////////////////////////////////////////////
///////////////////////////////////////////////
// BaseImageUnit��AniGifFieldForm�����ԃC���^�[�t�F�[�X

ref class BaseImageUnit;	// �O���Q��

// �t�B�[���h�ւ̃C���^�[�t�F�[�X�N���X
public ref class FieldControlInterface {
public:
	FieldControlInterface();
	~FieldControlInterface();
	void SetFieldSize( int w, int h );
	int GetFieldWidth(void) { return width; }
	int GetFieldHeight(void) { return height; }
	void PlaySound( String ^str, bool bLoop );
	void DoControl(void);
	void StopWatchStart(void);						// �X�g�b�v�E�H�b�`�J�n
	bool StopWatchWaitCheck2Unit(void);				// ���j�b�g�ւ̐�����s�^�C�~���O
	bool StopWatchWaitCheck2VSync(void);			// �`��̐�����s�^�C�~���O
	Int64 GetNowTime(void);							// ���ݎ�����Ԃ�
	void SetMouseX(int a) { iMouseX = a; }
	void SetMouseY(int a) { iMouseY = a; }
	void SetMousePush(bool b) { bMousePush = b; }
	int GetMouseX(void) { return iMouseX; }
	int GetMouseY(void) { return iMouseY; }
	void DoAllUnit(void);							// �S���j�b�g�ɍs���w��
	void DrawAllUnit(Graphics^ g);					// �S���j�b�g�ɕ`��w��
	void AddBackGroundUnit( String ^str, bool b, int x, int y );
	void AddEnemyUnit( String ^str, bool b, int x, int y, double dSpd, double dAng );
	void AddFriendUnit( String ^str, bool b, int x, int y );
	void AddFireArmsUnit(String ^str, bool b, int x, int y);						// �e�e���쐬����
private:
	cliext::list<BaseImageUnit^>	unit_idx;		// ���j�b�g�S�̂����X�g�Ǘ�
	SlimAudio2	^sa2;
	int			width;
	int			height;
	int			iMouseX;
	int			iMouseY;
	bool		bMousePush;							// �{�^���������t���O
	Diagnostics::Stopwatch		^sw;				// �X�g�b�v�E�H�b�`
//	bool	bSWTickFlag;							// �e�B�b�N���ԗp�t���O
//	Int64	sw_nowtime;								// ���ݎ���
	Int64	sw_vsync_keeptime;						// ���ԕۑ��p
	Int64	sw_vsync_ticktime;						// �e�B�b�N���ԊǗ��l(33msec�Ƃ�)
	Int64	sw_unit_keeptime;						// ���ԕۑ��p
	Int64	sw_unit_ticktime;						// �e�B�b�N���ԊǗ��l(33msec�Ƃ�)
};


///////////////////////////////////////////////

// ���j�b�g��{�N���X
public ref class BaseImageUnit abstract {
public:
	BaseImageUnit();
	BaseImageUnit( FieldControlInterface ^fc );
	void LoadImage( System::String ^str, bool b, int x, int y );	// �t�@�C�������w�肷��
	Bitmap ^GetBmp(void);
	void DoUnitBaseAction(void);					// �x�[�X�A�N�V����
	virtual void DoUnitAction(void) abstract;		// ���������z�֐�
	void DrawBaseUnit( Graphics^ g );				// �x�[�X�`��
	virtual void DrawUnit( Graphics^ g ) abstract;	// ���������z�֐�
	void ChangeFrame(void);
	int GetWaitTime( int frame_no );
	double	GetX(void) { return dPosX; }			// �A�N�Z�T
	double	GetY(void) { return dPosY; }			// �A�N�Z�T
	void SetSpeed(double s) { dSpeed = s; }			// �A�N�Z�T
	void SetAngle(double a) { dAngle = a; }			// �A�N�Z�T
protected:
	FieldControlInterface	^fcif;			// �t�B�[���h�R���g���[���C���^�t�F�[�X
	Bitmap					^bmp;			// ���j�b�g�̃r�b�g�}�b�v
	double	dPosX;										// ���W�w
	double	dPosY;										// ���W�x
	double	dSpeed;										// ���i���x
	double	dAngle;										// �i�s�p�x(0�`359.999���炢�܂�)
	Int64	keeptime;									// ���ԕۑ��p
	Int64	nowtime;									// ���ݎ���
	Int64	ticktime;									// �e�B�b�N���ԊǗ��l(33msec�Ƃ�)
	bool JudgeReflection(double width, double height);	// ���˂��W���b�W����
	void MoveUnitFromAngle(void);
private:
	System::Drawing::Imaging::FrameDimension	^fd;
	System::Drawing::Imaging::PropertyItem		^pitem;
	int		frame_count;
	bool	bIsAnime;
};

// �w�i���j�b�g
public ref class BackGroundImageUnit : BaseImageUnit {
public:
	BackGroundImageUnit() {}
	BackGroundImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};

// �G�L�������j�b�g
public ref class EnemyImageUnit : BaseImageUnit {
public:
	EnemyImageUnit() {}
	EnemyImageUnit( FieldControlInterface ^fc );
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
private:
	int hogecnt;
};

// �������j�b�g
public ref class FriendImageUnit : BaseImageUnit {
public:
	FriendImageUnit() {}
	FriendImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};

// �e�e���j�b�g
public ref class FireArmsImageUnit : BaseImageUnit {
public:
	FireArmsImageUnit() {}
	FireArmsImageUnit( FieldControlInterface ^fc ) : BaseImageUnit(fc)  {}
	virtual void DoUnitAction(void) override;
	virtual void DrawUnit( Graphics^ g ) override;
};


///////////////////////////////////////////////

// �A�j��GIF�t�B�[���h
public ref class AniGifFieldForm : public System::Windows::Forms::Form
{
public:
	AniGifFieldForm();
	~AniGifFieldForm();
	void CloseForm(void);
	void Start(void);
	void FieldInit(int,int);
	void UnitInit(void);
	void MainLoop(void);
private:
	FieldControlInterface				^fcif;			// �t�B�[���h�R���g���[���C���^�t�F�[�X
	System::ComponentModel::Container	^components;	// �R���|�[�l���g
	System::Windows::Forms::PictureBox	^picBox;		// �t�B�[���h�pPictureBox
	System::Drawing::Graphics			^grph;			// �t�B�[���h�pGraphics
	Bitmap								^picBmp;		// �t�B�[���hBitmap
	bool	bIsClosing;									// �t�H�[���I�����`�F�b�N
	bool	bIsClosed;									// �t�H�[���I���`�F�b�N
	int		width;
	int		height;
	BufferedGraphicsContext			^b_contex;			// �_�u���o�b�t�@�p
	BufferedGraphics				^b_bgrph;			// �_�u���o�b�t�@�p
	Byte							b_bufferingMode;	// �_�u���o�b�t�@�p
	Byte							b_dbcnt;			// �_�u���o�b�t�@�p
	System::Void picBoxPaintEvent(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);	// �h���[�C�x���g�����H
	System::Void picBoxMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);	// �}�E�X�C�x���g����
	System::Void picBoxMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);	// �}�E�X�{�^������
	System::Void ClosingEvent(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);
	System::Void ClosedEvent(System::Object^ sender, System::EventArgs^ e);
	void DrawToBuffer( Graphics^ g );
};
