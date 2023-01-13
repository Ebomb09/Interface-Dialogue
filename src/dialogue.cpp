#include <iostream>
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

	file.open(name);
	return file.good();
}

bool dialogue::closeFile(){

	if(file.is_open()){
		file.close();
		return true;
	}
	return false;
}

bool extract_line(std::istream& stream, std::string& data){

	std::string str;
	std::getline(stream, str);
	
	bool insideQuotes = false;
	bool firstOccurrence = false;

	int end = str.size()-1;

	// Find last occurrence of letter
	for(int i = end; i > 0; i --){
		if(str[i] != ' ' && str[i] != '\n')
			end = i + 1;
		break;
	}

	// Remove Pre whitespaces
	for(int i = 0; i < end; i ++){
		
		if(str[i] != ' ' && str[i] != '\n')
			firstOccurrence = true;

		if(str[i] == '"')
			insideQuotes = !insideQuotes;

		if(firstOccurrence && (insideQuotes || str[i] != ' '))
			data += str[i];
	}

	return !stream.eof();
}

int dialogue::read(const std::string& section){
	
	std::string data;

	while(extract_line(file, data)){

		if(data == ("[" + section + "]")){
			
			break;
		}
	}

	//Check next line Pre-emptively and return what needs to be done
	int position = file.tellg();
	data = "";
	extract_line(file, data);

	file.seekg(position);

	if(false)
		std::cout << "DUMMY";
	else
		return readState::Done;
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
