#include "UrlAssembler.hpp"
uint64_t UrlAssembler::cash(string s)
{
	uint64_t res = 0;
	uint64_t p = 1;
	for(int i = 0; i < sz(s); ++i) {
		res += (s[i] - 'a' + 1) * p;
		p *= base;
	}
	return res;
}
void UrlAssembler::clear()
{
	dict.clear();
	setTeg("","$");// Change "$$" to "$"
}
void UrlAssembler::setTeg(string name, string value)
{
	uint64_t hsh = cash(name); // string to int
	dict[hsh] = value;
}

std::string UrlAssembler::tegChange(string str)
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
				uint64_t tegcsh = cash(foundtag);
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

void UrlAssembler::AssembleURLs(MPDStruct::MPD* mpd)
{
	//if baseURL
	{
		std::list<Period *>::iterator currentPeriod;
		for(currentPeriod = mpd.period.begin(); currentPeriod != mpd.period.end(); currentPeriod++)
		{
			if(!currentPeriod.empty())
			{
				std::list<AdaptationSet *>::iterator currentASet;
				for(currentASet = mpd.period.begin(); currentASet != mpd.period.end(); currentASet++)
				{
					if(currentASet.empty())
					{
						std::list<Representation *>::iterator currentRepres;
						for(currentRepres = mpd.period.begin(); currentRepres != mpd.period.end(); currentRepres++)
						{
							if(!currentRepres.empty())
							{

							}
							else
							{
								string newUrl = new string(mpd.listBaseUrl.begin().URL + currentPeriod.listBaseUrl.begin().URL + currentRepres.listBaseUrl.begin().URL);
								//URLList::AddURL(newURL);
							}
						}
					}
					else
					{
						string newUrl = new string(mpd.listBaseUrl.begin().URL + currentPeriod.listBaseUrl.begin().URL);
						//URLList::AddURL(newURL);
					}
					
				}
			}
			else
			{
				//error -> invalid mpd
			}
			
		}
	}
}
