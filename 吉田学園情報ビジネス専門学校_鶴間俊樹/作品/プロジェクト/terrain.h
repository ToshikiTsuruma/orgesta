//=============================================================================
//
// �n�`���� [terrain.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �n�`�N���X
//*****************************************************************************
class CTerrain : public CObjectModel
{
public:
	CTerrain(CModel::MODELTYPE typeModel);	//�f�t�H���g�R���X�g���N�^
	~CTerrain();	//�f�X�g���N�^
	static CTerrain* Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static bool Collision(D3DXVECTOR3& posCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd);	//�Փ˔���
	static bool Collision(D3DXVECTOR3& posCollision);	//������x�N�g���̏Փ˔���

private:

};

#endif // !_TERRAIN_H_