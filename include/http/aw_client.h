#ifndef __AW_HTTP_CLIENT_H__
#define __AW_HTTP_CLIENT_H__

namespace Http {
	class IResponseListener;
	class CRequest;

	class IClient
	{
	public:
		virtual ~IClient() {}

		virtual void send(const CRequest& request) = 0;
		virtual void addListener(IResponseListener* listener) = 0;
		virtual void removeListener(IResponseListener* listener) = 0;
	};
}

#endif
