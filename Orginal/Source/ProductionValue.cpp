#include "ProductionValue.h"

#include "lib/Math.h"

#include "CameraManager.h"
#include "GameManager.h"
#include "Singleton.h"

ProductionValue::ProductionValue()
{
	mValueData.clear();
}

ProductionValue::~ProductionValue()
{
	mFont.Release();
	mValueData.clear();
}

void ProductionValue::Add(const int& value, const Vector3& pos, const Vector3& rgb)
{
	if (value <= 0) return;

	ValueData data;
	data.value = value;
	data.pos = pos;
	data.color = Vector4(rgb, 0.0f); // 最初見えないスタート
	data.alphaSign = 1; // ALPHAは最初 0 だからプラス
	data.timer = 0.0f;
	data.sinfactor = SIN_FACTOR_INIT;
	mValueData.emplace_back(data);
}

void ProductionValue::DelayAdd(const int& value, const Vector3& pos, const Vector3& rgb, const int delayFrame)
{
	DelayData data;
	data.value = value;
	data.pos = pos;
	data.rgb = rgb;
	data.delayFrame = delayFrame;
	data.elapsedFrame = 0;
	mDelayData.emplace_back(data);
}

void ProductionValue::Initialize()
{
	mFont.Initialize(FONT_SIZE, FONT_WEIGHT);
}

void ProductionValue::Update()
{
	for (auto it = mValueData.begin(); it != mValueData.end();)
	{
		auto& data = *it;

		// 座標を設定
		Vector2 scrPos = data.pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
		scrPos.y -= sinf(data.sinfactor) * 30.0f;
		
		// alphaの値を設定
		data.color.w += ALPHA_ADD * data.alphaSign;
		data.color.w = Math::Clamp01(data.color.w);

		// sinfactorの値を設定
		data.sinfactor += SIN_FACTOR_ADD;
		data.sinfactor = Math::Min(Define::PI, data.sinfactor);

		// フォントセット
		mFont.RenderSetValue(data.value, scrPos, Vector2(0.5f, 0.0f), data.color);

		// タイマーがDISP_TIME以上になれば、符号を反転させる(次からalpha値を下げる)
		if (data.timer >= DISP_TIME)
		{
			data.alphaSign = -data.alphaSign;

		}
		data.timer += GameManager::elapsedTime * data.alphaSign;


		// アルファが0より小さくなったらValueData削除
		if (data.color.w <= 0.0f)
		{
			it = mValueData.erase(it);
		}
		else ++it;
	}

	// 遅延データ処理
	for (auto it = mDelayData.begin(); it != mDelayData.end();)
	{
		auto& data = *it;

		if (data.elapsedFrame < data.delayFrame)
		{
			// 経過フレーム加算
			++data.elapsedFrame;
			++it; // イテレータも
		}
		else
		{
			// 通常データに追加
			Add(data.value, data.pos, data.rgb);

			// 遅延データから削除
			it = mDelayData.erase(it);
		}
	}
}

void ProductionValue::Render()
{
	mFont.Render();
}
