#include <iostream>
#include <fstream>
#include "dialogue.h"

// Size of the string to account the NULL character
char* init_string(char* buffer, int start, int end){
	int length = end-start+2;
	char* ptr = new char[length];

	memcpy(ptr, buffer+start, length-1);
	ptr[length-1] = '\0';

	return ptr;
}


/****
 * Opens a dialogue file
 *
 * Close whatever file is being used for the text resources
 * and open a new one.
 *
 * @param name	- File Name
 * @return		- Successful or not
 */
bool handler::openFile(const char* name){
	std::ifstream file(name);
	
	if(file.bad()){
		file.close();
		return false;
	}

	char* buffer;

	// Get length of file
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	// Copy file into the buffer
	buffer = new char[length];
	file.read(buffer, length);
	file.close();

	// State variables
	int mode = None;
	int start, end, quote_start, quote_end;
	int startline = 0;

	// Search the buffer for Sections, Dialogues, or Options
	for(int i = 0; i < length; i ++){

		if(buffer[i] == '\n')
			startline = i+1;

		switch(mode){

			default:
			case None:
			{

				// Found a possible Section Header
				if(buffer[i] == '['){
					mode = Section;
					start = i + 1;
				}

				if(buffer[i] == ':' && startline != i){
					mode = Dialogue;
					start = startline;
					end = i-1;

					quote_start = -1;
				}
				break;
			}

			/****
			 * Looking for a string within the section header
			 */
			case Section:
			{

				if(buffer[i] == ']'){
					mode = None;
					end = i - 1;

					if(start <= end){
						char* str = init_string(buffer, start, end);
						keyword word;
						word.type = Section;
						word.section.name = str;

						keywords.push_back(word);
					}

				}

				if(buffer[i] == '\n'){
					std::cerr << "Error Section goes to next line\n";
					mode = None;
				}
				break;
			}

			/****
			 * Looking for two strings within the dialogue
			 */
			case Dialogue:
			{		

				if(buffer[i] == '"' || buffer[i] == '“' || buffer[i] == '”'){
					
					if(quote_start == -1)
						quote_start = i+1;

					else{
						mode = None;
						quote_end = i-1;

						char* speaker = init_string(buffer, start, end);
						char* text = init_string(buffer, quote_start, quote_end);
						keyword word;
						word.type = Dialogue;
						word.dialogue.speaker = speaker;
						word.dialogue.text = text;

						keywords.push_back(word);
					}
				}
				break;
			}
		}
	}

	delete [] buffer;

	return true;
}


bool handler::gotoSection(const char* sectionName){

	for(int i = 0; i < keywords.size(); i ++){

		if(keywords[i].type == Section && strcmp(sectionName, keywords[i].section.name) == 0){
			position = i;
			return true;
		}
	}

	return false;
}


int handler::next(){
	position ++;

	if(position >= keywords.size())
		return None;

	if(keywords[position].type == Section)
		return None;

	return keywords[position].type;
}


const char* handler::getSpeaker(){

	if(keywords[position].type == Dialogue)
		return keywords[position].dialogue.speaker;
	
	return "";
}


const char* handler::getText(){

	if(keywords[position].type == Dialogue)
		return keywords[position].dialogue.text;
	
	return "";
}


int handler::getOptionCount(){

	if(keywords[position].type == Option)
		return keywords[position].option.count;
	
	return 0;
}


const char* handler::getOptionText(int index){

	if(index >= keywords[position].option.count)
		return "";

	return keywords[position].option.options[index];
}


void handler::select(int option){

}