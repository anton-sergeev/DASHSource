/*
 lib/src/CurlReceiver.hpp

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
#include <curl.h>
#include "CurlReceiver.hpp"

/******************************************************************
* STATIC DATA                                                     *
*******************************************************************/
static IHTTPReceiver *l_inst = nullptr; //TODO: move this somewhere

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
IHTTPReceiver *IHTTPReceiver::Instance(void)
{
	if(l_inst == nullptr) {
		//only curl support now
		l_inst = new CurlReceiver();
	}
	return l_inst;
}

bool IHTTPReceiver::SetInstance(IHTTPReceiver *inst)
{
	if(l_inst == nullptr) {
		delete l_inst;
	}
	l_inst = inst;
}

CurlReceiver::CurlReceiver()
{
	
}

CurlReceiver::~CurlReceiver()
{
	
}

bool CurlReceiver::Init()
{
	//global initialisation for curl
	return true;
}

bool CurlReceiver::Release()
{
	//global deinitialisation for curl
	return true;
}

bool CurlReceiver::Get(std::string url, IHTTPCallback *callback)
{
	m_callback = callback;
	//heree need to get data, periodically calling callback
	return true;
}

