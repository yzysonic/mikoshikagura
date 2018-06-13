#include "FadeScreen.h"

FadeScreen::FadeScreen(void)
{
	this->polygon = AddComponent<RectPolygon2D>("none", Layer::TOP, "default");
	this->polygon->SetSize((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY);
	this->polygon->SetOpacity(0.0f);
	this->callback = nullptr;

	state = Stop;

	SetActive(false);
}

void FadeScreen::Update()
{

	switch (state)
	{
	case Run:
		if (timer < fadeTime + 0.1f)
		{
			float t = this->polygon->GetOpacity();
			this->polygon->SetOpacity(Lerpf(oldOpacity, targetOpacity, timer / fadeTime));
		}
		else
		{
			state = Stop;
		}
		timer += Time::DeltaTime();
		break;

	case Stop:
		SetActive(false);
		if (callback)
		{
			auto call = callback;
			call();
			callback = nullptr;
		}
		break;
	}

}

void FadeScreen::SetActive(bool value)
{
	if (this->isActive == value)
		return;

	this->isActive = value;
}

void FadeScreen::Fade(FadeType type, Color color, float interval, Action callback)
{
	switch (type)
	{
	case FADE_IN:
		m_pInstance->targetOpacity = 0.0f;
		m_pInstance->GetComponent<RectPolygon2D>()->SetColor(color);
		m_pInstance->GetComponent<RectPolygon2D>()->SetOpacity(1.0f);
		break;
	case FADE_OUT:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->GetComponent<RectPolygon2D>()->SetColor(color);
		m_pInstance->GetComponent<RectPolygon2D>()->SetOpacity(0.0f);
		break;
	}

	m_pInstance->fadeTime = interval;

	if (m_pInstance->fadeTime > 0)
	{
		m_pInstance->timer = 0;
		m_pInstance->oldOpacity = m_pInstance->GetComponent<RectPolygon2D>()->GetOpacity();
		m_pInstance->SetActive(true);
		m_pInstance->state = Run;
		m_pInstance->callback = callback;
	}
	else
	{
		m_pInstance->GetComponent<RectPolygon2D>()->SetOpacity(m_pInstance->targetOpacity);
		m_pInstance->state = Stop;
		if(callback) callback();
	}
		
}

void FadeScreen::FadeIn(Color color, float interval, Action callback)
{
	Fade(FADE_IN, color, interval, callback);
}

void FadeScreen::FadeOut(Color color, float interval, Action callback)
{
	Fade(FADE_OUT, color, interval, callback);
}

void FadeScreen::SetTexture(const char * texture_name)
{
	m_pInstance->polygon->pTexture = Texture::Get(texture_name);
}

void FadeScreen::SetTexture(Texture * texture)
{
	m_pInstance->polygon->pTexture = texture;
}

bool FadeScreen::Finished(void)
{
	if (m_pInstance->state == Stop)
		return true;
	else
		return false;
}
