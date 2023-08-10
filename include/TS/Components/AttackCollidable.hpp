#pragma once

#include <BA/Components/Collidable.hpp>
#include <BA/Systems/EntityManager.hpp>
#include "TS/Entities/Bandit.hpp"

namespace TS {

class AttackCollidable : public ba::Collidable {
public:
	explicit AttackCollidable(ba::Entity* owner);

	virtual void onCollisionEnter(ba::IDtype otherID) override;
	virtual void onCollisionStay(ba::IDtype otherID) override;
	virtual void onCollisionExit(ba::IDtype otherID) override;

	void setDamageToDeliver(unsigned dmg);
private:
	unsigned m_damageToDeliver = 0u;

}; // class AttackCollidable

} // namespace TS