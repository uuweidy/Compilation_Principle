#include "WF.h"



WF::WF(char s[])
{
	left = s;
}

void WF::print()
{
	printf("%s->", left.c_str());
	set<string>::iterator it = right.begin();
	if (right.begin() != right.end())
	{
		printf("%s", it->c_str());
		it++;
	}
	for (; it != right.end(); it++)
		printf("|%s", it->c_str());
	puts("");
}

void WF::insert(char s[])
{
	right.insert(s);
}

