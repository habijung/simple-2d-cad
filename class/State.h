#pragma once

#include <string>
using namespace std;

class State
{
public:
	State(string name);
	string getStateName();

private:
	string mName;
};
