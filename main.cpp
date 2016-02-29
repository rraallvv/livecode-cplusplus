#include "livecode.h"

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
