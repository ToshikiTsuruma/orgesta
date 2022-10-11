//=============================================================================
//
// UI���f���I�u�W�F�N�g���� [objectModelUI.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_MODEL_UI_H_
#define _OBJECT_MODEL_UI_H_

#include "main.h"
#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// UI���f���I�u�W�F�N�g�N���X
//*****************************************************************************
class CObjectModelUI : public CObjectModel
{
public:

	CObjectModelUI();		//�f�t�H���g�R���X�g���N�^
	CObjectModelUI(CModel::MODELTYPE typeModel, bool bOutLine);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectModelUI();		//�f�X�g���N�^
	static CObjectModelUI* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

	void SetViewMatrix(D3DXMATRIX mtxView) { m_mtxView = mtxView; }	//�r���[�}�g���b�N�X�̐ݒ�
	void SetViewPos(D3DXVECTOR3 posV) { m_posV = posV; }		//�J�����̎��_�ݒ�
	void SetRotSpeed(D3DXVECTOR3 rotSpeed) { m_rotSpeed = rotSpeed; }	//��]���x�̐ݒ�
private:
	D3DXMATRIX m_mtxView;	//�r���[�}�g���b�N�X
	D3DXVECTOR3 m_posV;		//�J�����̎��_
	D3DXVECTOR3 m_rotSpeed;	//��]���x

};

#endif // !_OBJECT_MODEL_UI_H_