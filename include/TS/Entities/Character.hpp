#pragma once

#include <BA/Entities/Entity.hpp>

namespace TS {

class Character : public ba::Entity {
public:
	Character(ba::SharedContext* context);

	void setHp(unsigned hp);
	void setMaxHP(unsigned maxHP);

	void damage(unsigned dmg);
	void heal(unsigned healHP);

	unsigned getHP() const;
	unsigned getMaxHP() const;

private:
	unsigned m_hp		= 20u;
	unsigned m_maxHP	= 20u;

}; // class Character

} // namespace TS