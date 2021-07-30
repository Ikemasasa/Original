#include "ProductionValue.h"

#include "lib/Math.h"

#include "CameraManager.h"
#include "Singleton.h"

ProductionValue::ProductionValue()
{
	mValueData.clear();
}

ProductionValue::~ProductionValue()
{
	mFontValue.Release();
	mValueData.clear();
}

void ProductionValue::Add(const int& value, const Vector3& pos, const Vector3& rgb)
{
	ValueData data;
	data.value = value;
	data.pos = pos;
	data.scale = Vector2::ONE; // スケールは1固定
	data.center = Vector2(mFontValue.GetWidth(data.value) / 2.0f, 0.0f); // x中央 y一番上
	data.color = Vector4(rgb.x, rgb.y, rgb.z, 0.0f); // 最初見えないスタート
	data.alphaSign = 1; // ALPHAは最初 0 だからプラス

	mValueData.emplace_back(data);
}

void ProductionValue::Initialize()
{
	mFontValue.Initialize(FONT_SIZE, FONT_WEIGHT);
}

void ProductionValue::Update()
{
	const float POS_Y_MOVE = 0.01f; // 文字のY座標を動かすよう
	const float ALPHA_ADD = 0.05f;	// ALPHAの増減値
	const float SIGN_CHANGE_VALUE = 1.8f; // アルファが折り返す値

	for (auto it = mValueData.begin(); it != mValueData.end();)
	{
		auto& data = *it;

		// 徐々に上に移動
		data.pos.y += POS_Y_MOVE;
		Vector2 scrPos = data.pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
		
		// alphaの値を増減させる
		data.color.w = data.color.w + ALPHA_ADD * data.alphaSign;

		mFontValue.RenderSet(data.value, scrPos, data.center, data.scale, data.color);

		// アルファがSIGN_CHANGE_VALUE以上になれば、符号を反転させる(次からalpha値を下げる)
		if (data.color.w >= SIGN_CHANGE_VALUE)
		{
			data.alphaSign = -data.alphaSign; 
		}

		// アルファが0より小さくなったらValueData削除
		if (data.color.w <= 0.0f)
		{
			it = mValueData.erase(it);
		}
		else ++it;
	}
}

void ProductionValue::Render()
{
	mFontValue.Render();
}
