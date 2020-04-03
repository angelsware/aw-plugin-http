#ifndef __AW_HTTP_CLIENT_DEFAULT_H__
#define __AW_HTTP_CLIENT_DEFAULT_H__

#include <http/aw_client.h>

namespace Http {
	class CClient_Default
		: public IClient
	{
	private:
		void request(const Type::CUri& url, const char* content, int contentLength) override;
		void addListener(IResponseListener* listener) override;
		void removeListener(IResponseListener* listener) override;
	};
}

#endif
