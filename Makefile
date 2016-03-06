CC = llvm-g++
LIVEFILE = default.cpp
PCH_FILE = $(CC_PATH)/cocos/cocos2d.h
ARCH = $(arch)
CC_STATIC_LIB = "$(CC_PATH)/prebuilt/mac/libcocos2d Mac.a"


INCLUDES = -IClasses \
	-I$(CC_PATH)/cocos \
	-I$(CC_PATH)/cocos/editor-support \
	-I$(CC_PATH)/external \
	-I$(CC_PATH)/external/glfw3/include/mac

LD_FLAGS = $(CC_STATIC_LIB) \
	-framework Cocoa \
	-framework AudioToolbox \
	-framework OpenAL \
	-framework CoreVideo \
	-framework OpenGL \
	-framework IOKit \
	-lz \
	-liconv


all:
	g++ main.cpp Classes/AppDelegate.cpp Classes/HelloWorldScene.cpp -std=c++11 $(INCLUDES) $(LD_FLAGS) -arch $(ARCH) -o Contents/MacOS/AppDelegate

$(PCH_FILE):
	echo "Precompiling ofMain.h"
	g++ -x c++-header $(CC_PATH)/libs/openFrameworks/ofMain.h -c -std=c++11 -arch $(ARCH) $(INCLUDES)


live: $(PCH_FILE)
	g++ $(LIVEFILE) -c -std=c++11 -include $(CC_PATH)/libs/openFrameworks/ofMain.h -I$(CC_PATH)/libs/openFrameworks/ -arch $(ARCH) -o AppDelegate.o
	g++ -I. -dynamiclib -arch $(ARCH) -o AppDelegate.dylib AppDelegate.o \
		$(CC_STATIC_LIB) \
		$(CC_PATH)/libs/freetype/lib/osx/freetype.a \
		-undefined suppress -flat_namespace
	rm AppDelegate.o	


clean:
	rm $(PCH_FILE)
	rm AppDelegate.dylib
