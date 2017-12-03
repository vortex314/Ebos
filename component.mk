#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS=.

CXXFLAGS +="-DESP32_IDF=1"
CFLAGS :=-DARDUINO=1
# include $(IDF_PATH)/make/component_common.mk