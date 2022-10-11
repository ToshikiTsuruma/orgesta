//=============================================================================
//
// �������o���� [thunderEmitter.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _THUNDER_EMITTER_H_
#define _THUNDER_EMITTER_H_

#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �������o�N���X
//*****************************************************************************
class CThunderEmitter : public CObject
{
public:
	CThunderEmitter();	//�f�t�H���g�R���X�g���N�^
	CThunderEmitter(int nNumEmitDest);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CThunderEmitter();	//�f�X�g���N�^
	static CThunderEmitter* CreateStraight(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, D3DXVECTOR3 vecStraight, float fDist);	//���i�̗�����������
	static CThunderEmitter* CreateRound(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, float rotY, float fRadius);	//�~�`�̗�����������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��

private:
	D3DXVECTOR3* m_pPosEmitArray;	//���o�ڕW�����̕��o�ʒu�̓��I�z��̃|�C���^
	const int m_nNumEmitDest;	//���o����ڕW��
	int m_nNumEmit;		//���o������
	int m_nCntEmit;		//���o�̃J�E���^�[
	int m_nSpanEmit;	//���o�̃X�p��
};

#endif // !_THUNDER_EMITTER_H_