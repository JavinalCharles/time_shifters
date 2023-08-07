#pragma once

#include <chrono>
#include <functional>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <BA/Entities/Entity.hpp>
#include <BA/Components/AI/ProgrammedAI.hpp>
#include <BA/Components/Animation.hpp>
#include <BA/Components/Camera.hpp>
#include <BA/Components/Colliders/BoxCollider.hpp>
#include <BA/Components/KeyboardControl.hpp>
#include <BA/Components/MouseControl.hpp>
#include <BA/Components/SoundEmitter.hpp>
#include <BA/Components/Sprite.hpp>
#include <BA/Components/Velocity.hpp>
#include <BA/Utilities/Rect.hpp>
#include <BA/Utilities/Vector2.hpp>
#include <BA/Types.hpp>

#include "TS/Components/ModifiedBoxCollider.hpp"
#include "TS/Entities/Character.hpp"
#include "TS/Utility/Define.hpp"

using ba::IDtype;

namespace TS {

class Bandit : public Character {
public:
	Bandit(ba::SharedContext* context);

	enum State : IDtype {
		IDLE,
		AWARE,
		COMBAT,
		DEAD
	};

private:
	void loadResources();
	void populateAnimations();
	void programAIBehavior();

private:
	State	m_currentState = State::IDLE;
	float 	m_targetX = 0.f;
	float	m_timeSinceLastPrompt = 0.f;			


private:
	static bool s_resourcesLoaded;
	static const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> 	s_texturesToLoad;
	static const ba::IntRect 		s_RECT;
	static const ba::IntRect		s_RECT_RIGHT;

	static std::unordered_map<IDtype, std::pair<float, std::vector<IDtype>>> s_TR;

	static std::mt19937							s_engine;
	static std::uniform_int_distribution<int> 	s_distributor;
	static std::uniform_int_distribution<int> 	s_floater;
	
}; // class Bandit

} // namespace TS