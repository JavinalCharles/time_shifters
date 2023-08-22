#include <TS/Components/BaseBanditAI.hpp>
#include <iostream>

namespace TS {

using ba::IDtype;
using ba::Vector2f;
using ba::FloatRect;

std::mt19937 BaseBanditAI::s_randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> BaseBanditAI::s_distributor{1, 512};

BaseBanditAI::BaseBanditAI(ba::Entity* owner)
	: ba::AI(owner)
{
	if (dynamic_cast<TS::Bandit*>(owner) == nullptr) {
		throw std::invalid_argument("Error: This Component requires a Character subclass as an owner");
	}
}

void BaseBanditAI::behave(float deltaTime) {
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
			if (m_timeSinceStateChange >= 8.f) {
				setDestinationsQueue(Vector2f(
					pos.x + getRandomNumber(),
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
				changeState(IDLE);
			}
			else if (std::abs(pos.x - m_destinationsQueue.front().x) <= 32.f) {
				m_destinationsQueue.pop(); // remove current destination. Go to next destination.
			}
			else {
				const bool GOING_LEFT = pos.x > m_destinationsQueue.front().x;
				// std::clog << "pos.x " << pos.x << " dest.x " << m_destinationsQueue.front().x << " LEFT? " << GOING_LEFT << std::endl;

				animation->set(GOING_LEFT ? BANDIT_RUN: BANDIT_RUN_RIGHT);
				velocity->setX(GOING_LEFT ? -NORMAL_SPEED : NORMAL_SPEED);
			}

			break;
		case AWARE:
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
		destRect.t + destRect.h + 1.f,
		destRect.w,
		1.f
	));
	// if (!tiles.empty()) {
	// 	return false;
	// }

	// for (auto& collider: belowTiles) {
	// 	const IDtype collisionLayer = collider->getLayer();
	// 	if (collisionLayer <= TILE_3) {
	// 		return true;
	// 	}
	// }
	return (tiles.empty() && !belowTiles.empty());
}

void BaseBanditAI::setDestinationsQueue(const Vector2f& finalDestination) {
	
	auto collider = getOwner()->getCollider();
	const Vector2f& pos = getOwner()->getPosition();
	const FloatRect bounds = collider->getGlobalBounds();

	const FloatRect baseRect = {
		bounds.w * static_cast<int>(pos.x / bounds.w),
		bounds.t,
		bounds.w,
		bounds.h
	};
	float movement = pos.x < finalDestination.x ? bounds.w : -bounds.w;
	FloatRect currentRect = baseRect;
	// m_destinationsQueue.push(currentVector);

	do {
		if (checkIfDestinationValid(currentRect)) {
			m_destinationsQueue.push(Vector2f(
				currentRect.l + (currentRect.w / 2.f),
				pos.y
			));
			currentRect.l += movement;
		}
		else {
			break;
		}
	} while(m_destinationsQueue.back().distance(finalDestination) >= bounds.w);

	std::clog << "Bandit #" << getOwner()->ID << " patrolling with " << m_destinationsQueue.size() << " nodes.\n";
}

int BaseBanditAI::getRandomNumber() {
	int gen = s_distributor(s_randomEngine);
	if (gen <= 256) {
		return -gen; 
	}
	else {
		return gen - 256;
	}
}

} // namespace TS