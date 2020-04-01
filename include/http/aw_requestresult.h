#ifndef __AW_HTTP_REQUEST_RESULT_H__
#define __AW_HTTP_REQUEST_RESULT_H__

namespace Http {
	enum class ERequestResult {
		OK = 0,
		UNKNOWN_HOST = 1,
		MALFORMED_URL = 2,
		FILE_NOT_FOUND = 3,
		SOCKET_TIMEOUT = 4,
		ERROR = 5
	};
}

#endif

