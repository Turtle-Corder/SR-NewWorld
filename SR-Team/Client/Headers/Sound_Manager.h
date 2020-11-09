#pragma once
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

BEGIN(Client)
USING(Engine)

class CSoundManager
{

private:
	CSoundManager();
	~CSoundManager();

public:
	enum CHANNELID
	{
		BGM,
		PLAYER,

		//--------------------------------------------------
		// 유닛
		//--------------------------------------------------
		UNIT_START,
		UNIT01 = UNIT_START,
		UNIT02, UNIT03, UNIT04, UNIT05, UNIT06, UNIT07, UNIT08,
		UNIT_END = UNIT08,

		//--------------------------------------------------
		// 몬스터
		//--------------------------------------------------
		MONSTER_START,
		MONSTER01 = MONSTER_START,
		MONSTER02, MONSTER03, MONSTER04, MONSTER05, MONSTER06, MONSTER07, MONSTER08,
		MONSTER_END = MONSTER08,


		//--------------------------------------------------
		// 이펙트
		//--------------------------------------------------
		EFFECT_START,
		EFFECT01 = EFFECT_START,
		EFFECT02, EFFECT03, EFFECT04, EFFECT05, EFFECT06, EFFECT07, EFFECT08,
		EFFECT_END = EFFECT08,


		//--------------------------------------------------
		// UI
		//--------------------------------------------------
		UI_START,
		UI01 = UI_START,
		UI02, UI03,
		UI_END = UI03,

		MAXCHANNEL
	};


public:
	static CSoundManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSoundManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}


public:
	void Initialize();
	void Release();

public:
	// Player, UI 외에는 각 함수 사용 권장
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);

	void PlayUnit(TCHAR* pSoundKey);
	void PlayMonster(TCHAR* pSoundKey);
	void PlayEffect(TCHAR* pSoundKey);
	void PlayUI(TCHAR* pSoundKey);

	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

private:
	static CSoundManager*	m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL*	m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM*	m_pSystem;

	// 현재 각 채널
	BYTE			m_byCurUnit;
	BYTE			m_byCurMonster;
	BYTE			m_byCurEffect;
	BYTE			m_byCurUI;
};

END

#endif // !__SOUNDMANAGER_H__
