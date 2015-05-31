#include "tegger.hpp"
ull tegger::cash(string s)
{
	ull res = 0;
	ull p = 1;
	for(int i = 0; i < sz(s); ++i) {
		res += (s[i] - 'a' + 1) * p;
		p *= base;
	}
	return res;
}
void tegger::clear()
{
	dict.clear();
}
void tegger::setTeg(string name, string value)
{
	ull hsh = cash(name); // string to int
	dict[hsh] = value;
}

string tegger::tegChange(string str)
{
	string res;
	int i = 0;
	while(i < sz(str)) {
		if(str[i] != '$') {
			res = res + str[i];
			++i;
		} else {
			int idstart = i;
			int idfinish = i + 1;
			while(idfinish < sz(str) && str[idfinish] != '$') {
				++idfinish;
			}
			if(idfinish < sz(str)) {
				string foundtag = str.substr(idstart + 1, idfinish - idstart - 1);
				//cout<<foundtag;
				ull tegcsh = cash(foundtag);
				if(dict.count(tegcsh)) {
					res += dict[tegcsh];
					i = idfinish + 1;
				} else {
					res = res + str[i];
					++i;
				}
			} else {
				res = res + str[i];
				++i;
			}
		}
	}
	return res;
}
