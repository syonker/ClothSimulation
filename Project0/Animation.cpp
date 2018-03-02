
#include "Animation.h"

Animation::Animation() {

}

Animation::~Animation() {

}

bool Animation::Load(const char *file) {

	if (file == "") {

		file = "../animations/wasp2_walk.anim";
	}

	std::cerr << "file: " << file << std::endl;

	Tokenizer* token = new Tokenizer();
	token->Open(file);

	//get initial params
	token->FindToken("animation");	token->FindToken("{");

	token->FindToken("range");	tStart = token->GetFloat();	tEnd = token->GetFloat();

	token->FindToken("numchannels");	numChannels = token->GetFloat();

	//parse each channel
	for (int i = 0; i < numChannels; i++) {
		token->FindToken("channel");
		token->FindToken("{");

		Channel* newChannel = new Channel();
		channels.push_back(newChannel);
		newChannel->Load(token);

		token->FindToken("}");
	}

	// Finish
	token->FindToken("}");
	token->Close();


	Precompute();

	return true;
}



void Animation::Evaluate(float time, std::vector<float> &pose) {
	
	//for each channel get the new pose value
	for (int i = 0; i < numChannels; i++) {

		pose[i] = channels[i]->Evaluate(time);

	}

}


void Animation::Precompute() {

	//for each channel
	for (int i = 0; i < numChannels; i++) {

		channels[i]->Precompute();

	}

}