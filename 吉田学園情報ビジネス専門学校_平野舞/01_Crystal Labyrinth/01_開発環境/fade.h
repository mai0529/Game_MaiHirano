//-------------------------------------------------
//
//�t�F�[�h���[fade.h]
//Author:���앑
//
//-------------------------------------------------
#ifndef _FADE_H_
#define _FADE_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�t�B�[�h�̏��
typedef enum
{
	FADE_NONE = 0,		//�������Ă��Ȃ����
	FADE_IN,			//�t�F�[�h�C�����
	FADE_OUT,			//�t�F�[�h�A�E�g���
	FADE_MAX
}FADE;

//�v���g�^�C�v�錾
void InitFade(MODE modeNext);		//����������
void UninitFade(void);				//�I������
void UpdateFade(void);				//�X�V����
void DrawFade(void);				//�`�揈��
void SetFade(MODE modeNext);		//�ݒ菈��
FADE GetFade(void);					//���̎擾����

#endif