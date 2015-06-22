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
	return true; // TODO: change this return for default!!!!!!
}

CurlReceiver::CurlReceiver()
{
	m_curl = NULL;
	m_callback = NULL;
}

CurlReceiver::~CurlReceiver()
{
	Release();
}

bool CurlReceiver::Init()
{
	bool initialized = false;
	m_curl = curl_easy_init();
	
	if (m_curl)
		initialized = true;

	return initialized;
}

bool CurlReceiver::Release()
{
	curl_easy_cleanup(m_curl);
	return true;
}

std::string Callback_Function(void *ptr, size_t size, size_t nmemb, std::string &m_callback){
	std::string *pCallback = static_cast<std::string*>(ptr);
	m_callback = pCallback;
	delete pCallback;
	return m_callback;				// TODO: write to memory
}

bool CurlReceiver::Get(std::string url, std::string &callback)
{	
	bool res = false;
	//m_callback = callback;
	if (m_curl){
		FILE *file;
		CURLcode performed;
		
		char* m_url = new char[url.size() + 1];
		std::copy(url.begin(), url.end(), m_url);
		m_url[url.size()] = '\0';	
		
		char out_file_name[FILENAME_MAX] = "DownloadFile.mpd";
		file = fopen(out_file_name, "wb");
		curl_easy_setopt(m_curl, CURLOPT_URL, m_url);
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, NULL); //TODO: add callback function. 
								// It should return number of bytes; 
								// Now it uses function by default;
		//curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, file);
		
		if (curl_easy_perform(m_curl) == 0) {
			res = true;
		}
		delete[] m_url;
		fclose(file);
	}

	return res;
}

