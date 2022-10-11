//=============================================================================
//
// �Ռ��g���o���� [shockWaveEmitter.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SHOCKWAVE_EMITTER_H_
#define _SHOCKWAVE_EMITTER_H_

#include "main.h"
#include "shockWave.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �Ռ��g���o�N���X
//*****************************************************************************
class CShockWaveEmitter : public CObject
{
public:
	CShockWaveEmitter();	//�f�t�H���g�R���X�g���N�^
	~CShockWaveEmitter();	//�f�X�g���N�^
	static CShockWaveEmitter* Create(int nNumCreate, int nSpanCreate, D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��
	void SetColAddCreate(D3DXCOLOR col) { m_colAddCreate = col; }	//���������Ƃɑ����F�̐ݒ�

private:
	void CreateShockWave(void);	//�Ռ��g�𐶐�

	int m_nNumCreate;		//�������鐔
	int m_nNumCreateEnd;	//��������������
	int m_nSpanCreate;		//��������Ԋu
	int m_nCntCreate;		//�����̃J�E���^�[
	D3DXCOLOR m_colAddCreate;	//���������Ƃɑ����F

	//��������Ռ��g�̏��
	D3DXVECTOR3 m_posCreate;		//�����ʒu
	float m_fRadiusTop;		//�������̏�̔��a
	float m_fRadiusBottom;	//�������̉��̔��a
	float m_fAddRadius;		//������ɔ��a�����������
	float m_fHeight;		//�������鍂��		
	float m_fAddHeight;		//�������邲�Ƃɒǉ�����Ă�������
	int m_nLife;			//�Ռ��g�̃��C�t
	float m_fRotSpeed;		//��]���x
};

#endif // !_SHOCKWAVE_EMITTER_H_