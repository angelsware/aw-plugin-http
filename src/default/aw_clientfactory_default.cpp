#include <http/aw_clientfactory.h>
#include "aw_client_default.h"

namespace Http {
	IClient* CClientFactory::create() {
		return new CClient_Default();
	}

	void CClientFactory::destroy(IClient* client) {
		delete client;
	}
}
