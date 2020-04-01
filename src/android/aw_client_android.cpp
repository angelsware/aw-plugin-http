#include "aw_client_android.h"
#include <http/aw_requestresult.h>
#include <http/aw_responselistener.h>
#include <type/aw_uri.h>
#include <type/aw_vector.h>
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

JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onComplete(JNIEnv* env, jclass clazz, jint result, jbyteArray data) {
	if (data != nullptr) {
		Platform::CJniNativeByteArray array(data);
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			sListeners[i]->onHttpComplete(Http::ERequestResult(result), array.getData(), array.getSize());
		}
	} else {
		for (unsigned int i = 0; i < sListeners.getSize(); ++i) {
			sListeners[i]->onHttpComplete(Http::ERequestResult(result), nullptr, 0);
		}
	}
}

namespace Http {
	CClient_Android::CClient_Android() {
		mRequest = new Platform::CJniClass("com/angelsware/http/Request", "()V");
	}

	CClient_Android::~CClient_Android() {
		delete mRequest;
	}

	void CClient_Android::request(const Type::CUri& url, const char* content, int contentLength) {
		Platform::CJniString str(url.getUri().get());
		Platform::CJniByteArray body(const_cast<char*>(content), contentLength);
		mRequest->callVoidMethod("request", "(Ljava/lang/String;[BI)V", str.getText(), body.getByteArray(), body.getSize());
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
