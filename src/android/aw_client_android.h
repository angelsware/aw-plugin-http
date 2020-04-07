#ifndef __AW_HTTP_CLIENT_ANDROID_H__
#define __AW_HTTP_CLIENT_ANDROID_H__

#include <http/aw_client.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onProgress(JNIEnv* env, jclass clazz, jint bytesRead, jint length);
	JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onComplete(JNIEnv* env, jclass clazz, jint result, jint responseCode, jbyteArray data, jlong requestPtr);
#ifdef __cplusplus
}
#endif

namespace Http {
	class CClient_Android
		: public IClient
	{
	private:
		void send(const CRequest& request) override;
		void addListener(IResponseListener* listener) override;
		void removeListener(IResponseListener* listener) override;
	};
}

#endif
