#ifndef __AW_HTTP_RESPONSE_LISTENER_H__
#define __AW_HTTP_RESPONSE_LISTENER_H__


namespace Http {
	enum class ERequestResult;

	class IResponseListener
	{
	public:
		virtual void onHttpProgress(int bytesRead, int length) = 0;
		virtual void onHttpComplete(ERequestResult result, int responseCode, char* data, unsigned int size) = 0;

	protected:
		virtual ~IResponseListener() {}
	};
}

#endif
