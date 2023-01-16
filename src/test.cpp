#include <iostream>
#include "dialogue.h"

int main(int argc, char* argv[]){

	handler diag;
	diag.openFile("resource/test-dialogue");

	diag.gotoSection("Section1");

	int gameLoop = diag.next();

	while(gameLoop != None){
		int x;

		if(gameLoop == Dialogue)
			std::cout << "[" << diag.getSpeaker() << "]\t" << diag.getText() << "\n";
		

		if(gameLoop == Option){
			
			for(int i = 0; i < diag.getOptionCount(); i ++){
				std::cout << "(" << i << ".) " << diag.getOptionText(i) << "\n";
			}
			std::cout << "Please select a option... ";

			std::cin >> x;

		}else{
			std::cout << "Press [Enter] to continue... ";
			std::cin.ignore();
		}

		if(gameLoop == Dialogue || (gameLoop == Option && x >= 0))
			gameLoop = diag.next();
	}

	return 0;
}
