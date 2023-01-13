#ifndef DIALOGUE_INTERACTIONS_H
#define DIALOGUE_INTERACTIONS_H

#include <cstring>
#include <vector>

enum keywordIdentifiers{
	None,
	Section,
	Dialogue,
	Option
};

struct keyword{
	int type = None;

	union{
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
	int position;
	std::vector<keyword> keywords;

public:
	bool openFile(const char* name);

	bool bindString(const char* var, std::string* ptr);
	bool bindInt(const char* var, int* ptr);

	/* Process control */
	bool gotoSection(const char* sectionName = "");
	int next();

	/* Getters */
	const char* getSpeaker();
	const char* getText();

	int getOptionCount();
	const char* getOptionText(int index);
	void select(int option);
};

#endif