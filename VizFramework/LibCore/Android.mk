
LOCAL_PATH:= $(call my-dir)

# Build CeeViz LibCore
# -------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := CeeVizCore
LOCAL_CFLAGS := -DCVF_ANDROID -Werror
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
#LOCAL_LDLIBS := $(call host-path,$(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libsupc++.a)
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)


