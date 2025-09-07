//#include "sdl/sdlwin.hpp"
//#include <ktMini/AST.hpp>
#include <eda/emath.hpp>
int Index(int offset, int size, int i) {
	return offset + ((size + (i % size)) % size);
}
int main(void** appstate, int argc, char* argv[])
{
	return testFunc();
	//return SDL_AppInit(appstate,argc,argv);
}