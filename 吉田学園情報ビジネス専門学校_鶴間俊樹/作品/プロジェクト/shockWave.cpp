//=============================================================================
//
// �Ռ��g���� [shockWave.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "shockWave.h"
#include "manager.h"
#include "renderer.h"
#include "objectList.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DAMAGE_SHOCKWAVE (10)	//�Ռ��g�̃_���[�W

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CShockwave::CShockwave() : CMeshcylinder()
{

}

//=============================================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CShockwave::CShockwave(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fHeight) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, fRadiusTop, fRadiusBottom, 1, fHeight, true)
{
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fAddRadius = 0.0f;
	m_fAddHeight = 0.0f;
	m_fRotateSpeed = 0.0f;
	//�I�u�W�F�N�g���X�g�̐���
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShockwave::~CShockwave()
{
	//�I�u�W�F�N�g���X�g�̔j��
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// �Ռ��g�̐�������
//=============================================================================
CShockwave* CShockwave::Create(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed) {
	CShockwave* pShockwave;
	pShockwave = new CShockwave(pos, fRadiusTop, fRadiusBottom, fHeight);
	if (pShockwave == nullptr) return nullptr;

	//�Ռ��g�̃e�N�X�`����ݒ�
	pShockwave->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_SHOCKWAVE);

	pShockwave->m_nLife = nLife;
	pShockwave->m_fAddRadius = fAddRadius;
	pShockwave->m_fAddHeight = fAddHeight;
	pShockwave->m_fRotateSpeed = fRotSpeed;

	pShockwave->Init();

	return pShockwave;
}

//=============================================================================
// �Ռ��g�̏���������
//=============================================================================
HRESULT CShockwave::Init(void) {

	CMeshcylinder::Init();
	return S_OK;
}

//=============================================================================
// �Ռ��g�̏I������
//=============================================================================
void CShockwave::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// �Ռ��g�̍X�V����
//=============================================================================
void CShockwave::Update(void) {
	//���a�𑝉�
	AddRadius(m_fAddRadius);

	//�����𑝉�
	AddHeight(m_fAddHeight);

	//��]������
	Rotate();

	//�U���̓����蔻��
	AttackCollision(OBJTYPE_ENEMY);

	CMeshcylinder::Update();

	//���C�t�̌���
	m_nLife--;
	if (m_nLife <= 0) Uninit();
}

//=============================================================================
// �Ռ��g�̕`�揈��
//=============================================================================
void CShockwave::Draw(void) {
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
	//���Z����
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�`��
	CMeshcylinder::Draw();

	//�J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//���Z������߂�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Z�o�b�t�@�̍X�V
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ��]����
//=============================================================================
void CShockwave::Rotate(void) {
	SetRot(GetRot() + D3DXVECTOR3(0.0f, m_fRotateSpeed, 0.0f));
}

//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CShockwave::AttackCollision(int nObjtype) {
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
		D3DXVECTOR3 posObj = pObject->GetPos();	//�I�u�W�F�N�g�̈ʒu

		//�U���\�Ȕ͈͂ɂ��邩�ǂ���
		D3DXVECTOR2 vecObj = D3DXVECTOR2(posObj.x - GetPos().x, posObj.z - GetPos().z);	//�Ռ��g�̈ʒu�ƃI�u�W�F�N�g�̃x�N�g��
		float fDistObj = D3DXVec2Length(&vecObj);	//�U���ʒu�ƃI�u�W�F�N�g�̋���

		//�Ռ��g�͈͓̔��ɓG�����Ȃ��ꍇ�����蔻�莸�s	�˗͔g�ŃW�����v���邱�Ƃ�����̂ŏՌ��g����Ȃ瓖����Ȃ� �����ꂽ�ꍇ�̔�����
		if (fDistObj > GetRadius() || posObj.y - GetPos().y > GetHeight() + 20.0f) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���̃��[�v
		}

		//---------------------------
		//�����蔻�莞�̏���
		//---------------------------
		//�I�u�W�F�N�g�Ƀ_���[�W��^����
		pObject->Damage(DAMAGE_SHOCKWAVE, DAMAGE_TYPE::SHOCKWAVE, nullptr);

		//�I�u�W�F�N�g�����S���Ă��Ȃ��ꍇ�U���ς݃��X�g�ɒǉ�
		if (!pObject->GetDeath()) {
			if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}