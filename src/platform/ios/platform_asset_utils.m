#include "platform_asset_utils.h"
#include "platform_file_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

FileData get_asset_data(const char* relative_path) {
	assert(relative_path != NULL);
    
    NSMutableString* adjusted_relative_path = [[NSMutableString alloc] initWithString:@"/assets/"];
    [adjusted_relative_path appendString:[[NSString alloc] initWithCString:relative_path encoding:NSASCIIStringEncoding]];
    
    return get_file_data([[[NSBundle mainBundle] pathForResource:adjusted_relative_path ofType:nil] cStringUsingEncoding:NSASCIIStringEncoding]);
}

void release_asset_data(const FileData* file_data) {
    assert(file_data != NULL);
	release_file_data(file_data);
}
