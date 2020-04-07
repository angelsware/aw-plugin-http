#ifndef __AW_PLUGIN_HTTP_REQUEST_H__
#define __AW_PLUGIN_HTTP_REQUEST_H__

#include <type/aw_uri.h>
#include <type/aw_buffer.h>
#include <type/aw_vector.h>
#include <type/aw_pair.h>
#include <type/aw_string.h>

namespace Http {
	class CRequest
	{
	public:
		CRequest(const char* requestMethod, const Type::CUri& url);

		void setData(const char* data, unsigned int length);
		void addHeader(const char* name, const char* value);

		const char* getRequestMethod() const { return mRequestMethod.get(); }
		const Type::CUri& getUrl() const { return mUrl; }
		const Type::CBuffer<char>& getData() const { return mData; }
		const Type::CVector<Type::CPair<Type::CString, Type::CString> >& getHeaders() const { return mHeaders; }

	private:
		Type::CString mRequestMethod;
		Type::CUri mUrl;
		Type::CBuffer<char> mData;
		Type::CVector<Type::CPair<Type::CString, Type::CString> > mHeaders;
	};
}

#endif
