#pragma once

// �^�C���B

#include "stdafx.h"
//#include "Form1.h"

//using namespace StlTest02;

// �G�l�~�[
public ref class CEnemy {
public:
	System::Windows::Forms::PictureBox^	picBox;
	CEnemy();										// �R���X�g���N�^
	~CEnemy();										// �f�X�g���N�^
	virtual void SetImageFileName(void);			// �C���[�W�t�@�C�������Z�b�g����
	virtual void SetImageFileName(int);				// �C���[�W�t�@�C�������Z�b�g����
	void Create(int ymax, int level);				// �N���G�C�g
	System::Drawing::Point CEnemy::GetPoint(void);	// ���W�擾
	void SetPoint( System::Drawing::Point );		// ���W�w��P
	void SetPoint(int x, int y);					// ���W�w��Q
	void Draw(void);								// �`�揈��
	void Move(void);								// �ړ�����
	System::Windows::Forms::PictureBox^	GetpictureBox(void);	// Form�A���p
	// ���蔻��֐�
	bool HitCheck(System::Drawing::Point &obj1p, System::Drawing::Size &obj1s);
protected:
	System::String ^sImageFileName;
private:
	int iLevel;										// �G���x��
	int iSpeed;										// �G�X�s�[�h
	int iDirectionY;								// �x���W�̈ړ������B
	int iSpeedY;									// �x���W�̈ړ����x�B
	cliext::list<CEnemy^> lisidx;
};

// �^�C��
public ref class CTaiya : public CEnemy {
public:
	CTaiya();
	void SetImageFileName(void) new;	// ���z�֐����p�������ꍇ�unew�v�Ə����Ė������Ȃ����warning���o�ăE�U�C�B
	void SetImageFileName(int) new;
};

// ���e
public ref class CBomb : public CEnemy {
public:
	CBomb();
	void SetImageFileName(void) new;
	void SetImageFileName(int) new;
};

// �u���b�N
public ref class CBlock : public CEnemy {
public:
	CBlock();
	void SetImageFileName(void) new;
	void SetImageFileName(int) new;
};

public ref class CField {
public:
	int hoge;
private:
//	cliext::list<CEnemy^> lisidx3;
};
