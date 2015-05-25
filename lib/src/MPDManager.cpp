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
//#include "CurlReceiver.hpp"
/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/

/******************************************************************
* LOCAL MACROS                                                    *
*******************************************************************/
#define GETUintAttr(obj,attr) QueryUnsignedAttribute(#attr,&(obj.attr))

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
//	IHTTPReceiver* curl_receiver = IHTTPReceiver::Instance();
//	curl_receiver -> Init();
//	curl_receiver -> Get(m_url, NULL);
	std::string filename = "OfForestAndMen_10s_onDemand_2014_05_09.mpd"; // For example
	MPDFile = new XMLDocument();
	return MPDFile->LoadFile(filename.c_str());
}

bool MPDManager::Stop()
{
	//stop thread
	return true;
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
/*std::list <AdaptationSet> MPDManager::getAdaptationList()
{
	std::list <AdaptationSet> reslist;
	XMLHandle dochand(MPDFile);

	dochand = dochand.FirstChildElement("MPD").FirstChildElement("Period").FirstChildElement("AdaptationSet");
	XMLElement *curNodeSet = dochand.ToElement();
	while(curNodeSet) {
		std::cout << "Adaptation found" << std::endl;

		AdaptationSet curSet;

		curNodeSet->GETUintAttr(curSet, id);
		curNodeSet->GETUintAttr(curSet, group);
		curNodeSet->GETUintAttr(curSet, minBandwidth);
		curNodeSet->GETUintAttr(curSet, maxBandwidth);
		curNodeSet->GETUintAttr(curSet, minWidth);
		curNodeSet->GETUintAttr(curSet, maxWidth);
		curNodeSet->GETUintAttr(curSet, minHeight);
		curNodeSet->GETUintAttr(curSet, maxHeight);
		curNodeSet->GETUintAttr(curSet, subsegmentStartsWithSAP);

		curNodeSet->QueryBoolAttribute("bitstreamSwitching", &(curSet.bitstreamSwitching));
		curNodeSet->QueryUnsignedAttribute("segmentAlignment", (unsigned int *) & (curSet.segmentAlignment));
		curNodeSet->QueryUnsignedAttribute("subsegmentAlignment", (unsigned int *) & (curSet.subsegmentAlignment));
		//curNodeSet->QueryUnsignedAttribute("lang",(unsigned int *)&(curSet.language));
		const char *att = 0;

		att = curNodeSet->Attribute("contentType");
		if(att) {
			curSet.contentType = att;
			std::cout << curSet.maxFrameRate.frameRate << std::endl;
		}

		att = curNodeSet->Attribute("maxFrameRate");
		if(att) {
			curSet.maxFrameRate.frameRate = att;
			std::cout << curSet.maxFrameRate.frameRate << std::endl;
		}


		att = curNodeSet->Attribute("minFrameRate");
		if(att) {
			curSet.minFrameRate.frameRate = att;
			std::cout << curSet.minFrameRate.frameRate << std::endl;
		}

		att = curNodeSet->Attribute("par");
		if(att) {
			curSet.pictureAspectRatio = RatioType(std::string(att));
			std::cout << std::string(att) << std::endl;
		}
		reslist.push_back(curSet);
		curNodeSet = curNodeSet->NextSiblingElement("AdaptationSet");
	}
	return reslist;

}*/
// std::list<DASHRepresentation> MPDManager::GetRepresentationList(void)
// {
// 	return std::list;
// }
