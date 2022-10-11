//=============================================================================
//
// 3D�I�����j���[���� [selectMenu3D.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SELECT_MENU_3D_H_
#define _SELECT_MENU_3D_H_

#include "selectMenu.h"
#include "objectModelUI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// 3D�I�����j���[�N���X
//*****************************************************************************
class CSelectMenu3D : public CSelectMenu
{
public:

	CSelectMenu3D();		//�f�t�H���g�R���X�g���N�^
	CSelectMenu3D(int nNumSelect, D3DXVECTOR3 posCenter, float fRadiusModel, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CSelectMenu3D();		//�f�X�g���N�^
	static CSelectMenu3D* Create(int nNumSelect, D3DXVECTOR3 posCenter, float fRadius, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void) {}	//�`�揈��

	virtual void BeginChangeSelect(void);	//�I���̕ύX���J�n���ꂽ�Ƃ��̏���
	virtual void EndChangeSelect(void) {}	//�I���̕ύX���I�������Ƃ��̏���

	CObjectModelUI* GetModelUI(int nIdx);	//UI���f���I�u�W�F�N�g�̃|�C���^���擾
	void SetSpeedRotModel(float fSpeed) { m_fSpeedRotModel = fSpeed; }	//���f���ړ����̉�]���x�̐ݒ�

private:
	void CreateModelUI(void);	//���f���̐���
	void MoveModel(void);	//���f���ړ�

	const D3DXVECTOR3 m_posCenter;	//���j���[�̒����̈ʒu
	const float m_fRadiusModel;	//�I�u�W�F�N�g��z�u���锼�a

	CObjectModelUI** m_ppObjModelUIArray;	//UI���f���I�u�W�F�N�g�̔z��̃_�u���|�C���^
	CModel::MODELTYPE m_typeModel;	//�������郂�f���̎��
	bool m_bMoveModel;		//���f���ړ������ǂ���
	float m_fRotModelY;		//���f���̔z�u�ʒu�����߂�p�x
	float m_fSpeedRotModel;	//���f���ړ����̉�]���x
	float m_fDistCamera;	//���j���[�p�̃J�����̋���
	float m_fRotCameraY;	//���j���[�p�̃J������Y�p�x
	float m_fHeightCamera;	//���j���[�p�̃J�����̈ʒu�̍���
};

#endif // !_SELECT_MENU_3D_H_