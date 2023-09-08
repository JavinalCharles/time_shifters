#pragma once

#include <chrono>
#include <cmath>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <stdexcept>
#include <vector>

#include <BA/Components/AI/AI.hpp>
#include <BA/Entities/Entity.hpp>
#include <BA/Systems/EntityManager.hpp>
#include <BA/Systems/VelocityWithCollisionSystem.hpp>
#include <BA/Utilities/Line.hpp>
#include <BA/Utilities/Rect.hpp>
#include <BA/Utilities/Vector2.hpp>
#include <BA/Types.hpp>
#include <BA/Tools/DebugHelper.hpp>

#include "TS/Entities/Bandit.hpp"
#include "TS/Utility/Define.hpp"

namespace TS {

enum BanditState : ba::IDtype {
	IDLE,
	PATROL,
	AWARE,
	ENGAGED,
	FLEEING,
	RECOVERING,
	DEAD,
}; // enum BanditState

class BaseBanditAI : public ba::AI {
public:
	explicit BaseBanditAI(ba::Entity* owner);

	virtual void behave(float deltaTime) override;
private:
	void changeState(BanditState newState);
	bool checkIfDestinationValid(const ba::FloatRect& destRect);
	void setDestinationsQueue(const ba::Vector2f& finalDestination);
	void setPatrolPath(float distance);

	static int getRandomNumber();
private:
	BanditState m_currentState = IDLE;
	float m_timeSinceStateChange = 0.f;
	std::queue<ba::Vector2f> m_destinationsQueue{};

	static std::mt19937_64 		s_randomEngine;
	static std::binomial_distribution<int> s_distributor;
	static std::bernoulli_distribution s_2Dcompass;
}; // public BaseBanditAI

} // namespace TS
