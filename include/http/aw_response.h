#ifndef __AW_HTTP_RESPONSE_H__
#define __AW_HTTP_RESPONSE_H__

namespace Http {
	class CResponse
	{
	public:
		CResponse(int responseCode, const char* data, unsigned int dataSize);

		int getResponseCode() const { return mResponseCode; }
		const char* getData() const { return mData; }
		unsigned int getDataSize() const { return mDataSize; }

	private:
		int mResponseCode;
		const char* mData;
		unsigned int mDataSize;
	};
}

#endif
