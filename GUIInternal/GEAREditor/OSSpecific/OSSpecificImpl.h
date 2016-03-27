#pragma once
#import <Cocoa/Cocoa.h>
#include "OSSpecificInterface.h"

@interface OSSpecificImpl : NSObject
{
    
}

+ (void) saveAnimationFile: (std::string) rootDir;
@end