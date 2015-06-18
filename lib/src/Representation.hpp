#pragma once

#include <string>
#include "AdaptationSet.hpp"
#include "URLtypes.hpp"

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
struct SubRepresentationType
{
    unsigned int level;
    std::vector<unsigned int> dependencyLevel;
    unsigned int bandwidth;
    std::vector<std::string> contentComponent;
};

struct Representation
{
    public:
    RepresentationBaseType *m_base;
    BaseURLType BaseURL;
    SubRepresentationType SubRepresentation;
    //<xs:element name="SegmentBase" type="SegmentBaseType" minOccurs="0"/>
    //<xs:element name="SegmentList" type="SegmentListType" minOccurs="0"/>
    //<xs:element name="SegmentTemplate" type="SegmentTemplateType" minOccurs="0"
    std::string id;
    unsigned int bandwidth;
    unsigned int qualityRanking;
    std::vector<std::string> dependencyId;
    std::vector<std::string> mediaStreamStructureId;
};
