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
		// ����
		//--------------------------------------------------
		UNIT_START,
		UNIT01 = UNIT_START,
		UNIT02, UNIT03, UNIT04, UNIT05, UNIT06, UNIT07, UNIT08,
		UNIT_END = UNIT08,

		//--------------------------------------------------
		// ����
		//--------------------------------------------------
		MONSTER_START,
		MONSTER01 = MONSTER_START,
		MONSTER02, MONSTER03, MONSTER04, MONSTER05, MONSTER06, MONSTER07, MONSTER08,
		MONSTER_END = MONSTER08,


		//--------------------------------------------------
		// ����Ʈ
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
	// Player, UI �ܿ��� �� �Լ� ��� ����
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

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL*	m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM*	m_pSystem;

	// ���� �� ä��
	BYTE			m_byCurUnit;
	BYTE			m_byCurMonster;
	BYTE			m_byCurEffect;
	BYTE			m_byCurUI;
};

END

#endif // !__SOUNDMANAGER_H__
