//=============================================================================
//
// �˗͔g���� [repulsiveWave.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "repulsiveWave.h"
#include "manager.h"
#include "renderer.h"
#include "objectList.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CRepulsivewave::CRepulsivewave() : CMeshcylinder()
{

}

//=============================================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CRepulsivewave::CRepulsivewave(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, fRadiusInside, fRadiusOutside, 1, 0.0f, true)
{
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fAddRadius = 0.0f;
	m_fRotateSpeed = 0.0f;
	//�I�u�W�F�N�g���X�g�̐���
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRepulsivewave::~CRepulsivewave()
{
	//�I�u�W�F�N�g���X�g�̔j��
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// �˗͔g�̐�������
//=============================================================================
CRepulsivewave* CRepulsivewave::Create(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside, float fAddRadius, int nLife, float fRotSpeed) {
	CRepulsivewave* pRepulsivewave;
	pRepulsivewave = new CRepulsivewave(pos, fRadiusInside, fRadiusOutside);
	if (pRepulsivewave == nullptr) return nullptr;

	//�Ռ��g�̃e�N�X�`����ݒ�
	pRepulsivewave->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_REPULSIVEWAVE);

	pRepulsivewave->m_nLife = nLife;
	pRepulsivewave->m_fAddRadius = fAddRadius;
	pRepulsivewave->m_fRotateSpeed = fRotSpeed;

	pRepulsivewave->Init();

	return pRepulsivewave;
}

//=============================================================================
// �˗͔g�̏���������
//=============================================================================
HRESULT CRepulsivewave::Init(void) {

	CMeshcylinder::Init();
	return S_OK;
}

//=============================================================================
// �˗͔g�̏I������
//=============================================================================
void CRepulsivewave::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// �˗͔g�̍X�V����
//=============================================================================
void CRepulsivewave::Update(void) {
	//��]������
	Rotate();

	CMeshcylinder::Update();

	//���C�t���s�����ꍇ
	if (m_nLife <= 0) {
		//�����������a���L����
		AddRadiusTop(m_fAddRadius);
		//�������O�����傫���Ȃ�����j��
		if (GetRadius() <= GetRadiusTop()) Uninit();
		return;
	}

	//���a�𑝉�
	AddRadius(m_fAddRadius);

	//�U���̓����蔻��
	AttackCollision(OBJTYPE_ENEMY);


	//���C�t�̌���
	m_nLife--;
}

//=============================================================================
// �˗͔g�̕`�揈��
//=============================================================================
void CRepulsivewave::Draw(void) {
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
	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�`��
	CMeshcylinder::Draw();

	//�J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ��]����
//=============================================================================
void CRepulsivewave::Rotate(void) {
	SetRot(GetRot() + D3DXVECTOR3(0.0f, m_fRotateSpeed, 0.0f));
}

//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CRepulsivewave::AttackCollision(int nObjtype) {
	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & nObjtype) bMatchType = true;

		//���łɍU�����������Ă���ꍇ
		bool bAttacked = false;	//���łɍU������Ă��邩�ǂ���

		if (m_pListAttacked != nullptr) {
			//���X�g�ɂ��łɒǉ�����Ă���ꍇ
			bAttacked = m_pListAttacked->MatchObject(pObject);
		}

		bool bDeath = pObject->GetDeath();
		bool bEnableCollision = pObject->GetEnableCollision();

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bAttacked || bDeath || !bEnableCollision) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		//---------------------------
		//�����蔻��
		//---------------------------
		//�U���\�Ȕ͈͂ɂ��邩�ǂ���
		D3DXVECTOR2 vecObj = D3DXVECTOR2(pObject->GetPos().x - GetPos().x, pObject->GetPos().z - GetPos().z);	//�Ռ��g�̈ʒu�ƃI�u�W�F�N�g�̃x�N�g��
		float fDistObj = D3DXVec2Length(&vecObj);	//�U���ʒu�ƃI�u�W�F�N�g�̋���

		//�˗͔g�͈͓̔��ɓG�����Ȃ��ꍇ�����蔻�莸�s
		if (fDistObj > GetRadius()) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���̃��[�v
		}

		//---------------------------
		//�����蔻�莞�̏���
		//---------------------------
		D3DXVec2Normalize(&vecObj, &vecObj);	//�I�u�W�F�N�g�����ւ̃x�N�g���𐳋K��
		D3DXVECTOR3 moveObj = D3DXVECTOR3(vecObj.x * 4.0f, 15.0f, vecObj.y * 4.0f);	//�I�u�W�F�N�g�̈ړ���
		//�I�u�W�F�N�g�𐁂���΂�
		pObject->SetMove(moveObj);

		//�U���ς݃��X�g�ɒǉ�
		if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}