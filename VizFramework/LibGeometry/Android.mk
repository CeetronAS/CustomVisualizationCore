
LOCAL_PATH:= $(call my-dir)

# Build CeeViz LibGeometry
# -------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := CeeVizGeometry
LOCAL_CFLAGS := -DCVF_ANDROID -Werror
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../LibCore
LOCAL_SHARED_LIBRARIES := CeeVizCore
include $(BUILD_SHARED_LIBRARY)
