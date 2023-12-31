#include "TS/Components/AttackCollidable.hpp"

#include <iostream>

namespace TS {

AttackCollidable::AttackCollidable(ba::Entity* owner) :
	ba::Collidable::Collidable(owner)
{

}

void AttackCollidable::setDamageToDeliver(unsigned dmg) {
	m_damageToDeliver = dmg;
}

void AttackCollidable::onCollisionEnter(ba::IDtype) {
	
}

void AttackCollidable::onCollisionStay(ba::IDtype) {
	// DO NOTHING
}

void AttackCollidable::onCollisionExit(ba::IDtype otherID) {
	std::shared_ptr<ba::Entity> e = getOwner()->CONTEXT->entities->at(otherID);
	std::shared_ptr<TS::Bandit> ch = std::dynamic_pointer_cast<TS::Bandit>(e);

	if(ch != nullptr) {
		ch->damage(m_damageToDeliver);
	}
}

} // namespace TS