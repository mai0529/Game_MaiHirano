//-------------------------------------------
//
//���C�����[main.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

//�C���N���[�h�t�@�C��
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"	
#define DIRECTINPUT_VERSION	(0x800)		//�r���h���̌x���Ώ��p
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�����ɕK�v
#include "xinput.h"						//�W���C�p�b�h�����ɕK�v
#include <assert.h>						//�A�T�[�g�Ɏg��

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")		//�W���C�p�b�h�����ɕK�v

//�}�N����`
#define SCREEN_WIDTH		(1280)
#define SCREEN_HEIGHT		(720)
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define CLASS_NAME			"WindowClass"													//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"3DGame"														//�E�C���h�E�̖��O
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���W�A�@���A�J���[�A�e�N�X�`��

//���_���2D�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
} VERTEX_2D;

//���_���3D�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
} VERTEX_3D;


//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_GAMECLEAR,		//���U���g���
	MODE_GAMEOVER,		//�Q�[���I�[�o�[���
	MODE_RESULT,		//���U���g���
	MODE_RANKING,		//�����L���O���
	MODE_MAX
}MODE;

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//�E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstence, HWND hWnd, BOOL bWindow);							//����������
void Uninit(void);																	//�I������
void Update(void);																	//�X�V����
void Draw(void);																	//�`�揈��
LPDIRECT3DDEVICE9 GetDevice(void);													//�f�o�C�X�̎擾
void SetMode(MODE mode);															//���[�h�̐ݒ�
MODE GetMode(void);																	//���[�h�̎擾

#endif