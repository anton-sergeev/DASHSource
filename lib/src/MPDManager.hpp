/*
 lib/src/MPDManager.hpp

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
#pragma once
/******************************************************************
* INCLUDE FILES                                                   *
*******************************************************************/

#include <string>
#include <iostream>
#include <list>
#include "tinyxml2.h"
#include "AdaptationSet.hpp"
#include "Representation.hpp"
#include "Events.hpp"
#include "Period.hpp"
#include "Segment.hpp"
#include "MPDStruct.hpp"
#include <stdlib.h>
#include <thread>

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
class MPDManager
{
public:
	MPDManager();
	~MPDManager();

	bool Start(std::string &url);
	bool Stop();
	bool IsLive();

	MPD *CreateMPDStruct(tinyxml2::XMLElement *XMLRootElement);
	EventStream *CreateEventStream(tinyxml2::XMLElement *element);
	Period *CreatePeriod(tinyxml2::XMLElement *element);
	AdaptationSet *CreateAdaptationSet(tinyxml2::XMLElement *element);
	RepresentationBaseType *CreateRepresentationBaseType(tinyxml2::XMLElement *element);
	Representation *CreateRepresentation(tinyxml2::XMLElement *element);
	URLType* CreateURlType(tinyxml2::XMLElement*);
	SegmentURLType* CreateSegmentURLType(tinyxml2::XMLElement*);
	Stamp* CreateStamp(tinyxml2::XMLElement *);
	SegmentBaseType* CreateSegmentBaseType(tinyxml2::XMLElement *);
	SegmentTimelineType* CreateSegmentTimelineType(tinyxml2::XMLElement *);
	MultipleSegmentBaseType* CreateMultipleSegmentBaseType(tinyxml2::XMLElement *);
	SegmentTemplateType* CreateSegmentTemplateType(tinyxml2::XMLElement *);
	SegmentListType* CreateSegmentListType(tinyxml2::XMLElement *);

private:
	bool ThreadLoop();
	long int getTimeFromDuration(std::string str);
	MPD *MPDStruct;
// 	std::list<DASHRepresentation> GetRepresentationList(void);
	std::string m_url;
	std::thread *m_thr;
	tinyxml2::XMLDocument *MPDFile;
//
};
