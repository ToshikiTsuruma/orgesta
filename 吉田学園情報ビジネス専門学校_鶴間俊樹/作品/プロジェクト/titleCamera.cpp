//=============================================================================
//
// �^�C�g���J�������� [titleCamera.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "titleCamera.h"
#include "manager.h"
#include "renderer.h"
#include "appleTree.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_DRAW_DISTANCE (4000.0f)		//�`��\�ȍő�̋���
#define DEFAULT_CAMERA_DISTANCE (1000.0f)	//���_�ƒ����_�̋���

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitleCamera::CTitleCamera() : CCamera(MAX_DRAW_DISTANCE)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleCamera::~CTitleCamera()
{

}

//=============================================================================
// �^�C�g���J�����̐�������
//=============================================================================
CTitleCamera* CTitleCamera::Create(void) {
	CTitleCamera* pTitleCamera;
	pTitleCamera = new CTitleCamera();
	if (pTitleCamera == nullptr) return nullptr;

	pTitleCamera->Init();

	return pTitleCamera;
}

//=============================================================================
// �^�C�g���J�����̏���������
//=============================================================================
HRESULT CTitleCamera::Init(void) {
	SetPos(D3DXVECTOR3(0.0f, 200.0f + APPLETREE_POS_Y, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * -0.1f, D3DX_PI * 0.0f, 0.0f));
	//�����̐ݒ�
	SetDistance(DEFAULT_CAMERA_DISTANCE);

	CCamera::Init();

	//�J�����̃R���g���[���s��
	SetLockControll(true);

	return S_OK;
}

//=============================================================================
// �^�C�g���J�����̏I������
//=============================================================================
void CTitleCamera::Uninit(void) {
	CCamera::Uninit();
}

//=============================================================================
// �^�C�g���J�����̍X�V����
//=============================================================================
void CTitleCamera::Update(void) {
	CCamera::Update();

	D3DXVECTOR3 rot = GetRot();
	//��]������
	rot.y += 0.005f;
	if (rot.y > D3DX_PI) {
		rot.y -= D3DX_PI * 2;
	}
	SetRot(rot);

	//------------------------------------
	//���_�ƒ����_�̐ݒ�
	//------------------------------------
	D3DXVECTOR3 pos = GetPos();
	float fDist = GetDistance();

	SetPosR(pos);
	SetPosV(D3DXVECTOR3(pos.x + sinf(rot.y) * fDist * cosf(rot.x + D3DX_PI),
		pos.y + sinf(rot.x + D3DX_PI) * fDist,
		pos.z + cosf(rot.y) * fDist * cosf(rot.x + D3DX_PI)));	//���_�����S�̏ꍇ����ύX�����Ƃ��Ɉړ����Ȃ��悤��X��]��PI�𑫂��Ă���
}