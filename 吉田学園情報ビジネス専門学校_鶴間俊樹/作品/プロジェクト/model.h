//=============================================================================
//
// ���f������ [model.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODEL_FILE_PATH (128)	//���f���̃t�@�C���̃p�X�̒����̍ő�
#define MAX_MATERIAL (16)			//���f�����Ƃ̃}�e���A���̍ő吔

//*****************************************************************************
// ���f���N���X
//*****************************************************************************
class CModel
{
public:
	//���f���̎��
	enum class MODELTYPE{
		//�n�`
		TERRAIN = 0,	//�n�`

		//�I�u�W�F�N�g
		OBJ_APPLE,
		OBJ_APPLE_TREE,
		OBJ_APPLE_BULLET,
		OBJ_MENUAPPLE,
		OBJ_TREE,
		OBJ_SCAPEGOAT,
		OBJ_SEEDLING,
		OBJ_FOUNTAIN,
		OBJ_ROCK,

		//�v���C���[
		pl_0, pl_1, pl_2, pl_3, pl_4, pl_5, pl_6, pl_7, pl_8, 

		//�G
		eNo_0, eNo_1, eNo_2, eNo_3, eNo_4,	//�ʏ�G
		eHu_0, eHu_1, eHu_2, eHu_3, eHu_4, eHu_5, eHu_6, eHu_7, eHu_8, eHu_9,	//�ʏ�G
		eSo_0, eSo_1, eSo_2, eSo_3, eSo_4,	//���p�t�G

		ENUM_MAX	//�ő吔
	};

	//���f���f�[�^
	typedef struct
	{
		LPD3DXMESH pMesh;	//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
		DWORD nNumMat;	//�}�e���A���̐�
	} ModelData;

	//�F�̕ύX
	typedef struct {
		D3DXCOLOR colAdd;	//�ύX���̓t���[�����ɉ��Z�����F
		int nTimeFinish;	//�F�̕ύX�ɂ����鎞��
		int nCnt;			//�J�E���^�[
	} ChangeDiffuse;

	CModel();	//�f�t�H���g�R���X�g���N�^
	~CModel();	//�f�X�g���N�^
	static CModel* Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//��������
	static HRESULT Load(void);	//���f���f�[�^�̓ǂݍ���
	static void Unload(void);	//���f���f�[�^�̉��
	static ModelData GetModelData(MODELTYPE type);	//���f���f�[�^�̎擾
	static char* GetPathName(MODELTYPE type);		//�e�N�X�`���̃p�X�̕�����̎擾
	static D3DXCOLOR GetDefaultColor(MODELTYPE type, int nIdx);	//���f���̃f�t�H���g�̐F���擾

	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	void UpdateMtxWorld(void);		//���[���h�}�g���b�N�X�̍X�V
	D3DXMATRIX GetMtxWorld(void);			//���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX* GetPtrMtxWorld(void);		//���[���h�}�g���b�N�X�̃|�C���^�̎擾
	void SetModelType(MODELTYPE type);		//���f���̃^�C�v�̐ݒ�
	MODELTYPE GetModelType(void);			//���f���̃^�C�v�̎擾
	void SetMaterialDiffuse(D3DXCOLOR col, int nIdx);	//�}�e���A���̐F�̐ݒ�
	D3DXCOLOR GetMaterialDiffuse(int nIdx);				//�}�e���A���̐F�̎擾
	void SetMaterialDiffuseAlpha(float fAlpha, int nIdx);	//�}�e���A���̃A���t�@�l��ݒ�
	void SetMaterialDiffuseAlphaAll(float fAlpha);			//���ׂẴ}�e���A���̃A���t�@�l��ݒ�
	float GetMaterialDiffuseAlpha(int nIdx);				//�}�e���A���̃A���t�@�l���擾
	void SetMaterialSpecular(D3DXCOLOR col, int nIdx);	//�}�e���A���̃X�y�L�����[�F�̐ݒ�
	void SetMaterialEmissive(D3DXCOLOR col, int nIdx);	//�}�e���A���̔����F�̐ݒ�
	void SetMaterialPower(float fPower, int nIdx);		//�}�e���A���̔��˂̎����̐ݒ�
	void StartChangeMaterialDiffuse(int nIdxMat, D3DXCOLOR colDest, int nTimeFin);	//�}�e���A���̐F�̕ύX�̊J�n
	void SetColorGlow(D3DXCOLOR col);	//�֊s�̔����F�̐ݒ�

	void CreateCloneMesh(void);		//�������b�V���̐���
	void ExpansionCloneMesh(void);	//�������b�V����c��������
	void SetColorOutline(D3DXCOLOR col);	//�֊s�̐F�̐ݒ�
	void SetDrawOutline(bool bDraw);		//�֊s�̕\���ݒ�

private:
	void UpdateColorChange(void);	//�F�̕ύX�̍X�V

	static ModelData m_aModelData[(int)MODELTYPE::ENUM_MAX];	//�S���f���̏��
	static char m_asFilePath[(int)MODELTYPE::ENUM_MAX][MAX_MODEL_FILE_PATH];	// ���f���̃e�L�X�g�t�@�C���̃p�X
	static D3DXMATERIAL m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//���f���̃}�e���A���̏����l
	static CTexture::TEXTURE_TYPE m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//�}�e���A�����Ƃ̃e�N�X�`���̎��

	MODELTYPE m_modelType;	//���f���̎��
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//����
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	CModel* m_pParent;		//�e�̃��f���ւ̃|�C���^
	int m_nIdxParent;		//�e�̃C���f�b�N�X
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	//���f���̃}�e���A��
	ChangeDiffuse m_aChangeDiffuse[MAX_MATERIAL];	//���f���̃}�e���A���̐F��ύX����\���̂̔z��
	D3DXCOLOR m_colGlow;	//�֊s�̔����F�̐F

	bool m_bOutline;			//�֊s�����邩�ǂ���
	D3DXCOLOR m_colOutline;	//�֊s�̐F
	LPD3DXMESH m_pCloneMesh;	//�����������b�V���̃|�C���^
};

#endif // !_MODEL_H_