#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include <sys/stat.h>
#include <dlfcn.h>

USING_NS_CC;

using namespace cocostudio::timeline;

void HelloWorld::checkAndUpdate(float dt) {
	struct stat fileStat;
	if(stat(("../../" + livefile).c_str(), &fileStat) < 0) {
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

void HelloWorld::recompileAndReload() {
	clock_t t = clock();

	// call our makefile
	std::string cmd = "make -C ../../ live LIVEFILE=";
	cmd += livefile;
	system(cmd.c_str());
	reload();

	printf("Reload took %.0fms\n", (clock() - t) / (float)CLOCKS_PER_SEC * 1000);
}

void HelloWorld::reload() {
	// don't forget to clean up
	if(livecodeLib!=NULL) {
		if(dlclose(livecodeLib)) {
			printf("Error: %s\n", dlerror());
		} else {
			// TODO:
			// this is a workaround and needs to be fixed by
			// finding a way to close the library without calling dlclose twice
			dlclose(livecodeLib);
		}
	}
	livecodeLib = dlopen("../../Contents/MacOS/livecode.dylib", RTLD_LAZY);
	if (livecodeLib == NULL) {
		// report error ...
		printf("Error: %s\n", dlerror());
	} else {
		// use the result in a call to dlsym
		printf("Success loading\n");


		void *ptrFunc = dlsym(livecodeLib, "livecode");
		if(ptrFunc!=NULL) {

			livecodeFun = (void (*)(void))ptrFunc;
			livecodeFun();
		} else {
			printf("Couldn't find the livecode() function\n");
		}
	}
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

	livefile = "Classes/livecode.cpp";
	livecodeFun = nullptr;
	this->schedule(schedule_selector(HelloWorld::checkAndUpdate), 1.0f);

    return true;
}
