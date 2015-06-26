/*
 lib/src/DASHSource.hpp

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
// #include <string>
#include <list>
#include <chrono>
#include "IDASHSource.hpp"
#include "MPDManager.hpp"
#include "IHTTPReceiver.hpp"
#include "CurlReceiver.hpp"
#include <thread>
#include <unistd.h>
#include "URLList.hpp"

/******************************************************************
* EXPORTED TYPEDEFS                            [for headers only] *
*******************************************************************/
typedef std::chrono::high_resolution_clock Clock;
class DASHSource: public IDASHSource, public IHTTPCallback
{
public:
	DASHSource();
	virtual ~DASHSource();

	virtual bool Start(std::string &url);
	virtual bool Pause();
	virtual bool Stop();
// 	virtual bool Seek(uint64_t& time);

	virtual bool SetProperty(DASHSourceProperty_e type, void *);
	virtual bool GetProperty(DASHSourceProperty_e type, void *);
  bool DownloadSegment(std::string URL, uint32_t size);
  bool ReceivedData(char *ptr, size_t size);
  bool SwitchUp(uint64_t bitrate);
  bool SwitchDown(uint64_t bitrate);
private:
	MPDManager *m_mpd_manager;
  CurlReceiver *m_curl_receiver;
  std::list < Representation > Representationlist;
  std::list < Representation > ::iterator curRepresentation;
  uint8_t * curSegment;
  uint32_t curByte;
  Clock::time_point lastp;
	void ThreadLoop();
	std::string GetNewURL();

private:
	MPDManager *m_mpd_manager;
	std::thread *m_thread;
// 	std::string m_url;
};
