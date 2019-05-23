#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := tcp_client
EXTRA_COMPONENT_DIRS := $(abspath helix)	$(abspath lvgl) #$(abspath lv_examples)
include $(IDF_PATH)/make/project.mk

