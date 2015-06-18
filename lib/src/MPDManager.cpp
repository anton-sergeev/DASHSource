/*
 lib/src/MPDManager.cpp

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

/******************************************************************
* INCLUDE FILES                                                   *
*******************************************************************/
#include "MPDManager.hpp"
#include <string>
#include "CurlReceiver.hpp"
#include "IHTTPReceiver.hpp"
/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/

/******************************************************************
* LOCAL MACROS                                                    *
*******************************************************************/

#define GETUintAttr(obj,attr) QueryUnsignedAttribute(#attr,&(obj->attr))
#define sz(x) ((int)x.size())

MPDManager::MPDManager()
	: m_url()
{

}

MPDManager::~MPDManager()
{
}


bool MPDManager::Start(std::string &url)
{
	m_url = url;
	IHTTPReceiver* curl_receiver = IHTTPReceiver::Instance();
	curl_receiver -> Init();
	curl_receiver -> Get(m_url, NULL);
	std::string filename = "OfForestAndMen.zip"; // For example
	//MPDFile = new XMLDocument();
	//return MPDFile->LoadFile(filename.c_str());
	return true;
}

bool MPDManager::Stop()
{
	//stop thread
	return true;
}

bool MPDManager::CreateMPDStruct(tinyxml2::XMLElement *XMLRootElement) {
	if(!XMLRootElement)
		return false;
	MPD *newMPD = new MPD;
	if(XMLRootElement->Attribute("id")) {
		newMPD->id = XMLRootElement->Attribute("id");
	}
	if(XMLRootElement->Attribute("profiles")) {
		newMPD->profiles = XMLRootElement->Attribute("profiles");
	}
	if(XMLRootElement->Attribute("type")) {
		newMPD->type = XMLRootElement->Attribute("type");
	}
	if(XMLRootElement->Attribute("availabilityStartTime")) {
		newMPD->availabilityStartTime = XMLRootElement->Attribute("availabilityStartTime");
	}
	if(XMLRootElement->Attribute("availabilityEndTime")) {
		newMPD->availabilityEndTime = XMLRootElement->Attribute("availabilityEndTime");
	}
	if(XMLRootElement->Attribute("publishTime")) {
		newMPD->publishTime = XMLRootElement->Attribute("publishTime");
	}
	if(XMLRootElement->Attribute("mediaPresentationDuration")) {
		newMPD->mediaPresentationDuration = getTimeFromDuration(XMLRootElement->Attribute("mediaPresentationDuration"));
	}
	if(XMLRootElement->Attribute("minimumUpdatePeriod")) {
		newMPD->minimumUpdatePeriod = getTimeFromDuration(XMLRootElement->Attribute("minimumUpdatePeriod"));
	}
	if(XMLRootElement->Attribute("minBufferTime")) {
		newMPD->minBufferTime = getTimeFromDuration(XMLRootElement->Attribute("minBufferTime"));
	}
	if(XMLRootElement->Attribute("timeShiftBufferDepth")) {
		newMPD->timeShiftBufferDepth = getTimeFromDuration(XMLRootElement->Attribute("timeShiftBufferDepth"));
	}
	if(XMLRootElement->Attribute("suggestedPresentationDelay")) {
		newMPD->suggestedPresentationDelay = getTimeFromDuration(XMLRootElement->Attribute("suggestedPresentationDelay"));
	}
	if(XMLRootElement->Attribute("maxSegmentDuration")) {
		newMPD->maxSegmentDuration = getTimeFromDuration(XMLRootElement->Attribute("maxSegmentDuration"));
	}
	if(XMLRootElement->Attribute("maxSubsegmentDuration")) {
		newMPD->maxSubsegmentDuration = getTimeFromDuration(XMLRootElement->Attribute("maxSubsegmentDuration"));
	}
	for(tinyxml2::XMLElement *child_element = XMLRootElement->FirstChildElement("Period"); child_element; child_element = child_element->NextSiblingElement("Period")) {
		Period *newPeriod = CreatePeriod(child_element);
		newMPD->period.push_back(newPeriod);
	}
	for(tinyxml2::XMLElement *child_element = XMLRootElement->FirstChildElement("BaseURL"); child_element; child_element = child_element->NextSiblingElement("BaseURL")) {
		BaseURLType *newBaseURL = new BaseURLType;
		if(child_element->Attribute("serviceLocation")) {
			newBaseURL->serviceLocation = child_element->Attribute("serviceLocation");
		}
		if(child_element->Attribute("byteRange"))
			newBaseURL->byteRange = child_element->Attribute("byteRange");
		newBaseURL->URL = child_element->GetText();
		newMPD->listBaseURL.push_back(newBaseURL);
	}
	return true;
}

Period *MPDManager::CreatePeriod(tinyxml2::XMLElement *element) {
	Period *newPeriod = new Period;
	if(element->Attribute("id")) {
		newPeriod->id = element->Attribute("id");
	}
	if(element->Attribute("start")) {
		newPeriod->start = getTimeFromDuration(element->Attribute("start"));
	}
	if(element->Attribute("duration")) {
		newPeriod->duration = getTimeFromDuration(element->Attribute("duration"));
	}
	if(element->Attribute("bitstreamSwitching")) {
		element->QueryBoolAttribute("bitstreamSwitching", &(newPeriod->bitstreamSwitching));
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("EventStream"); child_element; child_element = child_element->NextSiblingElement("EventStream")) {
		EventStream *newEventStream = CreateEventStream(child_element);
		newPeriod->eventstream.push_back(newEventStream);
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("AdaptationSet"); child_element; child_element = child_element->NextSiblingElement("AdaptationSet")) {
		AdaptationSet *newAdaptationSet = CreateAdaptationSet(child_element);
		newPeriod->adaptationset.push_back(newAdaptationSet);
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("BaseURL"); child_element; child_element = child_element->NextSiblingElement("BaseURL")) {
		BaseURLType *newBaseURL = new BaseURLType;
		if(child_element->Attribute("serviceLocation")) {
			newBaseURL->serviceLocation = child_element->Attribute("serviceLocation");
		}
		if(child_element->Attribute("byteRange"))
			newBaseURL->byteRange = child_element->Attribute("byteRange");
		newBaseURL->URL = child_element->GetText();
		newPeriod->listBaseURL.push_back(newBaseURL);
	}
	return newPeriod;
}


AdaptationSet *MPDManager::CreateAdaptationSet(tinyxml2::XMLElement *element)
{
	tinyxml2::XMLElement* curNodeSet=element;
	AdaptationSet *newAdaptationSet = new AdaptationSet();
	newAdaptationSet->m_base = CreateRepresentationBaseType(element);
	if(element->Attribute("id")) {
		newAdaptationSet->id = atoi(element->Attribute("id"));
	}
	if(element->Attribute("group")) {
		newAdaptationSet->group = atoi(element->Attribute("group"));
	}
	if(element->Attribute("minBandwidth")) {
		newAdaptationSet->minBandwidth = atoi(element->Attribute("minBandwidth"));
	}
	if(element->Attribute("maxBandwidth")) {
		newAdaptationSet->maxBandwidth = atoi(element->Attribute("maxBandwidth"));
	}
	if(element->Attribute("minWidth")) {
		newAdaptationSet->minWidth = atoi(element->Attribute("minWidth"));
	}
	if(element->Attribute("maxWidth")) {
		newAdaptationSet->maxWidth = atoi(element->Attribute("maxWidth"));
	}
	if(element->Attribute("minHeight")) {
		newAdaptationSet->minHeight = atoi(element->Attribute("minHeight"));
	}
	if(element->Attribute("maxHeight")) {
		newAdaptationSet->maxHeight = atoi(element->Attribute("maxHeight"));
	}
	if(element->Attribute("subsegmentStartsWithSAP")) {
		newAdaptationSet->subsegmentStartsWithSAP = atoi(element->Attribute("subsegmentStartsWithSAP"));
	}
	if(element->Attribute("bitstreamSwitching")) {
		element->QueryBoolAttribute("bitstreamSwitching", &(newAdaptationSet->bitstreamSwitching));
	}
	if(element->Attribute("segmentAlignment")) {
		element->QueryUnsignedAttribute("segmentAlignment", (unsigned int *) & (newAdaptationSet->segmentAlignment));
	}
	if(element->Attribute("subsegmentAlignment")) {
		element->QueryUnsignedAttribute("subsegmentAlignment", (unsigned int *) & (newAdaptationSet->subsegmentAlignment));
	}
	//curNodeSet->QueryUnsignedAttribute("lang",(unsigned int *)&(curSet.language));
	if(element->Attribute("contentType")) {
		newAdaptationSet->contentType = element->Attribute("contentType");
	}
	if(element->Attribute("maxFrameRate")) {
		newAdaptationSet->maxFrameRate.frameRate = element->Attribute("maxFrameRate");
	}
	if(element->Attribute("minFrameRate")) {
		newAdaptationSet->minFrameRate.frameRate = element->Attribute("minFrameRate");
	}
	if(element->Attribute("par")) {
		newAdaptationSet->pictureAspectRatio = RatioType(element->Attribute("par"));
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("BaseURL"); child_element; child_element = child_element->NextSiblingElement("BaseURL")) {
		BaseURLType *newBaseURL = new BaseURLType;
		if(child_element->Attribute("serviceLocation")) {
			newBaseURL->serviceLocation = child_element->Attribute("serviceLocation");
		}
		if(child_element->Attribute("byteRange"))
			newBaseURL->byteRange = child_element->Attribute("byteRange");
		newBaseURL->URL = child_element->GetText();
		newAdaptationSet->listBaseURL.push_back(newBaseURL);
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("Representation"); child_element; child_element = child_element->NextSiblingElement("Representation")) {
		Representation *newRepresentation = CreateRepresentation(child_element);
		newAdaptationSet->listRepresentation.push_back(newRepresentation);
	}
	return newAdaptationSet;
}

Representation *MPDManager::CreateRepresentation(tinyxml2::XMLElement *element) {
	Representation *curRepr = new Representation;
	curRepr->m_base = CreateRepresentationBaseType(element);
	element->GETUintAttr(curRepr,bandwidth);
	element->GETUintAttr(curRepr,qualityRanking);
	const char * att;
	tinyxml2::XMLElement *childURL;
	if(childURL=element->FirstChildElement("BaseURL")){

		att=childURL->GetText();
		curRepr->BaseURL.serviceLocation=att;
	}
	if(att=element->Attribute("id"))
		curRepr->id = att;
	if(att=element->Attribute("dependencyId"))
		curRepr->dependencyId.push_back(std::string(att));
	if(att=element->Attribute("mediaStreamStructureId"))
		curRepr->mediaStreamStructureId.push_back(std::string(att));
	return NULL; //TODO: change this return!!!!!!!!!
}

RepresentationBaseType *MPDManager::CreateRepresentationBaseType(tinyxml2::XMLElement *element)
{
	RepresentationBaseType *fillRepr = new RepresentationBaseType;
	element->GETUintAttr(fillRepr,width);
	element->GETUintAttr(fillRepr,height);
	element->GETUintAttr(fillRepr,startWithSAP);

	element->QueryDoubleAttribute("maximumSAPPeriod",&(fillRepr->maximumSAPPeriod));
	element->QueryDoubleAttribute("maxPlayoutRate",&(fillRepr->maxPlayoutRate));

	element->QueryBoolAttribute("codingDependency", &(fillRepr->codingDependency));

	const char * att;
	if(att=element->Attribute("audioSamplingRate"))
		fillRepr->audioSamplingRate = att;
	if(att=element->Attribute("mimeType"))
		fillRepr->mimeType = att;
	if(att=element->Attribute("segmentProfiles"))
		fillRepr->segmentProfiles = att;
	if(att=element->Attribute("codecs"))
		fillRepr->codecs = att;
	if(att=element->Attribute("profiles"))
		fillRepr->profiles = att;
	if(att=element->Attribute("scanType"))
		fillRepr->scanType = att;
	//if(att=element->Attribute("frameRate"))
		//fillRepr->frameRate.frameRate = att;
		//***
	if(att = element->Attribute("sar"))
		fillRepr->sar = RatioType(std::string(att));
	return fillRepr;    
}

EventStream *MPDManager::CreateEventStream(tinyxml2::XMLElement *element)
{
	EventStream *newEventStream = new EventStream;
	if(element->Attribute("schemeIdUri")) {
		newEventStream->schemeIdUri = element->Attribute("schemeIdUri");
	}
	if(element->Attribute("value")) {
		newEventStream->value = element->Attribute("value");
	}
	if(element->Attribute("timescale")) {
		newEventStream->timescale = atol(element->Attribute("timescale"));
	}
	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("Event"); child_element; child_element = child_element->NextSiblingElement("Event")) {
		Event *newEvent = new Event;
		if(child_element->Attribute("presentationTime")) {
			newEvent->presentationTime = atol(child_element->Attribute("presentationTime"));
		}
		if(child_element->Attribute("duration")) {
			newEvent->duration = atol(child_element->Attribute("duration"));
		}
		if(child_element->Attribute("id")) {
			newEvent->id = atoi(child_element->Attribute("id"));
		}
		newEventStream->event.push_back(newEvent);
	}
	return newEventStream;
}

bool MPDManager::ThreadLoop()
{
	// loop {
	//  1. get MPD via IHTTPReceiver
	//  2. parse MPD passing it into tinyxml
	//  3. fill structures: Representation, Segments, e.g.
	// }

	return true;
}

long int MPDManager::getTimeFromDuration(std::string str) {
	
	double hours = 0, minutes = 0, seconds = 0;
	
	std::string tmp;
	for(int i = 2; i < str.size(); i++) {
		
		tmp = "";
		for(; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++) {
			tmp += str[i];
		}
	
		if(str[i] == 'H')
			hours = atof(tmp.c_str());
		if(str[i] == 'M')
			minutes = atof(tmp.c_str());
		if(str[i] == 'S')
			seconds = atof(tmp.c_str());
	}	
	
	return ((hours * 60 * 60 * 1000) + (minutes * 60 * 1000) + (seconds * 1000));
}

// std::list<DASHRepresentation> MPDManager::GetRepresentationList(void)
// {
// 	return std::list;
// }
