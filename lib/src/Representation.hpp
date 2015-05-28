#include <string>
#include "AdaptationSet.hpp"
typedef unsigned int ui;
class RepresentationBaseType
{
public:
	std::string profiles;
	ui width;
	ui height;
	RatioType  sar;
    FrameRateType frameRate;
    std::string audioSamplingRate;
    std::string mimeType;
    std::string segmentProfiles;
    std::string codecs;
    double maximumSAPPeriod;
    ui startWithSAP;// from 0 to 6
    double maxPlayoutRate;
    bool codingDependency;
    std::string scanType;
};
struct SubRepresentationType
{
    ui level;
    std::vector <ui> dependencyLevel;
    ui bandwidth;
    std::vector<std::string> contentComponent;
};
struct BaseURLType
{
    std::string serviceLocation;
    std::string byteRange;
};
class  Representation: public RepresentationBaseType
{
    public:
    Representation(): RepresentationBaseType()
    {
    }
    BaseURLType BaseURL;
    SubRepresentationType SubRepresentation;
    //<xs:element name="SegmentBase" type="SegmentBaseType" minOccurs="0"/>
    //<xs:element name="SegmentList" type="SegmentListType" minOccurs="0"/>
    //<xs:element name="SegmentTemplate" type="SegmentTemplateType" minOccurs="0"
    std::string id;
    ui bandwidth;
    ui qualityRanking;
    std::vector<std::string> dependencyId;
    std::vector<std::string> mediaStreamStructureId;
};
