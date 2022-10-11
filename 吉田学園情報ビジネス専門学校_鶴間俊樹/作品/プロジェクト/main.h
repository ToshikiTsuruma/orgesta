//=============================================================================
//
// ���C������ [main.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) //�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"		//���͏����ɕK�v
#include "xaudio2.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <XInput.h>
#include <vector>
#include <string>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")	//�`��ɕK�v
#pragma comment(lib, "d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")	//DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib") //���͏����ɕK�v
#pragma comment(lib, "winmm.lib")	//�b���v���ɕK�v
#pragma comment(lib, "xinput.lib")	//xinput�̓���

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH (1280) //�E�B���h�E�̕�
#define SCREEN_HEIGHT (720) //�E�B���h�E�̍���
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)	//3D�̏_��Ȓ��_�t�H�[�}�b�g	//���f���̕����݂̂Ɏg�p
#define FPS (60)		// FPS�̒l

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//2D���_�̍\���̂��`
typedef struct {
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0f�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_2D;

//���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_3D;

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
int GetFPS(void);

#endif