#pragma once

#include <BA/Components/Colliders/BoxCollider.hpp>


namespace TS {

class ModifiedBoxCollider : public ba::BoxCollider {
public:
	explicit ModifiedBoxCollider(ba::Entity* owner);

	void setOrigin(const ba::Vector2f& origin);
	const ba::Vector2f& getOrigin() const;

	virtual ba::FloatRect getGlobalBounds() const override;

private:
	ba::Vector2f m_origin{0.f, 0.f};
}; // class ModifiedBoxCollider

} // namespace TS