#include "TS/Entities/Character.hpp"

namespace TS {

Character::Character(ba::SharedContext* context) :
	ba::Entity::Entity(context)
{

}

void Character::setHp(unsigned hp) {
	m_hp = hp > m_maxHP ? m_maxHP : hp;
}

void Character::setMaxHP(unsigned maxHP) {
	m_maxHP = maxHP;
	if (m_hp > m_maxHP) {
		m_hp = m_maxHP;
	}
}

void Character::damage(unsigned dmg) {
	m_hp -= (dmg > m_hp ? m_hp : dmg);
}

void Character::heal(unsigned healHP) {
	m_hp += healHP;
	if (m_hp > m_maxHP) {
		m_hp = m_maxHP;
	}
}

unsigned Character::getHP() const {
	return m_hp;
}

unsigned Character::getMaxHP() const {
	return m_maxHP;
}

} // namespace TS