#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Lexer.h"

const char* HelpMessage();
const char* GetFileNameFromArguments(const int& argc, char** argv);
void OutputLexemes(std::list<std::tuple<bool, std::string, std::string>>& output);

int main(int argc, char** argv)
{
    try
    {
        std::string filename(GetFileNameFromArguments(argc, argv));
        std::ifstream file(filename.c_str());
        if (!file.good())
        {
            throw std::invalid_argument(HelpMessage());
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        std::list<std::tuple<bool, std::string, std::string>> output;

        SplitToLogicalLines(output, buffer);
        SplitToLexemes(output);

        OutputLexemes(output);
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return -1;
    }
}

void OutputLexemes(std::list<std::tuple<bool, std::string, std::string>>& output)
{
    std::cout << std::left << std::setw(60) << "Lexeme" << "Type\n";
    std::cout << std::left << std::string(120, '-') << std::endl;

    for (auto & it : output)
    {
        std::cout << std::left << std::setw(60) << std::get<1>(it) << (std::get<2>(it).empty() ? "Unknown" : std::get<2>(it)) << std::endl;
    }
    std::cout << std::endl;
}

const char* GetFileNameFromArguments(const int& argc, char** argv)
{
    if (argc > 2)
        throw std::invalid_argument(HelpMessage());

    return argv[1];
}

const char* HelpMessage()
{
    return "\n Usage: ./LexicalAnalysis <file-name>";
}