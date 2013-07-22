#include "platform_file_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

FileData get_file_data(const char* path) {
	assert(path != NULL);
		
	FILE* stream = fopen(path, "r");
	assert (stream != NULL);

	fseek(stream, 0, SEEK_END);	
	long stream_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	void* buffer = malloc(stream_size);
	fread(buffer, stream_size, 1, stream);

	assert(ferror(stream) == 0);
	fclose(stream);

	return (FileData) {stream_size, buffer, NULL};
}

void release_file_data(const FileData* file_data) {
	assert(file_data != NULL);	
	assert(file_data->data != NULL);

	free((void*)file_data->data);
}
