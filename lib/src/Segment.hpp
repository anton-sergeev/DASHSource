/*
 lib/src/Segment.hpp

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
#include <cstdint>
#include <vector>
#include <string>
#include <list>

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/

struct SegmentBaseType{
	unsigned int timescale;
	unsigned long presentationTimeOffset;
	unsigned int timeShiftBufferDepth;
	std::string indexRange;
	bool indexRangeExact;
	double availabilityTimeOffset;
	bool availabilityTimeComplete;
	std::string Initialization;
	std::string RepresentationIndex;
};
struct URLType{
	std::string sourceURL;
	std::string range;
};

struct Stamp
{
	uint64_t time;
	uint64_t duration;
	uint64_t range;
};

struct SegmentTimelineType{
	std::vector<Stamp> StampSeq;
	
};

struct MultipleSegmentBaseType: public SegmentBaseType{
	SegmentTimelineType SegmentTimeline;
	URLType BitstreamSwitching;
	uint32_t duration;
	uint32_t startNumber;
	
};

struct SegmentTemplateType:public MultipleSegmentBaseType{
	std::string name;
	uint32_t index;
	std::string media;
	std::string initialization;
	std::string bitstreamSwitching;
};
struct SegmentUrlType{
	std::string media;
	std::string mediaRange;
	std::string index;
	std::string indexRange;
};

struct SegmentListType: public MultipleSegmentBaseType{
	std::list<SegmentUrlType> SegmentURLs;
};
