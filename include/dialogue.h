#ifndef DIALOGUE_INTERACTIONS_H
#define DIALOGUE_INTERACTIONS_H

#include <cstring>
#include <vector>
#include <utility>

enum keywordIdentifiers{
	None,
	Section,
	Dialogue,
	Option,
	/* Sub Option Specifiers for functions */
	Subtract,
	Add,
	If
};

struct keyword{
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
class handler{

private:
	/* State Control */
	int start, position;
	std::vector<keyword> keywords;
	std::vector<std::pair<const char*, int> > variables;

public:
	~handler();

	bool openFile(const char* name);

	/* Inter Process Communication */
	int getvar(const char*);
	void assign(const char* var, int val);
	void doFunction();

	/* Process control */
	bool gotoSection(const char* sectionName = "");
	int next();
	int current();

	/* Getters */
	const char* getSpeaker();
	const char* getText();

	int getOptionCount();
	const char* getOptionText(int index);
	bool select(int option);
};

#endif