//=============================================================================
//
// ���f������ [model.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"
#define MAX_LOAD_TEXT (128)	//��x�Ƀt�@�C������ǂݍ��ރe�L�X�g�̃o�C�g��
#define OUTLINE_SIZE (1.0f)	//�֊s�̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CModel::ModelData CModel::m_aModelData[(int)MODELTYPE::ENUM_MAX] = {};
char CModel::m_asFilePath[(int)MODELTYPE::ENUM_MAX][MAX_MODEL_FILE_PATH] = {};
D3DXMATERIAL CModel::m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};
CTexture::TEXTURE_TYPE CModel::m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	m_modelType = (MODELTYPE) 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pParent = nullptr;
	m_nIdxParent = -1;
	memset(&m_aMat, 0, sizeof(m_aMat));
	memset(&m_aChangeDiffuse, 0, sizeof(m_aChangeDiffuse));
	m_colGlow = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_bOutline = false;
	m_colOutline = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pCloneMesh = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ���f���̐�������
//=============================================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine) {
	CModel* pModel;
	pModel = new CModel;
	if (pModel == nullptr) return nullptr;

	pModel->m_modelType = type;
	pModel->m_pos = pos;
	pModel->m_rot = rot;
	pModel->m_pParent = pParent;
	pModel->m_bOutline = bOutLine;
	//�}�e���A���̐ݒ�
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		//�f�t�H���g�̃}�e���A�����擾
		pModel->m_aMat[nCntMat] = m_aMatDefault[(int)type][nCntMat];
	}

	pModel->Init();

	return pModel;
}

//=============================================================================
//���f���f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CModel::Load(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return S_OK;

	//-------------------------------------
	//�t�@�C���̓ǂݍ���
	//-------------------------------------
	FILE *pFile;		//�t�@�C���ւ̃|�C���^
	char sModelText[MAX_LOAD_TEXT];//��s���ǂݍ��ރt�@�C���̃e�L�X�g
	char *pModelText;	//�t�@�C���̃e�L�X�g�𕪊�����������
	int nIdxType = 0;	//���ɓǂݍ��ރ��f���̎��

	pFile = fopen(TEXT_FILE_NAME_MODEL, "r");

	if (pFile != nullptr) {
		while (fgets(sModelText, MAX_LOAD_TEXT, pFile) != nullptr) //��s���Ƃɕ�������擾
		{
			pModelText = strtok(sModelText, " =\t\n");	//������̕����i�� �^�u ���s ���j
			if (pModelText != nullptr) {
				//�R�����g
				if (strstr(pModelText, "//") != nullptr) {
					continue;
				}
				//�f�B���N�g�����̃R�s�[
				if (strlen(pModelText) < MAX_MODEL_FILE_PATH) {
					strcpy(m_asFilePath[nIdxType], pModelText);
				}
				nIdxType++;	//�C���f�b�N�X�����Z
				if (nIdxType >= (int)MODELTYPE::ENUM_MAX) 	break;	//�ǂݍ��ރ��f���̍ő吔�𒴂�����I��	
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else {
		//�t�@�C���̓ǂݍ��ݎ��s��
		return S_OK;
	}

	//-------------------------------------
	//���f���̓ǂݍ���
	//-------------------------------------
	for (int nCntModel = 0; nCntModel < (int)MODELTYPE::ENUM_MAX; nCntModel++) {
		//���łɃ��f������������Ă����ꍇ
		//�}�e���A���o�b�t�@�̉��
		if (m_aModelData[nCntModel].pBuffMat != nullptr) {
			m_aModelData[nCntModel].pBuffMat->Release();
			m_aModelData[nCntModel].pBuffMat = nullptr;
		}
		//���b�V���̉��
		if (m_aModelData[nCntModel].pMesh != nullptr) {
			m_aModelData[nCntModel].pMesh->Release();
			m_aModelData[nCntModel].pMesh = nullptr;
		}

		//���f���̐���
		D3DXLoadMeshFromX(m_asFilePath[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,		//�����܂ŌŒ�
			&m_aModelData[nCntModel].pBuffMat,
			NULL,
			&m_aModelData[nCntModel].nNumMat,
			&m_aModelData[nCntModel].pMesh);

		//�e�N�X�`���^�C�v�̐ݒ�	
		if (m_aModelData[nCntModel].pBuffMat == nullptr || m_aModelData[nCntModel].nNumMat <= 0) continue;	//�}�e���A���o�b�t�@��NULL���A�}�e���A������0�ȉ��Ȃ�ȍ~�̏������΂�

		D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^
		pMat = (D3DXMATERIAL*)m_aModelData[nCntModel].pBuffMat->GetBufferPointer();	//�}�e���A�����ɑ΂���|�C���^���擾

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[nCntModel].nNumMat; nCntMat++) {
			//�f�t�H���g�̃}�e���A�����擾
			m_aMatDefault[nCntModel][nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != nullptr) {
				//�e�N�X�`���N���X�̃p�X�Ɣ�r
				for (int nCntTex = 1; nCntTex < (int)CTexture::TEXTURE_TYPE::ENUM_MAX; nCntTex++) {
					//�����񂪈�v�Ńe�N�X�`���̔ԍ������߂�
					if (strcmp(pMat[nCntMat].pTextureFilename, CTexture::GetPathName((CTexture::TEXTURE_TYPE)nCntTex)) == 0) {
						m_aTexType[nCntModel][nCntMat] = (CTexture::TEXTURE_TYPE)nCntTex;
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//���f���f�[�^�̉��
//=============================================================================
void CModel::Unload(void) {
	for (int nCnt = 0; nCnt < (int)MODELTYPE::ENUM_MAX; nCnt++) {
		//�}�e���A���o�b�t�@�̉��
		if (m_aModelData[nCnt].pBuffMat != nullptr) {
			m_aModelData[nCnt].pBuffMat->Release();
			m_aModelData[nCnt].pBuffMat = nullptr;
		}
		//���b�V���̉��
		if (m_aModelData[nCnt].pMesh != nullptr) {
			m_aModelData[nCnt].pMesh->Release();
			m_aModelData[nCnt].pMesh = nullptr;
		}
	}
}

//=============================================================================
// ���f���f�[�^�̎擾
//=============================================================================
CModel::ModelData CModel::GetModelData(MODELTYPE type) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return ModelData();
	return m_aModelData[(int)type];
}

//=============================================================================
// ���f���̃p�X�̎擾
//=============================================================================
char* CModel::GetPathName(MODELTYPE type) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return nullptr;
	return m_asFilePath[(int)type];
}

//=============================================================================
// ���f���̃f�t�H���g�̐F���擾
//=============================================================================
D3DXCOLOR CModel::GetDefaultColor(MODELTYPE type, int nIdx) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	return m_aMatDefault[(int)type][nIdx].MatD3D.Diffuse;
}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CModel::Init(void) {
	//�֊s������ꍇ
	if (m_bOutline) {
		//�������b�V���𐶐�
		CreateCloneMesh();
		//�������b�V����c��������
		ExpansionCloneMesh();
	}

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CModel::Uninit(void) {
	//�������b�V���̔j��
	if (m_pCloneMesh != nullptr) {
		m_pCloneMesh->Release();
		m_pCloneMesh = nullptr;
	}
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CModel::Update(void) {
	//�F�̕ύX�̍X�V
	UpdateColorChange();
}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CModel::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//���f���̏�񂪐������m�ۂ���Ă��Ȃ��ꍇ�`��I��
	if (m_aModelData[(int)m_modelType].pBuffMat == nullptr || m_aModelData[(int)m_modelType].pMesh == nullptr || m_aModelData[(int)m_modelType].nNumMat <= 0) return;

	//----------------------------------------------------
	//�}�g���b�N�X�̐ݒ�
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;			//�e�̃}�g���b�N�X

	//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != nullptr) {
		mtxParent = m_pParent->GetMtxWorld();	//�e�̃��f������}�g���b�N�X���擾
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//�Ō�ɃZ�b�g�������[���h�}�g���b�N�X��e�̃}�g���b�N�X�Ƃ��Đݒ�
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//�V�F�[�_�Ƀ}�g���b�N�X��ݒ�
	pRenderer->SetEffectMatrixWorld(m_mtxWorld);

	//----------------------------------------------------
	//���f���̕`��
	//----------------------------------------------------
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�֊s�̕`��	Z�l�e�N�X�`���ɂ͏������܂Ȃ�
	if (m_bOutline && m_pCloneMesh != nullptr && !pRenderer->GetDrawZTex()) {
		//����
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//Z�e�X�g
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		//Z�o�b�t�@�̍X�V
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		D3DMATERIAL9 matOutline;	//�֊s�p�}�e���A��
		memset(&matOutline, 0, sizeof(D3DMATERIAL9));
		//�}�e���A�����̐ݒ�
		matOutline.Diffuse = m_colOutline;

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//�}�e���A���̐ݒ�
			pRenderer->SetEffectMaterialDiffuse(matOutline.Diffuse);
			pRenderer->SetEffectMaterialEmissive(matOutline.Emissive);
			pRenderer->SetEffectMaterialSpecular(matOutline.Specular);
			pRenderer->SetEffectMaterialPower(matOutline.Power);

			pRenderer->BeginPassEffect(PASS_3D);
			//�֊s�̕`��	
			m_pCloneMesh->DrawSubset(nCntMat);

			pRenderer->EndPassEffect();
		}

		// �\��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
		//Z�e�X�g
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//Z�o�b�t�@�̍X�V
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//���f���̕`��
	if (m_aModelData[(int)m_modelType].pMesh != nullptr) {
		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//�e�N�X�`���̎擾
			LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(m_aTexType[(int)m_modelType][nCntMat]);
			//�e�N�X�`���̐ݒ�
			pRenderer->SetEffectTexture(pTexture);
			//�}�e���A���̐ݒ�
			pRenderer->SetEffectMaterialDiffuse(m_aMat[nCntMat].MatD3D.Diffuse);
			pRenderer->SetEffectMaterialEmissive(m_aMat[nCntMat].MatD3D.Emissive);
			pRenderer->SetEffectMaterialSpecular(m_aMat[nCntMat].MatD3D.Specular);
			pRenderer->SetEffectMaterialPower(m_aMat[nCntMat].MatD3D.Power);
			//�֊s�̔����F�̐ݒ�
			pRenderer->SetEffectColorGlow(m_colGlow);

			DWORD dwPassFlag = PASS_3D | PASS_LIGHT;
			//�e�N�X�`��������ꍇ�t���O��ǉ�
			if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;

			//�p�X�̊J�n
			pRenderer->BeginPassEffect(dwPassFlag);

			//���f���i�p�[�c�j�̕`��	
			m_aModelData[(int)m_modelType].pMesh->DrawSubset(nCntMat);

			pRenderer->EndPassEffect();
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ���f���̈ʒu���̐ݒ�
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

//=============================================================================
// ���f���̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) { return m_pos; }

//=============================================================================
// ���f���̊p�x�̐ݒ�
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// ���f���̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) { return m_rot; }

//=============================================================================
// ���[���h�}�g���b�N�X�̍X�V
//=============================================================================
void CModel::UpdateMtxWorld(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//----------------------------------------------------
	//�}�g���b�N�X�̐ݒ�
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;			//�e�̃}�g���b�N�X

	//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != nullptr) {
		mtxParent = m_pParent->GetMtxWorld();	//�e�̃��f������}�g���b�N�X���擾
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//�Ō�ɃZ�b�g�������[���h�}�g���b�N�X��e�̃}�g���b�N�X�Ƃ��Đݒ�
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
//���[���h�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void) {
	return m_mtxWorld;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̃|�C���^�̎擾
//=============================================================================
D3DXMATRIX* CModel::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// ���f���̃^�C�v�̐ݒ�
//=============================================================================
void CModel::SetModelType(MODELTYPE type) {
	//�ύX���Ȃ��ꍇ�I��
	if (m_modelType == type) return;

	//��ނ̕ύX
	m_modelType = type;

	//���f���̎�ނ������̏ꍇ
	if ((int)m_modelType < 0 || (int)m_modelType >= (int)CModel::MODELTYPE::ENUM_MAX) {
		m_modelType = (MODELTYPE)0;
	}

	//�֊s������ꍇ
	if (m_pCloneMesh != nullptr) {
		//�������b�V���̔j��
		m_pCloneMesh->Release();
		m_pCloneMesh = nullptr;
		//�������b�V������蒼��
		CreateCloneMesh();
		//�������b�V����c��
		ExpansionCloneMesh();
	}
}

//=============================================================================
// ���f���̃^�C�v�̎擾
//=============================================================================
CModel::MODELTYPE CModel::GetModelType(void) {
	return m_modelType;
}

//=============================================================================
// �}�e���A���̐F�̐ݒ�
//=============================================================================
void CModel::SetMaterialDiffuse(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Diffuse.r = col.r;
	m_aMat[nIdx].MatD3D.Diffuse.g = col.g;
	m_aMat[nIdx].MatD3D.Diffuse.b = col.b;
	m_aMat[nIdx].MatD3D.Diffuse.a = col.a;
}

//=============================================================================
// �}�e���A���̐F�̎擾
//=============================================================================
D3DXCOLOR CModel::GetMaterialDiffuse(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	return m_aMat[nIdx].MatD3D.Diffuse;
}

//=============================================================================
// �}�e���A���̃A���t�@�l��ݒ�
//=============================================================================
void CModel::SetMaterialDiffuseAlpha(float fAlpha, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Diffuse.a = fAlpha;
}

//=============================================================================
// ���ׂẴ}�e���A���̃A���t�@�l��ݒ�
//=============================================================================
void CModel::SetMaterialDiffuseAlphaAll(float fAlpha) {
	for (int nCnt = 0; nCnt < MAX_MATERIAL; nCnt++)
	{
		m_aMat[nCnt].MatD3D.Diffuse.a = fAlpha;
	}
}

//=============================================================================
// �}�e���A���̃A���t�@�l���擾
//=============================================================================
float CModel::GetMaterialDiffuseAlpha(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return 0.0f;

	return m_aMat[nIdx].MatD3D.Diffuse.a;
}

//=============================================================================
// �}�e���A���̃X�y�L�����[�F�̐ݒ�
//=============================================================================
void CModel::SetMaterialSpecular(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Specular.r = col.r;
	m_aMat[nIdx].MatD3D.Specular.g = col.g;
	m_aMat[nIdx].MatD3D.Specular.b = col.b;
	m_aMat[nIdx].MatD3D.Specular.a = col.a;
}

//=============================================================================
// �}�e���A���̔����F�̐ݒ�
//=============================================================================
void CModel::SetMaterialEmissive(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Emissive.r = col.r;
	m_aMat[nIdx].MatD3D.Emissive.g = col.g;
	m_aMat[nIdx].MatD3D.Emissive.b = col.b;
	m_aMat[nIdx].MatD3D.Emissive.a = col.a;
}

//=============================================================================
//�}�e���A���̔��˂̎����̐ݒ�
//=============================================================================
void CModel::SetMaterialPower(float fPower, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Power = fPower;
}

//=============================================================================
// �}�e���A���̐F�̕ύX�̊J�n
//=============================================================================
void CModel::StartChangeMaterialDiffuse(int nIdxMat, D3DXCOLOR colDest, int nTimeFin) {
	//���݂̃}�e���A���̐F�ƖڕW�̐F�Ƃ̍������疈�t���[�����Z�����F���v�Z
	m_aChangeDiffuse[nIdxMat].colAdd = (colDest - GetMaterialDiffuse(nIdxMat)) / (float)nTimeFin;
	//������
	m_aChangeDiffuse[nIdxMat].nTimeFinish = nTimeFin;
	m_aChangeDiffuse[nIdxMat].nCnt = 0;
}

//=============================================================================
// �֊s�̔����F�̐ݒ�
//=============================================================================
void CModel::SetColorGlow(D3DXCOLOR col) {
	m_colGlow = col;
}

//=============================================================================
// ���f���̕������b�V���̐���
//=============================================================================
void CModel::CreateCloneMesh(void) {
	//���łɕ������b�V��������ꍇ���A���f���̃f�[�^�����݂��Ȃ��ꍇ�I��
	if (m_pCloneMesh != nullptr || m_aModelData[(int)m_modelType].pMesh == nullptr) return;

	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//���b�V���̕���
	m_aModelData[(int)m_modelType].pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX_3D, pDevice, &m_pCloneMesh);

	m_bOutline = true;
}

//=============================================================================
// ���f���̕������b�V����c��������
//=============================================================================
void CModel::ExpansionCloneMesh(void) {
	//�������b�V�����Ȃ��ꍇ�I��
	if (m_pCloneMesh == nullptr) return;

	//���_���̎擾
	int nNumVtx = 0;
	nNumVtx = m_pCloneMesh->GetNumVertices();

	VERTEX_3D *pVtx;	//FVF��ύX���ĕ��������̂ł���ɍ��킹���\���̂̒��_�o�b�t�@�̃|�C���^

	//���_�o�b�t�@�̃��b�N
	m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
		//�S���_��@�������֖c��
		pVtx[nCnt].pos = D3DXVECTOR3(pVtx[nCnt].pos.x + pVtx[nCnt].nor.x * OUTLINE_SIZE,
			pVtx[nCnt].pos.y + pVtx[nCnt].nor.y * OUTLINE_SIZE,
			pVtx[nCnt].pos.z + pVtx[nCnt].nor.z * OUTLINE_SIZE);
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pCloneMesh->UnlockVertexBuffer();
}

//=============================================================================
// �֊s�̐F�̐ݒ�
//=============================================================================
void CModel::SetColorOutline(D3DXCOLOR col) {
	m_colOutline = col;
}

//=============================================================================
// �֊s�̕\���ݒ�
//=============================================================================
void CModel::SetDrawOutline(bool bDraw) {
	m_bOutline = bDraw;

	//�\��
	if (bDraw) {
		//���łɕ������b�V��������ꍇ
		if (m_pCloneMesh != nullptr) return;

		//�������b�V���𐶐�
		CreateCloneMesh();
		//�������b�V����c��
		ExpansionCloneMesh();
	}
	//��\��
	else {
		//�������b�V���̔j��
		if (m_pCloneMesh != nullptr) {
			m_pCloneMesh->Release();
			m_pCloneMesh = nullptr;
		}
	}
}

//=============================================================================
// �F�̕ύX�̍X�V
//=============================================================================
void CModel::UpdateColorChange(void) {
	for (int nIdxMat = 0; nIdxMat < MAX_MATERIAL; nIdxMat++)
	{
		//�J�E���^�[���I�����Ԃ��߂��Ă����ꍇ���[�v���΂�
		if (m_aChangeDiffuse[nIdxMat].nCnt >= m_aChangeDiffuse[nIdxMat].nTimeFinish) continue;

		//�J�E���^�[�����Z
		m_aChangeDiffuse[nIdxMat].nCnt++;

		//���f���̌��݂̐F���擾
		const D3DXCOLOR colModel = GetMaterialDiffuse(nIdxMat);
		//�F�̐ݒ�
		SetMaterialDiffuse(colModel + m_aChangeDiffuse[nIdxMat].colAdd, nIdxMat);
	}
}