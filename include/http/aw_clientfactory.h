#ifndef __AW_HTTP_CLIENT_FACTORY_H__
#define __AW_HTTP_CLIENT_FACTORY_H__

namespace Http {
	class IClient;

	class CClientFactory
	{
	public:
		static IClient* create();
		static void destroy(IClient* client);
	};
}

#endif

