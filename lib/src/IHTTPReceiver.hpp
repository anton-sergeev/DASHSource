/*
 lib/src/IHTTPReceiver.hpp

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
// #include <cstdint>
#include <stdint.h>

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
class IHTTPCallback
{
	virtual bool ReceivedData(uint8_t *ptr, uint32_t size) = 0;
};

class IHTTPReceiver
{
public:
	virtual ~IHTTPReceiver() {};

	static IHTTPReceiver *Instance(void);
	static bool SetInstance(IHTTPReceiver *);

	virtual bool Init() = 0;
	virtual bool Release() = 0;

	virtual bool Get(std::string url, IHTTPCallback *callback) = 0;
private:
// 	IHTTPCallback *m_callback;
};
