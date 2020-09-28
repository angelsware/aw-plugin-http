#include <http/aw_clientfactory.h>
#include "aw_client_ios.h"

namespace Http {
	IClient* CClientFactory::create() {
		return new CClient_Ios();
	}

	void CClientFactory::destroy(IClient* client) {
		delete client;
	}
}
