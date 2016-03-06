#include <iostream>

// required by AppDelegate-c++ to workls
extern "C" {	
	void livecode(void) {
		printf("It's alive!\n");
	}
};
