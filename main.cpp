#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ofAppGlutWindow.h"

#include "ofMain.h"

class forwarderApp: public ofBaseApp {
public:
	ofBaseApp *app;
	long prevUpdateTime = 0;
	float lastTimeChecked = 0;
	string livefile;
	void *livecodeLib = NULL;
	ofAppGlutWindow window;

	forwarderApp(string livefile, ofAppGlutWindow& window):livefile(livefile),window(window){ app = new ofBaseApp(); };
	virtual ~forwarderApp(){}

	void setup(){ app->setup(); }
	void update(){ idle(); app->update(); }
	void draw(){ app->draw(); }
	void exit(){ app->exit(); }

	void windowResized(int w, int h){app->windowResized(w, h); }

	void keyPressed( int key ){ app->keyPressed(key); }
	void keyReleased( int key ){app->keyReleased(key); }

	void mouseMoved( int x, int y ){ app->mouseMoved(x, y); }
	void mouseDragged( int x, int y, int button ){ app->mouseDragged(x, y, button); }
	void mousePressed( int x, int y, int button ){ app->mousePressed(x, y, button); }
	//void mouseReleased(){ app->mouseReleased(); }
	void mouseReleased(int x, int y, int button ){ app->mouseReleased(x, y, button); }

	void dragEvent(ofDragInfo dragInfo) { app->dragEvent(dragInfo); }
	void gotMessage(ofMessage msg){ app->gotMessage(msg); }

	void reload() {
		// don't forget to clean up
		if(livecodeLib!=NULL) {
			app->exit();
			delete app;
			if(dlclose(livecodeLib)) {
				printf("Error: %s\n", dlerror());
			} else {
				// TODO:
				// this is a workaround and needs to be fixed by
				// finding a way to close the library without calling dlclose twice
				dlclose(livecodeLib);
			}
		}
		livecodeLib = dlopen("livecode.dylib", RTLD_LAZY);
		if (livecodeLib == NULL) {
			// report error ...
			printf("Error: %s\n", dlerror());
		} else {
			// use the result in a call to dlsym
			printf("Success loading\n");


			void *ptrFunc = dlsym(livecodeLib, "getAppPtr");
			if(ptrFunc!=NULL) {

				app = ((ofBaseApp *(*)(ofAppBaseWindow&))ptrFunc)(window);
				app->setup();
			} else {
				printf("Couldn't find the getAppPtr() function\n");
			}
		}
	}

	void recompileAndReload() {
		long t = ofGetSystemTime();

		// call our makefile
		string cmd = "make live LIVEFILE=";
		cmd += livefile;
		system(cmd.c_str());
		reload();

		printf("Reload took %llums\n", ofGetSystemTime() - t);
	}

	void checkAndUpdate() {
		struct stat fileStat;
		if(stat(livefile.c_str(), &fileStat) < 0) {
			printf("Couldn't stat file\n");
			return;
		}
		long currUpdateTime = fileStat.st_mtime;
		if(currUpdateTime!=prevUpdateTime) {
			printf("Compile reload\n");
			recompileAndReload();
		}
		prevUpdateTime = currUpdateTime;
	}

	void idle(void) {
		//long t = ofGetSystemTime();
		float f = ofGetElapsedTimef();

		// check update time on the file

		if(f>lastTimeChecked+0.1) { // check every 300ms
			checkAndUpdate();
			lastTimeChecked = f;
		}
	}
};


int main(int argc, char** argv) {

	string livefile = "default.cpp";
	ofAppGlutWindow window;

	// if there's an argument,
	if(argc>1) {

		livefile = argv[1];
		printf("Using live file '%s'\n", argv[1]);
	} // otherwise using default
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new forwarderApp(livefile, window));
	
    return EXIT_SUCCESS;
}
