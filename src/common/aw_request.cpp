#include <http/aw_request.h>

namespace Http {
	CRequest::CRequest(const char* requestMethod, const Type::CUri& url)
		: mRequestMethod(requestMethod)
		, mUrl(url)
	{}

	void CRequest::setData(const char* data, unsigned int length) {
		mData.reset(data, length);
	}

	void CRequest::addHeader(const char* name, const char* value) {
		mHeaders.pushBack(Type::CPair<Type::CString, Type::CString>(name, value));
	}
}
