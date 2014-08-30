LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := wxsqlite_static
LOCAL_MODULE_FILENAME := libwxsqlite

LOCAL_SRC_FILES := \
sqlite3secure.c

LOCAL_EXPORT_LDLIBS := -lc -lz -lstdc++

LOCAL_CFLAGS := -DUSE_FILE32API -fexceptions

include $(BUILD_STATIC_LIBRARY)
