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


handler::~handler(){

	for(int i = 0; i < keywords.size(); i ++){

		switch(keywords[i].type){

			case Section: {

				if(keywords[i].section.name)
					delete [] keywords[i].section.name;
				break;
			}

			case Dialogue: {

				if(keywords[i].dialogue.speaker)
					delete [] keywords[i].dialogue.speaker;

				if(keywords[i].dialogue.text)
					delete [] keywords[i].dialogue.text;

				break;
			}

			case Option: {

				if(keywords[i].option.variable)
					delete [] keywords[i].option.variable;

				if(keywords[i].option.options){

					for(int j = 0; j < keywords[i].option.count; j ++){

						if(keywords[i].option.options[j])
							delete [] keywords[i].option.options[j];
					}

					delete [] keywords[i].option.options;
				}
				break;
			}
		}

	}
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
			case None:{

				// Found a possible Section Header
				if(buffer[i] == '['){
					start = i + 1;
					mode = Section;
				}

				if(buffer[i] == ':' && startline != i){
					start = startline;
					end = i-1;

					quote_start = -1;
					mode = Dialogue;
				}

				if(buffer[i] == '='){

					// Find variable name
					start = -1;
					end = -1;

					for(int j = startline; j < length; j ++){

						if(buffer[j] != ' ' && buffer[j] != '\t'){

							if(start == -1){
								start = j;

							}else{
								end = j;
							}
						}
					}

					if(start == -1 || end == -1){
						std::cerr << "Error no variable declared";
						break;
					}

					// Determine assignment
					int type = None;

					for(int j = i+1; j < length; j ++){

						if(buffer[j] != ' '){

							if(strncmp(buffer+j, "options(", 8) == 0){
								type = Option;
								i += 7;
								break;
							}

							if(strncmp(buffer+j, "add(", 4) == 0){
								type = Add;
								i += 3;
								break;
							}

							if(strncmp(buffer+j, "sub(", 4) == 0){
								type = Add;
								i += 3;
								break;
							}
						}

						if(buffer[j] == '\n' || buffer[j] == '"' || buffer[j] == '(')
							break;
					}
					
					if(type == None){
						std::cerr << "Error no function assigned";
						break;
					}

					// Find number of options
					int number_quotes = 0;

					for(int j = i+1; j < length; j ++){

						if(buffer[j] == '"')
							number_quotes ++;
						
						if(buffer[j] == '\n' || j >= length-1)
							break;
					}

					if(number_quotes < 2){
						std::cerr << "Error no options provided\n";
						break;
					}

					keyword word;
					word.type = type;
					word.option.count = number_quotes/2;
					word.option.variable = init_string(buffer, start, end);
					word.option.options = new char*[number_quotes/2];

					keywords.push_back(word);

					start = keywords.size()-1;
					end = 0;

					quote_start = -1;

					mode = Option;
				}
				break;
			}

			/****
			 * Looking for a string within the section header
			 */
			case Section:{

				if(buffer[i] == ']'){
					mode = None;
					end = i - 1;

					if(start <= end){
						keyword word;
						word.type = Section;
						word.section.name = init_string(buffer, start, end);

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
			case Dialogue:{		

				if(buffer[i] == '"'){
					
					if(quote_start == -1)
						quote_start = i+1;

					else{
						mode = None;
						quote_end = i-1;

						keyword word;
						word.type = Dialogue;
						word.dialogue.speaker = init_string(buffer, start, end);
						word.dialogue.text = init_string(buffer, quote_start, quote_end);

						keywords.push_back(word);
					}
				}
				break;
			}

			case Option: {

				if(buffer[i] == '"'){

					if(quote_start == -1){
						quote_start = i+1;

					}else{
						quote_end = i-1;

						keywords[start].option.options[end++] = init_string(buffer, quote_start, quote_end);

						quote_start = -1;

						if(end >= keywords[start].option.count)
							mode = None;
					}
				}

				if(buffer[i] == ')' && quote_start == -1)
					mode = None;

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
			start = i;
			position = i;
			return true;
		}
	}

	return false;
}


int handler::next(){
	position ++;

	if(current() > Option){

		next();
	}

	return current();
}


int handler::current(){

	if(position >= keywords.size())
		return None;

	if(keywords[position].type == Section && position != start)
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

	if(index >= getOptionCount())
		return "";

	return keywords[position].option.options[index];
}


void handler::select(int option){

}