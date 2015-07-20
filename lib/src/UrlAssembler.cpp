#include "UrlAssembler.hpp"
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

URLList *UrlAssembler::assembleURLsIFSegmentBase(MPD *mpd)
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
						string base;
						//if(((*(mpd->listBaseURL.begin()))->URL).empty())
						base=(*((*currentRepres)->BaseURL.begin()))->URL;//(*(mpd->listBaseURL.begin()))->URL ;//+ (*((*currentPeriod)->listBaseURL.begin()))->URL + (*((*currentASet)->listBaseURL.begin()))->URL + ;
//						string newUrl = (*(mpd->listBaseURL.begin()))->byteRange;
//						if(newUrl.find("$base$") != string::npos) {
//							newUrl = Replace(newUrl, "$base$", base);
//						}
//						if(newUrl.find("$first$-$last$") != string::npos) {
//							newUrl = Replace(newUrl, "$first$-$last$", (*currentRepres)->SegmentBase->indexRange);
//						}
//						result->AddURL(newUrl);
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
