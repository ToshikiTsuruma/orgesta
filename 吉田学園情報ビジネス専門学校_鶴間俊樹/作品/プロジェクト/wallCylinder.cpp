//=============================================================================
//
// �~���̕ǂ̏��� [wallCylinder.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "wallCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_POLYGON_XZ (100)	//�ǂ�XZ�����̃|���S���̕�����
#define WALL_HEIGHT (5000.0f)	//�ǂ̍���
#define WALL_COLOR (D3DXCOLOR(1.0f, 0.55f, 0.0f, 1.0f))	//�ǂ̐F
#define DISTANCE_COLOR_CHANGE (400.0f)	//�F���ς��n�߂鋗��	�v���C���[����J�����̋����ȏ�̒l�ɂ���
#define DISTANCE_COLOR_MAX (100.0f)		//�F�̕ω����ő�ɂȂ鋗��
#define MAX_COLOR_ALPHA (0.8f)	//�ύX�����F�̃A���t�@�l�̍ő�l

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CWallCylinder::CWallCylinder() : CMeshcylinder()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CWallCylinder::CWallCylinder(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), NUM_POLYGON_XZ, fRadius, fRadius, 1, fHeight, false)
{
	SetObjType(OBJTYPE_WALL);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

	//�`�揇�̐ݒ�
	if (bClear) {
		SetDrawPriority(DRAW_PRIORITY::CLEAR);	//���߃I�u�W�F�N�g
	}
	else {
		SetDrawPriority(DRAW_PRIORITY::BG);	//�w�i
	}

	SetTexType(typeTex);

	m_bClear = bClear;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWallCylinder::~CWallCylinder()
{

}

//=============================================================================
// �~���̕ǂ̐�������
//=============================================================================
CWallCylinder* CWallCylinder::Create(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear) {
	CWallCylinder* pWallCylinder;
	pWallCylinder = new CWallCylinder(pos, fRadius, fHeight, typeTex, bClear);
	if (pWallCylinder == nullptr) return nullptr;

	pWallCylinder->Init();

	return pWallCylinder;
}

//=============================================================================
// �~���̕ǂ̏���������
//=============================================================================
HRESULT CWallCylinder::Init(void) {
	CMeshcylinder::Init();
	if (m_bClear) {
		SetColor(WALL_COLOR);
	}

	return S_OK;
}

//=============================================================================
// �~���̕ǂ̏I������
//=============================================================================
void CWallCylinder::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// �~���̕ǂ̍X�V����
//=============================================================================
void CWallCylinder::Update(void) {

	//�J��������̋����ɂ��A���t�@�l�̕ύX
	if (m_bClear) ChangeColorAlpha();

	CMeshcylinder::Update();
}

//=============================================================================
// �~���̕ǂ̕`�揈��
//=============================================================================
void CWallCylinder::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	//Z�e�N�X�`���ɏ������܂Ȃ�
	if (pRenderer->GetDrawZTex()) return;

	//�J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//���C�g�𖳌�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�`��
	CMeshcylinder::Draw();

	//�J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �~���̕ǂƂ̓����蔻��
//=============================================================================
bool CWallCylinder::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 lastPos, float fRadius) {
	if (pPos == nullptr) return false;

	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
	
		//�I�u�W�F�N�g�^�C�v���قȂ�ꍇ
		if (!(pObject->GetObjType() & OBJTYPE_WALL)) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pObject);	//�~���̕ǃN���X�ɃL���X�g
		//�L���X�g���s��
		if (pWallCylinder == nullptr) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}
		//���S�t���O�������Ă���ꍇ
		if (pWallCylinder->GetDeath()) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		float fRadiusWall = pWallCylinder->GetRadius();
		D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();
		//�����蔻��
		float fDistance = sqrtf(powf(pPos->x - posWallCylinder.x, 2.0f) + powf(pPos->z - posWallCylinder.z, 2.0f));	//�I�u�W�F�N�g�̈ʒu�Ɖ~���̒��S�Ƃ̋���
		float fLastDistance = sqrtf(powf(lastPos.x - posWallCylinder.x, 2.0f) + powf(lastPos.z - posWallCylinder.z, 2.0f));	//�I�u�W�F�N�g�̑O�̈ʒu�Ɖ~���̒��S�Ƃ̋���
		//�I�u�W�F�N�g�̔��a�𑫂�
		fDistance += fRadius;

		//�͈͊O�ɏo�悤�Ƃ����Ƃ�
		if (fLastDistance <= fRadiusWall && fDistance > fRadiusWall) {
			D3DXVECTOR2 vecNor = D3DXVECTOR2(pPos->x - posWallCylinder.x, pPos->z - posWallCylinder.z);	//�~���̒��S����I�u�W�F�N�g�̈ʒu�ւ̐��K���x�N�g��
			D3DXVec2Normalize(&vecNor, &vecNor);	//���K��

			pPos->x = posWallCylinder.x + vecNor.x * (fRadiusWall - fRadius);
			pPos->z = posWallCylinder.z + vecNor.y * (fRadiusWall - fRadius);
			return true;
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
	return false;
}

//=============================================================================
// �ǂƐ����̌�_�i�ǂ̓�������O���ւ̃x�N�g���̂݁j
//=============================================================================
void CWallCylinder::CrossPoint(D3DXVECTOR2* pPosCross, const D3DXVECTOR3* pPosStart, const D3DXVECTOR3* pPosEnd, float fDelta) {
	if (pPosStart == nullptr || pPosEnd == nullptr) return;

	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
	
		//�I�u�W�F�N�g�^�C�v���قȂ�ꍇ
		if (!(pObject->GetObjType() & OBJTYPE_WALL)) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pObject);	//�~���̕ǃN���X�ɃL���X�g
		//�L���X�g���s��
		if (pWallCylinder == nullptr) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}
		//���S�t���O�������Ă���ꍇ
		if (pWallCylinder->GetDeath()) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}
		float fRadiusWall = pWallCylinder->GetRadius();
		//�ǂ̈ʒu
		D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();

		bool bOutStart = (sqrtf(powf(pPosStart->x - posWallCylinder.x, 2.0f) + powf(pPosStart->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);	//�����̎n�_���ǂ̊O�����ǂ���
		bool bOutEnd = (sqrtf(powf(pPosEnd->x - posWallCylinder.x, 2.0f) + powf(pPosEnd->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);		//�����̏I�_���ǂ̊O�����ǂ���

		//�������ǂ̓�������O���ł͂Ȃ��ꍇ
		if (bOutStart || !bOutEnd) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}
		D3DXVECTOR2 vecCenterToStart = D3DXVECTOR2(pPosStart->x - posWallCylinder.x, pPosStart->z - posWallCylinder.z);	//�~�̒��S��������̎n�_�̃x�N�g��
		D3DXVECTOR2 vecNorLine = D3DXVECTOR2(-(pPosEnd->z - pPosStart->z), pPosEnd->x - pPosStart->x);	//�����̖@���x�N�g��
		//�@���x�N�g���𐳋K��
		D3DXVec2Normalize(&vecNorLine, &vecNorLine);

		//�@���x�N�g����vecCenterToStart�����p�̏ꍇ
		if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) == 0) {
			//�����̃x�N�g�����~�̒��S��ʂ��Ă���̂�vecCenterToStart�𐳋K�����Ĕ��a�̒�����������
			D3DXVec2Normalize(&vecCenterToStart, &vecCenterToStart);
			*pPosCross = D3DXVECTOR2(posWallCylinder.x + vecCenterToStart.x * (fRadiusWall - fDelta), posWallCylinder.z + vecCenterToStart.y * (fRadiusWall - fDelta));
		}
		else {
			//�@���x�N�g����vecCenterToStart�̔��Α��̏ꍇ�x�N�g���𔽓]
			if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) < 0) {
				vecNorLine *= -1;
			}
			//�~�̒��S���璼���ւ̍ŒZ��������ςŌv�Z
			float fDistanceShort = D3DXVec2Dot(&vecNorLine, &vecCenterToStart);	//�ŒZ����
			//�@���x�N�g���ɍŒZ������������
			vecNorLine *= fDistanceShort;
			//�~�̒��S���璼���ւ̍ŒZ�����̈ʒu
			D3DXVECTOR2 posPointShort = D3DXVECTOR2(posWallCylinder.x + vecNorLine.x, posWallCylinder.z + vecNorLine.y);
			//posPointShort����pPosEnd�ւ̃x�N�g��
			D3DXVECTOR2 vecShortToEnd = D3DXVECTOR2(pPosEnd->x - posPointShort.x, pPosEnd->z - posPointShort.y);
			D3DXVec2Normalize(&vecShortToEnd, &vecShortToEnd);//���K��
			//vecShortToEnd�̒��������߂ĉ~�Ƃ̌�_�����߂�
			float fDistanceCross = sqrtf(powf((fRadiusWall - fDelta), 2.0f) - powf(fDistanceShort, 2.0f));	//posPointShort�����_�ւ̋���
			//��_��ݒ�
			*pPosCross = posPointShort + vecShortToEnd * fDistanceCross;
		}
		return;

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}

//=============================================================================
// �����x�̕ύX
//=============================================================================
void CWallCylinder::ChangeColorAlpha(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�V�[���̎擾
	CScene* pScene = pManager->GetScene();
	if (pScene == nullptr) return;
	//�v���C���[�̎擾
	CObject* pPlayer = (CObject*)pScene->GetPlayer();
	if (pPlayer == nullptr) return;

	float fRadiusWall = GetRadius();
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();	//�v���C���[�̈ʒu�̎擾
	float fDistance = sqrtf(powf(posPlayer.x - GetPos().x, 2.0f) + powf(posPlayer.z - GetPos().z, 2.0f));	//�v���C���[�̈ʒu�Ɖ~���̒��S�Ƃ̋���
	D3DXCOLOR col = GetColor();	//�F�̎擾

	if (fDistance > fRadiusWall - DISTANCE_COLOR_CHANGE) {
		col.a = (fDistance - (fRadiusWall - DISTANCE_COLOR_CHANGE)) / (fRadiusWall - DISTANCE_COLOR_MAX - (fRadiusWall - DISTANCE_COLOR_CHANGE));
		if (col.a > MAX_COLOR_ALPHA)col.a = MAX_COLOR_ALPHA;
	}
	else {
		col.a = 0.0f;
	}
	//�F�̐ݒ�
	SetColor(col);
}