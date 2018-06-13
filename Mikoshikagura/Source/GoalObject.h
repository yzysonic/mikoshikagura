#pragma once
#include "Core/Core.h"

template<class T>
class GoalObject : public Object
{
	static_assert(std::is_base_of<Scene, T>::value, "Not a subclass of Component.");

public:
	GoalObject(void);
	~GoalObject(void);
	void Update(void) override;
	void OnCollisionEnter(Object *object) override;

private:
	enum state
	{
		standby,
		fading,
		finish
	}goal_state;
};

template<class T>
inline GoalObject<T>::GoalObject(void)
{
	goal_state = standby;
}

template<class T>
inline GoalObject<T>::~GoalObject(void)
{
	if (goal_state == finish)
	{
		goal_state = standby;
		GameManager::GetInstance()->SetScene(new T);
	}
}

template<class T>
inline void GoalObject<T>::Update(void)
{
	switch (goal_state)
	{
	case fading:
		if (FadeScreen::Finished())
			goal_state = finish;
		break;
	case finish:
		Destroy();
		break;
	case standby:
	default:
		break;
	}
}

template<class T>
inline void GoalObject<T>::OnCollisionEnter(Object * object)
{
	if (object->type == ObjectType::Player && goal_state != fading)
	{
		FadeScreen::FadeOut(Color::white, 1.0f);
		goal_state = fading;
	}
}