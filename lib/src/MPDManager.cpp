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
#include <thread>
#include <unistd.h>//sleep(), need to replace by portable analog
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
	m_alive = true;
	m_thr = new std::thread(&MPDManager::ThreadLoop, this);

	//TO DO Solve problem about start of parsing
	//std::string filename = "OfForestAndMen.zip"; // For example
	//MPDFile = new XMLDocument();
	//return MPDFile->LoadFile(filename.c_str());
	return true;
}

bool MPDManager::Stop()
{
	//stop thread
	m_alive = false;
	return true;
}

bool MPDManager::IsLive(){
	if (!mpd || mpd->type == "static"){
		return false;
	}
	return true;
}

bool MPDManager::CreateMPDStruct(tinyxml2::XMLElement *XMLRootElement) {
	if(!XMLRootElement)
		return false;

	mpd = new MPD;

	if(XMLRootElement->Attribute("id")) {
		mpd->id = XMLRootElement->Attribute("id");
	}
	if(XMLRootElement->Attribute("profiles")) {
		std::string prof = XMLRootElement->Attribute("profiles");
		if (prof == "urn:mpeg:dash:profile:full:2011") {
			mpd->profile = eProfiles_full;
			}
		else if (prof == "urn:mpeg:dash:profile:isoff-on-demand:2011") {
			mpd->profile = eProfiles_on_demand;
		}
		else if (prof == "urn:mpeg:dash:profile:isoff-live:2011") {
			mpd->profile = eProfiles_live;
		}
		else if (prof == "urn:mpeg:dash:profile:isoff-main:2011") {
			mpd->profile = eProfiles_main;
		}
		else if (prof == "urn:mpeg:dash:profile:mp2t-main:2011") {
			mpd->profile = eProfiles_mpeg_2_ts_main;
		}
		else if (prof == "urn:mpeg:dash:profile:mp2t-simple:2011") {
			mpd->profile = eProfiles_mpeg_2_ts_simple;
		}
	}
	if(XMLRootElement->Attribute("type")) {
		mpd->type = XMLRootElement->Attribute("type");
	}
	if(XMLRootElement->Attribute("availabilityStartTime")) {
		mpd->availabilityStartTime = XMLRootElement->Attribute("availabilityStartTime");
	}
	if(XMLRootElement->Attribute("availabilityEndTime")) {
		mpd->availabilityEndTime = XMLRootElement->Attribute("availabilityEndTime");
	}
	if(XMLRootElement->Attribute("publishTime")) {
		mpd->publishTime = XMLRootElement->Attribute("publishTime");
	}
	if(XMLRootElement->Attribute("mediaPresentationDuration")) {
		mpd->mediaPresentationDuration = getTimeFromDuration(XMLRootElement->Attribute("mediaPresentationDuration"));
	}
	if(XMLRootElement->Attribute("minimumUpdatePeriod")) {
		mpd->minimumUpdatePeriod = getTimeFromDuration(XMLRootElement->Attribute("minimumUpdatePeriod"));
	}
	if(XMLRootElement->Attribute("minBufferTime")) {
		mpd->minBufferTime = getTimeFromDuration(XMLRootElement->Attribute("minBufferTime"));
	}
	if(XMLRootElement->Attribute("timeShiftBufferDepth")) {
		mpd->timeShiftBufferDepth = getTimeFromDuration(XMLRootElement->Attribute("timeShiftBufferDepth"));
	}
	if(XMLRootElement->Attribute("suggestedPresentationDelay")) {
		mpd->suggestedPresentationDelay = getTimeFromDuration(XMLRootElement->Attribute("suggestedPresentationDelay"));
	}
	if(XMLRootElement->Attribute("maxSegmentDuration")) {
		mpd->maxSegmentDuration = getTimeFromDuration(XMLRootElement->Attribute("maxSegmentDuration"));
	}
	if(XMLRootElement->Attribute("maxSubsegmentDuration")) {
		mpd->maxSubsegmentDuration = getTimeFromDuration(XMLRootElement->Attribute("maxSubsegmentDuration"));
	}
	for(tinyxml2::XMLElement *child_element = XMLRootElement->FirstChildElement("Period"); child_element; child_element = child_element->NextSiblingElement("Period")) {
		Period *newPeriod = CreatePeriod(child_element);
		mpd->period.push_back(newPeriod);
	}
	for(tinyxml2::XMLElement *child_element = XMLRootElement->FirstChildElement("BaseURL"); child_element; child_element = child_element->NextSiblingElement("BaseURL")) {
		BaseURLType *newBaseURL = new BaseURLType;
		if(child_element->Attribute("serviceLocation")) {
			newBaseURL->serviceLocation = child_element->Attribute("serviceLocation");
		}
		if(child_element->Attribute("byteRange"))
			newBaseURL->byteRange = child_element->Attribute("byteRange");
		newBaseURL->URL = child_element->GetText();
		mpd->listBaseURL.push_back(newBaseURL);
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
	if(element->FirstChildElement("SegmentBase")) {
		newPeriod->SegmentBase = CreateSegmentBaseType(element->FirstChildElement("SegmentBase"));
	}
	if(element->FirstChildElement("SegmentList")) {
		newPeriod->SegmentList = CreateSegmentListType(element->FirstChildElement("SegmentList"));
	}
	if(element->FirstChildElement("SegmentTemplate")) {
		newPeriod->SegmentTemplate = CreateSegmentTemplateType(element->FirstChildElement("SegmentList"));
	}
	return newPeriod;
}


AdaptationSet *MPDManager::CreateAdaptationSet(tinyxml2::XMLElement *element)
{
	tinyxml2::XMLElement* curNodeSet=element;
	AdaptationSet *newAdaptationSet = new AdaptationSet();
	newAdaptationSet->m_base = CreateRepresentationBaseType(element);

	element->QueryUnsignedAttribute("id", &(newAdaptationSet->id));

	// #define GETUintAttr(obj,attr) QueryUnsignedAttribute(#attr,&(obj->attr))

	// Given an attribute name, QueryIntAttribute() returns XML_NO_ERROR,
	// if the conversion can't be performed, or XML_NO_ATTRIBUTE if the attribute doesn't exist.

	// If successful, the result of the conversion will be written to 'value'.
	// If not successful, nothing will be written to 'value'.
	//
	curNodeSet->GETUintAttr(newAdaptationSet, group);
	curNodeSet->GETUintAttr(newAdaptationSet, minBandwidth);
	curNodeSet->GETUintAttr(newAdaptationSet, maxBandwidth);
	curNodeSet->GETUintAttr(newAdaptationSet, minWidth);
	curNodeSet->GETUintAttr(newAdaptationSet, maxWidth);
	curNodeSet->GETUintAttr(newAdaptationSet, minHeight);
	curNodeSet->GETUintAttr(newAdaptationSet, maxHeight);
	curNodeSet->GETUintAttr(newAdaptationSet, subsegmentStartsWithSAP);
	curNodeSet->QueryBoolAttribute("bitstreamSwitching", &(newAdaptationSet->bitstreamSwitching));
	curNodeSet->QueryUnsignedAttribute("segmentAlignment", (unsigned int *) & (newAdaptationSet->segmentAlignment));
	curNodeSet->QueryUnsignedAttribute("subsegmentAlignment", (unsigned int *) & (newAdaptationSet->subsegmentAlignment));
	//curNodeSet->QueryUnsignedAttribute("lang",(unsigned int *)&(newAdaptationSet.language));
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
	if(element->FirstChildElement("SegmentBase")) {
		newAdaptationSet->SegmentBase = CreateSegmentBaseType(element->FirstChildElement("SegmentBase"));
	}
	if(element->FirstChildElement("SegmentList")) {
		newAdaptationSet->SegmentList = CreateSegmentListType(element->FirstChildElement("SegmentList"));
	}
	if(element->FirstChildElement("SegmentTemplate")) {
		newAdaptationSet->SegmentTemplate = CreateSegmentTemplateType(element->FirstChildElement("SegmentList"));
	}
	return newAdaptationSet;
}

Representation *MPDManager::CreateRepresentation(tinyxml2::XMLElement *element) {
	Representation *curRepr = new Representation;
	curRepr->m_base = CreateRepresentationBaseType(element);
	element->GETUintAttr(curRepr,bandwidth);
	element->GETUintAttr(curRepr,qualityRanking);
	const char * att;

	for(tinyxml2::XMLElement *child_element = element->FirstChildElement("BaseURL"); child_element; child_element = child_element->NextSiblingElement("BaseURL")) {
		BaseURLType *newBaseURL = new BaseURLType;
		if(child_element->Attribute("serviceLocation")) {
			newBaseURL->serviceLocation = child_element->Attribute("serviceLocation");
		}
		if(child_element->Attribute("byteRange"))
			newBaseURL->byteRange = child_element->Attribute("byteRange");
		newBaseURL->URL = child_element->GetText();
		curRepr->BaseURL.push_back(newBaseURL);
	}
	if(att=element->Attribute("id"))
		curRepr->id = att;
	if(att=element->Attribute("dependencyId"))
		curRepr->dependencyId.push_back(std::string(att));
	if(att=element->Attribute("mediaStreamStructureId"))
		curRepr->mediaStreamStructureId.push_back(std::string(att));

	if(element->FirstChildElement("SegmentBase")) {
		curRepr->SegmentBase = CreateSegmentBaseType(element->FirstChildElement("SegmentBase"));
	}
	if(element->FirstChildElement("SegmentList")) {
		curRepr->SegmentList = CreateSegmentListType(element->FirstChildElement("SegmentList"));
	}
	if(element->FirstChildElement("SegmentTemplate")) {
		curRepr->SegmentTemplate = CreateSegmentTemplateType(element->FirstChildElement("SegmentList"));
	}
	return curRepr;
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

bool MPDManager::GetMPD(IHTTPReceiver *http_receiver)
{
	std::string MPD_content;

	//  1. get MPD via IHTTPReceiver
	//  2. parse MPD passing it into tinyxml
	//  3. fill structures: Representation, Segments, e.g.
	http_receiver->Get(m_url, MPD_content);
	MPDFile = new tinyxml2::XMLDocument();
	MPDFile->Parse(MPD_content.c_str());

	if(!CreateMPDStruct(MPDFile->RootElement())) {
		return false;
	}

	delete MPDFile;
    UrlAssembler * newurlA;
    newurlA->assembleURLsIFSegmentBase(mpd, m_url);
	return true;
}

bool MPDManager::ThreadLoop()
{
	IHTTPReceiver *curl_receiver = IHTTPReceiver::Instance();
	curl_receiver->Init();

	if(!MPDManager::IsLive()) {
		if(!GetMPD(curl_receiver)) {
			return false;
		}
	} else {
		while(m_alive) {
			if(!GetMPD(curl_receiver)) {
				return false;
			}
			//TODO: here should be paause described in mpd, temporary 1s now
			sleep(1);
		}
	}

	curl_receiver->Release();
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
URLType* MPDManager::CreateURlType(tinyxml2::XMLElement*element){
	URLType *curType= new URLType;
	if(element->Attribute("sourceURL")) {
		curType->sourceURL = element->Attribute("sourceURL");
	}
	if(element->Attribute("range")) {
		curType->range = element->Attribute("range");
	}
	return curType;
}
SegmentBaseType* MPDManager::CreateSegmentBaseType(tinyxml2::XMLElement * element){
	SegmentBaseType * curType= new SegmentBaseType;
	// default values
	curType->timescale=1;
	curType->presentationTimeOffset=0;
	curType->availabilityTimeComplete=true;
	element->GETUintAttr(curType,timescale);
	element->GETUintAttr(curType,timeShiftBufferDepth);
	if(element->Attribute("presentationTimeOffset"))
		curType->presentationTimeOffset=atol(element->Attribute("presentationTimeOffset"));

	element->QueryDoubleAttribute("availabilityTimeOffset",&(curType->availabilityTimeOffset));
	element->QueryBoolAttribute("indexRangeExact",&(curType->indexRangeExact));
	element->QueryBoolAttribute("availabilityTimeComplete",&(curType->availabilityTimeComplete));
	if(element->Attribute("indexRange")) {
		curType->indexRange = element->Attribute("indexRange");
	}
	if(element->Attribute("timeShiftBufferDepth")) {
		std::string buf=element->Attribute("timeShiftBufferDepth");
		curType->timeShiftBufferDepth=getTimeFromDuration(buf);
	}
	if(element->Attribute("Initialization")) {
		curType->Initialization = CreateURlType(element->FirstChildElement("Initialization"));
	}
	if(element->Attribute("RepresentationIndex")) {
		curType->RepresentationIndex = CreateURlType(element->FirstChildElement("RepresentationIndex"));
	}
	return curType;
}
Stamp *MPDManager::CreateStamp(tinyxml2::XMLElement * element){
	Stamp *curType= new Stamp;
	curType->r=0;
	curType->t=0;

	if(element->Attribute("t"))
	 curType->t=atol(element->Attribute("t"));
	if(element->Attribute("d"))
	 curType->t=atol(element->Attribute("d"));
	element->GETUintAttr(curType,r);
	return curType;
}
SegmentTimelineType * MPDManager::CreateSegmentTimelineType(tinyxml2::XMLElement * element){
	SegmentTimelineType * curType= new SegmentTimelineType;
	uint32_t startTime=0;
	for(tinyxml2::XMLElement *node=element->FirstChildElement("S");node;node=node->NextSiblingElement("S")){
		Stamp* curS=CreateStamp(node);
		if(curS->t==0) curS->t=startTime;
		startTime+=curS->d*(curS->r+1);
		curType->StampSeq.push_back(curS);
	}
	return curType;
}
MultipleSegmentBaseType *MPDManager::CreateMultipleSegmentBaseType(tinyxml2::XMLElement * element){
	MultipleSegmentBaseType *curType= new MultipleSegmentBaseType;
	curType->m_base = CreateSegmentBaseType(element);
	if(element->FirstChildElement("SegmentTimeline"))
		curType->SegmentTimeline=CreateSegmentTimelineType(element->FirstChildElement("SegmentTimeline"));
	if(element->FirstChildElement("BitstreamSwitching"))
		curType->BitstreamSwitching=CreateURlType(element->FirstChildElement("BitstreamSwitching"));
	element->GETUintAttr(curType,duration);
	element->GETUintAttr(curType,startNumber);
	return curType;
}
SegmentTemplateType* MPDManager::CreateSegmentTemplateType(tinyxml2::XMLElement * element){
	SegmentTemplateType *curType = new SegmentTemplateType;
	curType->m_base = CreateMultipleSegmentBaseType(element);
	element->GETUintAttr(curType,index);
	if(element->Attribute("name"))
		curType->name=element->Attribute("name");
	if(element->Attribute("media"))
		curType->media=element->Attribute("media");
	if(element->Attribute("initialization"))
		curType->initialization=element->Attribute("initialization");
	if(element->Attribute("bitstreamSwitching"))
		curType->bitstreamSwitching=element->Attribute("bitstreamSwitching");
	return curType;
}
SegmentURLType* MPDManager::CreateSegmentURLType(tinyxml2::XMLElement * element){
	SegmentURLType* curType= new SegmentURLType;
	if(element->Attribute("media"))
		curType->media=element->Attribute("media");
	if(element->Attribute("mediaRange"))
		curType->mediaRange=element->Attribute("mediaRange");
	if(element->Attribute("index"))
		curType->index=element->Attribute("index");
	if(element->Attribute("indexRange"))
		curType->indexRange=element->Attribute("indexRange");
	return curType;
};
SegmentListType *MPDManager::CreateSegmentListType(tinyxml2::XMLElement * element){
	SegmentListType* curType= new SegmentListType;
	curType->m_base=CreateMultipleSegmentBaseType(element);
	for(tinyxml2::XMLElement *node=element->FirstChildElement("SegmentURL"); node ; node= node->NextSiblingElement("SegmentURL"))
	{
		curType->SegmentURLs.push_back(CreateSegmentURLType(node));
	}
	return curType;
}
// std::list<DASHRepresentation> MPDManager::GetRepresentationList(void)
// {
// 	return std::list;
// }
