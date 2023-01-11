#include "dialogue.h"

/****
 * Opens a dialogue file
 *
 * Close whatever file is being used for the text resources
 * and open a new one.
 *
 * @param name	- File Name
 * @return		- Successful or not
 */
bool dialogue::openFile(const std::string name){
	closeFile();
}

bool dialogue::closeFile(){

}

int dialogue::read(const std::string& section){

}

const std::string& dialogue::getSpeaker(){
	return speaker;
}

const std::string& dialogue::getText(){
	return text;
}

int dialogue::getNumberofOptions(){
	return options.size();
}

const std::string& dialogue::getOptionText(int index){
	return options[index];
}

void dialogue::select(int option){

}
