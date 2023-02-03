#include <iostream>
#include "dialogue.h"

int main(int argc, char* argv[]){

	dialogue::handler diag;
	diag.openFile("resource/test-dialogue");

	diag.gotoSection("Section1");

	int gameLoop = diag.next();

	while(gameLoop != dialogue::None){
		int x = -1;

		if(gameLoop == dialogue::Dialogue)
			std::cout << "[" << diag.getSpeaker() << "]\t" << diag.getText() << "\n";
		

		if(gameLoop == dialogue::Option){
			
			for(int i = 0; i < diag.getOptionCount(); i ++){
				std::cout << "(" << i << ".) " << diag.getOptionText(i) << "\n";
			}
			std::cout << "Please select a option... ";

			std::cin >> x;
			std::cin.ignore();

		}else{
			std::cout << "Press [Enter] to continue... ";
			std::cin.ignore();
		}

		if(gameLoop == dialogue::Dialogue)
			gameLoop = diag.next();

		if(gameLoop == dialogue::Option && diag.select(x))
			gameLoop = diag.next();
	}

	return 0;
}
