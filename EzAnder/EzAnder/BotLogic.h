#pragma once
class BotLogic
{
	BotLogic(void);
	static BotLogic* _instance;

	// logic states
	typedef enum BotLogicState {
		BLSIdling,
		BLSMoving,
		BLSAttacking,
		BLSSearching,
		BLSChangingZone,
		BLSCount
	};

	BotLogicState _currentState;

public:
	static BotLogic& instance();
	~BotLogic(void);

	// control
	void pause();
	void stop();

	void runMain();
};

