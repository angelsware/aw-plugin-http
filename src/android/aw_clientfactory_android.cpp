#include <http/aw_clientfactory.h>
#include "aw_client_android.h"

namespace Http {
	IClient* CClientFactory::create() {
		return new CClient_Android();
	}

	void CClientFactory::destroy(IClient* client) {
		delete client;
	}
}
