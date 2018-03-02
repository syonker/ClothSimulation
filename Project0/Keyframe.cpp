
#include "Keyframe.h"

Keyframe::Keyframe() {

}

Keyframe::~Keyframe() {

}

bool Keyframe::Load(Tokenizer* token) {

	char isLetter;

	time = token->GetFloat();
	value = token->GetFloat();
	token->SkipWhitespace();

	isLetter = token->CheckChar();

	//parse a word
	if ((97 <= isLetter) && (isLetter <= 122)) {

		token->GetToken(ruleIn);
		token->SkipWhitespace();
		token->GetToken(ruleOut);
		
	}
	//parse a num
	else {

		//ruleIn = "basic";
		//ruleOut = "basic";
		tangIn = token->GetFloat();
		tangOut = token->GetFloat();

		std::cerr << "float tangents given" << std::endl;
	}

	return true;

}
