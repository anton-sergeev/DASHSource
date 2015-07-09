/*
 app/main.cpp

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
#include <cstdint>
#if (defined _WIN32)
	#include <windows.h> //include for Sleep()
	//#include <iostream>
#else
 #include <unistd.h> //sleep()
#endif
#include <IDASHSource.hpp>

/******************************************************************
* FUNCTION IMPLEMENTATION                     <Module>_<Word>+    *
*******************************************************************/
static int32_t portable_sleep(int32_t millsec)
{
	int32_t rc;
	//portable sleep, getted from http://curl.haxx.se/libcurl/c/multi-post.html
#ifdef _WIN32
	Sleep(millsec);
	rc = 0;
#else
	/* Portable sleep for platforms other than Windows. */
	struct timeval wait = { 0, millsec * 1000 }; /* 10s */
	rc = select(0, NULL, NULL, NULL, &wait);
#endif
	return rc;
}

int32_t main(int32_t argc, char *argv[])
{
	IDASHSource *src = IDASHSource::CreateInstance();
	std::string url("http://wc.exn.su:8888/attachments/download/3/OfForestAndMen.zip"); // TODO: CHANGE!!!

	if (argc == 2){
		url = std::string(argv[1]);
	}

	src->Start(url);

	portable_sleep(10 * 1000);

	src->Stop();
	delete src;
	return 0;
}
