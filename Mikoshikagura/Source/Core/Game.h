#pragma once
#include <functional>

// �Q�[���t���[�����[�N
class Game
{
public:
	static std::function<void()> EndProcess;

	// �Q�[��������
	static void Init(void);	

	// �Q�[�����[�v
	static void Run(void);	

	// �Q�[���I������
	static void Uninit(void);

	// �Q�[���I���m�F
	static bool EndPrompt(void);

	// �Q�[���I��
	static void Stop(void);

private:
	static bool run_game;
};