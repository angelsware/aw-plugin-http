#import "HttpClientDelegate.h"
#import "aw_client_ios.h"

@implementation HttpClientDelegate

+(void) onComplete:(void*) id result:(int) result responseCode:(int) responseCode data:(NSString*) data {
	Http::CClient_Ios::onComplete(id, result, responseCode, [data UTF8String]);
}

@end
