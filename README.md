livecode-cplusplus
==================

A simple C++ live coding environment based on Cocos2d-x. For OSX 10.8 and above.

The idea is that you have a single file open that you edit, and a window displaying visuals (and also a console). Every time you save the file you're editing, it gets recompiled and run in the window on the left. The console tells you if you made any mistakes in your code, and shows you any console output.

A version of Cocos2d-x is providing the visual functionality, sound probably works too but I haven't tried.

It takes about 500ms to compile and inject the code on my mac book air 1.8ghz.

Compilation is sped up by creating a compiled header of the main Cocos2d-x header.


SETUP
=====
1. Clone the repository
	- git clone https://github.com/rraallvv/livecode-cplusplus.git
2. cd into the folder
3. edit CC_PATH in setenv.sh to point to the Cocos2d-x path
4. type ". ./setenv.sh" (note it's dot space dot slash setenv.sh) to set the needed enviroment variables
5. type "make" to build everything
6. type "./Contents/MacOS/livecode" to start the program - the first time it starts up takes a moment.
7. edit the file livecode.cpp - the app will update every time you save

Alternatively you can build the project from Xcode, although you’ll still need to the app from the command line.
