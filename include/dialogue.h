#ifndef DIALOGUE_INTERACTIONS_H
#define DIALOGUE_INTERACTIONS_H

#include <string>

/****
 * Defines the dialogue class to be used for handling text playback
 * similar to popular RPGs.
 */
class dialogue{

private:
	std::string speaker, text, current_section;
	//map

public:
	bool openFile(const std::string name);
	bool closeFile();

	bool bindString(const std::string, std::string* ptr);
	bool bindInt(const std::string, int* ptr);

	enum readState{
		Done,
		More,
		Option
	};
	int read(const std::string& section = "");
	const std::string& getSpeaker();
	const std::string& getText();

	int getNumberofOptions();
	bool getOption(int index);
	void select(int option);
};

#endif
