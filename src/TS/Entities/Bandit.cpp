#include "TS/Entities/Bandit.hpp"
#include "TS/Components/BaseBanditAI.hpp"

#include <iostream>

using ba::Animation;
using ba::Camera;
using ba::BoxCollider;
using ba::KeyboardControl;
using ba::MouseControl;
using ba::Sprite;
using ba::SoundEmitter;
using ba::Timer;
using ba::Velocity;
using ba::Sequence;
using ba::Frame;
using ba::FrameAction;
using ba::FloatRect;
using ba::IntRect;
using ba::Vector2f;
using ba::Vector2i;


namespace TS {



bool Bandit::s_resourcesLoaded = false;

const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> Bandit::s_texturesToLoad{
	{BANDIT_IDLE,
		{1.f,
			{
				"LightBandit/Idle/LightBandit_Idle_0.png",
				"LightBandit/Idle/LightBandit_Idle_1.png",
				"LightBandit/Idle/LightBandit_Idle_2.png",
				"LightBandit/Idle/LightBandit_Idle_3.png",
			}
		}
	},
	{BANDIT_RUN,
		{1.f,
			{
				"LightBandit/Run/LightBandit_Run_0.png",
				"LightBandit/Run/LightBandit_Run_1.png",
				"LightBandit/Run/LightBandit_Run_2.png",
				"LightBandit/Run/LightBandit_Run_3.png",
				"LightBandit/Run/LightBandit_Run_4.png",
				"LightBandit/Run/LightBandit_Run_5.png",
				"LightBandit/Run/LightBandit_Run_6.png",
				"LightBandit/Run/LightBandit_Run_7.png",
			}
		}
	},
	{BANDIT_COMBAT_IDLE,
		{0.5f,
			{
				"LightBandit/Combat Idle/LightBandit_Combat Idle_0.png",
				"LightBandit/Combat Idle/LightBandit_Combat Idle_1.png",
				"LightBandit/Combat Idle/LightBandit_Combat Idle_2.png",
				"LightBandit/Combat Idle/LightBandit_Combat Idle_3.png",
			}
		}
	},
	{BANDIT_ATTACK,
		{0.85f,
			{
				"LightBandit/Attack/LightBandit_Attack_0.png",
				"LightBandit/Attack/LightBandit_Attack_1.png",
				"LightBandit/Attack/LightBandit_Attack_2.png",
				"LightBandit/Attack/LightBandit_Attack_3.png",
				"LightBandit/Attack/LightBandit_Attack_4.png",
				"LightBandit/Attack/LightBandit_Attack_5.png",
				"LightBandit/Attack/LightBandit_Attack_6.png",
				"LightBandit/Attack/LightBandit_Attack_7.png",
			}
		}
	},
	{BANDIT_HURT,
		{0.6f,
			{
				"LightBandit/Hurt/LightBandit_Hurt_0.png",
				"LightBandit/Hurt/LightBandit_Hurt_1.png",
			}
		}
	},
	{BANDIT_RECOVER,
		{1.5f,
			{
				"LightBandit/Recover/LightBandit_Recover_0.png",
				"LightBandit/Recover/LightBandit_Recover_1.png",
				"LightBandit/Recover/LightBandit_Recover_2.png",
				"LightBandit/Recover/LightBandit_Recover_3.png",
				"LightBandit/Recover/LightBandit_Recover_4.png",
				"LightBandit/Recover/LightBandit_Recover_5.png",
				"LightBandit/Recover/LightBandit_Recover_6.png",
				"LightBandit/Recover/LightBandit_Recover_7.png",
			}
		}
	},
	{BANDIT_DEATH,
		{0.2f,
			{
				"LightBandit/Death/LightBandit_Death_0.png",
			}
		}
	}
};

const IntRect Bandit::s_RECT {
	{0, 0},
	{48, 48}
};

const IntRect Bandit::s_RECT_RIGHT {
	{48, 0},
	{-48, 48}
};

std::unordered_map<IDtype, std::pair<float, std::vector<IDtype>>> Bandit::s_TR{};

Bandit::Bandit(ba::SharedContext* context) :
	Character::Character(context)
{
	loadResources();
	loadGlobalSounds(this->CONTEXT->resources);

	this->setMaxHP(40);
	this->setHp(40);

	auto animation = this->addComponent<Animation>();
	auto collider = this->addComponent<ModifiedBoxCollider>();
	auto ai = this->addComponent<BaseBanditAI>();
	auto sprite = this->addComponent<Sprite>();
	auto sound = this->addComponent<SoundEmitter>();
	auto timer = this->addComponent<Timer>();
	auto velocity = this->addComponent<Velocity>();

	this->populateAnimations();
	// this->programAIBehavior();

	sprite->setDrawLayer(30u);

	const Vector2f COLLIDER_SIZE{24.f, 38.f};

	collider->setSize(COLLIDER_SIZE);
	collider->setOrigin(COLLIDER_SIZE * 0.5f);
	collider->setLayer(NPC_1);

	this->setOrigin({s_RECT.w / 2.f, (s_RECT.h / 2)+3.f});
	this->setScale({2.f, 2.f});

	velocity->setMax({512.f, 512.f});
	velocity->setY(Y_GRAVITY);

	animation->set(BANDIT_IDLE);
}

void Bandit::damage(unsigned dmg) {
	Character::damage(dmg);
	auto animation = this->getComponent<Animation>();
	const IDtype CURR = animation->getCurrentAnimationID();
	if (CURR < BANDIT_HURT) {
		animation->set(CURR % 2 == 0 ? BANDIT_HURT_RIGHT : BANDIT_HURT);
	}
}

void Bandit::startCountdown() {
	if (this->m_haveFinalCountdown) {
		return;
	}
	std::clog << "#" << this->ID << ": Bandit::startCountdown();";
	auto timer = this->getComponent<Timer>();
	timer->setTimer(std::bind([this]() {
		if (this->getHP() == 0) {
			this->queueForRemoval();
		}
	}), 3.f, false);

	this->m_haveFinalCountdown = true;
	std::clog << std::endl;
}

void Bandit::loadResources() {
	if (s_resourcesLoaded) {
		return;
	}

	for (auto& [id, pair] : s_texturesToLoad) {
		for (const std::string& str : pair.second) {
			if (!s_TR.contains(id)) {
				s_TR.insert_or_assign(id, std::make_pair(pair.first, std::vector<IDtype>{}));
				s_TR.insert_or_assign(id+1u, std::make_pair(pair.first, std::vector<IDtype>{}));
			}
			IDtype loadedID = this->CONTEXT->resources->loadTexture(str);
			s_TR.at(id).second.push_back(loadedID);
			s_TR.at(id+1u).second.push_back(loadedID);
		}
	}
	s_TR.insert_or_assign(BANDIT_DYING, std::make_pair(s_TR.at(BANDIT_RECOVER).first, std::vector<IDtype>()));
	s_TR.insert_or_assign(BANDIT_DYING_RIGHT, std::make_pair(s_TR.at(BANDIT_RECOVER).first, std::vector<IDtype>()));

	s_TR.at(BANDIT_DYING).second.insert(s_TR.at(BANDIT_DYING).second.end(), s_TR.at(BANDIT_RECOVER).second.rbegin(), s_TR.at(BANDIT_RECOVER).second.rend());
	s_TR.at(BANDIT_DYING_RIGHT).second.insert(s_TR.at(BANDIT_DYING_RIGHT).second.end(), s_TR.at(BANDIT_RECOVER_RIGHT).second.rbegin(), s_TR.at(BANDIT_RECOVER_RIGHT).second.rend());

	s_resourcesLoaded = true;
}

void Bandit::populateAnimations() {
	auto animation = this->getComponent<Animation>();
	auto timer = this->getComponent<Timer>();
	auto velocity = this->getComponent<Velocity>();

	for (auto& [animationID, pair] : s_TR) {
		const float SECONDS_PER_FRAME = pair.first / pair.second.size();
		ba::Sequence sequence;
		for (const auto& TEXTURE_ID : pair.second) {
			sequence.frames.push_back( ba::Frame {
				TEXTURE_ID,
				animationID % 2 == 0 ? s_RECT_RIGHT : s_RECT,
				SECONDS_PER_FRAME,
			});
		}
		if (animationID < BANDIT_HURT) {
			sequence.looped = true;
		}

		animation->add(animationID, sequence);
	}

	ba::SequenceAction checkStillAlive = std::bind([this, animation]() {
		const IDtype CURR = animation->getCurrentAnimationID();
		if(this->getHP() > 0) {
			animation->set(CURR % 2 == 0 ? BANDIT_COMBAT_IDLE_RIGHT : BANDIT_COMBAT_IDLE);
			// this->m_currentState = State::COMBAT;
		}
		else {
			animation->set(CURR % 2 == 0 ? BANDIT_DYING_RIGHT : BANDIT_DYING);
			// this->m_currentState = State::DEAD;
		}
	});

	ba::SequenceAction dyingToDead = std::bind([ animation, velocity] () {
		const IDtype CURR = animation->getCurrentAnimationID();
		if (CURR < BANDIT_DYING || CURR > BANDIT_DYING_RIGHT) {
			return;
		}
		animation->set(CURR % 2 == 0 ? BANDIT_DEATH_RIGHT : BANDIT_DEATH);
		velocity->setX(0.f);
	});
	ba::SequenceAction postRecovery = std::bind([animation]() {
		const IDtype CURR = animation->getCurrentAnimationID();
		animation->set(CURR % 2 == 0 ? BANDIT_COMBAT_IDLE_RIGHT : BANDIT_COMBAT_IDLE);
	});


	animation->addSequenceAction(BANDIT_HURT, checkStillAlive);
	animation->addSequenceAction(BANDIT_HURT_RIGHT, checkStillAlive);
	animation->addSequenceAction(BANDIT_RECOVER, postRecovery);
	animation->addSequenceAction(BANDIT_RECOVER_RIGHT, postRecovery);
	animation->addSequenceAction(BANDIT_DYING, dyingToDead);
	animation->addSequenceAction(BANDIT_DYING_RIGHT, dyingToDead);
	animation->addSequenceAction(BANDIT_DEATH, std::bind(&Bandit::startCountdown, this));
	animation->addSequenceAction(BANDIT_DEATH_RIGHT, std::bind(&Bandit::startCountdown, this));

	animation->set(BANDIT_IDLE);
}

} // namespace TS