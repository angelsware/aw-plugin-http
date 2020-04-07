#include <http/aw_response.h>

namespace Http {
	CResponse::CResponse(int responseCode, const char* data, unsigned int dataSize)
		: mResponseCode(responseCode)
		, mData(data)
		, mDataSize(dataSize)
	{}
}
