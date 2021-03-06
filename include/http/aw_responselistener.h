#ifndef __AW_HTTP_RESPONSE_LISTENER_H__
#define __AW_HTTP_RESPONSE_LISTENER_H__


namespace Http {
	class CResponse;
	enum class ERequestResult;

	class IResponseListener
	{
	public:
		virtual void onHttpProgress(int bytesRead, int length) = 0;
		virtual void onHttpComplete(Http::ERequestResult result, const CResponse& response) = 0;

	protected:
		virtual ~IResponseListener() {}
	};
}

#endif
