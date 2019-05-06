#pragma once
#include <string>
#include <set>
#include <iterator>

using namespace std;

class WF
{
public:
	string left;
	set<string> right;

public:
	WF(char s[]);
	void print();
	void insert(char s[]);
};
