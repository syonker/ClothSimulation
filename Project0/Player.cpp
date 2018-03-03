#include "Player.h"



Player::Player(ParticleSystem* system, float tstart, float inc, float tend)
{
	
	System = system;
	tStart = tstart;
	time = tStart;
	tEnd = tend;
	increment = inc;
	active = false;

}


Player::~Player()
{
}

void Player::SetActive(bool toggle) {
	active = toggle;
}

void Player::Update() {

	if (active) {

		if (time < tEnd) {

			System->Update(increment);
			time += increment;

		}

	}
}


void Player::Reset() {

	System->ResetParticles();
	time = tStart;
	active = false;

}