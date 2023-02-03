#include <iostream>
#include <fstream>
#include "dialogue.h"

using namespace dialogue;

/**
 * Initializes a copy of a string
 * 
 * Creates a copy of a string based on the start position
 * and the end position. If start == end then get the single 
 * character as a string. Adds 1 to the expected length
 * to fulfill the required NULL character.
 */
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


bool handler::openFile(const char* name){
	std::ifstream file(name);
	
	if(file.fail()){
		file.close();
		return false;
	}

	keywords.clear();

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

				// Found a possible Dialogue Header
				if(buffer[i] == ':' && startline != i){
					start = startline;
					end = i-1;

					quote_start = -1;
					mode = Dialogue;
				}

				// Found a possible Function Header
				if(buffer[i] == '('){

					// Find variable name if exists
					bool variable = false;
					start = -1;
					end = -1;

					for(int j = startline; j < i; j ++){

						if(buffer[j] == '=' && start != -1){
							variable = true;
							break;
						}

						if(isalpha(buffer[j])){

							if(start == -1){
								start = j;
								end = j;

							}else{
								end = j;
							}
						}
					}

					// Determine Function
					int type = None;

					for(int j = startline; j < i; j ++){

						if(isalpha(buffer[j])){

							if(strncmp(buffer+j, "options(", 8) == 0){
								type = Option;
								break;
							}

							if(strncmp(buffer+j, "add(", 4) == 0){
								type = Add;
								break;
							}

							if(strncmp(buffer+j, "sub(", 4) == 0){
								type = Subtract;
								break;
							}

							if(strncmp(buffer+j, "if(", 3) == 0){
								type = If;
								break;
							}

							if(strncmp(buffer+j, "goto(", 5) == 0){
								type = GoTo;
								break;
							}

						}

						if(buffer[j] == '\n' || buffer[j] == '"')
							break;
					}
					
					if(type == None){
						std::cerr << "Error no function assigned\n";
						break;
					}

					// Find number of options
					int number_quotes = 0;

					for(int j = i+1; j < length; j ++){

						if(buffer[j] == '"')
							number_quotes ++;
						
						if(buffer[j] == '\n' || buffer[j] == ')')
							break;
					}

					if(number_quotes < 2){
						std::cerr << "Error no options provided\n";
						break;
					}

					// Set up word and use the default variable if none declared
					keyword word;
					word.type = type;
					word.option.count = number_quotes/2;
					if(variable){
						word.option.variable = init_string(buffer, start, end);
					}else{
						word.option.variable = new char[8];
						strcpy(word.option.variable, "default");
					}
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
			next();
			return true;
		}
	}

	return false;
}


int handler::next(){
	position ++;

	if(current() > Option){
		doFunction();
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

	if(keywords[position].type >= Option)
		return keywords[position].option.count;
	
	return 0;
}


const char* handler::getOptionText(int index){

	if(current() < Option)
		return NULL;

	if(index < 0 || index >= getOptionCount())
		return NULL;

	return keywords[position].option.options[index];
}


const char* handler::getOptionVariable(){

	if (current() >= Option)
		return keywords[position].option.variable;

	return NULL;
}


bool handler::select(int option){

	if(current() == Option && option >= 0 && option < getOptionCount()){
		assign(getOptionVariable(), option);
		return true;
	}

	return false;
}


void handler::doFunction(){

	switch(keywords[position].type){

		case Subtract:{

			if(getOptionCount() == 2){
				int x = getvar(getOptionText(0));
				int y = getvar(getOptionText(1));
				assign(getOptionVariable(), x-y);
			}
			break;
		}

		case Add:{

			if(getOptionCount() == 2){
				int x = getvar(getOptionText(0));
				int y = getvar(getOptionText(1));
				assign(getOptionVariable(), x+y);
			}
			break;
		}

		// Check if condition and when true do the next line
		case If:{

			if(getOptionCount() == 3){
				const char* symbol = getOptionText(1);
				int x = getvar(getOptionText(0));
				int y = getvar(getOptionText(2));
				int cond = 0;

				if(strcmp(symbol, "<") == 0)		cond = x<y;
				else if(strcmp(symbol, ">") == 0)	cond = x>y;
				else if(strcmp(symbol, "<=") == 0)	cond = x<=y;
				else if(strcmp(symbol, ">=") == 0)	cond = x>=y;
				else if(strcmp(symbol, "==") == 0)	cond = x==y;
				
				assign(getOptionVariable(), cond);				

				if(!cond)
					position ++;
			}
			break;
		}

		case GoTo:{

			if(getOptionCount() == 1){
				const char* var = getOptionVariable();
				const char* section = getOptionText(0);

				bool success = gotoSection(section);
				assign(var, success);
			}
			break;
		}

	}
}


int handler::getvar(const char* var){

	for(int i = 0; i < variables.size(); i ++){

		if(strcmp(var, variables[i].first) == 0)
			return variables[i].second;
	}

	// Try to convert or error 0
	return strtol(var, NULL, 10);
}


void handler::assign(const char* var, int val){

	for(int i = 0; i < variables.size(); i ++){

		if(strcmp(var, variables[i].first) == 0){
			variables[i].second = val;
			return;
		}
	}
	variables.push_back(std::pair<const char*, int>(var, val));
}