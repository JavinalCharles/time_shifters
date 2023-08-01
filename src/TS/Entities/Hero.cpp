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
	HERO_JUMP,
	HERO_JUMP_LEFT,
	HERO_FALL,
	HERO_FALL_LEFT,
	HERO_ATTACK_1,
	HERO_ATTACK_1_LEFT,
	HERO_ATTACK_2,
	HERO_ATTACK_2_LEFT,
	HERO_ATTACK_3,
	HERO_ATTACK_3_LEFT,
	HERO_BLOCK_IDLE,
	HERO_BLOCK_IDLE_LEFT,
	HERO_BLOCK_EFFECT,
	HERO_BLOCK_EFFECT_LEFT,
};

const float Y_GRAVITY = 400.f;
const float NORMAL_SPEED = 256.f;

const float Hero::s_MAX_JUMP_TIME = 0.4f;

bool Hero::s_resourcesLoaded = false;
std::unordered_map<IDtype, std::pair<float,std::vector<IDtype>>> Hero::s_R{};
const std::unordered_map<IDtype, std::pair<float,std::vector<std::string>>> Hero::s_resourcesToLoad{
	{HERO_IDLE, 
		{1.f, 
			{
				"HeroKnight/Idle/HeroKnight_Idle_0.png",
				"HeroKnight/Idle/HeroKnight_Idle_1.png",
				"HeroKnight/Idle/HeroKnight_Idle_2.png",
				"HeroKnight/Idle/HeroKnight_Idle_3.png",
				"HeroKnight/Idle/HeroKnight_Idle_4.png",
				"HeroKnight/Idle/HeroKnight_Idle_5.png",
				"HeroKnight/Idle/HeroKnight_Idle_6.png",
				"HeroKnight/Idle/HeroKnight_Idle_7.png"
			}
		}
	},
	{HERO_RUN, 
		{1.f,
			{
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
		}
	},
	{HERO_ATTACK_1, 
		{ 0.5f,
			{
				"HeroKnight/Attack1/HeroKnight_Attack1_0.png",
				"HeroKnight/Attack1/HeroKnight_Attack1_1.png",
				"HeroKnight/Attack1/HeroKnight_Attack1_2.png",
				"HeroKnight/Attack1/HeroKnight_Attack1_3.png",
				"HeroKnight/Attack1/HeroKnight_Attack1_4.png",
				"HeroKnight/Attack1/HeroKnight_Attack1_5.png",
			}
		}
	},
	{HERO_ATTACK_2, 
		{ 0.5f,
			{
				"HeroKnight/Attack2/HeroKnight_Attack2_0.png",
				"HeroKnight/Attack2/HeroKnight_Attack2_1.png",
				"HeroKnight/Attack2/HeroKnight_Attack2_2.png",
				"HeroKnight/Attack2/HeroKnight_Attack2_3.png",
				"HeroKnight/Attack2/HeroKnight_Attack2_4.png",
				"HeroKnight/Attack2/HeroKnight_Attack2_5.png"
			}
		}
	},
	{HERO_ATTACK_3,
		{ 0.5f,
			{
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
	},
	{HERO_BLOCK_IDLE,
		{ 1.f,
			{
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_0.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_1.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_2.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_3.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_4.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_5.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_6.png",
				"HeroKnight/BlockIdle/HeroKnight_Block Idle_7.png",
			}
		}
	},
	{HERO_BLOCK_EFFECT,
		{0.4f,
			{
				"HeroKnight/Block/HeroKnight_Block_0.png",
				"HeroKnight/Block/HeroKnight_Block_1.png",
				"HeroKnight/Block/HeroKnight_Block_2.png",
				"HeroKnight/Block/HeroKnight_Block_3.png",
				"HeroKnight/Block/HeroKnight_Block_4.png",
			}
		}
	},
	{HERO_FALL,
		{0.4f,
			{
				"HeroKnight/Fall/HeroKnight_Fall_0.png",
				"HeroKnight/Fall/HeroKnight_Fall_1.png",
				"HeroKnight/Fall/HeroKnight_Fall_2.png",
				"HeroKnight/Fall/HeroKnight_Fall_3.png",
			}
		}
	},
	{HERO_JUMP,
		{0.6f,
			{
				"HeroKnight/Jump/HeroKnight_Jump_0.png",
				"HeroKnight/Jump/HeroKnight_Jump_1.png",
				"HeroKnight/Jump/HeroKnight_Jump_2.png",
			}
			
		}
	}
};

Hero::Hero(SharedContext* context) :
	Entity::Entity(context)
{
	loadResources();
	
	auto animation = this->addComponent<Animation>();
	auto camera = this->addComponent<Camera>();
	auto collider = this->addComponent<ModifiedBoxCollider>();
	auto kc = this->addComponent<KeyboardControl>();
	auto mc = this->addComponent<MouseControl>();
	auto sprite = this->addComponent<Sprite>();
	auto velocity = this->addComponent<Velocity>();
	auto updateable = this->addComponent<Updateable>();
	// 
	const Vector2f COLLISION_AREA{23.F, 43.F};

	// COLLIDER
	collider->setSize(COLLISION_AREA);
	collider->setLayer(8u);
	collider->setOrigin({COLLISION_AREA.x / 2.f, COLLISION_AREA.y / 2.f - 7.f});
	
	// CAMERA
	camera->setView(CONTEXT->window->getView());

	// ANIMATIONS
	this->populateAnimation(animation);
	// Key and Button Bindings
	this->setMouseButtonBindings(mc);
	this->setKeyBindings(kc);
	// SPRITE
	sprite->setDrawLayer(32);
	sprite->setSortOrder(200);

	animation->set(HERO_IDLE);

	this->setPosition({256.f, 256.f});
	this->setScale({2.0f, 2.0f});
	this->setOrigin({50.f, 27.5});

	velocity->setMax({600.f, 600.f});
	velocity->setY(Y_GRAVITY);

	Update u = std::bind(&Hero::updatePreviousPosition, this, std::placeholders::_1);
	updateable->addUpdate(u);
} // Hero::Hero()

void Hero::updatePreviousPosition(float deltaTime) {
	m_previousPosition = m_thisPosition;
	m_thisPosition = this->getPosition();

	auto animation = this->getComponent<ba::Animation>();
	const IDtype CURR = animation->getCurrentAnimationID();
	if (this->m_jumping) {
		m_jumpedTime += deltaTime;
		if (m_jumpedTime >= s_MAX_JUMP_TIME || ((m_previousPosition.y - m_thisPosition.y) < 4)) {
			if (CURR % 2 == 0) {
				animation->set(HERO_FALL_LEFT);
			}
			else {
				animation->set(HERO_FALL);
			}
			this->getComponent<Velocity>()->setY(Y_GRAVITY);
		}
	}
	else if ((m_thisPosition.y - m_previousPosition.y) > 4.f) {
		if (CURR % 2 == 0) {
			animation->set(HERO_FALL_LEFT);
		}
		else {
			animation->set(HERO_FALL);
		}
	}
}


void Hero::loadResources() {
	// std::clog << "Hero::loadResources()" << std::endl;
	if (s_resourcesLoaded == true) {
		// std::clog << "Resources already reloaded. Returning." << std::endl;
		return;
	}
	for (auto& [id, pair] : s_resourcesToLoad) {
		for (auto& str : pair.second) {
			if (!s_R.contains(id)) {
				// std::clog << "Assigning animation: " << id << std::endl;
				s_R.insert_or_assign(id, std::make_pair(pair.first, std::vector<IDtype>{}));
			}
			s_R.at(id).second.push_back(this->CONTEXT->resources->loadTexture(str));
		}
	}
	s_R.insert_or_assign(HERO_IDLE_LEFT, s_R.at(HERO_IDLE));
	s_R.insert_or_assign(HERO_RUN_LEFT, s_R.at(HERO_RUN));
	s_R.insert_or_assign(HERO_ATTACK_1_LEFT, s_R.at(HERO_ATTACK_1));
	s_R.insert_or_assign(HERO_ATTACK_2_LEFT, s_R.at(HERO_ATTACK_2));
	s_R.insert_or_assign(HERO_ATTACK_3_LEFT, s_R.at(HERO_ATTACK_3));
	s_R.insert_or_assign(HERO_BLOCK_IDLE_LEFT, s_R.at(HERO_BLOCK_IDLE));
	s_R.insert_or_assign(HERO_BLOCK_EFFECT_LEFT, s_R.at(HERO_BLOCK_EFFECT));
	s_R.insert_or_assign(HERO_FALL_LEFT, s_R.at(HERO_FALL));
	s_R.insert_or_assign(HERO_JUMP_LEFT, s_R.at(HERO_JUMP));

	s_resourcesLoaded = true;
	// std::clog << "Returning from Hero::loadResources();" << std::endl;
}

void Hero::setMouseButtonBindings(std::shared_ptr<MouseControl>& mc) {
	auto a = this->getComponent<Animation>();
	auto ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>();
	auto mi = this->CONTEXT->inputs->getInput<ba::MouseInput>();
	auto v = this->getComponent<Velocity>();

	auto startAttack = std::bind([a, ki, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if(HERO_ATTACK_1 <= curr && curr <= HERO_JUMP_LEFT) {
			return;
		}
		const bool ATTACK_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : ((curr == HERO_IDLE || curr == HERO_RUN) ? true : false));

		a->set(ATTACK_RIGHT ? HERO_ATTACK_1 : HERO_ATTACK_1_LEFT);
		v->setX(0.f);
	});

	auto startBlock = std::bind([a, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if (HERO_ATTACK_1 <= curr && curr <= HERO_ATTACK_3_LEFT) {
			return;
		}

		switch(curr) {
			case HERO_RUN_LEFT:
			case HERO_IDLE_LEFT:
			case HERO_BLOCK_IDLE_LEFT:
				a->set(HERO_BLOCK_IDLE_LEFT);
				break;
			case HERO_RUN:
			case HERO_IDLE:
			case HERO_BLOCK_IDLE:
			default:
				a->set(HERO_BLOCK_IDLE);
				
		}
		v->setX(0.f);
	});

	auto releaseBlock = std::bind([a, mi, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if (HERO_JUMP <= curr && curr <= HERO_ATTACK_3_LEFT) {
			return;
		}

		if (mi->isButtonActive(ba::MouseButton::LEFT) || mi->isButtonActive(ba::MouseButton::RIGHT)) {
			return;
		}

		switch(curr) {
			case HERO_BLOCK_IDLE:
				a->set(HERO_IDLE);
				break;
			default:
				a->set(HERO_IDLE_LEFT);
		}

	});

	mc->bindOnMouseButtonPressed(ba::MouseButton::LEFT, startAttack);
	mc->bindOnMouseButtonActive(ba::MouseButton::RIGHT, startBlock);
	mc->bindOnMouseButtonReleased(ba::MouseButton::RIGHT, releaseBlock);
}

void Hero::setKeyBindings(std::shared_ptr<KeyboardControl>& kc) {
	auto a = this->getComponent<Animation>();
	auto v = this->getComponent<Velocity>();
	// ba::MouseInput* mi = this->CONTEXT->inputs->getInput<ba::MouseInput>().get();
	ba::KeyboardInput* ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>().get();

	auto jump = std::bind([a, ki, v, this]() {
		IDtype curr = a->getCurrentAnimationID();
		if (HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
			return;
		}

		if (!this->m_jumping) {
			this->m_jumping = true;
			this->m_jumpedTime = 0.0f;
			v->setY(-Y_GRAVITY);
			switch(curr) {
				case HERO_IDLE_LEFT:
				case HERO_RUN_LEFT:
					a->set(HERO_JUMP_LEFT);
					break;
				case HERO_IDLE:
				case HERO_RUN:
					a->set(HERO_JUMP);
			}
		}
		else if (this->m_jumping && !this->m_doubleJumped) {
			v->setY(-Y_GRAVITY);
			this->m_jumpedTime = 0.0;
			this->m_doubleJumped = true;
			switch(curr) {
				case HERO_JUMP_LEFT:
				case HERO_FALL_LEFT:
					a->set(HERO_JUMP_LEFT);
					break;
				case HERO_JUMP:
				case HERO_FALL:
					a->set(HERO_JUMP);
			}
		}
	});

	auto run = std::bind([a, ki, v, this](){
		IDtype curr = a->getCurrentAnimationID();
		if (HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
			return;
		}

		if (ki->isKeyActive(SDLK_a)) {
			switch (curr) {
				case HERO_JUMP:
				case HERO_JUMP_LEFT:
					a->set(HERO_JUMP_LEFT);
					v->setX(-NORMAL_SPEED * 1.2f);
					break;
				case HERO_FALL:
				case HERO_FALL_LEFT:
					a->set(HERO_FALL_LEFT);
					v->setX(-NORMAL_SPEED * 1.2f);
					break;
				default:
					a->set(HERO_RUN_LEFT);
					v->setX(-NORMAL_SPEED);
			}
		}
		else if (ki->isKeyActive(SDLK_d)) {
			switch (curr) {
				case HERO_JUMP:
				case HERO_JUMP_LEFT:
					a->set(HERO_JUMP);
					v->setX(NORMAL_SPEED * 1.2f);
					break;
				case HERO_FALL:
				case HERO_FALL_LEFT:
					a->set(HERO_FALL);
					v->setX(NORMAL_SPEED * 1.2f);
					break;
				default:
					a->set(HERO_RUN);
					v->setX(NORMAL_SPEED);
			}
		}
	});


	auto idle = std::bind([a, ki, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if (HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
			return;
		}

		if (ki->isKeyActive(SDLK_a) || ki->isKeyActive(SDLK_d)) {
			return;
			
		}
		v->setX(0.f);

		switch(curr) {
			case HERO_RUN:
				a->set(HERO_IDLE);
				break;
			case HERO_RUN_LEFT:
				a->set(HERO_IDLE_LEFT);
				break;
			// OTHERS
		}

	});

	kc->bindOnKeyPressed(SDLK_w, jump);
	kc->bindOnKeyActive(SDLK_a, run);
	kc->bindOnKeyActive(SDLK_d, run);
	kc->bindOnKeyReleased(SDLK_w, idle);
	kc->bindOnKeyReleased(SDLK_s, idle);
	kc->bindOnKeyReleased(SDLK_a, idle);
	kc->bindOnKeyReleased(SDLK_d, idle);
}

void Hero::populateAnimation(std::shared_ptr<Animation>& a) {
	const IntRect RECT{
		0, 0, 100, 55
	};
	const IntRect LEFT_RECT{
		100, 0, -100, 55
	};
	
	for (auto& [animationID, pair] : s_R) {
		const float ANIMATION_TIME = pair.first / pair.second.size();
		Sequence s;
		for (const auto& RES_ID : pair.second) {
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

	auto fromEffectToIdleBlock = std::bind([mi, a](){
		IDtype curr = a->getCurrentAnimationID();
		if (mi->isButtonActive(ba::MouseButton::RIGHT)) {
			a->set(curr == HERO_BLOCK_EFFECT ? HERO_BLOCK_IDLE : HERO_BLOCK_IDLE_LEFT);
		}
		else {
			a->set(curr == HERO_BLOCK_EFFECT ? HERO_IDLE : HERO_IDLE_LEFT);
		}
	});

	auto fromFallToIdle = std::bind([mi, a, this]() {
		if (this->m_previousPosition.y > this->m_thisPosition.y) {
			return;
		}
		IDtype curr = a->getCurrentAnimationID();
		a->set(curr == HERO_FALL ? HERO_IDLE : HERO_IDLE_LEFT);
		this->m_jumping = false;
		this->m_doubleJumped = false;
		this->m_jumpedTime = 0.f;
	});

	a->addFrameAction(HERO_ATTACK_1, 5, to2ndAttack);
	a->addFrameAction(HERO_ATTACK_1_LEFT, 5, to2ndAttack);
	a->addFrameAction(HERO_ATTACK_2, 5, to3rdAttack);
	a->addFrameAction(HERO_ATTACK_2_LEFT, 5, to3rdAttack);
	a->addFrameAction(HERO_ATTACK_3, 7, end3rdAttack);
	a->addFrameAction(HERO_ATTACK_3_LEFT, 7, end3rdAttack);
	a->addFrameAction(HERO_BLOCK_EFFECT, 4, fromEffectToIdleBlock);
	a->addFrameAction(HERO_BLOCK_EFFECT_LEFT, 4, fromEffectToIdleBlock);

	a->addFrameAction(HERO_FALL, 0, fromFallToIdle);
	a->addFrameAction(HERO_FALL, 1, fromFallToIdle);
	a->addFrameAction(HERO_FALL, 2, fromFallToIdle);
	a->addFrameAction(HERO_FALL, 3, fromFallToIdle);
	a->addFrameAction(HERO_FALL_LEFT, 0, fromFallToIdle);
	a->addFrameAction(HERO_FALL_LEFT, 1, fromFallToIdle);
	a->addFrameAction(HERO_FALL_LEFT, 2, fromFallToIdle);
	a->addFrameAction(HERO_FALL_LEFT, 3, fromFallToIdle);


	a->set(HERO_IDLE);
}



} // namespace TS