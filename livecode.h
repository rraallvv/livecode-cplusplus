#ifndef __LIVECODE__
#define __LIVECODE__

#include "ofAppGlutWindow.h"
#include "ofMain.h"

class forwarderApp: public ofBaseApp {
	ofBaseApp *app;
	long prevUpdateTime = 0;
	float lastTimeChecked = 0;
	string livefile;
	void *livecodeLib = NULL;
	ofAppGlutWindow window;

public:
	forwarderApp(string livefile, ofAppGlutWindow& window);
	virtual ~forwarderApp(){}

	void setup(){ app->setup(); }
	void update(){ idle(); app->update(); }
	void draw(){ app->draw(); }
	void exit(){ app->exit(); }

	void windowResized( int w, int h ){ app->windowResized(w, h); }

	void keyPressed( int key ){ app->keyPressed(key); }
	void keyReleased( int key ){ app->keyReleased(key); }

	void mouseMoved( int x, int y ){ app->mouseMoved(x, y); }
	void mouseDragged( int x, int y, int button ){ app->mouseDragged(x, y, button); }
	void mousePressed( int x, int y, int button ){ app->mousePressed(x, y, button); }
	//void mouseReleased(){ app->mouseReleased(); }
	void mouseReleased(int x, int y, int button ){ app->mouseReleased(x, y, button); }

	void dragEvent( ofDragInfo dragInfo ){ app->dragEvent(dragInfo); }
	void gotMessage( ofMessage msg ){ app->gotMessage(msg); }

	void reload();
	void recompileAndReload();
	void checkAndUpdate();
	void idle( void );
};

#endif //__LIVECODE__