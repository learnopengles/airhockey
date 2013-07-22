#CFLAGS = -O2 -I. -I../../core -I../common -Wall -Wextra
CFLAGS = -O2 -I. -I../../core -I../common -I../../3rdparty/libpng -I../../3rdparty/libzlib -Wall -Wextra
LDFLAGS = --llvm-lto 1 --closure 1 --embed-file ../../../assets@/ --compression $(EMSCRIPTEN_ROOT)/third_party/lzma.js/lzma-native,$(EMSCRIPTEN_ROOT)/third_party/lzma.js/lzma-decoder.js,LZMA.decompress

SOURCES = main.c \
		  platform_asset_utils.c \
		  ../common/platform_log.c \
		  ../common/platform_file_utils.c \
		  ../../core/buffer.c \
		  ../../core/asset_utils.c \
		  ../../core/game.c \
		  ../../core/image.c \
		  ../../core/shader.c \
		  ../../core/texture.c
OBJECTS = main.o \
		  platform_asset_utils.o \
		  ../common/platform_log.o \
		  ../common/platform_file_utils.o \
		  ../../core/buffer.o \
		  ../../core/asset_utils.o \
		  ../../core/game.o \
		  ../../core/image.o \
		  ../../core/shader.o \
		  ../../core/texture.o \
		  ../../3rdparty/libpng/png.o \
		  ../../3rdparty/libpng/pngerror.o \
		  ../../3rdparty/libpng/pngget.o \
		  ../../3rdparty/libpng/pngmem.o \
		  ../../3rdparty/libpng/pngpread.o \
		  ../../3rdparty/libpng/pngread.o \
		  ../../3rdparty/libpng/pngrio.o \
		  ../../3rdparty/libpng/pngrtran.o \
		  ../../3rdparty/libpng/pngrutil.o \
		  ../../3rdparty/libpng/pngset.o \
		  ../../3rdparty/libpng/pngtrans.o \
		  ../../3rdparty/libpng/pngwio.o \
		  ../../3rdparty/libpng/pngwrite.o \
		  ../../3rdparty/libpng/pngwtran.o \
		  ../../3rdparty/libpng/pngwutil.o \
		  ../../3rdparty/libzlib/adler32.o \
		  ../../3rdparty/libzlib/crc32.o \
		  ../../3rdparty/libzlib/deflate.o \
		  ../../3rdparty/libzlib/infback.o \
		  ../../3rdparty/libzlib/inffast.o \
		  ../../3rdparty/libzlib/inflate.o \
		  ../../3rdparty/libzlib/inftrees.o \
		  ../../3rdparty/libzlib/trees.o \
		  ../../3rdparty/libzlib/zutil.o
TARGET = airhockey.html

# Targets start here.
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	$(RM) $(TARGET) $(OBJECTS)

depend:
	@$(CC) $(CFLAGS) -MM $(SOURCES)
	
# list targets that do not create files (but not all makes understand .PHONY)
.PHONY:	all clean depend

# Dependences (call 'make depend' to generate); do not delete:
# Build for these is implicit, no need to specify compiler command lines.

main.o: main.c ../../core/game.h
platform_asset_utils.o: platform_asset_utils.c \
  ../common/platform_asset_utils.h ../common/platform_file_utils.h
../common/platform_log.o: ../common/platform_log.c ../common/platform_log.h \
  ../common/platform_macros.h ../../core/config.h
../common/platform_file_utils.o: ../common/platform_file_utils.c \
  ../common/platform_file_utils.h
../../core/buffer.o: ../../core/buffer.c ../../core/buffer.h platform_gl.h
../../core/asset_utils.o: ../../core/asset_utils.c ../../core/asset_utils.h \
  platform_gl.h ../../core/image.h ../common/platform_asset_utils.h \
  ../common/platform_file_utils.h ../../core/shader.h \
  ../../core/texture.h
../../core/game.o: ../../core/game.c ../../core/game.h ../../core/buffer.h \
  platform_gl.h ../../core/asset_utils.h ../../core/image.h \
  ../common/platform_asset_utils.h ../common/platform_file_utils.h \
  ../../core/shader.h ../../core/texture.h
../../core/image.o: ../../core/image.c ../../core/image.h platform_gl.h \
  ../common/platform_asset_utils.h ../common/platform_file_utils.h \
  ../common/platform_log.h ../common/platform_macros.h \
  ../../core/config.h ../../3rdparty/libpng/png.h \
  ../../3rdparty/libpng/pnglibconf.h ../../3rdparty/libpng/pngconf.h
../../core/shader.o: ../../core/shader.c ../../core/shader.h platform_gl.h \
  ../common/platform_log.h ../common/platform_macros.h \
  ../../core/config.h
../../core/texture.o: ../../core/texture.c ../../core/texture.h platform_gl.h