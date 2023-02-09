#ifndef DIALOGUE_INTERACTIONS_H
#define DIALOGUE_INTERACTIONS_H

#include <cstring>
#include <vector>
#include <utility>

namespace dialogue{ 
	struct keyword;
	enum keywordIdentifiers{
		None,
		Section,
		Dialogue,
		Option,
		/* Sub Option Specifiers for functions */
		Subtract,
		Add,
		Assign,
		If,
		GoTo
	};

	class handler;
};

struct dialogue::keyword{
	int type;

	union{
		struct{
			int x;
			int y;
			int z;
		}generic;

		struct{
			char* name;
		} section;

		struct{
			char* speaker;
			char* text;
		} dialogue;

		struct{
			char* variable;
			char** options;
			int count;
		} option;
	};
};

/****
 * Defines the dialogue class to be used for handling text playback
 * similar to popular RPGs.
 */
class dialogue::handler{

private:
	/* State Control */
	int start, position;
	std::vector<keyword> keywords;
	std::vector<std::pair<const char*, int> > variables;

public:
	~handler();

	/**
	 * Opens a dialogue file
	 *
	 * Opens the file and repopulates the keywords
	 * associated with the dialogue script.
	 */
	bool openFile(const char* name);

	/**
	 * Gets a variable
	 * 
	 * Looks up the local variable name and returns 
	 * its integer value.
	 */
	int getvar(const char*);

	/**
	 * Assign a variable
	 * 
	 * Assigns a local variable by name and val.
	 */
	void assign(const char* var, int val);

	/**
	 * Manually do function
	 * 
	 * Do the current function, shouldn't have to 
	 * call this as next will trigger automatically.
	 */
	void doFunction();

	/**
	 * Go to section with header name
	 * 
	 * Goes to the section according to sectionName.
	 * 
	 * Returns false on fail and true on sucess.
	 */
	bool gotoSection(const char* sectionName);

	/**
	 * Go to the next keyword
	 * 
	 * Incremenets the position and returns the current()
	 * type.
	 */
	int next();

	/**
	 * Get current section
	 * 
	 * Retrieves the current type, and how you should 
	 * process the current keyword.
	 */
	int current();

	/**
	 * Gets the speaker 
	 * 
	 * Use when the current() type is a Dialogue.
	 */
	const char* getSpeaker();

	/**
	 * Gets the speaker's text 
	 * 
	 * Use when the current() type is a Dialogue.
	 */
	const char* getText();

	/**
	 * Gets number of options
	 * 
	 * Use when the current() type is an Option.
	 */
	int getOptionCount();

	/**
	 * Gets option text
	 * 
	 * Gets the text according to index of the option
	 * use when the current() type is an Option.
	 */
	const char* getOptionText(int index);

	/**
	 * Gets the option variable
	 * 
	 * Gets the name of the option variable, name 
	 * should not be modified.
	 * Use when the current() type is an Option.
	 */
	const char* getOptionVariable();

	/**
	 * Sets option variable
	 * 
	 * Sets the Option Variable to the integer option
	 * Use when the current() type is an Option.
	 * 
	 * Returns false on fail or true on success.
	 */
	bool select(int option);
};

#endif