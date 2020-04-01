#ifndef __AW_HTTP_CLIENT_ANDROID_H__
#define __AW_HTTP_CLIENT_ANDROID_H__

#include <http/aw_client.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onProgress(JNIEnv* env, jclass clazz, jint bytesRead, jint length);
	JNIEXPORT void JNICALL Java_com_angelsware_http_Request_onComplete(JNIEnv* env, jclass clazz, jint result, jbyteArray data);
#ifdef __cplusplus
}
#endif

namespace Platform {
	class CJniClass;
}

namespace Http {
	class CClient_Android
		: public IClient
	{
	public:
		CClient_Android();
		~CClient_Android();

	private:
		void request(const Type::CUri& url, const char* content, int contentLength) override;
		void addListener(IResponseListener* listener) override;
		void removeListener(IResponseListener* listener) override;

		Platform::CJniClass* mRequest;
	};
}

#endif
