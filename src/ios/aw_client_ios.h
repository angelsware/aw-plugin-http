#ifndef __AW_HTTP_CLIENT_IOS_H__
#define __AW_HTTP_CLIENT_IOS_H__

#include <http/aw_client.h>

namespace Http {
	class CClient_Ios
		: public IClient
	{
	public:
		CClient_Ios();
		~CClient_Ios();

		static void onComplete(void* ptr, int result, int responseCode, const char* data);

	private:
		void send(const CRequest& request) override;
		void addListener(IResponseListener* listener) override;
		void removeListener(IResponseListener* listener) override;

		void* mHttpClient;
	};
}

#endif
