
#include "DOF.h"


DOF::DOF()
{
	//set default values
	value = 0.0f;
	min = -100000.0f;
	max = 100000.0f;

}

DOF::~DOF()
{
}

void DOF::SetValue(float newValue) {

	//if (newValue < min) {
	//	newValue = min;
	//}
	//else if (newValue > max) {
	//	newValue = max;
	//}

	value = newValue;

}

float DOF::GetValue() {

	return value;

}

void DOF::SetMinMax(float minValue, float maxValue) {

	min = minValue;
	max = maxValue;

}

float DOF::Add(float addVal) {

	value += addVal;

	return value;

}