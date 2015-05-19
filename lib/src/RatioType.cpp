/*
lib/src/RatioType.cpp

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

#include "RatioType.hpp"

/******************************************************************
* FUNCTION IMPLEMENTATION                     <Module>_<Word>+    *
*******************************************************************/

RatioType::RatioType()
{
	X = Y = 1;
}

RatioType::RatioType(unsigned int inputX, unsigned int inputY)
{
	X = inputX;
	Y = inputY;
}

RatioType::RatioType(std::string inputRatio)
{
	std::vector<unsigned int> res = RatioType::Split(inputRatio, ':');

	if(res.size() > 2) {
		throw "Input data has incorrect format.";
	}

	X = res[0];
	Y = res[1];
}

RatioType::~RatioType()
{

}

unsigned int RatioType::GetX() const
{
	return X;
}

unsigned int RatioType::GetY() const
{
	return Y;
}

std::string RatioType::GetRatio() const
{
	return (std::to_string(X) + ":" + std::to_string(Y));
}

std::vector<unsigned int> RatioType::Split(std::string str, char delimiter) const
{
	std::vector<unsigned int> result;
	std::stringstream ss(str);
	std::string temp;

	while(getline(ss, temp, delimiter)) {
		result.push_back(std::stoi(temp));
	}

	return result;
}
