#include "URLList.hpp"
uint64_t URLList::getHash(std::string srcstr) {
	const uint32_t base = 53;
	uint64_t res = 0;
	uint64_t p = 1;
	for(int i = 0; i < (uint32_t)(srcstr.size()); ++i) {
		res += (srcstr[i] - 'a' + 1) * p;
		p *= base;
	}
	return res;
}
bool URLList::isEmpty() {
	return SegmentURLs.empty();
}
void URLList::AddURL(std::string newURL) {
	SegmentURLs.push_back(newURL);
	hashList.push_back(getHash(newURL));
}
void URLList::RemoveUrl() {
	SegmentURLs.pop_front();
	hashList.pop_front();
}
void URLList::Clear() {
	SegmentURLs.clear();
	hashList.clear();
}
void URLList::Sync(std::list<std::string> checklist) {
	std::list<std::string>::iterator itURL;
	if(isEmpty()) {
		for(itURL = checklist.begin(); itURL != checklist.end(); ++itURL) {
			AddURL(*itURL);
		}
		return;
	}

	//prepare
	std::list<uint64_t> checkHash;
	for(std::list<std::string>::iterator it = checklist.begin(); it != checklist.end(); ++it) {
		checkHash.push_back(getHash(*it));
	}
	std::list<uint64_t>::iterator itcheck, itlist;

	// ADDING in tail
	for(itcheck = checkHash.begin(), itURL = checklist.begin(); itcheck != checkHash.end(); ++itcheck, ++itURL) {
		bool found = false;
		for(itlist = hashList.begin(); itlist != hashList.end(); ++itlist)
			if(*itcheck == *itlist) {
				found = true;
				break;
			}
		if(!found) {
			AddURL(*itURL);
		}
	}
	// REMOVING from head
	bool lost = false;
	do {
		itcheck = checkHash.begin();
		itlist = hashList.begin();
		lost = (*itcheck != *itlist);
		if(lost)	{
			RemoveUrl();
		}
	}
	while(lost && !isEmpty());

}
