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
#include <tinyxml2.h>
#include "MPDManager.hpp"
#include "CurlReceiver.hpp"
/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
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
	IHTTPReceiver *curl_receiver = IHTTPReceiver::Instance();
	curl_receiver -> Init();
	curl_receiver -> Get(m_url, NULL);
	return true;
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
		newEventStream->timescale = atol(el->Attribute("timescale"));
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

// std::list<DASHRepresentation> MPDManager::GetRepresentationList(void)
// {
// 	return std::list;
// }
