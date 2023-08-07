#include "TS/Components/ModifiedBoxCollider.hpp"
#include <BA/Entities/Entity.hpp>


namespace TS {

ModifiedBoxCollider::ModifiedBoxCollider(ba::Entity* owner) :
	ba::BoxCollider::BoxCollider(owner)
{

}

void ModifiedBoxCollider::setOrigin(const ba::Vector2f& origin) {
	m_origin = origin;
}

const ba::Vector2f& ModifiedBoxCollider::getOrigin() const {
	return m_origin;
}

ba::FloatRect ModifiedBoxCollider::getGlobalBounds() const {
	ba::Vector2f scale = getOwner()->getScale();
	ba::Vector2f scaledOrigin{
		m_origin.x * scale.x,
		m_origin.y * scale.y
	};
	ba::Vector2f scaledSize{
		this->getSize().x * scale.x,
		this->getSize().y * scale.y
	};
	ba::Vector2f pos = getOwner()->getPosition();

	return ba::FloatRect{
		ba::Vector2f{
			pos.x - scaledOrigin.x,
			pos.y - scaledOrigin.y
		},
		scaledSize
	};
}


} // namespace TS