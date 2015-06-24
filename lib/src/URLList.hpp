#ifndef __URLLIST__
#define  __URLLIST__
#include <string>
#include <list>
#include <cstdint>
class URLList{
	public:
	bool isEmpty();
	void AddURL(std::string); // in tail
	void RemoveUrl(); // from head
	void Sync(std::list < std::string > );
	private:
	uint64_t getHash(std::string);
	std::list<uint64_t> hashList;
	std::list<std::string> SegmentURLs;
};
#endif
