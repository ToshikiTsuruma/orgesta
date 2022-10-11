//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �^�C�}�[�N���X
//*****************************************************************************
class CTimer : public CScore
{
public:
	CTimer();	//�f�t�H���g�R���X�g���N�^
	CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CTimer();	//�f�X�g���N�^
	static CTimer* Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��
	int GetCountTimer(void) { return m_nCntTime; }	//�^�C�}�[�̃J�E���g���擾
	void SetStop(bool bStop) { m_bStop = bStop; }	//�^�C�}�[�̒�~��Ԃ�ݒ�

private:
	bool m_bStop;		//�^�C�}�[����~�����ǂ���
	int m_nCntTime;		//�^�C�}�[�̎c�莞�Ԃ�����܂ł̃J�E���g
};

#endif // !_TIMER_H_