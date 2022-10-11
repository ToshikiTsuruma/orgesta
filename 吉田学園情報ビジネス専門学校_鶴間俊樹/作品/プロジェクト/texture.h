//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_FILE_PATH (128)	//テクスチャのファイルのパスの長さの最大

//*****************************************************************************
// テクスチャクラス
//*****************************************************************************
class CTexture
{
public:
	enum class TEXTURE_TYPE {
		NONE = 0,

		//UIのテクスチャ
		BG_TITLE,
		TEXT_TITLENAME,
		TEXT_GAMESTART,
		TEXT_TUTORIAL,
		TEXT_CREDIT,
		QUIT_GAME,
		TUTORIAL,
		CREDIT,
		SELECT_ICON,

		MENU_BG,
		RATE_BG,
		TIMER_FRAME,
		SCORE_FRAME,
		ICON_NONE,
		ICON_APPLE,
		ICON_HP,
		SILHOUETTE_TREE,
		MOON,
		ICON_INTERACT_KEYBOARD,
		ICON_INTERACT_GAMEPAD,
		ICON_NIX_SIGN,

		GAUGE_HORIZONTAL,
		GAUGE_VERTICAL,
		HP_GAUGE_FRAME,
		GROW_GAUGE_FRAME,
		TEXT_GROWUP,
		ARROW_LEFT,
		ARROW_RIGHT,
		PAUSE_BG,

		UPDATE_HISCORE,
		TEXT_GAMECLEAR,
		TEXT_GAMEOVER,
		TEXT_QUIT,
		TEXT_RETRY,
		TEXT_CONTINUE,
		TEXT_TITLE,
		TEXT_ENERGY,
		TEXT_COMBO,
		TEXT_X1,
		TEXT_X2,
		TEXT_X3,
		TEXT_X5,
		TEXT_SCORE,
		TEXT_HISCORE,

		//モデルのテクスチャ

		//エフェクトのテクスチャ
		EFFECT_EXPLOSION,		// 爆発エフェクト
		EFFECT_DAMAGE_PLAYER,	// プレイヤー攻撃のダメージ
		EFFECT_DAMAGE_ENEMY,	// 敵の攻撃のダメージ
		EFFECT_DAMAGE_BULLET,	// 弾のダメージエフェクト
		EFFECT_DEATH,			// 死亡エフェクト
		EFFECT_HEAL_APPLE,		// 林檎による回復エフェクト
		EFFECT_DRAIN_APPLE,		// 林檎によるHP吸収エフェクト
		EFFECT_REPULSIVEWAVE_APPLE,	// 林檎による斥力波生成エフェクト
		EFFECT_FIRE_BOLL,		// 火の玉
		EFFECT_FIRE,			// 火
		EFFECT_THUNDERBOLT,		// 落雷
		EFFECT_THUNDER,			// 雷
		EFFECT_WIND,			// 風の渦
		EFFECT_PARTICLE,		// パーティクル
		EFFECT_SHOCKWAVE,		// 衝撃波
		EFFECT_REPULSIVEWAVE,	// 斥力波

		//ナンバーのテクスチャ
		NUMBER_001,
		NUMBER_002,
		NUMBER_003,
		NUMBER_004,
		NUMBER_005,
		NUMBER_006,

		ENUM_MAX
	};

	CTexture();		//デフォルトコンストラクタ
	~CTexture();	//デストラクタ

	static HRESULT Load(void);	//テクスチャデータの読み込み
	static void Unload(void);	//テクスチャデータの解放
	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE_TYPE type);	//テクスチャの取得
	static char* GetPathName(TEXTURE_TYPE type);				//テクスチャのパスの文字列の取得

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX];	//テクスチャへのポインタ
	static char m_asFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_TEXTURE_FILE_PATH];	// ファイルのパス
};

#endif // !_TEXTURE_H_