
#include "Channel.h"

Channel::Channel() {

}

Channel::~Channel() {

}

bool Channel::Load(Tokenizer* token) {

	token->FindToken("extrapolate");
	token->SkipWhitespace();
	token->GetToken(extrapIn);
	token->SkipWhitespace();
	token->GetToken(extrapOut);

	token->FindToken("keys"); 
	numKeys = token->GetFloat();
	token->FindToken("{");

	//parse each keyframe
	for (int i = 0; i < numKeys; i++) {

		Keyframe* newKeyframe = new Keyframe();
		keyframes.push_back(newKeyframe);
		newKeyframe->Load(token);

	}

	token->FindToken("}");

	return true;

}



void Channel::Precompute() {

	Keyframe* currKey;

	if (numKeys == 1) {
		keyframes[0]->tangIn = 0;
		keyframes[0]->tangOut = 0;
		return;
	}

	//compute tangents from rules
	for (int i = 0; i < numKeys; i++) {

		currKey = keyframes[i];

		//compute tangIn
		if (strcmp(currKey->ruleIn, "flat") == 0) {

			currKey->tangIn = 0;

		}
		else if (strcmp(currKey->ruleIn, "linear") == 0) {

			if ((i < numKeys - 1) && i > 0) {

				currKey->tangIn = ((currKey->value - keyframes[i - 1]->value) / (currKey->time - keyframes[i - 1]->time));

			}
			else if (i==0) {

				currKey->tangIn = ((keyframes[i + 1]->value - currKey->value) / (keyframes[i + 1]->time - currKey->time));

			}
			else {

				currKey->tangIn = ((currKey->value - keyframes[i - 1]->value) / (currKey->time - keyframes[i - 1]->time));

			}

		}
		else if (strcmp(currKey->ruleIn, "smooth") == 0) {

			//use linear if first or last key
			if (i == 0) {
				currKey->tangIn = ((keyframes[i + 1]->value - currKey->value) / (keyframes[i + 1]->time - currKey->time));
			}
			else if (i == numKeys - 1) {
				currKey->tangIn = ((currKey->value - keyframes[i - 1]->value) / (currKey->time - keyframes[i - 1]->time));
			}
			//if not first or last key
			else {
				currKey->tangIn = ((keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time));
			}

		}
		else {
			std::cerr << "NO TANGENT RULE" << std::endl;

		}


		//compute tangOut
		if (strcmp(currKey->ruleOut, "flat") == 0) {

			currKey->tangOut = 0;

		}
		else if (strcmp(currKey->ruleOut, "linear") == 0) {


			if ((i < numKeys - 1) && i > 0) {

				currKey->tangOut = ((keyframes[i + 1]->value - currKey->value) / (keyframes[i + 1]->time - currKey->time));

			}
			else if (i == 0) {

				currKey->tangOut = currKey->tangIn;

			}
			else {

				currKey->tangOut = currKey->tangIn;

			}


		}
		else if (strcmp(currKey->ruleOut, "smooth") == 0) {

			//use linear if first or last key
			if (i == 0) {
				currKey->tangOut = ((keyframes[i + 1]->value - currKey->value) / (keyframes[i + 1]->time - currKey->time));
			}
			else if (i == numKeys - 1) {
				currKey->tangOut = ((currKey->value - keyframes[i - 1]->value) / (currKey->time - keyframes[i - 1]->time));
			}
			//if not first or last key
			else {
				currKey->tangOut = ((keyframes[i + 1]->value - keyframes[i - 1]->value) / (keyframes[i + 1]->time - keyframes[i - 1]->time));
			}

		}
		else {
			std::cerr << "NO TANGENT RULE" << std::endl;

		}


	}



	//compute cubic coefficients from tangent data
	for (int i = 0; i < numKeys - 1; i++) {

		glm::vec4 g;
		g.x = keyframes[i]->value;
		g.y = keyframes[i + 1]->value;
		g.z = (keyframes[i + 1]->time - keyframes[i]->time)*keyframes[i]->tangOut;
		g.w = (keyframes[i + 1]->time - keyframes[i]->time)*keyframes[i+1]->tangIn;

		keyframes[i]->a = 2 * g.x - 2 * g.y + g.z + g.w;
		keyframes[i]->b = -3 * g.x + 3 * g.y - 2 * g.z - 1 * g.w;
		keyframes[i]->c = g.z;
		keyframes[i]->d = g.x;

	}

}

float Channel::Evaluate(float time) {

	float curr;
	float *currPtr = &curr;
	float duration = keyframes[numKeys - 1]->time - keyframes[0]->time;
	float oddOffset, evenOffset;
	bool oddIteration = true;

	//t is before the first key (use extrapIn)
	if (time < keyframes[0]->time) {

		if (strcmp(extrapIn, "constant") == 0) {

			curr = keyframes[0]->value;

		}
		else if (strcmp(extrapIn, "linear") == 0) {

			curr = (keyframes[0]->tangIn)*(time) + ((keyframes[0]->value) - (keyframes[0]->tangIn)*(keyframes[0]->time));

		}
		else if (strcmp(extrapIn, "cycle") == 0) {

			curr = Evaluate(time + duration);

		}
		else if (strcmp(extrapIn, "cycle_offset") == 0) {

			curr = keyframes[0]->value - (keyframes[numKeys-1]->value - Evaluate(time + duration));

		}
		else if (strcmp(extrapIn, "bounce") == 0) {

			if (oddIteration) {

				curr = Evaluate(time + duration) + oddOffset;
				oddIteration = false;

			}
			else {

				curr = Evaluate(time + duration) + evenOffset;
				oddIteration = true;

			}

		}
		else {
			std::cerr << "INVALID EXTRAPIN!" << std::endl;
		}

	}

	//t is after the last key(use extrapOut)
	else if (time > keyframes[numKeys-1]->time) {

		if (strcmp(extrapOut, "constant") == 0) {

			curr = keyframes[numKeys - 1]->value;

		}
		else if (strcmp(extrapOut, "linear") == 0) {

			curr = (keyframes[numKeys - 1]->tangIn)*(time)+((keyframes[numKeys - 1]->value) - (keyframes[numKeys - 1]->tangIn)*(keyframes[numKeys - 1]->time));

		}
		else if (strcmp(extrapOut, "cycle") == 0) {

			curr = Evaluate(time - duration);

		}
		else if (strcmp(extrapOut, "cycle_offset") == 0) {

			curr = keyframes[numKeys-1]->value + (Evaluate(time - duration) - keyframes[0]->value);

		}
		else if (strcmp(extrapOut, "bounce") == 0) {

			if (oddIteration) {

				curr = Evaluate(time - duration) + oddOffset;
				oddIteration = false;

			}
			else {

				curr = Evaluate(time - duration) + evenOffset;
				oddIteration = true;

			}

		}
		else {
			std::cerr << "INVALID extrapOut!" << std::endl;
		}

	}

	//t falls exactly on some key(return key value)
	else if (ExactlyOnKey(time, currPtr)) {
		//curr set in method already

	}
	
	//t falls between two keys(evaluate cubic equation)
	else {

		//find which keys it lies between
		float t0, t1;
		int index = -1;
		for (int i = 0; i < numKeys-1; i++) {
			if ((keyframes[i]->time < time) && (keyframes[i + 1]->time > time)) {
				t0 = keyframes[i]->time;
				t1 = keyframes[i+1]->time;
				index = i;
				break;
			}
		}

		float u = ((time - t0) / (t1 - t0));

		curr = keyframes[index]->d + u * (keyframes[index]->c + u * (keyframes[index]->b + u * (keyframes[index]->a)));

	}

	return curr;

}

bool Channel::ExactlyOnKey(float time, float *value) {

	for (int i = 0; i < numKeys; i++) {

		if (keyframes[i]->time == time) {

			*value = keyframes[i]->value;
			return true;
		}

	}

	return false;

}