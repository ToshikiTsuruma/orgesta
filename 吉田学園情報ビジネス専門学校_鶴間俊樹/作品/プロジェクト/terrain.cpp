//=============================================================================
//
// �n�`���� [terrain.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "terrain.h"
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTerrain::CTerrain(CModel::MODELTYPE typeModel) : CObjectModel(typeModel, false)
{
	SetObjType(OBJTYPE_TERRAIN);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::BG);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTerrain::~CTerrain()
{

}

//=============================================================================
// �n�`�̐�������
//=============================================================================
CTerrain* CTerrain::Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CTerrain* pTerrain;
	pTerrain = new CTerrain(typeModel);
	if (pTerrain == nullptr) return nullptr;

	pTerrain->SetPos(pos);
	pTerrain->SetRot(rot);
	pTerrain->Init();

	return pTerrain;
}

//=============================================================================
// �n�`�̏���������
//=============================================================================
HRESULT CTerrain::Init(void) {
	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// �n�`�̏I������
//=============================================================================
void CTerrain::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// �n�`�̍X�V����
//=============================================================================
void CTerrain::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// �n�`�̕`�揈��
//=============================================================================
void CTerrain::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// �n�`�Ƃ̏Փ˔���
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3& posCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd) {
	CObject* pObject;	//�I�u�W�F�N�g�ւ̃|�C���^
	pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
	
		//�I�u�W�F�N�g�^�C�v���قȂ�ꍇ
		if (!(pObject->GetObjType() & OBJTYPE_TERRAIN)) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pObject);	//�n�`�N���X�ɃL���X�g
		//�L���X�g���s��
		if (pTerrain == nullptr) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���[�v���΂�
		}
		//���S�t���O�������Ă���ꍇ
		if (pTerrain->GetDeath()) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���[�v���΂�
		}

		//�n�`�̃��f���f�[�^�̎擾
		CModel::MODELTYPE modelType = pTerrain->GetModelType();	//���f���̎�ނ̎擾
		CModel::ModelData modelData = CModel::GetModelData(modelType);	//���f���̃f�[�^�̎擾
		LPD3DXMESH pMesh = modelData.pMesh;	//���b�V���f�[�^�̎擾
		//���b�V����null�̏ꍇ
		if (pMesh == nullptr) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���[�v���΂�
		}

		D3DXVECTOR3 posTerrain = pTerrain->GetPos();//�n�`�̈ʒu				
		D3DXVECTOR3 rotTerrain = pTerrain->GetRot();//�n�`�̉�]				

		D3DXMATRIX mtxWorldTerrain;	//�n�`�̃��[���h�}�g���b�N�X
		D3DXMATRIX mtxTrans, mtxRot;	//�v�Z�p�}�g���b�N�X
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorldTerrain);
		//�n�`�̉�]���s��ɂ�����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
		D3DXMatrixMultiply(&mtxWorldTerrain, &mtxWorldTerrain, &mtxRot);
		//�n�`�̈ʒu���W���s��ɂ�����
		D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
		D3DXMatrixMultiply(&mtxWorldTerrain, &mtxWorldTerrain, &mtxTrans);

		D3DXMATRIX mtxInverseTerrain;	//�n�`�̃��[���h�}�g���b�N�X�̋t�s��
		//�n�`�̍s��̋t�s����v�Z
		D3DXMatrixInverse(&mtxInverseTerrain, nullptr, &mtxWorldTerrain);
		//���C�ɒn�`�̋t�s���������
		D3DXVec3TransformCoord(&vecStart, &vecStart, &mtxInverseTerrain);
		D3DXVec3TransformCoord(&vecEnd, &vecEnd, &mtxInverseTerrain);

		D3DXVECTOR3 vecDir = vecEnd - vecStart;	//���C�̕����x�N�g��

		BOOL bHit;		//�Փ˔���t���O
		DWORD faceIdx;	//�������Ɉ�ԋ߂��ʂ̃C���f�b�N�X
		float fU, fV;	//���������O�p�`�̏d�S���W?

		//�����蔻��
		D3DXIntersect(pMesh, &vecStart, &vecDir, &bHit, &faceIdx, &fU, &fV, nullptr, nullptr, nullptr);

		//���C���Փ˂��Ă��Ȃ��ꍇ
		if (bHit == FALSE) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	//���[�v���΂�
		}

		//-------------------------------------------------
		//���肪�������O�p�`�̒��_�̃C���f�b�N�X�̎擾
		//-------------------------------------------------
		int nIdx[3];	//�O�p�`�̒��_�̃C���f�b�N�X
		WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^
		//�C���f�b�N�X�o�b�t�@�̃��b�N
		pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
		for (int nCnt = 0; nCnt < 3; nCnt++) {
			nIdx[nCnt] = pIdx[(faceIdx * 3) + nCnt];
		}

		//�C���f�b�N�X�o�b�t�@�̃A�����b�N
		pMesh->UnlockIndexBuffer();

		//-------------------------------------------------
		//���肪�������O�p�`�̒��_�̈ʒu�̎擾
		//-------------------------------------------------
		D3DXVECTOR3 posVtx[3];	//�Փ˂����O�p�`�̒��_�̈ʒu���W

		DWORD dwStride = pMesh->GetNumBytesPerVertex();	//���_���Ƃ̃o�C�g��

		BYTE* pbVtx = nullptr;
		float* pfVtx = nullptr;

		//���_�o�b�t�@�̃��b�N
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pbVtx);

		for (int nCntVtx = 0; nCntVtx < 3; nCntVtx++) {
			//�C���f�b�N�X�ɉ��������_���W���擾
			pfVtx = (float*)&pbVtx[dwStride * nIdx[nCntVtx]];

			posVtx[nCntVtx].x = pfVtx[0];
			posVtx[nCntVtx].y = pfVtx[1];
			posVtx[nCntVtx].z = pfVtx[2];
		}

		//���_�o�b�t�@�̃A�����b�N
		pMesh->UnlockVertexBuffer();

		D3DXVECTOR3 posCol = D3DXVECTOR3(posVtx[0] + fU * (posVtx[1] - posVtx[0]) + fV * (posVtx[2] - posVtx[0]));	//�n�`�̃��[�J�����W�ł̏Փˈʒu

		//�n�`�Ƃ̏Փˈʒu�����[���h���W�ɕϊ�
		D3DXVec3TransformCoord(&posCol, &posCol, &mtxWorldTerrain);
		//�Փ˂����I�u�W�F�N�g�̈ʒu��ݒ�
		posCollision = posCol;
		return true;
	
		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}

	return false;
}

//=============================================================================
// ������x�N�g���̒n�`�Ƃ̏Փ˔���
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3& posCollision) {
	D3DXVECTOR3 vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = posCollision;
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	//�n�`�Ƃ̓����蔻��
	return CTerrain::Collision(posCollision, vecStart, vecEnd);
}