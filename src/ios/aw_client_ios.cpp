#include "aw_client_ios.h"
#include <http/aw_request.h>
#include <http/aw_requestresult.h>
#include <http/aw_responselistener.h>
#include <http/aw_response.h>
#include <type/aw_uri.h>
#include <type/aw_vector.h>

extern "C" {
	void* HttpClient_create();
	void HttpClient_destroy(void* ptr);
	void HttpClient_send(void* ptr, void* request);

	void* HttpRequest_create();
	void HttpRequest_destroy(void* ptr);
	void HttpRequest_addHeader(void* ptr, const char* key, const char* value);
	void HttpRequest_setMethod(void* ptr, const char* method);
	void HttpRequest_setUrl(void* ptr, const char* url);
	void HttpRequest_setData(void* ptr, const char* data);
}

static Type::CVector<Http::IResponseListener*> sListeners;
static Type::CVector<void*> sRequests;

namespace Http {
	void CClient_Ios::onComplete(void* ptr, int result, int responseCode, const char* data) {
		CResponse response(responseCode, data, (unsigned int)strlen(data));

		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			sListeners[i]->onHttpComplete(ERequestResult(result), response);
		}

		for (unsigned int i = 0; i < sRequests.getSize(); ++i) {
			if (sRequests[i] == ptr) {
				HttpRequest_destroy(sRequests[i]);
				sRequests.eraseAndReplaceWithLast(i);
				break;
			}
		}
	}

	CClient_Ios::CClient_Ios() {
		mHttpClient = HttpClient_create();
	}

	CClient_Ios::~CClient_Ios() {
		HttpClient_destroy(mHttpClient);
	}

	void CClient_Ios::send(const CRequest& request) {
		void* r = HttpRequest_create();
		HttpRequest_setUrl(r, request.getUrl().getUri().get());
		HttpRequest_setMethod(r, request.getRequestMethod());
		HttpRequest_setData(r, request.getData().data());
		const Type::CVector<Type::CPair<Type::CString, Type::CString> >& headers = request.getHeaders();
		for (int i = 0; i < headers.getSize(); ++i) {
			HttpRequest_addHeader(r, headers[i].getFirst().get(), headers[i].getSecond().get());
		}
		HttpClient_send(mHttpClient, r);
		sRequests.pushBack(r);
	}

	void CClient_Ios::addListener(IResponseListener* listener) {
		sListeners.pushBack(listener);
	}

	void CClient_Ios::removeListener(IResponseListener* listener) {
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			if (sListeners[i] == listener) {
				sListeners.eraseAndReplaceWithLast(i);
				break;
			}
		}
	}
}
