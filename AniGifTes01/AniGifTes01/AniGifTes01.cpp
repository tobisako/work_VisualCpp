// AniGifTes01.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "AniField.h"

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// �R���g���[�����쐬�����O�ɁAWindows XP �r�W���A�����ʂ�L���ɂ��܂�
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// ���C�� �E�B���h�E���쐬���āA���s���܂�
	AniGifFieldForm	^obj = gcnew AniGifFieldForm();
	obj->Start();
	return 0;
}

/*

�E�ۑ�F
�t�B�[���h�ƃC���^�[�t�F�[�X�́u�a�����v�W�J�̖����B
�a�����ɂ���ׂɂ́A�ǂ̗l�ɂ���΂悢���B

�t�B�[���h
���j�b�g
�R���g���[���C���^�[�t�F�[�X

�R���g���[���N���X�B
�R���g���[���N���X���A�t�B�[���h���A���j�b�g���A���䂵�Ȃ���΁B

�R���g���[���C���^�t�F�[�X�́A
���j�b�g�ׂ̈ɁA���J���Ă����Ȃ���΁B

*/
