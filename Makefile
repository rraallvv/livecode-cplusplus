CC = llvm-g++
LIVEFILE = default.cpp
PCH_FILE = $(CC_PATH)/cocos/cocos2d.h
ARCH = $(arch)
CC_STATIC_LIB = "$(CC_PATH)/prebuilt/mac/libcocos2d Mac.a"


LD_FLAGS = $(CC_STATIC_LIB) \
	-framework OpenGL \
	-framework GLUT \
	-framework Cocoa \
	-framework CoreAudio \
	-framework CoreVideo \
	-framework IOkit \
	$(CC_PATH)/libs/cairo/lib/osx/cairo.a \
	$(CC_PATH)/libs/cairo/lib/osx/pixman-1.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoCrypto.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoDataSQLite.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoNetSSL.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoZip.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoData.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoFoundation.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoUtil.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoNet.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoXML.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoJSON.a \
	$(CC_PATH)/libs/poco/lib/osx/PocoMongoDB.a \
	$(CC_PATH)/libs/freetype/lib/osx/freetype.a \
	$(CC_PATH)/libs/FreeImage/lib/osx/freeimage.a \
	$(CC_PATH)/libs/boost/lib/osx/boost.a \
	$(CC_PATH)/libs/boost/lib/osx/boost_system.a \
	$(CC_PATH)/libs/glew/lib/osx/glew.a \
	$(CC_PATH)/libs/glfw/lib/osx/glfw3.a \
	$(CC_PATH)/libs/tess2/lib/osx/tess2.a \
	-lcrypto \
	-lssl \
	-F $(CC_PATH)/libs/glut/lib/osx \
	$(CC_PATH)/libs/fmodex/lib/osx/libfmodex.dylib

INCLUDES = -I$(CC_PATH)/libs/openFrameworks/ \
	-I$(CC_PATH)/libs/openFrameworks/3d \
	-I$(CC_PATH)/libs/openFrameworks/app \
	-I$(CC_PATH)/libs/openFrameworks/communication \
	-I$(CC_PATH)/libs/openFrameworks/events \
	-I$(CC_PATH)/libs/openFrameworks/gl \
	-I$(CC_PATH)/libs/openFrameworks/graphics \
	-I$(CC_PATH)/libs/openFrameworks/math \
	-I$(CC_PATH)/libs/openFrameworks/sound \
	-I$(CC_PATH)/libs/openFrameworks/types \
	-I$(CC_PATH)/libs/openFrameworks/utils \
	-I$(CC_PATH)/libs/openFrameworks/video \
	-I$(CC_PATH)/libs/glew/include \
	-I$(CC_PATH)/libs/tess2/include \
	-I$(CC_PATH)/libs/cairo/include/cairo \
	-I$(CC_PATH)/libs/fmodex/include \
	-I$(CC_PATH)/libs/poco/include \
	-I$(CC_PATH)/libs/json/include \
	-I$(CC_PATH)/libs/utf8cpp/include \
	-I$(CC_PATH)/libs/openFrameworks/utils \
	-I$(CC_PATH)/libs/boost/include \
	-I$(CC_PATH)/libs/glfw/include


all:
	g++ main.cpp livecode.cpp -std=c++11 $(INCLUDES) $(LD_FLAGS) -arch $(ARCH) -o livecode

$(PCH_FILE):
	echo "Precompiling ofMain.h"
	g++ -x c++-header $(CC_PATH)/libs/openFrameworks/ofMain.h -c -std=c++11 -arch $(ARCH) $(INCLUDES)


live: $(PCH_FILE)
	g++ $(LIVEFILE) -c -std=c++11 -include $(CC_PATH)/libs/openFrameworks/ofMain.h -I$(CC_PATH)/libs/openFrameworks/ -arch $(ARCH) -o livecode.o
	g++ -I. -dynamiclib -arch $(ARCH) -o livecode.dylib livecode.o \
		$(CC_STATIC_LIB) \
		$(CC_PATH)/libs/freetype/lib/osx/freetype.a \
		-undefined suppress -flat_namespace
	rm livecode.o	


clean:
	rm $(PCH_FILE)
	rm livecode.dylib
