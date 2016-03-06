#if 0

#include <dlfcn.h>
#include <sys/stat.h>
#include "ofMain.h"

#include "AppDelegate.h"

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
	livecodeLib = dlopen("AppDelegate.dylib", RTLD_LAZY);
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

#endif

#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
	livefile = "livecode.cpp";
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("NewCocosProject", Rect(0, 0, 960, 640));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
