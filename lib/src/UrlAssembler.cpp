#include "UrlAssembler.hpp"
#include "MPDManager.hpp"
string UrlAssembler::Replace(const string source, const string subString, const string replacingString)
{
	string result = source;
	string::size_type found = result.find(subString);
	if(found != string::npos) {
		result.replace(found, subString.size(), replacingString);
	} else {
		result = "DidNotFound";
	}
	return result;

}

URLList *UrlAssembler::assembleURLsIFSegmentBase(MPD *mpd, string m_url)
{
	URLList *result= new URLList;
	std::list<Period *>::iterator currentPeriod;
	string base="";
	if(!mpd->listBaseURL.empty())
        base+=(*(mpd->listBaseURL.begin()))->URL;
    else{
            base+=m_url; // удалить кусок сконца!
            while(base[base.length()-1]!='/')
            base.pop_back();
    }
	for(currentPeriod = mpd->period.begin(); currentPeriod != mpd->period.end(); currentPeriod++) {
        if(!(*currentPeriod)->listBaseURL.empty())
            base+=(*((*currentPeriod)->listBaseURL.begin()))->URL;
		if(!((*currentPeriod)->adaptationset.empty())) {
			std::list<AdaptationSet *>::iterator currentASet;
			for(currentASet = (*currentPeriod)->adaptationset.begin(); currentASet != (*currentPeriod)->adaptationset.end(); currentASet++) {
				if(!(*currentASet)->listBaseURL.empty())
                    base+=(*((*currentASet)->listBaseURL.begin()))->URL;
				if(!((*currentASet)->listRepresentation.empty())) {
					std::list<Representation *>::iterator currentRepres;
					for(currentRepres = (*currentASet)->listRepresentation.begin(); currentRepres != (*currentASet)->listRepresentation.end(); currentRepres++) {
						string newUrl;
						if(!(*currentRepres)->BaseURL.empty())
                            newUrl=base+(*((*currentRepres)->BaseURL.begin()))->URL;
						if(!mpd->listBaseURL.empty()){
                            string newUrlWithByteRange = (*(mpd->listBaseURL.begin()))->byteRange;
                            if(newUrl.find("$base$") != string::npos) {
                                newUrlWithByteRange = Replace(newUrlWithByteRange, "$base$", newUrl);
                            }
                            if(newUrl.find("$first$-$last$") != string::npos) {
                                newUrlWithByteRange = Replace(newUrlWithByteRange, "$first$-$last$", (*currentRepres)->SegmentBase->indexRange);
                            }
                            newUrl = newUrlWithByteRange;
						}
						result->AddURL(newUrl);
					}
				}
			}
		}
		 else
		 {
            throw "Invalid MPD. There are no periods!";
		 }

	}
	return result;
}

URLList *UrlAssembler::assembleURLsIFSegmentTemplate(MPD *mpd)
{
	URLList *result;
	std::list<Period *>::iterator currentPeriod;
	for(currentPeriod = mpd->period.begin(); currentPeriod != mpd->period.end(); currentPeriod++) {
		if(!((*currentPeriod)->adaptationset.empty())) {
			std::list<AdaptationSet *>::iterator currentASet;
			for(currentASet = (*currentPeriod)->adaptationset.begin(); currentASet != (*currentPeriod)->adaptationset.end(); currentASet++) {
				if(!((*currentASet)->listRepresentation.empty())) {
					std::list<Representation *>::iterator currentRepres;
					int i = 0;
					for(currentRepres = (*currentASet)->listRepresentation.begin(); currentRepres != (*currentASet)->listRepresentation.end(); currentRepres++, i++) {
						string newUrl = (*(mpd->listBaseURL.begin()))->URL +
										(*((*currentPeriod)->listBaseURL.begin()))->URL +
										(*((*currentASet)->listBaseURL.begin()))->URL +
										(*currentASet)->SegmentTemplate->media;
						if(newUrl.find("$RepresentationID$") != string::npos) {
							Replace(newUrl, "$RepresentationID$", (*currentRepres)->id);
						}
						if(newUrl.find("$Number$") != string::npos) {
							uint32_t number = i + (*currentRepres)->SegmentTemplate->m_base->startNumber;
							Replace(newUrl, "$Number$", std::to_string(number));
						} else if(newUrl.find("$Time$") != string::npos) {
							uint32_t number = i + (*currentRepres)->SegmentTemplate->m_base->startNumber;
							Replace(newUrl, "$Time$", std::to_string(number * (*currentRepres)->SegmentTemplate->m_base->duration));
						}
						if(newUrl.find("$Bandwidth$") != string::npos) {
							Replace(newUrl, "$Bandwidth$", std::to_string((*currentRepres)->bandwidth));
						}
						result->AddURL(newUrl);
					}
				}
			}
		}
		// else
		// {
		// 	//error -> invalid mpd
		// }

	}
	return result;
}

URLList *UrlAssembler::assembleURLsIFSegmentList(MPD *mpd)
{
	URLList *result;
	std::list<Period *>::iterator currentPeriod;
	for(currentPeriod = mpd->period.begin(); currentPeriod != mpd->period.end(); currentPeriod++) {
		if(!((*currentPeriod)->adaptationset.empty())) {
			std::list<AdaptationSet *>::iterator currentASet;
			for(currentASet = (*currentPeriod)->adaptationset.begin(); currentASet != (*currentPeriod)->adaptationset.end(); currentASet++) {
				if(!((*currentASet)->listRepresentation.empty())) {
					std::list<Representation *>::iterator currentRepres;
					for(currentRepres = (*currentASet)->listRepresentation.begin(); currentRepres != (*currentASet)->listRepresentation.end(); currentRepres++) {
						std::list<SegmentURLType *>::iterator currentUrl;
						for(currentUrl = (*currentRepres)->SegmentList->SegmentURLs.begin(); currentUrl != (*currentRepres)->SegmentList->SegmentURLs.end(); currentUrl++) {
							result->AddURL((*(mpd->listBaseURL.begin()))->URL +
										   (*((*currentPeriod)->listBaseURL.begin()))->URL +
										   (*((*currentASet)->listBaseURL.begin()))->URL +
										   (*currentUrl)->media);
						}
					}
				}
			}
		}
		// else
		// {
		// 	//error -> invalid mpd
		// }

	}
	return result;

}
