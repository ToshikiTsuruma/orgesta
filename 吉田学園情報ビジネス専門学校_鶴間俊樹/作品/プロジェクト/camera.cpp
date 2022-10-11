//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "scene.h"
#include "gameScene.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//�J�����̉�]���x(Y��)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//�J�����̉�]���x(X��)
#define MAX_ROTATE_PITCH (-0.05f)	//�J������X����]�̍ő�
#define MIN_ROTATE_PITCH (-0.18f)	//�J������X����]�̍Œ�

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CCamera::CCamera() : m_fMaxDrawDist(1000.0f)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDist = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CCamera::CCamera(float fMaxDrawDist) : m_fMaxDrawDist(fMaxDrawDist)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDist = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::Init(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//�r���[�|�[�g�̐ݒ�
	m_viewport.X = (DWORD)0;
	m_viewport.Y = (DWORD)0;
	m_viewport.Width = (DWORD)SCREEN_WIDTH;
	m_viewport.Height = (DWORD)SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	//�r���[�|�[�g�̐ݒ�
	if (pDevice != nullptr) pDevice->SetViewport(&m_viewport);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//��p 360�x
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//��ʔ䗦
		10.0f,										//��O�̈ʒu
		m_fMaxDrawDist);							//���̈ʒu

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pRenderer->SetEffectMatrixProj(m_mtxProjection);

	//��x�N�g���̐ݒ�
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�r���[�}�g���b�N�X�̐ݒ�
	SetCamera();

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void) {

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void) {
	//�ړ�����
	//Move();

	//��]����
	if(!m_bLockControll) Rotate();
}

//=============================================================================
// �J�����̃Z�b�g
//=============================================================================
void CCamera::SetCamera(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	//------------------------------------
	//�r���[�}�g���b�N�X�̍쐬
	//------------------------------------
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);	//�r���{�[�h�̂��߂Ɏc��
	//�V�F�[�_�Ƀr���[�}�g���b�N�X��ݒ�
	pRenderer->SetEffectMatrixView(m_mtxView);
}

//=============================================================================
// �ړ�
//=============================================================================
void CCamera::Move(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;

	//--------------------
	//���͏���
	//--------------------
	float fMoveSpeed = 5.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 3;
	}

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//�O�ړ�
		m_pos.z += fMoveSpeed * cosf(m_rot.y);
		m_pos.x += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//���ړ�
		m_pos.z -= fMoveSpeed * cosf(m_rot.y);
		m_pos.x -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//���ړ�
		m_pos.x -= fMoveSpeed * cosf(m_rot.y);
		m_pos.z += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//�E�ړ�
		m_pos.x += fMoveSpeed * cosf(m_rot.y);
		m_pos.z -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//�㏸
		m_pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//���~
		m_pos.y -= fMoveSpeed;
	}
}

//=============================================================================
// ��]
//=============================================================================
void CCamera::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;

	//--------------------
	//���͏���
	//--------------------
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		//���_��������
		m_rot.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x > MAX_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MAX_ROTATE_PITCH * D3DX_PI;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		//���_���グ��
		m_rot.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x < MIN_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MIN_ROTATE_PITCH * D3DX_PI;
		}
	}

	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		//���_������]
		m_rot.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y < -D3DX_PI) {
			m_rot.y = m_rot.y + D3DX_PI * 2;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		//���_���E��]
		m_rot.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y > D3DX_PI) {
			m_rot.y = m_rot.y - D3DX_PI * 2;
		}
	}

}