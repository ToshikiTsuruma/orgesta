//=============================================================================
//
// 衝撃波放出処理 [shockWaveEmitter.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SHOCKWAVE_EMITTER_H_
#define _SHOCKWAVE_EMITTER_H_

#include "main.h"
#include "shockWave.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 衝撃波放出クラス
//*****************************************************************************
class CShockWaveEmitter : public CObject
{
public:
	CShockWaveEmitter();	//デフォルトコンストラクタ
	~CShockWaveEmitter();	//デストラクタ
	static CShockWaveEmitter* Create(int nNumCreate, int nSpanCreate, D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理
	void SetColAddCreate(D3DXCOLOR col) { m_colAddCreate = col; }	//生成数ごとに足す色の設定

private:
	void CreateShockWave(void);	//衝撃波を生成

	int m_nNumCreate;		//生成する数
	int m_nNumCreateEnd;	//生成完了した数
	int m_nSpanCreate;		//生成する間隔
	int m_nCntCreate;		//生成のカウンター
	D3DXCOLOR m_colAddCreate;	//生成数ごとに足す色

	//生成する衝撃波の情報
	D3DXVECTOR3 m_posCreate;		//生成位置
	float m_fRadiusTop;		//生成時の上の半径
	float m_fRadiusBottom;	//生成時の下の半径
	float m_fAddRadius;		//生成後に半径が増加する量
	float m_fHeight;		//生成する高さ		
	float m_fAddHeight;		//生成するごとに追加されていく高さ
	int m_nLife;			//衝撃波のライフ
	float m_fRotSpeed;		//回転速度
};

#endif // !_SHOCKWAVE_EMITTER_H_