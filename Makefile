CC = llvm-g++
LIVEFILE = default.cpp
OF_PATH=..
PCH_FILE = $(OF_PATH)/libs/openFrameworks/ofMain.h.gch
ARCH = -arch x86_64
OF_STATIC_LIB = $(OF_PATH)/libs/openFrameworksCompiled/lib/osx/openFrameworks.a

LD_FLAGS = $(OF_STATIC_LIB) \
				-framework OpenGL \
				-framework GLUT \
				-framework Cocoa \
				-framework CoreAudio \
				-framework CoreVideo \
				-framework IOkit \
				$(OF_PATH)/libs/cairo/lib/osx/cairo.a \
				$(OF_PATH)/libs/cairo/lib/osx/pixman-1.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoCrypto.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoDataSQLite.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoNetSSL.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoZip.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoData.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoFoundation.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoUtil.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoNet.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoXML.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoJSON.a \
				$(OF_PATH)/libs/poco/lib/osx/PocoMongoDB.a \
				$(OF_PATH)/libs/freetype/lib/osx/freetype.a \
				$(OF_PATH)/libs/FreeImage/lib/osx/freeimage.a \
				$(OF_PATH)/libs/boost/lib/osx/boost.a \
				$(OF_PATH)/libs/boost/lib/osx/boost_system.a \
				$(OF_PATH)/libs/glew/lib/osx/glew.a \
				$(OF_PATH)/libs/glfw/lib/osx/glfw3.a \
				$(OF_PATH)/libs/tess2/lib/osx/tess2.a \
				-lcrypto \
				-lssl \
				-F $(OF_PATH)/libs/glut/lib/osx \
				$(OF_PATH)/libs/fmodex/lib/osx/libfmodex.dylib

INCLUDES = -I$(OF_PATH)/libs/openFrameworks/ \
		-I$(OF_PATH)/libs/openFrameworks/3d \
		-I$(OF_PATH)/libs/openFrameworks/app \
		-I$(OF_PATH)/libs/openFrameworks/communication \
		-I$(OF_PATH)/libs/openFrameworks/events \
		-I$(OF_PATH)/libs/openFrameworks/gl \
		-I$(OF_PATH)/libs/openFrameworks/graphics \
		-I$(OF_PATH)/libs/openFrameworks/math \
		-I$(OF_PATH)/libs/openFrameworks/sound \
		-I$(OF_PATH)/libs/openFrameworks/types \
		-I$(OF_PATH)/libs/openFrameworks/utils \
		-I$(OF_PATH)/libs/openFrameworks/video \
		-I$(OF_PATH)/libs/glew/include \
		-I$(OF_PATH)/libs/tess2/include \
		-I$(OF_PATH)/libs/cairo/include/cairo \
		-I$(OF_PATH)/libs/fmodex/include \
		-I$(OF_PATH)/libs/poco/include \
		-I$(OF_PATH)/libs/json/include \
		-I$(OF_PATH)/libs/utf8cpp/include \
		-I$(OF_PATH)/libs/openFrameworks/utils \
		-I$(OF_PATH)/libs/boost/include \
		-I$(OF_PATH)/libs/glfw/include


all: $(OF_STATIC_LIB)
	g++ main.cpp -std=c++11 $(INCLUDES) $(LD_FLAGS) $(ARCH) -o livecode

$(OF_STATIC_LIB):
	xcodebuild -configuration Release -project $(OF_PATH)/libs/openFrameworksCompiled/project/osx/openFrameworksLib.xcodeproj
	
$(PCH_FILE):
	echo "Precompiling ofMain.h"
	g++ -x c++-header $(OF_PATH)/libs/openFrameworks/ofMain.h -c -std=c++11 $(ARCH) $(INCLUDES)


live: $(PCH_FILE) $(OF_STATIC_LIB)
	g++ $(LIVEFILE) -c -std=c++11 -include $(OF_PATH)/libs/openFrameworks/ofMain.h -I$(OF_PATH)/libs/openFrameworks/ $(ARCH) -o livecode.o
	g++ -I. -dynamiclib $(ARCH) -o livecode.dylib livecode.o \
		$(OF_STATIC_LIB) \
		$(OF_PATH)/libs/freetype/lib/osx/freetype.a \
		-undefined suppress -flat_namespace
	rm livecode.o	


clean:
	rm $(OF_STATIC_LIB)
	rm $(PCH_FILE)
	rm livecode.dylib
