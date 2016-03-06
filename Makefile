CC = llvm-g++
LIVEFILE = Classes/livecode.cpp
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
	g++ -x c++-header $(CC_PATH)/cocos/cocos2d.h -c -std=c++11 -arch $(ARCH) $(INCLUDES)


live: $(PCH_FILE)
	g++ $(LIVEFILE) -c -std=c++11 -include $(CC_PATH)/cocos/cocos2d.h -I$(CC_PATH)/cocos -arch $(ARCH) -o livecode.o
	g++ -I. -dynamiclib -arch $(ARCH) -o Contents/MacOS/livecode.dylib livecode.o \
		$(CC_STATIC_LIB) \
		-undefined suppress -flat_namespace
	rm livecode.o


clean:
	rm $(PCH_FILE)
	rm Contents/MacOS/livecode.dylib
