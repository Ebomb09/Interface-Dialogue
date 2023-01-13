#include <iostream>
#include "dialogue.h"

int main(int argc, char* argv[]){

	std::cout << "Starting test\n";

	dialogue diag;
	std::cout << "Created Dialogue object\n";

	bool good = diag.openFile("resource/test-dialogue");
	std::cout << "Opened file " << good << "\n";

	diag.read("Section1");
	std::cout << "Read Section1";

	return 0;
}
