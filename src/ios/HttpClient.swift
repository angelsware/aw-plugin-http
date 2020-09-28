class HttpClient: NSObject {
	func send(ptr: UnsafeMutablePointer<HttpRequest>, httpRequest: HttpRequest) {
		var request = URLRequest(url: httpRequest.url!)
		request.httpMethod = httpRequest.method
		request.httpBody = httpRequest.data
		httpRequest.headers.forEach { (element) in
			let (key, value) = element
			request.addValue(value, forHTTPHeaderField: key)
		}
		
		let task = URLSession.shared.dataTask(with: request) {(data, response, error) in
			var result: Int = 0
			if (error != nil) {
				let code = (error! as NSError).code
				switch (code) {
				case NSURLErrorCannotConnectToHost, -1003:
					result = 1
					break
				case NSURLErrorBadURL, NSURLErrorUnsupportedURL, -1002:
					result = 2
					break
				case NSURLErrorFileDoesNotExist:
					result = 3
					break
				case NSURLErrorTimedOut:
					result = 4
					break
				default:
					result = 5
					break
				}
			}

			var statusCode: Int = 0
			if let httpResponse = response as? HTTPURLResponse {
				statusCode = httpResponse.statusCode
			}

			HttpClientDelegate.onComplete(ptr, result: Int32(result), responseCode: Int32(statusCode), data: String(data: data ?? Data(), encoding: .utf8)!)
		}

		task.resume()
	}
}

class HttpRequest: NSObject {
	var headers = [String : String]()
	var method: String = "GET"
	var url: URL?
	var data: Data?
}

@_cdecl("HttpRequest_create")
func HttpRequest_create() -> UnsafeMutablePointer<HttpRequest> {
	let httpRequest = UnsafeMutablePointer<HttpRequest>.allocate(capacity: 1)
	httpRequest.initialize(to: HttpRequest())
	return httpRequest
}

@_cdecl("HttpRequest_destroy")
func HttpRequest_destroy(ptr: UnsafeMutablePointer<HttpRequest>) {
	ptr.deallocate()
}

@_cdecl("HttpRequest_addHeader")
func HttpRequest_addHeader(ptr: UnsafeMutablePointer<HttpRequest>, key: UnsafePointer<CChar>, value: UnsafePointer<CChar>) {
	ptr.pointee.headers[String.init(cString: key)] = String.init(cString: value)
}

@_cdecl("HttpRequest_setMethod")
func HttpRequest_setMethod(ptr: UnsafeMutablePointer<HttpRequest>, method: UnsafePointer<CChar>) {
	ptr.pointee.method = String.init(cString: method)
}

@_cdecl("HttpRequest_setUrl")
func HttpRequest_setUrl(ptr: UnsafeMutablePointer<HttpRequest>, url: UnsafePointer<CChar>) {
	ptr.pointee.url = URL(string: String.init(cString: url))!
}

@_cdecl("HttpRequest_setData")
func HttpRequest_setDataLength(ptr: UnsafeMutablePointer<HttpRequest>, data: UnsafePointer<CChar>) {
	ptr.pointee.data?.append(contentsOf: Data(String.init(cString: data).utf8))
}

@_cdecl("HttpClient_create")
func HttpClient_create() -> UnsafeMutablePointer<HttpClient> {
	let httpClient = UnsafeMutablePointer<HttpClient>.allocate(capacity: 1)
	httpClient.initialize(to: HttpClient())
	return httpClient
}

@_cdecl("HttpClient_destroy")
func HttpClient_destroy(ptr: UnsafeMutablePointer<HttpClient>) {
	ptr.deallocate()
}

@_cdecl("HttpClient_send")
func HttpClient_send(ptr: UnsafeMutablePointer<HttpClient>, request: UnsafeMutablePointer<HttpRequest>) {
	ptr.pointee.send(ptr: request, httpRequest: request.pointee)
}
