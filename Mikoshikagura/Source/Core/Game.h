#pragma once
#include <functional>

// ゲームフレームワーク
class Game
{
public:
	static std::function<void()> EndProcess;

	// ゲーム初期化
	static void Init(void);	

	// ゲームループ
	static void Run(void);	

	// ゲーム終了処理
	static void Uninit(void);

	// ゲーム終了確認
	static bool EndPrompt(void);

	// ゲーム終了
	static void Stop(void);

private:
	static bool run_game;
};