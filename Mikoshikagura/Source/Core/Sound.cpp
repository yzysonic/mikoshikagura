#include "sound.h"
#include "Common.h"
#include "Game.h"
#include "Math.h"

#include <tchar.h>
#include <mmsystem.h>

// ライブラリをインクルート
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

IDirectSound8 * Sound::pDirectSound = NULL; // サウンドインターフェース

Sound * Sound::InternalLoad(std::string name, std::string ext)
{
	auto fullpath	= GetFullPath(name + ext);
	auto sound		= new Sound(name);

	auto LoadProcess = [&] {

		// MMIO = マルチメディア入出力、の略。
		LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// 曲データの総合バッファ
		DSBUFFERDESC buff;						// バッファ設定構造体

		HMMIO hMmio = NULL;						// MMIOハンドル
		MMIOINFO mmioInfo;						// マルチメディアデータ構造体

		MMRESULT mmRes;								// MM処理結果格納変数
		MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
		DWORD size;									// データサイズ格納用
		WAVEFORMATEX pcm;							// 曲データフォーマット構造体

		LPVOID pBlock;				// セカンダリバッファのデータ書き込み先アドレス
		DWORD  dwSize;				// セカンダリバッファのサイズ


		// 1.ハンドルをもとにファイルを開く
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen((LPSTR)fullpath.c_str(), &mmioInfo, MMIO_READ);
		if (!hMmio)
		{
		}

		// 2.ファイル解析① RIFFチャンク検索
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// 検索キーワード
		mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// キーワードをもとに検索

		if (mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
		{
			mmioClose(hMmio, 0);
			return false;
		}

		// 3.ファイル解析② フォーマットチャンク検索
		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
		mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索

		if (mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
		{
			mmioClose(hMmio, 0);
			return false;
		}

		size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// 検索情報をもとに読み込み

		if (size != formatChunk.cksize) 		// 正しく読み込めなかったら異常終了
		{
			mmioClose(hMmio, 0);
			return false;
		}

		mmioAscend(hMmio, &formatChunk, 0);

		// 4.ファイル解析③ データチャンク検索
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// 検索キーワード
		mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索

		if (mmRes != MMSYSERR_NOERROR) 		// 見つからなかったら異常終了
		{
			mmioClose(hMmio, 0);
			return false;
		}

		// 5.データ読み込み
		char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保して
		size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む

		if (size != dataChunk.cksize)  		// 正しく読み込めなかったら異常終了
		{
			delete[] pData;
			return false;
		}

		// 6.曲を読み込む「セカンダリバッファ」を用意
		ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// まず初期化
		buff.dwSize = sizeof(DSBUFFERDESC);			// そこから各種設定
		buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
		buff.dwBufferBytes = size;
		buff.lpwfxFormat = &pcm;

		// 総合バッファを作る
		if (FAILED(pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
			return false;

		// サウンドバッファを取り出す
		if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&sound->pBuffer)))
			return false;

		// 取り出したら総合バッファを捨てる
		pBaseBuffer->Release();

		// 7.作られたセカンダリバッファにデータを転送
		// まずは書き込みできるようバッファをロック
		if (FAILED(sound->pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
			return false;

		memcpy(pBlock, pData, dwSize);	// データ転送
		delete[] pData;					// 元の読み込み領域を消す

		// バッファロック解除
		sound->pBuffer->Unlock(pBlock, dwSize, 0, 0);

		return true;
	};

	while (!LoadProcess())
	{
		switch (ShowErrorMessage(name.c_str(), ""))
		{
		case IDABORT:
			Game::Stop();
		case IDIGNORE:
			delete sound;
			return nullptr;
		}
	}
	return sound;
}

HRESULT Sound::Init(HWND hWnd)
{
	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &pDirectSound, NULL)))
		return E_FAIL;

	// 協調レベル設定 
	if (FAILED(pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	return S_OK;
}

void Sound::Uninit(void)
{
	SafeRelease(pDirectSound);
}

Sound::Sound(std::string name) : Resource<Sound>(name)
{
	pBuffer = NULL;
	loop = false;
	volume = 1.0f;
}

Sound::~Sound(void)
{
	SafeRelease(pBuffer);
}

void Sound::Play(void)
{
	pBuffer->SetCurrentPosition(0);
	pBuffer->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
}

void Sound::Stop(void)
{
	if(IsPlaying())
		pBuffer->Stop();	// 意味的にはPauseになる。
}

void Sound::SetVolume(float value)
{
	volume = clamp(value, 0.0f, 1.0f);
	pBuffer->SetVolume(volume > 0.0f ? (long)(2000.0f*log10f(volume)) : MinVolume);
}

float Sound::GetVolume(void)
{
	return volume;
}

bool Sound::IsPlaying(void)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)
	{
		return true;
	}
	return false;
}

DWORD Sound::GetCurrentPosition(void)
{
	DWORD playCursor;
	pBuffer->GetCurrentPosition(&playCursor, NULL);
	return playCursor;
}
