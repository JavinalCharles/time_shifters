#include "TS/Entities/Bandit.hpp"

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
using ba::ProgrammedAI;
using ba::Vector2f;
using ba::Vector2i;

namespace TS {

enum BanditAnimations : IDtype {
	NONE = 0ul,
	BANDIT_IDLE,
	BANDIT_IDLE_RIGHT,
	BANDIT_RUN,
	BANDIT_RUN_RIGHT,
	BANDIT_COMBAT_IDLE,
	BANDIT_COMBAT_IDLE_RIGHT,
	BANDIT_ATTACK,
	BANDIT_ATTACK_RIGHT,
	BANDIT_HURT,
	BANDIT_HURT_RIGHT,
	BANDIT_RECOVER,
	BANDIT_RECOVER_RIGHT,
	BANDIT_DEATH,
	BANDIT_DEATH_RIGHT
};

bool Bandit::s_resourcesLoaded = false;

const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> Bandit::s_texturesToLoad{
	{BANDIT_IDLE,
		{0.4f,
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
		{0.4f,
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
	
	auto animation = this->addComponent<Animation>();
	auto collider = this->addComponent<BoxCollider>();
	auto programmedAI = this->addComponent<ProgrammedAI>();
	auto sprite = this->addComponent<Sprite>();
	auto sound = this->addComponent<SoundEmitter>();
	auto velocity = this->addComponent<Velocity>();

	this->populateAnimations();

	collider->setSize({static_cast<float>(s_RECT.w), static_cast<float>(s_RECT.h)});
	collider->setLayer(NPC_1);
	this->setOrigin({s_RECT.w / 2.f, s_RECT.h / 2.f});
	this->setScale({2.f, 2.f});

	velocity->setMax({512.f, 512.f});
	velocity->setY(Y_GRAVITY);

	animation->set(BANDIT_IDLE);
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


	s_resourcesLoaded = true;
}

void Bandit::populateAnimations() {
	auto animation = this->getComponent<Animation>();

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

	animation->set(BANDIT_IDLE);
}

} // namespace TS