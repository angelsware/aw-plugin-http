#include "aw_client_android.h"
#include <http/aw_request.h>
#include <http/aw_requestresult.h>
#include <http/aw_responselistener.h>
#include <type/aw_uri.h>
#include <type/aw_vector.h>
#include <log/aw_log.h>
#include <platform/android/aw_jniclass.h>
#include <platform/android/aw_jni.h>
#include <platform/android/aw_jnistring.h>
#include <platform/android/aw_jnibytearray.h>
#include <platform/android/aw_jninativebytearray.h>

static Type::CVector<Http::IResponseListener*> sListeners;

JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onProgress(JNIEnv* env, jclass clazz, jint bytesRead, jint length) {
	for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
		sListeners[i]->onHttpProgress(bytesRead, length);
	}
}

JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onComplete(JNIEnv* env, jclass clazz, jint result, jint responseCode, jbyteArray data) {
	if (data != nullptr) {
		Platform::CJniNativeByteArray array(data);
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			sListeners[i]->onHttpComplete(Http::ERequestResult(result), responseCode, array.getData(), array.getSize());
		}
	} else {
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			sListeners[i]->onHttpComplete(Http::ERequestResult(result), responseCode, nullptr, 0);
		}
	}
}

namespace Http {
	void CClient_Android::send(const CRequest& request) {
		Platform::CJniClass* javaRequest = new Platform::CJniClass("com/angelsware/http/Request", "()V");
		const Type::CBuffer<char>& data = request.getData();
		LOG_INFO("data count: %d", data.getCount());
		Platform::CJniByteArray body(const_cast<char*>(data.data()), data.getCount());
		Platform::CJniString requestMethod(request.getRequestMethod());
		javaRequest->callVoidMethod("setRequestMethod", "(Ljava/lang/String;)V", requestMethod.getText());
		javaRequest->callVoidMethod("setData", "([BI)V", body.getByteArray(), body.getSize());
		const Type::CVector<Type::CPair<Type::CString, Type::CString> >& headers = request.getHeaders();
		for (unsigned int i = 0; i < headers.getSize(); ++i) {
			Platform::CJniString key(headers[i].getFirst().get());
			Platform::CJniString value(headers[i].getSecond().get());
			javaRequest->callVoidMethod("addHeader", "(Ljava/lang/String;Ljava/lang/String;)V", key.getText(), value.getText());
		}
		Platform::CJniString url(request.getUrl().getUri().get());
		javaRequest->callVoidMethod("send", "(Ljava/lang/String;)V", url.getText());

		// TODO: Delete request.
	}

	void CClient_Android::addListener(IResponseListener* listener) {
		sListeners.pushBack(listener);
	}

	void CClient_Android::removeListener(IResponseListener* listener) {
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			if (sListeners[i] == listener) {
				sListeners.eraseAndReplaceWithLast(i);
				break;
			}
		}
	}
}
