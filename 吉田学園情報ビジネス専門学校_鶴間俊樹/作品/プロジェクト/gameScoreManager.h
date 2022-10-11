//=============================================================================
//
// �Q�[���X�R�A�}�l�[�W���[���� [gameScoreManager.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAMESCORE_MANAGER_H_
#define _GAMESCORE_MANAGER_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;
class CGauge2D;
class CScore;

//*****************************************************************************
// �Q�[���X�R�A�}�l�[�W���[�N���X
//*****************************************************************************
class CGameScoreManager : public CObject
{
public:
	CGameScoreManager();	//�f�t�H���g�R���X�g���N�^
	~CGameScoreManager();	//�f�X�g���N�^
	static CGameScoreManager* Create(D3DXVECTOR3 pos, float fSize);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��

	void AddKillCombo(void);		//�|�����R���{�̉��Z
	float GetScoreRate(void) { return m_fScoreRate; }	//�X�R�A�̔{�����擾

private:
	int m_nKillCombo;	//�|�����R���{��
	int m_nTimeCombo;	//�R���{�̎c�莞��
	int m_nTimeComboMax;	//�R���{�̎c�莞�Ԃ̍ő�
	float m_fScoreRate;		//�X�R�A�̔{��

	CObject2D* m_pScoreRate;		//�X�R�A�{���̃e�L�X�g
	CObject2D* m_pTextCombo;		//�R���{�̃e�L�X�g
	CGauge2D* m_pComboTimeGauge;	//�R���{�̎c�莞�Ԃ̃Q�[�W
	CScore* m_pComboScore;			//�R���{��
};

#endif // !_GAMESCORE_MANAGER_H_