#pragma once

// �Q�[���t���[�����[�N
class Game
{
public:
	// �Q�[��������
	static void Init(void);	

	// �Q�[�����[�v
	static void Run(void);	

	// �Q�[���I������
	static void Uninit(void);

	// �Q�[���I���m�F
	static bool End(void);

	// �Q�[���I��
	static void Stop(void);

private:
	static bool run_game;
};