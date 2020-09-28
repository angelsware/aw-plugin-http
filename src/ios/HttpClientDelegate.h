#import <Foundation/Foundation.h>

@interface HttpClientDelegate : NSObject

+(void) onComplete:(void*) id result:(int) result responseCode:(int) responseCode data:(NSString*) data;

@end
