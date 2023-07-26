#include "TS/Entities/Hero.hpp"
#include <iostream>

using ba::Animation;
using ba::Camera;
using ba::BoxCollider;
using ba::KeyboardControl;
using ba::MouseControl;
using ba::Sprite;
using ba::Velocity;
using ba::Sequence;
using ba::Frame;
using ba::FrameAction;
using ba::FloatRect;
using ba::IntRect;
using ba::Vector2f;
using ba::Vector2i;

namespace TS {

enum HeroAnimationss : IDtype {
	NONE = 0,
	HERO_IDLE,
	HERO_IDLE_LEFT,
	HERO_RUN,
	HERO_RUN_LEFT,
	HERO_ATTACK_1,
	HERO_ATTACK_1_LEFT,
	HERO_ATTACK_2,
	HERO_ATTACK_2_LEFT,
	HERO_ATTACK_3,
	HERO_ATTACK_3_LEFT
};

bool Hero::s_resourcesLoaded = false;
std::unordered_map<IDtype, std::vector<IDtype>> Hero::s_R{};
const std::unordered_map<IDtype, std::vector<std::string>> Hero::s_resourcesToLoad = {
	{HERO_IDLE, {
		"HeroKnight/Idle/HeroKnight_Idle_0.png",
		"HeroKnight/Idle/HeroKnight_Idle_1.png",
		"HeroKnight/Idle/HeroKnight_Idle_2.png",
		"HeroKnight/Idle/HeroKnight_Idle_3.png",
		"HeroKnight/Idle/HeroKnight_Idle_4.png",
		"HeroKnight/Idle/HeroKnight_Idle_5.png",
		"HeroKnight/Idle/HeroKnight_Idle_6.png",
		"HeroKnight/Idle/HeroKnight_Idle_7.png"
		}
	},
	{HERO_RUN, {
		"HeroKnight/Run/HeroKnight_Run_0.png",
		"HeroKnight/Run/HeroKnight_Run_1.png",
		"HeroKnight/Run/HeroKnight_Run_2.png",
		"HeroKnight/Run/HeroKnight_Run_3.png",
		"HeroKnight/Run/HeroKnight_Run_4.png",
		"HeroKnight/Run/HeroKnight_Run_5.png",
		"HeroKnight/Run/HeroKnight_Run_6.png",
		"HeroKnight/Run/HeroKnight_Run_7.png",
		"HeroKnight/Run/HeroKnight_Run_8.png",
		"HeroKnight/Run/HeroKnight_Run_9.png",
		}
	},
	{HERO_ATTACK_1, {
		"HeroKnight/Attack1/HeroKnight_Attack1_0.png",
		"HeroKnight/Attack1/HeroKnight_Attack1_1.png",
		"HeroKnight/Attack1/HeroKnight_Attack1_2.png",
		"HeroKnight/Attack1/HeroKnight_Attack1_3.png",
		"HeroKnight/Attack1/HeroKnight_Attack1_4.png",
		"HeroKnight/Attack1/HeroKnight_Attack1_5.png",
		}
	},
	{HERO_ATTACK_2, {
		"HeroKnight/Attack2/HeroKnight_Attack2_0.png",
		"HeroKnight/Attack2/HeroKnight_Attack2_1.png",
		"HeroKnight/Attack2/HeroKnight_Attack2_2.png",
		"HeroKnight/Attack2/HeroKnight_Attack2_3.png",
		"HeroKnight/Attack2/HeroKnight_Attack2_4.png",
		"HeroKnight/Attack2/HeroKnight_Attack2_5.png"
		}
	},
	{HERO_ATTACK_3, {
		"HeroKnight/Attack3/HeroKnight_Attack3_0.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_1.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_2.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_3.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_4.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_5.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_6.png",
		"HeroKnight/Attack3/HeroKnight_Attack3_7.png"
		}
	}
};

Hero::Hero(SharedContext* context) :
	Entity::Entity(context)
{
	loadResources();
	
	auto animation = this->addComponent<Animation>();
	auto camera = this->addComponent<Camera>();
	auto collider = this->addComponent<BoxCollider>();
	auto kc = this->addComponent<KeyboardControl>();
	auto mc = this->addComponent<MouseControl>();
	auto sprite = this->addComponent<Sprite>();
	auto velocity = this->addComponent<Velocity>();
	// 
	const Vector2f COLLISION_AREA{25.F, 45.F};
	this->setOrigin(Vector2f{35.f, 10.f} + Vector2f(COLLISION_AREA * 0.5f));

	// COLLIDER
	collider->setSize(COLLISION_AREA);
	collider->setLayer(8);
	
	// CAMERA
	camera->setView(CONTEXT->window->getView());

	// ANIMATIONS
	this->populateAnimation(animation);
	// Key and Button Bindings
	this->setMouseButtonBindings(mc);
	this->setKeyBindings(kc);

	this->setPosition({128.f, 128.f});

	velocity->setMax({256.f, 256.f});

} // Hero::Hero()

void Hero::loadResources() {
	std::clog << "Hero::loadResources()" << std::endl;
	if (s_resourcesLoaded == true) {
		std::clog << "Resources already reloaded. Returning." << std::endl;
		return;
	}
	for (auto& [id, v] : s_resourcesToLoad) {
		for (auto& str : v) {
			if (!s_R.contains(id)) {
				std::clog << "Assigning animation: " << id << std::endl;
				s_R.insert_or_assign(id, std::vector<IDtype>{});
			}
			s_R.at(id).push_back(this->CONTEXT->resources->loadTexture(str));
		}
	}
	s_R.insert_or_assign(HERO_IDLE_LEFT, s_R.at(HERO_IDLE));
	s_R.insert_or_assign(HERO_RUN_LEFT, s_R.at(HERO_RUN));
	s_R.insert_or_assign(HERO_ATTACK_1_LEFT, s_R.at(HERO_ATTACK_1));
	s_R.insert_or_assign(HERO_ATTACK_2_LEFT, s_R.at(HERO_ATTACK_2));
	s_R.insert_or_assign(HERO_ATTACK_3_LEFT, s_R.at(HERO_ATTACK_3));

	s_resourcesLoaded = true;
	std::clog << "Returning from Hero::loadResources();" << std::endl;
}

void Hero::setMouseButtonBindings(std::shared_ptr<MouseControl>& mc) {
	std::clog << "Hero::setMouseButtonBindings():" << std::endl;
	auto a = this->getComponent<Animation>();
	auto ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>();

	auto startAttack = std::bind([a, ki]() {
		IDtype curr = a->getCurrentAnimationID();
		if(curr == HERO_ATTACK_1 || curr == HERO_ATTACK_1_LEFT || curr == HERO_ATTACK_2 || curr == HERO_ATTACK_2_LEFT || curr == HERO_ATTACK_3 || curr == HERO_ATTACK_3_LEFT) {
			return;
		}
		const bool ATTACK_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : ((curr == HERO_IDLE || curr == HERO_RUN) ? true : false));

		a->set(ATTACK_RIGHT ? HERO_ATTACK_1 : HERO_ATTACK_1_LEFT);
	});

	mc->bindOnMouseButtonPressed(ba::MouseButton::LEFT, startAttack);
	std::clog << "Returning from Hero::setMouseButtonBindings();" << std::endl;
}

void Hero::setKeyBindings(std::shared_ptr<KeyboardControl>& kc) {
	std::clog << "Hero::setKeyBindings();" << std::endl;
	auto a = this->getComponent<Animation>();
	auto v = this->getComponent<Velocity>();

	auto runRight = std::bind([a, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if(curr == HERO_ATTACK_1 || curr == HERO_ATTACK_1_LEFT || curr == HERO_ATTACK_2 || curr == HERO_ATTACK_2_LEFT || curr == HERO_ATTACK_3 || curr == HERO_ATTACK_3_LEFT) {
			return;
		}
		v->moveRight();
		a->set(HERO_RUN);
	});
	auto runLeft = std::bind([a, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if(curr == HERO_ATTACK_1 || curr == HERO_ATTACK_1_LEFT || curr == HERO_ATTACK_2 || curr == HERO_ATTACK_2_LEFT || curr == HERO_ATTACK_3 || curr == HERO_ATTACK_3_LEFT) {
			return;
		}
		v->moveLeft();
		a->set(HERO_RUN_LEFT);
	});
	auto idleRight = std::bind([a]() {
		IDtype curr = a->getCurrentAnimationID();
		if(curr == HERO_ATTACK_1 || curr == HERO_ATTACK_1_LEFT || curr == HERO_ATTACK_2 || curr == HERO_ATTACK_2_LEFT || curr == HERO_ATTACK_3 || curr == HERO_ATTACK_3_LEFT) {
			return;
		}
		a->set(HERO_IDLE);
	});
	auto idleLeft = std::bind([a]() {
		IDtype curr = a->getCurrentAnimationID();
		if(curr == HERO_ATTACK_1 || curr == HERO_ATTACK_1_LEFT || curr == HERO_ATTACK_2 || curr == HERO_ATTACK_2_LEFT || curr == HERO_ATTACK_3 || curr == HERO_ATTACK_3_LEFT) {
			return;
		}
		a->set(HERO_IDLE_LEFT);
	});

	kc->bindOnKeyActive(SDLK_a, runLeft);
	kc->bindOnKeyActive(SDLK_d, runRight);
	kc->bindOnKeyReleased(SDLK_a, idleLeft);
	kc->bindOnKeyReleased(SDLK_d, idleRight);
	std::clog << "Returning from Hero::setKeyBindings();" << std::endl;
}

void Hero::populateAnimation(std::shared_ptr<Animation>& a) {
	std::clog << "Hero::populateAnimation();" << std::endl;
	const IntRect RECT{
		0, 0, 100, 55
	};
	const IntRect LEFT_RECT{
		100, 0, -100, 55
	};
	
	for (auto& [animationID, resourcesIDs] : s_R) {
		const float ANIMATION_TIME = 0.8f / resourcesIDs.size();
		Sequence s;
		for (const auto& RES_ID : resourcesIDs) {
			s.frames.push_back(Frame{
				RES_ID,
				animationID%2==0 ? LEFT_RECT : RECT,
				ANIMATION_TIME,
				{}
			});	
		}
		s.looped = true;

		a->add(animationID, s);
	}

	ba::MouseInput* mi = this->CONTEXT->inputs->getInput<ba::MouseInput>().get();
	ba::KeyboardInput* ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>().get();

	auto to2ndAttack = std::bind([a, mi, ki]() {
		IDtype curr = a->getCurrentAnimationID();
		const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_1 ? true : false));
		IDtype next = mi->isButtonActive(ba::MouseButton::LEFT) ? (NEXT_IS_RIGHT ? HERO_ATTACK_2 : HERO_ATTACK_2_LEFT) : (NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);

		a->set(next);
	});

	auto to3rdAttack = std::bind([a, mi, ki]() {
		IDtype curr = a->getCurrentAnimationID();
		const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_2 ? true : false));
		IDtype next = mi->isButtonActive(ba::MouseButton::LEFT) ? (NEXT_IS_RIGHT ? HERO_ATTACK_3 : HERO_ATTACK_3_LEFT) : (NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);

		a->set(next);
	});

	auto end3rdAttack = std::bind([a, ki]() {
		IDtype curr = a->getCurrentAnimationID();
		const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_3 ? true : false));
		
		a->set(NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);
	});

	a->addFrameAction(HERO_ATTACK_1, 5, to2ndAttack);
	a->addFrameAction(HERO_ATTACK_1_LEFT, 5, to2ndAttack);
	a->addFrameAction(HERO_ATTACK_2, 5, to3rdAttack);
	a->addFrameAction(HERO_ATTACK_2_LEFT, 5, to3rdAttack);
	a->addFrameAction(HERO_ATTACK_3, 7, end3rdAttack);
	a->addFrameAction(HERO_ATTACK_3_LEFT, 7, end3rdAttack);
	a->set(HERO_IDLE);

	std::clog << "Returning from Hero::populateAnimation();" << std::endl;
}



} // namespace TS