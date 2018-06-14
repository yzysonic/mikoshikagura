#pragma once
#include "Core\Core.h"
#include "Core\Singleton.h"

enum FadeType
{
	FADE_IN,
	FADE_OUT,
};

class FadeScreen : public Object, public Singleton<FadeScreen>
{
public:
	static constexpr float DefaultFadeTime = 0.3f;

public:
	FadeScreen(void);
	void Update(void) override;
	void SetActive(bool value) override;
	static void Fade(FadeType type, Color color = Color::none, float interval = DefaultFadeTime, Action callback = nullptr);
	static void FadeIn(Color color, float interval = DefaultFadeTime, Action callback = nullptr);
	static void FadeOut(Color color, float interval = DefaultFadeTime, Action callback = nullptr);
	static void SetTexture(const char* texture_name);
	static void SetTexture(Texture* texture);
	static bool Finished(void);

private:
	void(FadeScreen::*pUpdate)(void);
	float fadeTime;
	float timer;
	float targetOpacity;
	float oldOpacity;
	Color fadeColor;
	RectPolygon2D* polygon;
	Event callback;

	enum AnimeSet
	{
		Run,
		Stop,
	};

	AnimeSet state;

};