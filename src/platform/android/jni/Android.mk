LOCAL_PATH := $(call my-dir)
PROJECT_ROOT_PATH := $(LOCAL_PATH)/../../../
CORE_RELATIVE_PATH := ../../../core/

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := platform_asset_utils.c \
                   platform_log.c \
                   renderer_wrapper.c \
				   $(CORE_RELATIVE_PATH)/asset_utils.c \
				   $(CORE_RELATIVE_PATH)/buffer.c \
				   $(CORE_RELATIVE_PATH)/game_objects.c \
                   $(CORE_RELATIVE_PATH)/game.c \
                   $(CORE_RELATIVE_PATH)/image.c \
                   $(CORE_RELATIVE_PATH)/program.c \
                   $(CORE_RELATIVE_PATH)/shader.c \
                   $(CORE_RELATIVE_PATH)/texture.c \
                  
LOCAL_C_INCLUDES := $(PROJECT_ROOT_PATH)/platform/common/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/3rdparty/linmath/
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_LDLIBS := -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(PROJECT_ROOT_PATH)/3rdparty)
$(call import-module,libpng)