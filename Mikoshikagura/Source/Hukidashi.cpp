#include "Hukidashi.h"

#define READCOUNT 2
#define POPCOUNT 15

Hukidashi::Hukidashi(void)
{
	this->AddComponent<RectPolygon2D>("hukidashi")->SetSize(1080, 360);
	this->AddComponent<Text>()->LoadFont("Data/Font/APJapanesefont.ttf");
	hukidashi_pos = Vector2(0, 175);
	text_pos = Vector2(200, 80);
	text_size = Vector2(880, 210);
	this->transform.position = Vector3(hukidashi_pos.x, hukidashi_pos.y, 0.0f);
	area = &this->GetComponent<Text>()->area;
	*area = { (int)text_pos.x, (int)text_pos.y, (int)(text_pos.x + text_size.x), (int)(text_pos.y + text_size.y) };
	pop_count = 0;
	read_count = 0;
	str_head = 0;
	this->state = none;
	this->SetActive(false);
}

void Hukidashi::Update(void)
{
#ifdef _DEBUG
	if (ImGui::SliderFloat2("hukidashi_pos", (float *)&hukidashi_pos, -500, 500))
	{
		this->transform.position = Vector3(hukidashi_pos.x, hukidashi_pos.y, 0.0f);
	}
	if (ImGui::SliderFloat2("text_pos", (float *)&text_pos, 0, 500)
		|| ImGui::SliderFloat2("text_size", (float *)&text_size, 100, 1000))
	{
		*area = { (int)text_pos.x, (int)text_pos.y, (int)(text_pos.x + text_size.x), (int)(text_pos.y + text_size.y) };
	}
#endif

	switch (this->state)
	{
	case none:
		break;
	case popping:
		pop_count++;
		this->GetComponent<RectPolygon2D>()->SetSize(1080 * pop_count / POPCOUNT, 360 * pop_count / POPCOUNT);
		if (pop_count == POPCOUNT)
		{
			this->state = popped;
		}
		break;
	case popped:
		read_count++;
		if (read_count == READCOUNT)
		{
			read_count = 0;
			if (str_head < message.length())
			{
				this->GetComponent<Text>()->AddText(message[str_head]);
				str_head++;
			}
		}
		break;
	case unpopping:
		pop_count--;
		this->GetComponent<RectPolygon2D>()->SetSize(1080 * pop_count / POPCOUNT, 360 * pop_count / POPCOUNT);
		if (pop_count == 0)
		{
			this->state = none;
			this->SetActive(false);
		}
		break;
	}
}

void Hukidashi::Pop(std::string message)
{
	if (this->state != popped)
	{
		this->message = message;
		this->state = popping;
		this->SetActive(true);
		str_head = 0;
	}
}

void Hukidashi::Unpop(void)
{
	this->message.clear();
	this->GetComponent<Text>()->ClearText();
	this->state = unpopping;
}
