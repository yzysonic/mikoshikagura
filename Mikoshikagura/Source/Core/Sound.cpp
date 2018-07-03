#include "sound.h"
#include "Common.h"
#include "Game.h"
#include "Math.h"

#include <tchar.h>
#include <mmsystem.h>

// ���C�u�������C���N���[�g
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

float Sound::global_volume = 1.0f;
IDirectSound8 * Sound::pDirectSound = NULL; // �T�E���h�C���^�[�t�F�[�X

void Sound::SetGlobalVolume(float value)
{
	global_volume = clamp(value, 0.0f, 1.0f);
	for (auto & pair : name_map)
	{
		auto sound = pair.second.get();
		sound->SetVolume(sound->volume);
	}
}

float Sound::GetGlobalVolume(void)
{
	return global_volume;
}

Sound * Sound::InternalLoad(std::string name, std::string ext)
{
	auto fullpath	= GetFullPath(name + ext);
	auto sound		= new Sound(name);

	auto LoadProcess = [&] {

		// MMIO = �}���`���f�B�A���o�́A�̗��B
		LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// �ȃf�[�^�̑����o�b�t�@
		DSBUFFERDESC buff;						// �o�b�t�@�ݒ�\����

		HMMIO hMmio = NULL;						// MMIO�n���h��
		MMIOINFO mmioInfo;						// �}���`���f�B�A�f�[�^�\����

		MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
		MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
		DWORD size;									// �f�[�^�T�C�Y�i�[�p
		WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

		LPVOID pBlock;				// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
		DWORD  dwSize;				// �Z�J���_���o�b�t�@�̃T�C�Y


		// 1.�n���h�������ƂɃt�@�C�����J��
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen((LPSTR)fullpath.c_str(), &mmioInfo, MMIO_READ);
		if (!hMmio)
		{
			return false;
		}

		// 2.�t�@�C����͇@ RIFF�`�����N����
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// �����L�[���[�h
		mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// �L�[���[�h�����ƂɌ���

		if (mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
		{
			mmioClose(hMmio, 0);
			return false;
		}

		// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
		mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���

		if (mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
		{
			mmioClose(hMmio, 0);
			return false;
		}

		size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// �����������Ƃɓǂݍ���

		if (size != formatChunk.cksize) 		// �������ǂݍ��߂Ȃ�������ُ�I��
		{
			mmioClose(hMmio, 0);
			return false;
		}

		mmioAscend(hMmio, &formatChunk, 0);

		// 4.�t�@�C����͇B �f�[�^�`�����N����
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// �����L�[���[�h
		mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���

		if (mmRes != MMSYSERR_NOERROR) 		// ������Ȃ�������ُ�I��
		{
			mmioClose(hMmio, 0);
			return false;
		}

		// 5.�f�[�^�ǂݍ���
		char *pData = new char[dataChunk.cksize];					// �K�v�ȑ傫���̗̈���m�ۂ���
		size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// �f�[�^��ǂݍ���

		if (size != dataChunk.cksize)  		// �������ǂݍ��߂Ȃ�������ُ�I��
		{
			delete[] pData;
			return false;
		}

		// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
		ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// �܂�������
		buff.dwSize = sizeof(DSBUFFERDESC);			// ��������e��ݒ�
		buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
		buff.dwBufferBytes = size;
		buff.lpwfxFormat = &pcm;

		// �����o�b�t�@�����
		if (FAILED(pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
			return false;

		// �T�E���h�o�b�t�@�����o��
		if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&sound->pBuffer)))
			return false;

		// ���o�����瑍���o�b�t�@���̂Ă�
		pBaseBuffer->Release();

		// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
		// �܂��͏������݂ł���悤�o�b�t�@�����b�N
		if (FAILED(sound->pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
			return false;

		memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
		delete[] pData;					// ���̓ǂݍ��ݗ̈������

		// �o�b�t�@���b�N����
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
	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &pDirectSound, NULL)))
		return E_FAIL;

	// �������x���ݒ� 
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
		pBuffer->Stop();	// �Ӗ��I�ɂ�Pause�ɂȂ�B
}

void Sound::SetVolume(float value)
{
	volume = clamp(value, 0.0f, 1.0f);
	auto final_volume = global_volume * volume;
	pBuffer->SetVolume(final_volume > 0.0f ? (long)(2000.0f*log10f(final_volume)) : MinVolume);
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
