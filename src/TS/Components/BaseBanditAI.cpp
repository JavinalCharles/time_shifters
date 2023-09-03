#include <TS/Components/BaseBanditAI.hpp>
#include <iostream>

namespace TS {

using ba::IDtype;
using ba::Vector2f;
using ba::FloatRect;

std::mt19937_64 BaseBanditAI::s_randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
std::binomial_distribution<int> BaseBanditAI::s_distributor(1024, 0.67);
std::bernoulli_distribution BaseBanditAI::s_2Dcompass(0.5);

BaseBanditAI::BaseBanditAI(ba::Entity* owner)
	: ba::AI(owner)
{
	if (dynamic_cast<TS::Bandit*>(owner) == nullptr) {
		throw std::invalid_argument("Error: This Component requires a Character subclass as an owner");
	}
}

void BaseBanditAI::behave(float deltaTime) {
	// ba::debug << "BaseBanditAI::behave()" << std::endl;
	TS::Bandit* owner = dynamic_cast<TS::Bandit*>(this->getOwner());
	auto animation = getOwner()->getComponent<ba::Animation>();
	auto velocity = getOwner()->getComponent<ba::Velocity>();
	auto collider = getOwner()->getCollider();
	auto cs = getOwner()->CONTEXT->entities->getSystem<ba::VelocityWithCollisionSystem>();

	if(animation == nullptr || velocity == nullptr || collider == nullptr || cs == nullptr) {
		throw std::runtime_error("Error: V.I. lacks necessary resource to compute for behavior.");
	}

	const IDtype CURRENT_ANIMATION = animation->getCurrentAnimationID();

	m_timeSinceStateChange += deltaTime;
	if (owner->getHP() == 0) {
		changeState(DEAD);
	}

	const ba::Vector2f& pos = owner->getPosition();

	switch (m_currentState) {
		case IDLE:
			// TODO Check if an enemy hero within sight
			if (m_timeSinceStateChange >= 6.f) {
				setDestinationsQueue(Vector2f(
					pos.x + static_cast<float>(getRandomNumber()),
					pos.y
				));
				// if (!m_destinationsQueue.empty()) {
				changeState(PATROL);
				// }
			}
			break;
		case PATROL:
			// TDDO Check if an enemy is within sight
			if (m_destinationsQueue.empty()) {
				velocity->resetVelocity();
				if (owner->hasBeenInCombat()) {
					animation->set(CURRENT_ANIMATION%2==0?BANDIT_COMBAT_IDLE_RIGHT: BANDIT_COMBAT_IDLE);
					changeState(AWARE);
				}
				else {
					animation->set(CURRENT_ANIMATION%2==0?BANDIT_IDLE_RIGHT: BANDIT_IDLE);
					changeState(IDLE);
				}
			}
			else if (std::abs(pos.x - m_destinationsQueue.front().x) <= 24.f) {
				m_destinationsQueue.pop();
			}
			else {
				const bool GOING_LEFT =  m_destinationsQueue.front().x < pos.x;
				
				ba::debug << ba::IntLine(
					owner->getPosition(), m_destinationsQueue.back()
				);

				animation->set(GOING_LEFT ? BANDIT_RUN: BANDIT_RUN_RIGHT);
				velocity->setX(GOING_LEFT ? -NORMAL_SPEED: NORMAL_SPEED);
			}

			break;
		case AWARE:
			// TODO: Check if enemy is on sight.
			if (m_timeSinceStateChange >= 3.f) {
				setDestinationsQueue(Vector2f(
					pos.x + static_cast<float>(getRandomNumber()),
					pos.y
				));
				// if (!m_destinationsQueue.empty()) {
				changeState(PATROL);
				// }
			}
			break;
		case ENGAGED:
			break;
		case FLEEING:
			break;
		case RECOVERING:
			if (CURRENT_ANIMATION < BANDIT_RECOVER) {
				changeState(AWARE);
			}
			break;
		case DEAD:
			if (CURRENT_ANIMATION < BANDIT_DYING) {
				velocity->resetVelocity();
				animation->set(CURRENT_ANIMATION%2==0? BANDIT_DYING_RIGHT: BANDIT_DYING);
			}
			else if (BANDIT_DEATH <= CURRENT_ANIMATION && CURRENT_ANIMATION <= BANDIT_DEATH_RIGHT) {
				if (owner->getHP() > 0) {
					animation->set(CURRENT_ANIMATION%2==0?BANDIT_RECOVER_RIGHT: BANDIT_RECOVER);
					changeState(RECOVERING);
				}
				else if (m_timeSinceStateChange >= 8.f) {
					owner->startCountdown();
				}
			}
			break;
	}

}

void BaseBanditAI::changeState(BanditState newState) {
	m_currentState = newState;
	m_timeSinceStateChange = 0.f;
}

bool BaseBanditAI::checkIfDestinationValid(const ba::FloatRect& destRect) {
	auto cs = getOwner()->CONTEXT->entities->getSystem<ba::VelocityWithCollisionSystem>();

	auto tiles = cs->searchStatic(destRect);
	auto belowTiles = cs->searchStatic(ba::FloatRect(
		destRect.l,
		destRect.t + destRect.h,
		destRect.w,
		1.f
	));

	return (tiles.empty() && !belowTiles.empty());
}

void BaseBanditAI::setDestinationsQueue(const Vector2f& finalDestination) {
	auto collider = getOwner()->getCollider();
	const Vector2f& pos = getOwner()->getPosition();

	const FloatRect bounds = collider->getGlobalBounds();

	float movement = pos.x > finalDestination.x? -bounds.w : bounds.w;

	FloatRect currentRect = bounds;
	currentRect.l += movement;

	do {
		if (checkIfDestinationValid(currentRect)) {
			m_destinationsQueue.push(Vector2f(
				currentRect.l + (currentRect.w * 0.5f),
				pos.y
			));
			ba::debug << currentRect;
			currentRect.l += movement;
		}
		else {
			break;
		}
	}while(ba::distance(finalDestination, m_destinationsQueue.back()) > bounds.w);

}

int BaseBanditAI::getRandomNumber() {
	int number = s_distributor(s_randomEngine);
	bool compass = s_2Dcompass(s_randomEngine);

	int res = compass ? -number : number;
	return res;
}

} // namespace TS