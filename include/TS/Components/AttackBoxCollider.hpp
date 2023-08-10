#pragma once

#include <BA/Components/Colliders/BoxCollider.hpp>
#include <BA/Entities/Entity.hpp>

namespace TS {

class AttackBoxCollider : public ba::BoxCollider {
public:
	explicit AttackBoxCollider(ba::Entity* owner);

	virtual void resolve(const ba::Vector2f& resolution) override;
private:

}; // class AttackBoxCollider

} // namespace TS