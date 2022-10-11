//=============================================================================
//
// �Q�[���J�������� [gameCamera.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gameCamera.h"
#include "manager.h"
#include "renderer.h"
#include "gameScene.h"
#include "appleTree.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_DRAW_DISTANCE (5000.0f)		//�`��\�ȍő�̋���
#define DEFAULT_CAMERA_DISTANCE (1500.0f)	//���_�ƒ����_�̋���

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGameCamera::CGameCamera() : CCamera(MAX_DRAW_DISTANCE)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameCamera::~CGameCamera()
{

}

//=============================================================================
// �Q�[���J�����̐�������
//=============================================================================
CGameCamera* CGameCamera::Create(void) {
	CGameCamera* pGameCamera;
	pGameCamera = new CGameCamera();
	if (pGameCamera == nullptr) return nullptr;

	pGameCamera->Init();

	return pGameCamera;
}

//=============================================================================
// �Q�[���J�����̏���������
//=============================================================================
HRESULT CGameCamera::Init(void) {
	SetPos(D3DXVECTOR3(0.0f, APPLETREE_POS_Y + 250.0f, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * -0.15f, D3DX_PI * -0.0f, 0.0f));
	//�����̐ݒ�
	SetDistance(DEFAULT_CAMERA_DISTANCE);

	CCamera::Init();

	return S_OK;
}

//=============================================================================
// �Q�[���J�����̏I������
//=============================================================================
void CGameCamera::Uninit(void) {
	CCamera::Uninit();
}

//=============================================================================
// �Q�[���J�����̍X�V����
//=============================================================================
void CGameCamera::Update(void) {
	CCamera::Update();

	//------------------------------------
	//�v���C���[�Ƃ̈ʒu�̊Ԃ��擾
	//------------------------------------
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGameScene = pManager->GetGameScene();
	//�Q�[���̖؂ƃv���C���[�̎擾
	CObject* pAppleTree = nullptr;
	CObject* pPlayer = nullptr;
	if (pGameScene != nullptr) {
		pAppleTree = pGameScene->GetAppleTree();
		pPlayer = pGameScene->GetPlayer();
	}

	D3DXVECTOR3 posTree = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pAppleTree != nullptr) posTree = pAppleTree->GetPos();

	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();

	D3DXVECTOR3 posCamera = posTree + (posPlayer - posTree) / 2.0f;
	posCamera.y = APPLETREE_POS_Y + 250.0f;	//Y�͌Œ�
	//�ʒu�̐ݒ�
	SetPos(posCamera);

	//------------------------------------
	//���_�ƒ����_�̐ݒ�
	//------------------------------------
	D3DXVECTOR3 rot = GetRot();
	float fDist = GetDistance();

	SetPosR(posCamera);
	SetPosV(posCamera + D3DXVECTOR3(sinf(rot.y) * fDist * cosf(rot.x + D3DX_PI),
		sinf(rot.x + D3DX_PI) * fDist,
		cosf(rot.y) * fDist * cosf(rot.x + D3DX_PI)));
}