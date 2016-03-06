#include <dlfcn.h>
#include <sys/stat.h>
#include "ofMain.h"

#include "livecode.h"

forwarderApp::forwarderApp(string livefile, ofAppGlutWindow& window):livefile(livefile),window(window) {
	app = new ofBaseApp();
};

void forwarderApp::reload() {
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

void forwarderApp::recompileAndReload() {
	long t = ofGetSystemTime();

	// call our makefile
	string cmd = "make live LIVEFILE=";
	cmd += livefile;
	system(cmd.c_str());
	reload();

	printf("Reload took %llums\n", ofGetSystemTime() - t);
}

void forwarderApp::checkAndUpdate() {
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

void forwarderApp::idle(void) {
	//long t = ofGetSystemTime();
	float f = ofGetElapsedTimef();

	// check update time on the file

	if(f>lastTimeChecked+0.1) { // check every 300ms
		checkAndUpdate();
		lastTimeChecked = f;
	}
}
