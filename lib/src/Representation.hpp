/*
lib/src/Representation.hpp

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

#include <string>
#include "FrameRate.hpp"
// #include "AdaptationSet.hpp"
#include "RatioType.hpp"
#include "URLtypes.hpp"
#include "Segment.hpp"

class RepresentationBaseType {

public:
	std::string profiles;
	unsigned int width;
	unsigned int height;
	RatioType  sar;
	FrameRateType frameRate;
    	std::string audioSamplingRate;
    	std::string mimeType;
    	std::string segmentProfiles;
    	std::string codecs;
    	double maximumSAPPeriod;
    	unsigned int startWithSAP;// from 0 to 6
    	double maxPlayoutRate;
    	bool codingDependency;
    	std::string scanType;
};

struct SubRepresentationType {
    	unsigned int level;
    	std::vector<unsigned int> dependencyLevel;
    	unsigned int bandwidth;
    	std::vector<std::string> contentComponent;
};

struct Representation {
    public:
    	RepresentationBaseType *m_base;
    	std::list<BaseURLType *> BaseURL;
    	SubRepresentationType SubRepresentation;
	SegmentBaseType *SegmentBase;
	SegmentListType *SegmentList;
	SegmentTemplateType *SegmentTemplate;

    	std::string id;
    	unsigned int bandwidth;
    	unsigned int qualityRanking;
    	std::vector<std::string> dependencyId;
    	std::vector<std::string> mediaStreamStructureId;
};
