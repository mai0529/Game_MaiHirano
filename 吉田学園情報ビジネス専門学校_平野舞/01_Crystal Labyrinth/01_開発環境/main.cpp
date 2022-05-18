//-------------------------------------------
//
//3D�Q�[������[main.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include <windows.h>
#include "main.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "gameclear.h"
#include "fade.h"
#include "gameover.h"
#include "ranking.h"
#include "maze.h"
#include "camera.h"
#include "result.h"
#include "sound.h"
#include "controller.h"

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direst3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;					//���݂̃��[�h
bool g_MeshField;							//���b�V���t�B�[���h
bool g_Billboard;							//���C���[�t���[���\�����邩�ǂ���

//-------------------------------------------
//���C���֐�
//-------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,
		0,
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)		//�t�@�C���̃A�C�R��
	};

	HWND hWnd;			//�E�C���h�E�n���h��(���ʎq)
	MSG msg;			//���b�Z�[�W���i�[����ϐ�

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,			//�g���E�C���h�E�X�^�C��
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,					//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),		//�E�C���h�E�̕�
		(rect.bottom - rect.top),		//�E�C���h�E�̍���
		NULL,							//�e�E�C���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��̓E�C���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL);							//�E�C���h�E�쐬�f�[�^

	DWORD dwCurrentTime;		//���ݎ���
	DWORD dwExecLastTime;		//�Ō�ɏ�����������

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//�����������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾	
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//60����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();
			}
		}
	}

	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//-------------------------------------------
//�E�C���h�E�v���V�[�W��
//-------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;		//�Ԃ�l���i�[
	//HDC hDC;

	switch (uMsg)
	{
	case WM_DESTROY:		//�E�C���h�E�j���̃��b�Z�[�W
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:		//ESC�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_TOPMOST);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:		//����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_TOPMOST);
		if (nID == IDYES)
		{
			//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//-------------------------------------------
//�����ݒ�
//-------------------------------------------
HRESULT Init(HINSTANCE hInstence, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^�[

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//�p�����[�^�[�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;								//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;								//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;								//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;											//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;								//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;											//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_��CPU)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_������CPU�ōs��)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏������ݒ�
	if (FAILED(InitKeyboard(hInstence, hWnd)))
	{
		return E_FAIL;
	}

	//�R���g���[���[�̏���������
	if (FAILED(InitController()))
	{
		return E_FAIL;
	}

	//���H�̏�����
	InitMaze();

	//���U���g�̏�����
	ResetResult();

	//�����L���O�̏�����
	ResetRanking();

	//�T�E���h�̏���������
	InitSound(hWnd);

	//���[�h�ݒ�
	InitFade(g_mode);

	return S_OK;
}

//-------------------------------------------
//�I������
//-------------------------------------------
void Uninit(void)
{
	//����\��߂�
	timeEndPeriod(1);

	//�T�E���h�̏I������
	UninitSound();

	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�`���[�g���A����ʂ̏I������
	UninitTutorial();

	//�Q�[����ʂ̏I������
	UninitGame();

	//�Q�[���N���A��ʂ̏I������
	UninitGameclear();

	//�Q�[���I�[�o�[��ʂ̏I������
	UninitGameover();

	//���U���g��ʂ̏I������
	UninitResult();

	//�����L���O��ʂ̏I������
	UninitRanking();

	//�t�F�[�h��ʂ̏I������
	UninitFade();

	//�R���g���[���[�̏I������
	UninitController();

	////�L�[�{�[�h�̏I������
	UninitKeyboard();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�R���g���[���[�̍X�V����
	UpdateController();

	switch (g_mode)
	{
	case MODE_TITLE:			//�^�C�g�����
		UpdateTitle();
		break;
	case MODE_TUTORIAL:			//�`���[�g���A�����
		UpdateTutorial();
		break;
	case MODE_GAME:				//�Q�[�����
			UpdateGame();
		break;
	case MODE_GAMECLEAR:		//�Q�[���N���A���
		UpdateGameclear();
		break;
	case MODE_GAMEOVER:			//�Q�[���I�[�o�[���
		UpdateGameover();
		break;
	case MODE_RESULT:			//���U���g���
		UpdateResult();
		break;
	case MODE_RANKING:			//�����L���O���
		UpdateRanking();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		switch (g_mode)
		{
		case MODE_TITLE:			//�^�C�g�����
			DrawTitle();
			break;
		case MODE_TUTORIAL:			//�`���[�g���A�����
			DrawTutorial();
			break;
		case MODE_GAME:				//�Q�[�����
			DrawGame();
			break;
		case MODE_GAMECLEAR:		//�Q�[���N���A���
			DrawGameclear();
			break;
		case MODE_GAMEOVER:			//�Q�[���I�[�o�[���
			DrawGameover();
			break;
		case MODE_RESULT:			//���U���g���
			DrawResult();
			break;
		case MODE_RANKING:			//�����L���O���
			DrawRanking();
			break;
		}

		MakeMaze();
		CreateMaze();

		//�t�F�[�h�̕`�揈��
		DrawFade();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-------------------------------------------
//�f�o�C�X�̎擾
//-------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-------------------------------------------
//���[�h�̐ݒ�
//-------------------------------------------
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:			//�^�C�g�����
		UninitTitle();
		break;
	case MODE_TUTORIAL:			//�`���[�g���A�����
		UninitTutorial();
		break;
	case MODE_GAME:				//�Q�[�����
		UninitGame();	
		break;
	case MODE_GAMECLEAR:		//�Q�[���N���A���
		UninitGameclear();
		break;
	case MODE_GAMEOVER:			//�Q�[���I�[�o�[���
		UninitGameover();
		break;
	case MODE_RESULT:			//���U���g���
		UninitResult();
		break;
	case MODE_RANKING:			//�����L���O���
		UninitRanking();
		break;
	}

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:			//�^�C�g�����
		InitTitle();
		break;
	case MODE_TUTORIAL:			//�`���[�g���A�����
		InitTutorial();
		break;
	case MODE_GAME:				//�Q�[�����
		InitGame(); 
		break;
	case MODE_GAMECLEAR:		//�Q�[���N���A���
		InitGameclear();
		break;
	case MODE_GAMEOVER:			//�Q�[���I�[�o�[���
		InitGameover();
		break;
	case MODE_RESULT:			//���U���g���
		InitResult();
		break;
	case MODE_RANKING:			//�����L���O���
		InitRanking();
		break;
	}

	g_mode = mode;		//���݂̉��(���[�h)��؂�ւ���
}

//-------------------------------------------
//���[�h�̎擾
//-------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}