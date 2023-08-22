#pragma once

#include <chrono>
#include <functional>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <BA/Entities/Entity.hpp>
#include <BA/Components/Animation.hpp>
#include <BA/Components/Camera.hpp>
#include <BA/Components/Colliders/BoxCollider.hpp>
#include <BA/Components/KeyboardControl.hpp>
#include <BA/Components/MouseControl.hpp>
#include <BA/Components/SoundEmitter.hpp>
#include <BA/Components/Sprite.hpp>
#include <BA/Components/Timer.hpp>
#include <BA/Components/Velocity.hpp>
#include <BA/Utilities/Rect.hpp>
#include <BA/Utilities/Vector2.hpp>
#include <BA/Types.hpp>

#include "TS/Components/BaseBanditAI.hpp"
#include "TS/Components/ModifiedBoxCollider.hpp"
#include "TS/Entities/Character.hpp"
#include "TS/Utility/Define.hpp"

using ba::IDtype;

namespace TS {

enum BanditAnimations : IDtype {
	NONE = 0ul,
	BANDIT_IDLE,
	BANDIT_IDLE_RIGHT,
	BANDIT_RUN,
	BANDIT_RUN_RIGHT,
	BANDIT_COMBAT_IDLE,
	BANDIT_COMBAT_IDLE_RIGHT,
	BANDIT_ATTACK,
	BANDIT_ATTACK_RIGHT,
	BANDIT_HURT,
	BANDIT_HURT_RIGHT,
	BANDIT_RECOVER,
	BANDIT_RECOVER_RIGHT,
	BANDIT_DYING,
	BANDIT_DYING_RIGHT,
	BANDIT_DEATH,
	BANDIT_DEATH_RIGHT
};

class BaseBanditAI;

class Bandit : public Character {
public:
	friend class BaseBanditAI;

	Bandit(ba::SharedContext* context);

	virtual void damage(unsigned dmg) override;

	
private:
	void loadResources();
	void populateAnimations();

	void startCountdown();
private:
	// State	m_currentState = State::IDLE;
	float 	m_targetX = 0.f;
	float	m_timeSinceLastPrompt = 0.f;			

	bool	m_haveFinalCountdown = false;

private:
	static bool s_resourcesLoaded;
	static const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> 	s_texturesToLoad;
	static const ba::IntRect 		s_RECT;
	static const ba::IntRect		s_RECT_RIGHT;

	static std::unordered_map<IDtype, std::pair<float, std::vector<IDtype>>> s_TR;

	// static std::mt19937							s_engine;
	// static std::uniform_int_distribution<int> 	s_distributor;
	// static std::uniform_int_distribution<int> 	s_floater;
	
}; // class Bandit

} // namespace TS