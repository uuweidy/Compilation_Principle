#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <map>
#include <stack>
#include "WF.h"

#define DEBUG
#define MAX 507

using namespace std;

map<string, set<char> > first;
map<string, set<char> > follow;
map<string, int> VN_dic;
vector<WF> VN_set;
vector<bool> has;
bool used[MAX];

void input(int n)
{
	for (int i = 0; i < n; i++)
	{
		char s[MAX];
		scanf("%s", s);
		int len = strlen(s), j;
		for (j = 0; j < len; j++)
			if (s[j] == '-') break;
		s[j] = 0;
		if (!VN_dic[s])
		{
			VN_set.push_back(s);
			VN_dic[s] = VN_set.size();
		}
		int x = VN_dic[s] - 1;

		string temp = "";
		for (int k = j + 2; k < len; k++)
		{
			if (s[k] != '|')
			{
				temp = temp + s[k];
			}
			else
			{
				char ans[MAX];
				strcpy(ans, temp.c_str());
				VN_set[x].insert(ans);
				temp = "";
			}
		}
		char ans[MAX];
		strcpy(ans, temp.c_str());
		VN_set[x].insert(ans);
	}
}

void dfs(int x)
{
	if (used[x]) return;
	used[x] = 1;
	string& left = VN_set[x].left;
	set<string>& right = VN_set[x].right;
	set<string>::iterator it = right.begin();
	for (; it != right.end(); it++)
	{
		for (int i = 0; i < it->length(); i++)
		{
			if (!isupper(it->at(i)) && it->at(i) != '\'')
			{
				first[left].insert(it->at(i));
				break;
			}
			if (isupper(it->at(i)))
			{
				int y;
				if (i != it->length() - 1 && it->at(i + 1) == '\'')
					y = VN_dic[it->substr(i, 2)] - 1;
				else
					y = VN_dic[it->substr(i, 1)] - 1;
				string& tleft = VN_set[y].left;
				dfs(y);
				set<char>& temp = first[tleft];
				set<char>::iterator it1 = temp.begin();
				bool flag = true;
				for (; it1 != temp.end(); it1++)
				{
					if (*it1 == '~')
						flag = false;
					else
						first[left].insert(*it1);
				}
				if (flag)
					break;
			}
			else
				continue;
		}
	}
	it = right.begin();
	for (; it != right.end(); it++)
	{
		bool has = true;
		int cnt = 0;
		for (int i = 0; i < it->length(); i++)
		{
			if (isupper(it->at(i)))
			{
				cnt++;
				string temp;
				if (i != it->length() - 1 && it->at(i + 1) == '\'')
					temp = it->substr(i, 2);
				else
					temp = it->substr(i, 1);

				if (first[temp].find('~') == first[temp].end())
				{
					has = false;
					
					break;
				}

			}
		}
		if (has && cnt != 0)
		{
			first[left].insert('~');
			break;
		}
	}
}

void make_first()
{
	memset(used, 0, sizeof(used));
	for (int i = 0; i < VN_set.size(); i++)
		dfs(i);

#ifdef DEBUG
	map<string, set<char> >::iterator it = first.begin();
	for (; it != first.end(); it++)
	{
		printf("FIRST(%s)={", it->first.c_str());
		set<char> & temp = it->second;
		set<char>::iterator it1 = temp.begin();
		bool flag = false;
		for (; it1 != temp.end(); it1++)
		{
			if (flag) printf(",");
			printf("%c", *it1);
			flag = true;
		}
		puts("}");
	}
#endif
}

void append(const string& str1, const string& str2)
{
	set<char>& from = follow[str1];
	set<char>& to = follow[str2];
	set<char>::iterator it = from.begin();
	for (; it != from.end(); it++)
		to.insert(*it);
}

void make_follow()
{
	while (true)
	{
		bool goon = false;
		for (int i = 0; i < VN_set.size(); i++)
		{
			string& left = VN_set[i].left;
			set<string>& right = VN_set[i].right;
			set<string>::iterator it = right.begin();
			for (; it != right.end(); it++)
			{
				bool flag = true;
				const string& str = *it;
				for (int j = it->length() - 1; j >= 0; j--)
				{
					if (str[j] == '\'')
					{
						int x = VN_dic[it->substr(j - 1, 2)] - 1;
						if (flag)
						{
							int tt = follow[it->substr(j - 1, 2)].size();
							append(left, it->substr(j - 1, 2));
							int tt1 = follow[it->substr(j - 1, 2)].size();
							if (tt1 > tt) goon = true;
							if (!VN_set[x].right.count("~"))
								flag = false;
						}
						for (int k = j + 1; k < it->length(); k++)
						{
							if (isupper(str[k]))
							{
								string id;
								if (k != it->length() - 1 && str[k + 1] == '\'')
									id = it->substr(k, 2);
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j - 1, 2)];
								int tt = to.size();
								set<char>::iterator it1 = from.begin();
								for (; it1 != from.end(); it1++)
									if (*it1 != '~')
										to.insert(*it1);
								int tt1 = follow[it->substr(j - 1, 2)].size();
								if (tt1 > tt) goon = true;
								if (!VN_set[VN_dic[id] - 1].right.count("~"))
									break;
							}
							else if (str[k] != '\'')
							{
								int tt = follow[it->substr(j - 1, 2)].size();
								follow[it->substr(j - 1, 2)].insert(str[k]);
								int tt1 = follow[it->substr(j - 1, 2)].size();
								if (tt1 > tt)
									goon = true;
								break;
							}
							else continue;
						}
						j--;
					}
					else if (isupper(str[j]))
					{
						int x = VN_dic[it->substr(j, 1)] - 1;
						if (flag)
						{
							int tt = follow[it->substr(j, 1)].size();
							append(left, it->substr(j, 1));
							if (!VN_set[x].right.count("~"))
								flag = false;
							int tt1 = follow[it->substr(j, 1)].size();
							if (tt1 > tt) goon = true;
						}
						for (int k = j + 1; k < it->length(); k++)
						{
							if (isupper(str[k]))
							{
								string id;
								if (k != it->length() - 1 && str[k + 1] == '\'')
									id = it->substr(k, 2);
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j, 1)];
								set<char>::iterator it1 = from.begin();
								int tt = follow[it->substr(j, 1)].size();
								for (; it1 != from.end(); it1++)
									if (*it1 != '~')
										to.insert(*it1);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								if (!VN_set[VN_dic[id] - 1].right.count("~"))
									break;
							}
							else if (str[k] != '\'')
							{
								int tt = follow[it->substr(j, 1)].size();
								follow[it->substr(j, 1)].insert(str[k]);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								break;
							}
							else continue;
						}
					}
					else flag = false;
				}
			}
		}
		if (!goon) break;
	}

#ifdef DEBUG
	map<string, set<char> >::iterator it = follow.begin();
	for (; it != follow.end(); it++)
	{
		printf("FOLLOW(%s)={", it->first.c_str());
		set<char> & temp = it->second;
		temp.insert('$');
		set<char>::iterator it1 = temp.begin();
		bool flag = false;
		for (; it1 != temp.end(); it1++)
		{
			if (flag) printf(",");
			printf("%c", *it1);
			flag = true;
		}
		puts("}");
	}
#endif
}

vector<map<char, string> > predict_table;

//检查一个字符是否属于一个字符串的FIRST集合
bool check_first(const string& text, char ch)
{
    for (int i = 0; i < text.length(); i++)
    {
        bool hasEmpty = false;
        if (!isupper(text[i]) && text[i] != '\'')
        {
            if (text[i] != ch) return false;
            else return true;
        }
        else if (isupper(text[i]))
        {
            string temp;
            if (i != text.length() - 1 && text[i + 1] == '\'')
                temp = text.substr(i, 2);
            else
                temp = text.substr(i, 1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for (; it != dic.end(); it++)
            {
                if (*it == '~') hasEmpty = true;
                if (*it == ch) return true;
            }
            if (!hasEmpty) break;
        }
        else continue;
    }
    return false;
}

//检查一个字符是否属于一个字符串的FOLLOW集合
bool check_follow(const string& text, char ch)
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for (; it != dic.end(); it++)
        if (*it == ch) return true;
    return false;
}

void make_table()
{
    map<char, string> temp;
    vector<char> letter;
    bool vis[500];
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < VN_set.size(); i++)
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        // 寻找终结符
        for (; it != right.end(); it++)
            for (int j = 0; j < it->length(); j++)
                if (!isupper(it->at(j)) && it->at(j) != '\'' && it->at(j)!= '~')
                {
                    if (vis[it->at(j)]) continue;
                    vis[it->at(j)] = true;
                    letter.push_back(it->at(j));
                }
    }
    letter.push_back('#');
    for (int i = 0; i < VN_set.size(); i++)
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for (; it != right.end(); it++)
            for (int j = 0; j < letter.size(); j++)
            {
                if (check_first(*it, letter[j]))
                {
                    temp[letter[j]] = *it;
                }
                if (it->at(0) == '~' && check_follow(left, letter[j]))
                {
                    temp[letter[j]] = *it;
                }
                if (it->at(0) == '~')
                {
                    //TODO:
                    temp['#'] = *it;
                }
            }
        predict_table.push_back(temp);
    }
#ifdef DEBUG
    for (int i = 0; i <= (letter.size() + 1) * 10; i++)
        printf("-");
    puts("");
    printf("|%9s", "|");
    for (int i = 0; i < letter.size(); i++)
        printf("%5c%5s", letter[i], "|");
    puts("");
    for (int i = 0; i <= (letter.size() + 1) * 10; i++)
        printf("-");
    puts("");
    for (int i = 0; i < VN_set.size(); i++)
    {
        printf("|%5s%4s", VN_set[i].left.c_str(), "|");
        for (int j = 0; j < letter.size(); j++)
            if (predict_table[i].count(letter[j]))
                printf("%7s%3s", predict_table[i][letter[j]].c_str(), "|");
            else printf("%10s", "|");
            puts("");
            for (int i = 0; i <= (letter.size() + 1) * 10; i++)
                printf("-");
            puts("");
    }
#endif
}
/*
void analys(char s[])
{
    int i = 0;
    stack<string> stack;
    stack.push(VN_set[0].left.c_str());
    string x = stack.top();
    while (!stack.empty())
    {
        if (x.length() == 1)
        {
            if (*(x.c_str()) == s[i])
            {
                stack.pop();
                i++;
            }
            else if(islower(x.at(0)))
            {
                error();
            }
            else if(predict_table[VN_dic.at(x)][s[i]] == "")
            {
                error();
            }
            else
            {
                string temp = predict_table[VN_dic.at(x)][s[i]];
                for (int j = temp.length(); j > 0; j--)
                {
                    string xx;
                    if (temp.at(j) != '\'')
                        xx = temp.at(j);
                    else
                        xx = temp.at(j - 1) + temp.at(j--);
                    stack.push(xx);
                }
                cout << x << '->' << predict_table[VN_dic.at(x)][s[i]];
                stack.pop();
            }
        }
        else
        {
            if (x.at(0) == s[i] && x.at(1) == s[i+1])
            {
                stack.pop();
                i += 2;
            }
            else if (islower(x.at(0)))
            {
                error();
            }
            else if (predict_table[VN_dic.at(x)][s[i]] == "")
            {
                error();
            }
            else
            {
                string temp = predict_table[VN_dic.at(x)][s[i]];
                for (int j = temp.length(); j > 0; j--)
                {
                    string xx;
                    if (temp.at(j) != '\'')
                        xx = temp.at(j);
                    else
                        xx = temp.at(j - 1) + temp.at(j--);
                    stack.push(xx);
                }
                cout << x << '->' << predict_table[VN_dic.at(x)][s[i]];
                stack.pop();
            }
        }
        x = stack.top();
    }
}
*/

void print(int steps, stack<string> stk, string src, string wf, int x)
{
    printf("%-20d", steps);
    string out = "";
    while (!stk.empty())
    {
        out = stk.top() + out;
        stk.pop();
    }
    printf("#%-18s", out.c_str());
    out = "";
    for (int i = x; i < src.length(); i++)
        out += src[i];
    printf("%-20s", (out + "#").c_str());
    printf("%-20s\n", wf.c_str());
}

bool analyse(const string& src)
{
    bool isa = true;
    stack<string> stk;
    stk.push("E");
    int steps = 0;
    int idx = 0;
#ifdef DEBUG
    printf("%-20s%-20s%-20s%-20s\n", "步骤", "符号栈", "输入串", "所用产生式");
#endif
    while (!stk.empty())
    {
        string u = stk.top();
        string tmp = "";
        stk.pop();
        if (!isupper(u[0]))     //终结符
        {
            if (idx == src.length() && u[0] == '~');
            else if (src[idx] == u[0])
            {
                idx++;
                tmp = "match " + u;
            }
            else
            {
                cout << "an error occured! 弹栈，弹出终结符'" << u << "'" << endl;
                isa = false;
            }
        }
        else
        {
            int x = VN_dic[u] - 1;
            if (idx == src.length())
            {
                tmp = '~';
            }
            else
            {
                tmp = predict_table[x][src[idx]];
                if (tmp == "")
                {
                    isa = false;
                    if (!check_first(u, src[idx]) && !check_follow(u, src[idx]))
                    {
                        cout << "an error occured! 输入串跳过记号'" << src[idx] << "'" << endl;
                        idx++;
                        stk.push(u);
                        continue;
                    }
                    else
                        cout << "an error occured! 弹栈，弹出非终结符'" << u << "'" << endl;
                    
                }
            }
            for (int i = tmp.length() - 1; i >= 0; i--)
            {
                string v;
                if (tmp[i] == '\'')
                {
                    v = tmp.substr(i - 1, 2);
                    i--;
                }
                else
                {
                    v = tmp.substr(i, 1);
                }
                if (v == "~")
                    ;
                else
                {
                    stk.push(v);
                }
                
            }
            tmp = u + "->" + tmp;
        }
#ifdef DEBUG
        print(steps++, stk, src, tmp, idx);
#endif
    }
    return isa;
}


int main()
{
	int n;
	scanf("%d", &n);
	input(n);
	make_first();
	make_follow();
    make_table();
    string s;
    while (1)
    {
        cout << "分析式为:";
        cin >> s;
        if (analyse(s))
        {
            cout << "此表达式是该文法生成的语句。" << endl;
        }
        else
        {
            cout << "在生成过程中出现了错误，可能是输入出错。" << endl;
        }
    }
	system("pause");
	return 0;
}

