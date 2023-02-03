# Read Me

## Usage
In order to use this library the `lib` and `include` folder will need to be respectively linked and included with the additional option `-ldialogue`.

Create a `dialogue::handler` object and call `openFile()` to fill in the keywords if an error occurs openFile will return false, files will be closed by the object automatically.

Once an object is appropriately setup you can use `gotoSection()`, `current()`, `next()` for positional settings and the `get-()` functions to retrieve data from the keywords.

## Format
Dialogue scripts follow a similar format to ini: 
> [%SECTION_NAME%]
> %SPEAKER% : %TEXT%
> %optional var% = %command%("%args%")

### Commands
- options("option1" "option2" ... "option n") = `select(int)`
- add("n1" "n2") = `n1 + n2`
- sub("n1" "n2") = `n1 - n2`
- if("var1" "&& | == | <= | ... | != " "var2") = `var1 && var2`
- goto("section") = `gotoSection("section")`

An example is provided in the resources.

## Build

### Required
- C++ Compiler
- Make

Build command to just compile the library is, `make`
If you want to try to run the test, `make run-test`
