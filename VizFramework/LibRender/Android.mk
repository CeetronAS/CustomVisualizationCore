
LOCAL_PATH:= $(call my-dir)

# Build CeeViz LibRender
# -------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := CeeVizRender
LOCAL_CFLAGS := -DCVF_ANDROID -DCVF_OPENGL_ES -Werror
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST := $(subst "$(LOCAL_PATH)/", "\n", $(FILE_LIST))
FILE_LIST := $(filter-out %_FF.cpp, $(FILE_LIST))
LOCAL_SRC_FILES := $(FILE_LIST)
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../LibCore \
	$(LOCAL_PATH)/../LibGeometry
LOCAL_SHARED_LIBRARIES := CeeVizCore CeeVizGeometry
LOCAL_LDLIBS := -lGLESv2
include $(BUILD_SHARED_LIBRARY)
