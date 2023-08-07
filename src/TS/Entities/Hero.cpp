#include "TS/Entities/Hero.hpp"
#include <iostream>

using ba::Animation;
using ba::Camera;
using ba::BoxCollider;
using ba::KeyboardControl;
using ba::MouseControl;
using ba::Sprite;
using ba::SoundEmitter;
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

// const float Y_GRAVITY = 400.f;
// const float NORMAL_SPEED = 256.f;
// const float SLOW_SPEED = 64.f;

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
	Character::Character(context)
{
	loadResources();
	loadGlobalSounds(this->CONTEXT->resources);
	
	auto animation = this->addComponent<Animation>();
	auto camera = this->addComponent<Camera>();
	auto collider = this->addComponent<ModifiedBoxCollider>();
	auto kc = this->addComponent<KeyboardControl>();
	auto mc = this->addComponent<MouseControl>();
	auto sprite = this->addComponent<Sprite>();
	auto sound = this->addComponent<SoundEmitter>();
	auto velocity = this->addComponent<Velocity>();
	auto updateable = this->addComponent<Updateable>();
	// 
	const Vector2f COLLISION_AREA{22.F, 42.F};

	// COLLIDER
	collider->setSize(COLLISION_AREA);
	collider->setLayer(PLAYER_1);
	collider->setOrigin({COLLISION_AREA.x / 2.f, COLLISION_AREA.y / 2.f - 4.f});
	
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
		if (CURR == HERO_JUMP || CURR == HERO_JUMP_LEFT)
		{
			m_jumpedTime += deltaTime;
			if (m_jumpedTime >= s_MAX_JUMP_TIME || ((m_previousPosition.y - m_thisPosition.y) < 4)) {
				animation->set(CURR%2==0? HERO_FALL_LEFT : HERO_FALL);
				this->getComponent<Velocity>()->setY(Y_GRAVITY);
				this->m_jumping = false;
			}
		}
	}
	else if (CURR == HERO_FALL || CURR == HERO_FALL_LEFT) {
		if (this->m_thisPosition.y <= this->m_previousPosition.y) {
			animation->set(CURR % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
			this->getComponent<SoundEmitter>()->emitSound(g_SFXR.at(FOREST_1));
			this->m_jumping = false;
			this->m_doubleJumped = false;
			this->m_jumpedTime = 0.f;
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

const ba::Vector2f& Hero::getPreviousPosition() const {
	return m_previousPosition;
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
}

void Hero::setMouseButtonBindings(std::shared_ptr<MouseControl>& mc) {
	auto a = this->getComponent<Animation>();
	auto ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>();
	auto mi = this->CONTEXT->inputs->getInput<ba::MouseInput>();
	auto v = this->getComponent<Velocity>();

	auto startAttack = std::bind([a, ki, v]() {
		IDtype curr = a->getCurrentAnimationID();
		if(HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
			return;
		}

		a->set(curr % 2 == 0 ? HERO_ATTACK_1_LEFT : HERO_ATTACK_1);
		v->set({0.f, Y_GRAVITY});
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

		if (this->m_doubleJumped) {
			return;
		}

		switch (curr) {
			case HERO_JUMP:
			case HERO_JUMP_LEFT:
			case HERO_FALL:
			case HERO_FALL_LEFT:
				this->m_doubleJumped = true;
				[[fallthrough]];
			case HERO_IDLE:
			case HERO_IDLE_LEFT:
			case HERO_RUN:
			case HERO_RUN_LEFT:
				this->m_jumping = true;
				this->m_jumpedTime = 0.0f;
				v->setY(-Y_GRAVITY);
				a->set(curr % 2 == 0 ? HERO_JUMP_LEFT : HERO_JUMP);
				break;
		}
	});

	auto run = std::bind([a, ki, v, this](){
		IDtype curr = a->getCurrentAnimationID();
		// if (HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
		// 	return;
		// }

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
				case HERO_RUN:
				case HERO_IDLE:
				case HERO_RUN_LEFT:
				case HERO_IDLE_LEFT:
					a->set(HERO_RUN_LEFT);
					v->setX(-NORMAL_SPEED);
					break;
				case HERO_BLOCK_IDLE:
				case HERO_BLOCK_IDLE_LEFT:
					a->set(HERO_BLOCK_IDLE_LEFT);
					v->setX(-SLOW_SPEED);
					break;
				case HERO_ATTACK_1:
				case HERO_ATTACK_1_LEFT:
				case HERO_ATTACK_2:
				case HERO_ATTACK_2_LEFT:
				case HERO_ATTACK_3:
				case HERO_ATTACK_3_LEFT:
					v->setX(-SLOW_SPEED);
					break;
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
				case HERO_RUN:
				case HERO_IDLE:
				case HERO_RUN_LEFT:
				case HERO_IDLE_LEFT:
					a->set(HERO_RUN);
					v->setX(NORMAL_SPEED);
					break;
				case HERO_BLOCK_IDLE:
				case HERO_BLOCK_IDLE_LEFT:
					a->set(HERO_BLOCK_IDLE);
					v->setX(SLOW_SPEED);
					break;
				case HERO_ATTACK_1:
				case HERO_ATTACK_1_LEFT:
				case HERO_ATTACK_2:
				case HERO_ATTACK_2_LEFT:
				case HERO_ATTACK_3:
				case HERO_ATTACK_3_LEFT:
					v->setX(SLOW_SPEED);
					break;
			}
		}
	});


	auto idle = std::bind([a, ki, v]() {
		IDtype curr = a->getCurrentAnimationID();
		// if (HERO_ATTACK_1 <= curr && curr <= HERO_BLOCK_EFFECT_LEFT) {
		// 	return;
		// }

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
			// case HERO_ATTACK_1:
			// case HERO_ATTACK_1_LEFT:
			// case HERO_ATTACK_2:
			// case HERO_ATTACK_2_LEFT:
			// case HERO_ATTACK_3:
			// case HERO_ATTACK_3_LEFT:
			// 	// TODO SOMETHING
			// OTHERS
		}

	});

	kc->bindOnKeyPressed(SDLK_w, jump);
	kc->bindOnKeyActive(SDLK_a, run);
	kc->bindOnKeyActive(SDLK_d, run);
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
		const float SECONDS_PER_FRAME = pair.first / pair.second.size();
		Sequence s;
		for (const auto& RES_ID : pair.second) {
			s.frames.push_back(Frame{
				RES_ID,
				animationID%2==0 ? LEFT_RECT : RECT,
				SECONDS_PER_FRAME,
			});	
		}
		s.looped = true;

		a->add(animationID, s);
	}

	[[maybe_unused]] ba::MouseInput* mi = this->CONTEXT->inputs->getInput<ba::MouseInput>().get();
	[[maybe_unused]] ba::KeyboardInput* ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>().get();
	[[maybe_unused]] auto s = this->getComponent<SoundEmitter>();
	[[maybe_unused]] auto v = this->getComponent<Velocity>();
	

	// auto to2ndAttack = std::bind([a, mi, ki]() {
	// 	IDtype curr = a->getCurrentAnimationID();
	// 	const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_1 ? true : false));
	// 	IDtype next = mi->isButtonActive(ba::MouseButton::LEFT) ? (NEXT_IS_RIGHT ? HERO_ATTACK_2 : HERO_ATTACK_2_LEFT) : (NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);

	// 	a->set(next);
	// });

	// auto to3rdAttack = std::bind([a, mi, ki]() {
	// 	IDtype curr = a->getCurrentAnimationID();
	// 	const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_2 ? true : false));
	// 	IDtype next = mi->isButtonActive(ba::MouseButton::LEFT) ? (NEXT_IS_RIGHT ? HERO_ATTACK_3 : HERO_ATTACK_3_LEFT) : (NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);

	// 	a->set(next);
	// });

	// auto end3rdAttack = std::bind([a, ki]() {
	// 	IDtype curr = a->getCurrentAnimationID();
	// 	const bool NEXT_IS_RIGHT = ki->isKeyActive(SDLK_d) ? true : (ki->isKeyActive(SDLK_a) ? false : (curr == HERO_ATTACK_3 ? true : false));
		
	// 	a->set(NEXT_IS_RIGHT ? HERO_IDLE : HERO_IDLE_LEFT);
	// });

	auto fromEffectToIdleBlock = std::bind([mi, a](){
		IDtype curr = a->getCurrentAnimationID();
		if (mi->isButtonActive(ba::MouseButton::RIGHT)) {
			a->set(curr == HERO_BLOCK_EFFECT ? HERO_BLOCK_IDLE : HERO_BLOCK_IDLE_LEFT);
		}
		else {
			a->set(curr == HERO_BLOCK_EFFECT ? HERO_IDLE : HERO_IDLE_LEFT);
		}
	});

	auto from1stTransitionTo2ndAttack = std::bind([mi, ki, a]() {
		IDtype curr = a->getCurrentAnimationID();
		float deltaTimeClicked = mi->timeSinceLastClicked(ba::MouseButton::LEFT);
		if (0 <= deltaTimeClicked && deltaTimeClicked <= 0.48) {
			if (ki->isKeyActive(SDLK_a)) {
				a->set(HERO_ATTACK_2_LEFT);
			}
			else if (ki->isKeyActive(SDLK_d)){
				a->set(HERO_ATTACK_2);
			}
			else {
				a->set(curr % 2 == 0 ? HERO_ATTACK_2_LEFT : HERO_ATTACK_2);
			}
		}
		else {
			a->set(curr % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
		}
	});

	auto from2ndTransitionTo3rdAttack = std::bind([mi, ki, a]() {
		IDtype curr = a->getCurrentAnimationID();
		float deltaTimeClicked = mi->timeSinceLastClicked(ba::MouseButton::LEFT);
		if (0 <= deltaTimeClicked && deltaTimeClicked <= 0.48) {
			if (ki->isKeyActive(SDLK_a)) {
				a->set(HERO_ATTACK_3_LEFT);
			}
			else if (ki->isKeyActive(SDLK_d)){
				a->set(HERO_ATTACK_3);
			}
			else {
				a->set(curr % 2 == 0 ? HERO_ATTACK_3_LEFT : HERO_ATTACK_3);
			}
		}
		else {
			a->set(curr % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
		}
	});

	auto from3rdTransitionTo1stAttack = std::bind([mi, ki, a]() {
		IDtype curr = a->getCurrentAnimationID();
		float deltaTimeClicked = mi->timeSinceLastClicked(ba::MouseButton::LEFT);
		if (0 <= deltaTimeClicked && deltaTimeClicked <= 0.48) {
			if (ki->isKeyActive(SDLK_a)) {
				a->set(HERO_ATTACK_1_LEFT);
			}
			else if (ki->isKeyActive(SDLK_d)){
				a->set(HERO_ATTACK_1);
			}
			else {
				a->set(curr % 2 == 0 ? HERO_ATTACK_1_LEFT : HERO_ATTACK_1);
			}
		}
		else {
			a->set(curr % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
		}
	});

	// auto fromFallToIdle = std::bind([v, a, this]() {
	// 	if (this->m_previousPosition.y > this->m_thisPosition.y) {
	// 		return;
	// 	}

	// 	IDtype curr = a->getCurrentAnimationID();

	// 	a->set(curr % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
	// 	v->setY(Y_GRAVITY);
	// });

	auto resetJump = std::bind([a, this]() {
		if (this->m_jumping || this->m_doubleJumped) {
			this->m_jumping = false;
			this->m_doubleJumped = false;
			this->m_jumpedTime = 0.f;
		}
	});

	IDtype forestSFX = g_SFXR.at(FOREST_1);
	auto emitForestSFX = std::bind([s, forestSFX]() {
		s->emitSound(forestSFX);
	});


	a->addFrameAction(HERO_IDLE, 0, resetJump);
	a->addFrameAction(HERO_IDLE_LEFT, 0, resetJump);
	a->addFrameAction(HERO_RUN, 0, resetJump);
	a->addFrameAction(HERO_RUN_LEFT, 0, resetJump);
	a->addFrameAction(HERO_RUN, 4, emitForestSFX);
	a->addFrameAction(HERO_RUN, 8, emitForestSFX);
	a->addFrameAction(HERO_RUN_LEFT, 4, emitForestSFX);
	a->addFrameAction(HERO_RUN_LEFT, 8, emitForestSFX);

	a->addSequenceAction(HERO_ATTACK_1, from1stTransitionTo2ndAttack);
	a->addSequenceAction(HERO_ATTACK_1_LEFT, from1stTransitionTo2ndAttack);
	a->addSequenceAction(HERO_ATTACK_2, from2ndTransitionTo3rdAttack);
	a->addSequenceAction(HERO_ATTACK_2_LEFT, from2ndTransitionTo3rdAttack);
	a->addSequenceAction(HERO_ATTACK_3, from3rdTransitionTo1stAttack);
	a->addSequenceAction(HERO_ATTACK_3_LEFT, from3rdTransitionTo1stAttack);


	// a->addFrameAction(HERO_ATTACK_1, 5, to2ndAttack);
	// a->addFrameAction(HERO_ATTACK_1_LEFT, 5, to2ndAttack);
	// a->addFrameAction(HERO_ATTACK_2, 5, to3rdAttack);
	// a->addFrameAction(HERO_ATTACK_2_LEFT, 5, to3rdAttack);
	// a->addFrameAction(HERO_ATTACK_3, 7, end3rdAttack);
	// a->addFrameAction(HERO_ATTACK_3_LEFT, 7, end3rdAttack);
	// a->addFrameAction(HERO_BLOCK_EFFECT, 4, fromEffectToIdleBlock);
	// a->addFrameAction(HERO_BLOCK_EFFECT_LEFT, 4, fromEffectToIdleBlock);


	a->set(HERO_IDLE);
}



} // namespace TS