/*
 lib/src/DASHSource.c

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
#include "DASHSource.hpp"

/******************************************************************
* FUNCTION IMPLEMENTATION                     <Module>_<Word>+    *
*******************************************************************/
IDASHSource* IDASHSource::CreateInstance(void)
{
	return new DASHSource();
}

DASHSource::DASHSource()
	: m_mpd_manager(nullptr)
{
	
}

DASHSource::~DASHSource()
{
	Stop();
}

bool DASHSource::Start(std::string &url)
{
	if(m_mpd_manager == nullptr) {
		m_mpd_manager = new MPDManager();
	}
	m_mpd_manager->Start(url);
	return true;
}

bool DASHSource::Pause()
{
	return true;
}

bool DASHSource::Stop()
{
	if(m_mpd_manager) {
		delete m_mpd_manager;
		m_mpd_manager = nullptr;
	}
	return true;
}

// bool DASHSource::Seek(uint64_t& time)
// {
// 	return true;
// }

bool DASHSource::SetProperty(DASHSourceProperty_e type, void *)
{
	return true;
}

bool DASHSource::GetProperty(DASHSourceProperty_e type, void *)
{
	return true;
}
bool DASHSource::SwitchUp(uint64_t bitrate){
  auto NextRepresentation = ++curRepresentation;
  if(NextRepresentation != Representationlist.end() && NextRepresentation->bandwidth < bitrate){
    curRepresentation = NextRepresentation;
    return true;
  }
  return false;
}
bool DASHSource::SwitchDown(uint64_t bitrate){
  if(curRepresentation == Representationlist.begin())
    return false;
  curRepresentation = --curRepresentation;  
  return true;
}
bool DASHSource::ReceivedData(char *ptr, size_t size){
  Clock::time_point newPoint = Clock::now();
  uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(newPoint - lastp).count();
  lastp = newPoint;
  uint64_t myBitStream=(size*8*1000)/time; //bit per second
  uint64_t NecessarySize = (time * curRepresentation->bandwidth)/( 1000 * 8 ); // bandwidth = bits/sec
  bool isOkay = true;
  if(NecessarySize < size)
      isOkay = !(SwitchUp(myBitStream));
    else
      isOkay = !(SwitchDown(myBitStream));
  if(isOkay){
        memcpy(curSegment+curByte,ptr,size);
        curByte += size;
        return true;
  }      
    else{
        curByte = 0;
        //free(curSegment);
        return false;
      }
  return true;
}
bool DASHSource::DownloadSegment(std::string URL,uint32_t size){
  lastp = Clock::now();
  curSegment = (uint8_t *)std::malloc(size);
  curByte = 0;
  if(!curSegment) return false;
  m_curl_receiver->GetAsync(URL,*((IHTTPCallback*)this)); // This is C, not C++ 
  return true;
}
