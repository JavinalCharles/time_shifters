#include "TS/Components/AttackBoxCollider.hpp"

namespace TS {

AttackBoxCollider::AttackBoxCollider(ba::Entity* owner) :
	ba::BoxCollider::BoxCollider(owner)
{

}

void AttackBoxCollider::resolve(const ba::Vector2f&) {
	getOwner()->queueForRemoval();
}

} // namespace TS