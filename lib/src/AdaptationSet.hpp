/*
lib/src/AdaptationSet.hpp

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

#include <iostream>
#include "RatioType.hpp"

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
class MPDManager;

enum Language { Russian, English };

struct FrameRateType {
	std::string frameRate;
};

union ConditionalUintType {
	unsigned int value;
	bool flag;
};

class AdaptationSet
{
public :
	friend MPDManager;
private:
	/* Elements */
	/*	...		*/

	/* Atributes */

	unsigned int id;
	unsigned int group;
	unsigned int minBandwidth;
	unsigned int maxBandwidth;
	unsigned int minWidth;
	unsigned int maxWidth;
	unsigned int minHeight;
	unsigned int maxHeight;
	unsigned int subsegmentStartsWithSAP;
	bool bitstreamSwitching;
	std::string contentType;

	ConditionalUintType segmentAlignment;
	ConditionalUintType subsegmentAlignment;
	Language language;
	FrameRateType minFrameRate;
	FrameRateType maxFrameRate;
	RatioType pictureAspectRatio;

public:
	AdaptationSet();
	~AdaptationSet();
};
