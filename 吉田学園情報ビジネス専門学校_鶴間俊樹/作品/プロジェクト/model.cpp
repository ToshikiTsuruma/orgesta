//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"
#define MAX_LOAD_TEXT (128)	//一度にファイルから読み込むテキストのバイト数
#define OUTLINE_SIZE (1.0f)	//輪郭のサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CModel::ModelData CModel::m_aModelData[(int)MODELTYPE::ENUM_MAX] = {};
char CModel::m_asFilePath[(int)MODELTYPE::ENUM_MAX][MAX_MODEL_FILE_PATH] = {};
D3DXMATERIAL CModel::m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};
CTexture::TEXTURE_TYPE CModel::m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CModel::CModel()
{
	m_modelType = (MODELTYPE) 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pParent = nullptr;
	m_nIdxParent = -1;
	memset(&m_aMat, 0, sizeof(m_aMat));
	memset(&m_aChangeDiffuse, 0, sizeof(m_aChangeDiffuse));
	m_colGlow = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_bOutline = false;
	m_colOutline = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pCloneMesh = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// モデルの生成処理
//=============================================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine) {
	CModel* pModel;
	pModel = new CModel;
	if (pModel == nullptr) return nullptr;

	pModel->m_modelType = type;
	pModel->m_pos = pos;
	pModel->m_rot = rot;
	pModel->m_pParent = pParent;
	pModel->m_bOutline = bOutLine;
	//マテリアルの設定
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		//デフォルトのマテリアルを取得
		pModel->m_aMat[nCntMat] = m_aMatDefault[(int)type][nCntMat];
	}

	pModel->Init();

	return pModel;
}

//=============================================================================
//モデルデータの読み込み
//=============================================================================
HRESULT CModel::Load(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	//-------------------------------------
	//ファイルの読み込み
	//-------------------------------------
	FILE *pFile;		//ファイルへのポインタ
	char sModelText[MAX_LOAD_TEXT];//一行ずつ読み込むファイルのテキスト
	char *pModelText;	//ファイルのテキストを分割した文字列
	int nIdxType = 0;	//次に読み込むモデルの種類

	pFile = fopen(TEXT_FILE_NAME_MODEL, "r");

	if (pFile != nullptr) {
		while (fgets(sModelText, MAX_LOAD_TEXT, pFile) != nullptr) //一行ごとに文字列を取得
		{
			pModelText = strtok(sModelText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
			if (pModelText != nullptr) {
				//コメント
				if (strstr(pModelText, "//") != nullptr) {
					continue;
				}
				//ディレクトリ名のコピー
				if (strlen(pModelText) < MAX_MODEL_FILE_PATH) {
					strcpy(m_asFilePath[nIdxType], pModelText);
				}
				nIdxType++;	//インデックスを加算
				if (nIdxType >= (int)MODELTYPE::ENUM_MAX) 	break;	//読み込むモデルの最大数を超えたら終了	
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
		return S_OK;
	}

	//-------------------------------------
	//モデルの読み込み
	//-------------------------------------
	for (int nCntModel = 0; nCntModel < (int)MODELTYPE::ENUM_MAX; nCntModel++) {
		//すでにモデルが生成されていた場合
		//マテリアルバッファの解放
		if (m_aModelData[nCntModel].pBuffMat != nullptr) {
			m_aModelData[nCntModel].pBuffMat->Release();
			m_aModelData[nCntModel].pBuffMat = nullptr;
		}
		//メッシュの解放
		if (m_aModelData[nCntModel].pMesh != nullptr) {
			m_aModelData[nCntModel].pMesh->Release();
			m_aModelData[nCntModel].pMesh = nullptr;
		}

		//モデルの生成
		D3DXLoadMeshFromX(m_asFilePath[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,		//ここまで固定
			&m_aModelData[nCntModel].pBuffMat,
			NULL,
			&m_aModelData[nCntModel].nNumMat,
			&m_aModelData[nCntModel].pMesh);

		//テクスチャタイプの設定	
		if (m_aModelData[nCntModel].pBuffMat == nullptr || m_aModelData[nCntModel].nNumMat <= 0) continue;	//マテリアルバッファがNULLか、マテリアル数が0以下なら以降の処理を飛ばす

		D3DXMATERIAL *pMat;	//マテリアルへのポインタ
		pMat = (D3DXMATERIAL*)m_aModelData[nCntModel].pBuffMat->GetBufferPointer();	//マテリアル情報に対するポインタを取得

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[nCntModel].nNumMat; nCntMat++) {
			//デフォルトのマテリアルを取得
			m_aMatDefault[nCntModel][nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != nullptr) {
				//テクスチャクラスのパスと比較
				for (int nCntTex = 1; nCntTex < (int)CTexture::TEXTURE_TYPE::ENUM_MAX; nCntTex++) {
					//文字列が一致でテクスチャの番号を決める
					if (strcmp(pMat[nCntMat].pTextureFilename, CTexture::GetPathName((CTexture::TEXTURE_TYPE)nCntTex)) == 0) {
						m_aTexType[nCntModel][nCntMat] = (CTexture::TEXTURE_TYPE)nCntTex;
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//モデルデータの解放
//=============================================================================
void CModel::Unload(void) {
	for (int nCnt = 0; nCnt < (int)MODELTYPE::ENUM_MAX; nCnt++) {
		//マテリアルバッファの解放
		if (m_aModelData[nCnt].pBuffMat != nullptr) {
			m_aModelData[nCnt].pBuffMat->Release();
			m_aModelData[nCnt].pBuffMat = nullptr;
		}
		//メッシュの解放
		if (m_aModelData[nCnt].pMesh != nullptr) {
			m_aModelData[nCnt].pMesh->Release();
			m_aModelData[nCnt].pMesh = nullptr;
		}
	}
}

//=============================================================================
// モデルデータの取得
//=============================================================================
CModel::ModelData CModel::GetModelData(MODELTYPE type) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return ModelData();
	return m_aModelData[(int)type];
}

//=============================================================================
// モデルのパスの取得
//=============================================================================
char* CModel::GetPathName(MODELTYPE type) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return nullptr;
	return m_asFilePath[(int)type];
}

//=============================================================================
// モデルのデフォルトの色を取得
//=============================================================================
D3DXCOLOR CModel::GetDefaultColor(MODELTYPE type, int nIdx) {
	if (type < MODELTYPE(0) || type >= MODELTYPE::ENUM_MAX) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	return m_aMatDefault[(int)type][nIdx].MatD3D.Diffuse;
}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CModel::Init(void) {
	//輪郭がある場合
	if (m_bOutline) {
		//複製メッシュを生成
		CreateCloneMesh();
		//複製メッシュを膨張させる
		ExpansionCloneMesh();
	}

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CModel::Uninit(void) {
	//複製メッシュの破棄
	if (m_pCloneMesh != nullptr) {
		m_pCloneMesh->Release();
		m_pCloneMesh = nullptr;
	}
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CModel::Update(void) {
	//色の変更の更新
	UpdateColorChange();
}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CModel::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//モデルの情報が正しく確保されていない場合描画終了
	if (m_aModelData[(int)m_modelType].pBuffMat == nullptr || m_aModelData[(int)m_modelType].pMesh == nullptr || m_aModelData[(int)m_modelType].nNumMat <= 0) return;

	//----------------------------------------------------
	//マトリックスの設定
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			//親のマトリックス

	//各パーツの親のマトリックスを設定
	if (m_pParent != nullptr) {
		mtxParent = m_pParent->GetMtxWorld();	//親のモデルからマトリックスを取得
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//最後にセットしたワールドマトリックスを親のマトリックスとして設定
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//算出したワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//シェーダにマトリックスを設定
	pRenderer->SetEffectMatrixWorld(m_mtxWorld);

	//----------------------------------------------------
	//モデルの描画
	//----------------------------------------------------
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//輪郭の描画	Z値テクスチャには書き込まない
	if (m_bOutline && m_pCloneMesh != nullptr && !pRenderer->GetDrawZTex()) {
		//裏面
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		D3DMATERIAL9 matOutline;	//輪郭用マテリアル
		memset(&matOutline, 0, sizeof(D3DMATERIAL9));
		//マテリアル情報の設定
		matOutline.Diffuse = m_colOutline;

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//マテリアルの設定
			pRenderer->SetEffectMaterialDiffuse(matOutline.Diffuse);
			pRenderer->SetEffectMaterialEmissive(matOutline.Emissive);
			pRenderer->SetEffectMaterialSpecular(matOutline.Specular);
			pRenderer->SetEffectMaterialPower(matOutline.Power);

			pRenderer->BeginPassEffect(PASS_3D);
			//輪郭の描画	
			m_pCloneMesh->DrawSubset(nCntMat);

			pRenderer->EndPassEffect();
		}

		// 表面
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//モデルの描画
	if (m_aModelData[(int)m_modelType].pMesh != nullptr) {
		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//テクスチャの取得
			LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(m_aTexType[(int)m_modelType][nCntMat]);
			//テクスチャの設定
			pRenderer->SetEffectTexture(pTexture);
			//マテリアルの設定
			pRenderer->SetEffectMaterialDiffuse(m_aMat[nCntMat].MatD3D.Diffuse);
			pRenderer->SetEffectMaterialEmissive(m_aMat[nCntMat].MatD3D.Emissive);
			pRenderer->SetEffectMaterialSpecular(m_aMat[nCntMat].MatD3D.Specular);
			pRenderer->SetEffectMaterialPower(m_aMat[nCntMat].MatD3D.Power);
			//輪郭の発光色の設定
			pRenderer->SetEffectColorGlow(m_colGlow);

			DWORD dwPassFlag = PASS_3D | PASS_LIGHT;
			//テクスチャがある場合フラグを追加
			if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;

			//パスの開始
			pRenderer->BeginPassEffect(dwPassFlag);

			//モデル（パーツ）の描画	
			m_aModelData[(int)m_modelType].pMesh->DrawSubset(nCntMat);

			pRenderer->EndPassEffect();
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// モデルの位置情報の設定
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

//=============================================================================
// モデルの位置情報の取得
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) { return m_pos; }

//=============================================================================
// モデルの角度の設定
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// モデルの角度の取得
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) { return m_rot; }

//=============================================================================
// ワールドマトリックスの更新
//=============================================================================
void CModel::UpdateMtxWorld(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//----------------------------------------------------
	//マトリックスの設定
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			//親のマトリックス

	//各パーツの親のマトリックスを設定
	if (m_pParent != nullptr) {
		mtxParent = m_pParent->GetMtxWorld();	//親のモデルからマトリックスを取得
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//最後にセットしたワールドマトリックスを親のマトリックスとして設定
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//算出したワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
//ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void) {
	return m_mtxWorld;
}

//=============================================================================
// ワールドマトリックスのポインタの取得
//=============================================================================
D3DXMATRIX* CModel::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// モデルのタイプの設定
//=============================================================================
void CModel::SetModelType(MODELTYPE type) {
	//変更がない場合終了
	if (m_modelType == type) return;

	//種類の変更
	m_modelType = type;

	//モデルの種類が無効の場合
	if ((int)m_modelType < 0 || (int)m_modelType >= (int)CModel::MODELTYPE::ENUM_MAX) {
		m_modelType = (MODELTYPE)0;
	}

	//輪郭がある場合
	if (m_pCloneMesh != nullptr) {
		//複製メッシュの破棄
		m_pCloneMesh->Release();
		m_pCloneMesh = nullptr;
		//複製メッシュを作り直す
		CreateCloneMesh();
		//複製メッシュを膨張
		ExpansionCloneMesh();
	}
}

//=============================================================================
// モデルのタイプの取得
//=============================================================================
CModel::MODELTYPE CModel::GetModelType(void) {
	return m_modelType;
}

//=============================================================================
// マテリアルの色の設定
//=============================================================================
void CModel::SetMaterialDiffuse(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Diffuse.r = col.r;
	m_aMat[nIdx].MatD3D.Diffuse.g = col.g;
	m_aMat[nIdx].MatD3D.Diffuse.b = col.b;
	m_aMat[nIdx].MatD3D.Diffuse.a = col.a;
}

//=============================================================================
// マテリアルの色の取得
//=============================================================================
D3DXCOLOR CModel::GetMaterialDiffuse(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	return m_aMat[nIdx].MatD3D.Diffuse;
}

//=============================================================================
// マテリアルのアルファ値を設定
//=============================================================================
void CModel::SetMaterialDiffuseAlpha(float fAlpha, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Diffuse.a = fAlpha;
}

//=============================================================================
// すべてのマテリアルのアルファ値を設定
//=============================================================================
void CModel::SetMaterialDiffuseAlphaAll(float fAlpha) {
	for (int nCnt = 0; nCnt < MAX_MATERIAL; nCnt++)
	{
		m_aMat[nCnt].MatD3D.Diffuse.a = fAlpha;
	}
}

//=============================================================================
// マテリアルのアルファ値を取得
//=============================================================================
float CModel::GetMaterialDiffuseAlpha(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return 0.0f;

	return m_aMat[nIdx].MatD3D.Diffuse.a;
}

//=============================================================================
// マテリアルのスペキュラー色の設定
//=============================================================================
void CModel::SetMaterialSpecular(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Specular.r = col.r;
	m_aMat[nIdx].MatD3D.Specular.g = col.g;
	m_aMat[nIdx].MatD3D.Specular.b = col.b;
	m_aMat[nIdx].MatD3D.Specular.a = col.a;
}

//=============================================================================
// マテリアルの発光色の設定
//=============================================================================
void CModel::SetMaterialEmissive(D3DXCOLOR col, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Emissive.r = col.r;
	m_aMat[nIdx].MatD3D.Emissive.g = col.g;
	m_aMat[nIdx].MatD3D.Emissive.b = col.b;
	m_aMat[nIdx].MatD3D.Emissive.a = col.a;
}

//=============================================================================
//マテリアルの反射の質感の設定
//=============================================================================
void CModel::SetMaterialPower(float fPower, int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_MATERIAL) return;

	m_aMat[nIdx].MatD3D.Power = fPower;
}

//=============================================================================
// マテリアルの色の変更の開始
//=============================================================================
void CModel::StartChangeMaterialDiffuse(int nIdxMat, D3DXCOLOR colDest, int nTimeFin) {
	//現在のマテリアルの色と目標の色との差分から毎フレーム加算される色を計算
	m_aChangeDiffuse[nIdxMat].colAdd = (colDest - GetMaterialDiffuse(nIdxMat)) / (float)nTimeFin;
	//初期化
	m_aChangeDiffuse[nIdxMat].nTimeFinish = nTimeFin;
	m_aChangeDiffuse[nIdxMat].nCnt = 0;
}

//=============================================================================
// 輪郭の発光色の設定
//=============================================================================
void CModel::SetColorGlow(D3DXCOLOR col) {
	m_colGlow = col;
}

//=============================================================================
// モデルの複製メッシュの生成
//=============================================================================
void CModel::CreateCloneMesh(void) {
	//すでに複製メッシュがある場合か、モデルのデータが存在しない場合終了
	if (m_pCloneMesh != nullptr || m_aModelData[(int)m_modelType].pMesh == nullptr) return;

	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//メッシュの複製
	m_aModelData[(int)m_modelType].pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX_3D, pDevice, &m_pCloneMesh);

	m_bOutline = true;
}

//=============================================================================
// モデルの複製メッシュを膨張させる
//=============================================================================
void CModel::ExpansionCloneMesh(void) {
	//複製メッシュがない場合終了
	if (m_pCloneMesh == nullptr) return;

	//頂点数の取得
	int nNumVtx = 0;
	nNumVtx = m_pCloneMesh->GetNumVertices();

	VERTEX_3D *pVtx;	//FVFを変更して複製したのでそれに合わせた構造体の頂点バッファのポインタ

	//頂点バッファのロック
	m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
		//全頂点を法線方向へ膨張
		pVtx[nCnt].pos = D3DXVECTOR3(pVtx[nCnt].pos.x + pVtx[nCnt].nor.x * OUTLINE_SIZE,
			pVtx[nCnt].pos.y + pVtx[nCnt].nor.y * OUTLINE_SIZE,
			pVtx[nCnt].pos.z + pVtx[nCnt].nor.z * OUTLINE_SIZE);
	}

	//頂点バッファのアンロック
	m_pCloneMesh->UnlockVertexBuffer();
}

//=============================================================================
// 輪郭の色の設定
//=============================================================================
void CModel::SetColorOutline(D3DXCOLOR col) {
	m_colOutline = col;
}

//=============================================================================
// 輪郭の表示設定
//=============================================================================
void CModel::SetDrawOutline(bool bDraw) {
	m_bOutline = bDraw;

	//表示
	if (bDraw) {
		//すでに複製メッシュがある場合
		if (m_pCloneMesh != nullptr) return;

		//複製メッシュを生成
		CreateCloneMesh();
		//複製メッシュを膨張
		ExpansionCloneMesh();
	}
	//非表示
	else {
		//複製メッシュの破棄
		if (m_pCloneMesh != nullptr) {
			m_pCloneMesh->Release();
			m_pCloneMesh = nullptr;
		}
	}
}

//=============================================================================
// 色の変更の更新
//=============================================================================
void CModel::UpdateColorChange(void) {
	for (int nIdxMat = 0; nIdxMat < MAX_MATERIAL; nIdxMat++)
	{
		//カウンターが終了時間を過ぎていた場合ループを飛ばす
		if (m_aChangeDiffuse[nIdxMat].nCnt >= m_aChangeDiffuse[nIdxMat].nTimeFinish) continue;

		//カウンターを加算
		m_aChangeDiffuse[nIdxMat].nCnt++;

		//モデルの現在の色を取得
		const D3DXCOLOR colModel = GetMaterialDiffuse(nIdxMat);
		//色の設定
		SetMaterialDiffuse(colModel + m_aChangeDiffuse[nIdxMat].colAdd, nIdxMat);
	}
}