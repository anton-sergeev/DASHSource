#pragma once

#include <string>
#include "AdaptationSet.hpp"
#include "URLtypes.hpp"
#include "Segment.hpp"

class RepresentationBaseType {

public:
	std::string profiles;
	unsigned int width;
	unsigned int height;
	RatioType  sar;
	//FrameRateType frameRate;
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
