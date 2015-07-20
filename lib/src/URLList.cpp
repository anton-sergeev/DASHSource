/*
 lib/src/URLList.cpp

Copyright (c) 2015, Elecard Multimedia School
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "URLList.hpp"

URLList *g_URLList = NULL;

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
	return listURL.empty();
}

void URLList::AddURL(std::string newURL) {
	SegmentComplexType tmp;
	tmp.sURL = newURL;
	tmp.hash = getHash(newURL);
	tmp.flag = false;
	listURL.push_back(tmp);
}

void URLList::RemoveUrl() {
	listURL.pop_front();
}

void URLList::Clear() {
	listURL.clear();
}

void URLList::Sync(std::list<SegmentComplexType> checklist) {
	std::list<SegmentComplexType>::iterator itURL1;
	std::list<SegmentComplexType>::iterator itURL2;
	if(isEmpty()) {
		for(itURL1 = checklist.begin(); itURL1 != checklist.end(); ++itURL1) {
			AddURL(itURL1->sURL);
		}
		return;
	}

	// ADDING in tail
	bool found;
	for(itURL1 = checklist.begin(); itURL1 != checklist.end(); ++itURL1) {
		found = false;
		for(itURL2 = listURL.begin(); (itURL2 != listURL.end()) && (!found); ++itURL2) {
			if(itURL1->hash == itURL2->hash) {
				found = true;
			}
		}
		if(!found) {
			AddURL(itURL1->sURL);
		}
	}
	
	// REMOVING from head
	bool lost = false;
	do {
		itURL1 = checklist.begin();
		itURL2 = listURL.begin();
		lost = (itURL1->hash != itURL2->hash);
		if(lost)	{
			RemoveUrl();
		}
	}
	while(lost && !isEmpty());

}
