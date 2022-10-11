//=============================================================================
//
// �������� [thunder.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "thunder.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "objectList.h"
#include "terrain.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define THUNDER_WIDTH (400.0f)
#define THUNDER_HEIGHT (1000.0f)

#define DAMAGE_THUNDER (40)	//�����̃_���[�W
#define COLLISION_RADIUS (120.0f)	//�����蔻��̔��a

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CThunder::CThunder() : CEffect(EFFECT_TYPE::THUNDERBOLT, false)
{
	//�I�u�W�F�N�g���X�g�̐���
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CThunder::~CThunder()
{
	//�I�u�W�F�N�g���X�g�̔j��
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// �����̐�������
//=============================================================================
CThunder* CThunder::Create(D3DXVECTOR3 pos) {
	CThunder* pThunder;
	pThunder = new CThunder();
	if (pThunder == nullptr) return nullptr;

	pThunder->SetPos(pos);
	pThunder->SetSize(D3DXVECTOR3(THUNDER_WIDTH, THUNDER_HEIGHT, 0.0f));

	pThunder->Init();

	return pThunder;
}

//=============================================================================
// �����̏���������
//=============================================================================
HRESULT CThunder::Init(void) {
	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = GetPos();	//�G�̈ʒu���擾
	vecStart.y = 5000.0f;	//�n�`�ɓ�����悤�ɓK���ȍ���
	vecEnd = vecStart;
	vecEnd.y += -1.0f;	//�������̃x�N�g��

	bLand = CTerrain::Collision(posColTerrain, vecStart, vecEnd);
	//�ڒn��
	if (bLand) {
		SetPos(posColTerrain + D3DXVECTOR3(0.0f, (THUNDER_HEIGHT * 0.7f) / 2.0f, 0.0f));	//�ʒu�̈ړ� �r���{�[�h�̒��S��pos�Ȃ̂ŉ����n�ʂɗ���悤�ɒ�������
	}

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�������̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::THUNDERBOLT);

	CEffect::Init();
	return S_OK;
}

//=============================================================================
// �����̏I������
//=============================================================================
void CThunder::Uninit(void) {
	CEffect::Uninit();
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void CThunder::Update(void) {
	//�U���̓����蔻��
	AttackCollision(OBJTYPE_ENEMY | OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT);

	CEffect::Update();
}

//=============================================================================
// �����̕`�揈��
//=============================================================================
void CThunder::Draw(void) {
	//�`��
	CEffect::Draw();
}

//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CThunder::AttackCollision(int nObjtype) {
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

		//�����͈͓̔��ɓG�����Ȃ��ꍇ�����蔻�莸�s
		if (fDistObj > COLLISION_RADIUS) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���̃��[�v
		}

		//---------------------------
		//�����蔻�莞�̏���
		//---------------------------
		//�I�u�W�F�N�g�Ƀ_���[�W��^����
		pObject->Damage(DAMAGE_THUNDER, DAMAGE_TYPE::THUNDERBOLT, nullptr);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		//�T�E���h�̎擾
		CSound *pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();

		//�_���[�W���̍Đ�
		//if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);

		//�I�u�W�F�N�g�����S���Ă��Ȃ��ꍇ�U���ς݃��X�g�ɒǉ�
		if (!pObject->GetDeath()) {
			if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}