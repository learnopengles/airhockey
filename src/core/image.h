#include "platform_gl.h"

typedef struct {
	const int width;
	const int height;
	const int size;
	const GLenum gl_color_format;
	const void* data;
} RawImageData;

/* Returns the decoded image data, or aborts if there's an error during decoding. */
RawImageData get_raw_image_data_from_png(const void* png_data, const int png_data_size);
void release_raw_image_data(const RawImageData* data);
