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
	HERO_NONE = 0,
	HERO_IDLE,
	HERO_IDLE_LEFT,
	HERO_RUN,
	HERO_RUN_LEFT,
	HERO_ROLL,
	HERO_ROLL_LEFT,
	HERO_JUMP,
	HERO_JUMP_LEFT,
	HERO_FALL,
	HERO_FALL_LEFT,
	HERO_WALL_SLIDE,
	HERO_WALL_SLIDE_LEFT,
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
	HERO_HURT,
	HERO_HURT_LEFT,
	HERO_DEATH,
	HERO_DEATH_LEFT,
};

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
	{HERO_ROLL,
		{0.7F,
			{
				"HeroKnight/Roll/HeroKnight_Roll_0.png",
				"HeroKnight/Roll/HeroKnight_Roll_1.png",
				"HeroKnight/Roll/HeroKnight_Roll_2.png",
				"HeroKnight/Roll/HeroKnight_Roll_3.png",
				"HeroKnight/Roll/HeroKnight_Roll_4.png",
				"HeroKnight/Roll/HeroKnight_Roll_5.png",
				"HeroKnight/Roll/HeroKnight_Roll_6.png",
				"HeroKnight/Roll/HeroKnight_Roll_7.png",
				"HeroKnight/Roll/HeroKnight_Roll_8.png",
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
	},
	{HERO_WALL_SLIDE,
		{0.34f,
			{
				"HeroKnight/WallSlide/HeroKnight_Slide_0.png",
				"HeroKnight/WallSlide/HeroKnight_Slide_1.png",
				"HeroKnight/WallSlide/HeroKnight_Slide_2.png",
				"HeroKnight/WallSlide/HeroKnight_Slide_3.png",
				"HeroKnight/WallSlide/HeroKnight_Slide_4.png",
			}
		}
	},
	{HERO_HURT,
		{0.2F,
			{
				"HeroKnight/Hurt/HeroKnight_Hurt_0.png",
				"HeroKnight/Hurt/HeroKnight_Hurt_1.png",
				"HeroKnight/Hurt/HeroKnight_Hurt_2.png",
			}
		}
	},
	{HERO_DEATH,
		{1.25f,
			{
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_0.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_1.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_2.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_3.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_4.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_5.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_6.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_7.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_8.png",
				"HeroKnight/DeathNoBlood/HeroKnight_DeathNoBlood_9.png",
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
	auto collider = this->getCollider();
	auto velocity = this->getComponent<ba::Velocity>();
	
	
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
		else if ((velocity->get().x != 0.f) && (m_thisPosition.x == m_previousPosition.x)) {
			animation->set(CURR % 2 == 0 ? HERO_WALL_SLIDE_LEFT : HERO_WALL_SLIDE);
		}
	}
	else if (CURR == HERO_WALL_SLIDE || CURR == HERO_WALL_SLIDE_LEFT) {
		if (m_thisPosition.y <= m_previousPosition.y) {
			animation->set(CURR % 2 == 0 ? HERO_IDLE_LEFT : HERO_IDLE);
			this->getComponent<SoundEmitter>()->emitSound(g_SFXR.at(FOREST_1));
			this->m_jumping = false;
			this->m_doubleJumped = false;
			this->m_jumpedTime = 0.f;
		}
		else if (m_thisPosition.x > m_previousPosition.x) {
			animation->set(HERO_FALL);
		}
		else if (m_thisPosition.x < m_previousPosition.x) {
			animation->set(HERO_FALL_LEFT);
		}
		else {
			auto cs = this->CONTEXT->entities->getSystem<ba::VelocityWithCollisionSystem>();
			FloatRect rect(collider->getGlobalBounds());
			FloatRect onePixelWide{
				(CURR % 2 == 0) ?  Vector2f{rect.l - 1.f, rect.t} : Vector2f{rect.l+rect.w+1.f, rect.t},
				Vector2f{1.f, rect.h}
			};
			auto vc = cs->searchStatic(onePixelWide);
			bool sideHaveTiles = false;
			for (auto& j_collider : vc) {
				const IDtype& layer = j_collider->getLayer();
				if (layer == TILE_1 || layer == TILE_2 || layer == TILE_3) {
					sideHaveTiles = true;
					break;
				}
			}
			if (!sideHaveTiles) {
				animation->set(CURR % 2 == 0 ? HERO_FALL : HERO_FALL_LEFT);
			}
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

void Hero::spawnDust() {
	ba::SharedContext* context = const_cast<ba::SharedContext*>(this->CONTEXT);
	const auto collider = this->getCollider();
	const auto animation = this->getComponent<Animation>();
	const IDtype curr = animation->getCurrentAnimationID();
	FloatRect gb = collider->getGlobalBounds();

	std::shared_ptr<SlideDust> dustEntity = std::make_shared<SlideDust>(context);

	dustEntity->setPosition((curr % 2 == 0) ? 
		Vector2f{gb.l, gb.t + gb.h} : 
		Vector2f{gb.l + gb.w, gb.t + gb.w}
	);

	this->CONTEXT->entities->add(dustEntity);
}

void Hero::loadResources() {
	if (s_resourcesLoaded) {
		return;
	}
	for (auto& [id, pair] : s_resourcesToLoad) {
		if (!s_R.contains(id)) {
			s_R.insert_or_assign(id, std::make_pair(pair.first, std::vector<IDtype>{}));
			s_R.insert_or_assign(id+1, std::make_pair(pair.first, std::vector<IDtype>()));
		}
		for (auto& str : pair.second) {
			const IDtype textureID = this->CONTEXT->resources->loadTexture(str);
			s_R.at(id).second.push_back(textureID);
			s_R.at(id+1).second.push_back(textureID);
		}
	}

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

	auto jump = std::bind([a, v, this]() {
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
			case HERO_WALL_SLIDE:
			case HERO_WALL_SLIDE_LEFT:
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
				this->spawnDust();
				break;
		}
	});

	auto run = std::bind([a, ki, v](){
		IDtype curr = a->getCurrentAnimationID();

		if (ki->isKeyActive(SDLK_a)) {
			switch (curr) {
				case HERO_JUMP:
				case HERO_JUMP_LEFT:
					a->set(HERO_JUMP_LEFT);
					v->setX(-NORMAL_SPEED * 1.2f);
					break;
				case HERO_FALL:
				case HERO_FALL_LEFT:
				case HERO_WALL_SLIDE:
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
				case HERO_WALL_SLIDE_LEFT:
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
		if (animationID < HERO_HURT) {
			s.looped = true;
		}
		

		a->add(animationID, s);
	}

	[[maybe_unused]] ba::MouseInput* mi = this->CONTEXT->inputs->getInput<ba::MouseInput>().get();
	[[maybe_unused]] ba::KeyboardInput* ki = this->CONTEXT->inputs->getInput<ba::KeyboardInput>().get();
	[[maybe_unused]] auto s = this->getComponent<SoundEmitter>();
	[[maybe_unused]] auto v = this->getComponent<Velocity>();

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

	auto stillAlive = std::bind([this, a, v]() {
		const IDtype& curr = a->getCurrentAnimationID();
		if (this->getHP() == 0) {
			a->set(curr%2==0 ? HERO_DEATH_LEFT : HERO_DEATH);
			v->setX(0.f);
			v->setY(Y_GRAVITY);
		}
		else {
			a->set(curr%2==0 ? HERO_IDLE_LEFT : HERO_IDLE);
		}
	});

	// const float ATK1_TIME = s_R.at(PLAYER_ATTACK_1).first / s_R.at(PLAYER_ATTACK_1).second.size();
	// const float ATK2_TIME = s_R.at(PLAYER_ATTACK_2).first / s_R.at(PLAYER_ATTACK_2).second.size();
	// const float ATK3_TIME = s_R.at(PLAYER_ATTACK_3).first / s_R.at(PLAYER_ATTACK_3).second.size();

	auto conjureDust = std::bind(&Hero::spawnDust, this);
	
	auto damageEntities = std::bind(&Hero::attack, this);


	a->addFrameAction(HERO_IDLE, 0, resetJump);
	a->addFrameAction(HERO_IDLE_LEFT, 0, resetJump);
	a->addFrameAction(HERO_RUN, 0, resetJump);
	a->addFrameAction(HERO_RUN_LEFT, 0, resetJump);
	a->addFrameAction(HERO_RUN, 4, emitForestSFX);
	a->addFrameAction(HERO_RUN, 8, emitForestSFX);
	a->addFrameAction(HERO_RUN_LEFT, 4, emitForestSFX);
	a->addFrameAction(HERO_RUN_LEFT, 8, emitForestSFX);

	a->addFrameAction(HERO_WALL_SLIDE, 0, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE, 1, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE, 2, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE, 3, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE, 4, conjureDust);

	a->addFrameAction(HERO_WALL_SLIDE_LEFT, 0, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE_LEFT, 1, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE_LEFT, 2, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE_LEFT, 3, conjureDust);
	a->addFrameAction(HERO_WALL_SLIDE_LEFT, 4, conjureDust);

	a->addFrameAction(HERO_ATTACK_1, 2, damageEntities);
	a->addFrameAction(HERO_ATTACK_1_LEFT, 2, damageEntities);

	a->addFrameAction(HERO_ATTACK_2, 1, damageEntities);
	a->addFrameAction(HERO_ATTACK_2_LEFT, 1, damageEntities);

	a->addFrameAction(HERO_ATTACK_3, 2, damageEntities);
	a->addFrameAction(HERO_ATTACK_3_LEFT, 2, damageEntities);

	a->addSequenceAction(HERO_ATTACK_1, from1stTransitionTo2ndAttack);
	a->addSequenceAction(HERO_ATTACK_1_LEFT, from1stTransitionTo2ndAttack);
	a->addSequenceAction(HERO_ATTACK_2, from2ndTransitionTo3rdAttack);
	a->addSequenceAction(HERO_ATTACK_2_LEFT, from2ndTransitionTo3rdAttack);
	a->addSequenceAction(HERO_ATTACK_3, from3rdTransitionTo1stAttack);
	a->addSequenceAction(HERO_ATTACK_3_LEFT, from3rdTransitionTo1stAttack);

	a->addSequenceAction(HERO_HURT, stillAlive);
	a->addSequenceAction(HERO_HURT_LEFT, stillAlive);

	a->addSequenceAction(HERO_WALL_SLIDE, conjureDust);
	a->addSequenceAction(HERO_WALL_SLIDE_LEFT, conjureDust);

	a->set(HERO_IDLE);
}

void Hero::attack() {
	auto cs = this->CONTEXT->entities->getSystem<ba::VelocityWithCollisionSystem>();

	auto animation = this->getComponent<Animation>();
	auto collider = this->getCollider();
	const IDtype& CURR = animation->getCurrentAnimationID();

	FloatRect gb = collider->getGlobalBounds();
	gb.l += (CURR % 2 == 0) ? -gb.w : gb.w;

	ba::debug << gb;

	auto objectsFound = cs->searchNonStatic(gb);
	for (auto& j_collider : objectsFound) {
		if ((j_collider->getLayer() & (NPC_1 | NPC_2 | NPC_3)) != 0) {
			auto e2 = std::dynamic_pointer_cast<Character>(this->CONTEXT->entities->at(j_collider->getOwner()->ID));
			if (e2 != nullptr) {
				e2->damage(8u);
			}
		}
	}
}

} // namespace TS