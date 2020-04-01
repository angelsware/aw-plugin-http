#ifndef __AW_HTTP_CLIENT_H__
#define __AW_HTTP_CLIENT_H__

namespace Type {
	class CUri;
}

namespace Http {
	class IResponseListener;

	class IClient
	{
	public:
		virtual ~IClient() {}

		virtual void request(const Type::CUri& url, const char* content, int contentLength) = 0;
		virtual void addListener(IResponseListener* listener) = 0;
		virtual void removeListener(IResponseListener* listener) = 0;
	};
}

#endif
