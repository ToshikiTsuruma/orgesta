//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define MAX_OBJECT (1024)
#define MAX_OBJECT_NAME (32)
//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CObject
{
public:
	//オブジェクトの種類 (当たり判定や全消去で必要な場合のもの)
	typedef enum {
		OBJTYPE_NONE		= 0b00000000,	//無し
		OBJTYPE_TERRAIN		= 0b00000001,	//地形
		OBJTYPE_WALL		= 0b00000010,	//壁	
		OBJTYPE_PLAYER		= 0b00000100,	//プレイヤー
		OBJTYPE_ENEMY		= 0b00001000,	//敵
		OBJTYPE_ITEM		= 0b00010000,	//アイテム
		OBJTYPE_APPLETREE	= 0b00100000,	//林檎の木
		OBJTYPE_APPLE		= 0b01000000,	//林檎
		OBJTYPE_SCAPEGOAT	= 0b10000000,	//生贄
		OBJTYPE_SEEDLING   = 0b100000000,	//苗木
	} OBJTYPE;

	//オブジェクトのフラグ
	typedef enum {
		OBJFLAG_NONE = 0b00000000,	//無し
		OBJFLAG_ENABLECOLLISION = 0b00000001,	//当たり判定が有効である
	} OBJFLAG;

	//更新順（更新停止でも必要な場合は定義）
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//更新順が影響しないもの
		PLAYER,			//プレイヤー
		ENEMY,			//敵
		ENUM_MAX
	};	

	enum class DRAW_PRIORITY {
		NONE = 0,	
		BG,			//背景
		DEFAULT,	//デフォルト
		CHARA,		//プレイヤー等のキャラクターに関連するもの
		CLEAR,		//透過するオブジェクト
		EFFECT,		//エフェクト
		BILLBOARD_UI,//UIのビルボード
		UI_BG,		//UIの背景
		UI,			//UI
		FRONT,		//最前
		ENUM_MAX
	};	//描画順

	//ダメージの種類
	enum class DAMAGE_TYPE {
		NONE = 0,
		PLAYER_PUNCH,	//プレイヤーのパンチ
		ENEMY_PUNCH,	//敵の攻撃
		BULLET,			//弾
		SHOCKWAVE,		//衝撃波
		THUNDERBOLT,	//落雷
		FIRE,			//炎
		EXPLOSION,		//爆発
		ENUM_MAX
	};

	CObject();			//デフォルトコンストラクタ
	virtual ~CObject();	//デストラクタ
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	static void ReleasePtrAll(CObject* pReleaseObj);	//すべてのオブジェクトにReleasePtrを行う
	virtual void ReleasePtr(CObject* pReleaseObj) {}	//このオブジェクトが保持している、破棄される予定のオブジェクトのポインタをnullにする

	virtual void SetPos(D3DXVECTOR3 pos) {}	//位置座標の設定
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//位置座標の取得
	virtual D3DXVECTOR3 GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//最後の位置座標の取得
	virtual D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//角度の取得
	virtual void SetMove(D3DXVECTOR3 move) {}	//移動量の設定
	virtual D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//移動量の取得
	virtual float GetRadius(void) { return 0.0f; }		//当たり判定の半径の取得
						
	virtual int GetNumCollisionParts(void) { return 1; }	//当たり判定があるパーツの数の取得(基本は１つとする)
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//当たり判定の情報の取得
	virtual void UpdateMtxWorldAll(void) {}			//オブジェクトの全モデルのワールドマトリックスの更新
	virtual void Heal(int nHeal) {}	//回復
	virtual void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {}	//ダメージ
	virtual void Dead(void) {}						//死亡処理
	virtual bool GetItem(int nTypeItem) { return false; }	//アイテム取得時の処理
	virtual void SetRatioWidth(float fRatio) {}		//幅の割合の設定
	virtual void SetRatioHeight(float fRatio) {}	//高さの割合の設定
	virtual void SetColor(D3DXCOLOR col) {};	//色の設定

	static void ReleaseAll(void);	//全オブジェクトの解放処理
	static void UpdateAll(void);	//全オブジェクトの更新処理
	static void DrawAll(void);		//全オブジェクトの描画処理
	static CObject* GetObjectTopAll(void);	//全オブジェクトのリストのTopを取得
	static CObject* GetObjectNextAll(CObject* pObject);	//全オブジェクトのリストのNextを取得
	static void ReleaseObjtype(int nObjtype);	//指定したタイプのオブジェクトの解放処理
	static void DeadObjtype(int nObjtype);		//指定したタイプの死亡処理

	static void SetUpdatePauseLevel(int nLevel) { m_nUpdatePauseLevel = nLevel; }	//更新可能なポーズレベル設定
	static int AddUpdatePauseLevel(void) { return ++m_nUpdatePauseLevel; }			//更新可能なポーズレベル加算
	static int SubUpdatePauseLevel(void) { return --m_nUpdatePauseLevel; }			//更新可能なポーズレベル減算
	void SetPauseLevel(int nLevel) { m_nPauseLevel = nLevel; }	//ポーズレベルの設定
	int GetPauseLevel(void) { return m_nPauseLevel; }			//ポーズレベルの取得

	static CObject* GetNearObject(const D3DXVECTOR3 pos, int nObjType, int nObjFlag, D3DXVECTOR3* pPosNearObj, float* pfDistNearObj, D3DXVECTOR3* pVecNearObj);	//ある位置からオブジェクトへの最も近い距離を求める

	bool GetDeath(void);	//死亡フラグの取得
	void SetObjType(OBJTYPE objType);	//オブジェクトの種類の設定
	OBJTYPE GetObjType(void);			//オブジェクトの種類の取得
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//更新の優先順位の設定
	void SetDrawPriority(DRAW_PRIORITY priority);		//描画の優先順位の設定

	void SetTexType(CTexture::TEXTURE_TYPE type);	//テクスチャの種類の設定
	CTexture::TEXTURE_TYPE GetTexType(void);		//テクスチャの種類の取得
	void SetEnableCollision(bool bEnable);	//当たり判定の有効状態を設定
	bool GetEnableCollision(void);			//当たり判定の有効状態を取得
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }	//描画状態の設定
	bool GetDraw(void) { return m_bDraw; }			//描画状態の取得

protected:
	void Release(void);	//オブジェクトの解放処理

private:
	bool m_bDeath;		//死亡フラグ
	OBJTYPE m_objType;	//オブジェクトの種類
	UPDATE_PRIORITY m_updatePriority;	//更新の優先順位
	DRAW_PRIORITY m_drawPriority;		//描画の優先順位

	CTexture::TEXTURE_TYPE m_texType;	//テクスチャのタイプ
	bool m_bEnableCollision;	//当たり判定が有効
	bool m_bDraw;	//描画するかどうか

	static int m_nUpdatePauseLevel;	//更新可能なポーズレベル
	int m_nPauseLevel;	//ポーズのレベル

	//全オブジェクトのリスト
	static CObject* m_pTopAll;	//先頭のオブジェクトのポインタ
	static CObject* m_pCurAll;	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevAll;		//前のオブジェクトへのポインタ
	CObject* m_pNextAll;		//次のオブジェクトへのポインタ
	
	//更新順のリスト
	static CObject* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CObject* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevUpdate;		//前のオブジェクトへのポインタ
	CObject* m_pNextUpdate;		//次のオブジェクトへのポインタ

	//描画順のリスト
	static CObject* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CObject* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevDraw;		//前のオブジェクトへのポインタ
	CObject* m_pNextDraw;		//次のオブジェクトへのポインタ
};

#endif // !_OBJECT_H_