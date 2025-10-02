#ifndef LEXICALANALYSIS_LEXER_H
#define LEXICALANALYSIS_LEXER_H

#include <list>
#include <string>

void SplitToLogicalLines(std::list<std::tuple<bool, std::string, std::string>>& output, std::stringstream& stream);
void SplitToLexemes(std::list<std::tuple<bool, std::string, std::string>>& output);
void TrimComment(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it);
void TrimLineContinuation(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it);
void ProceedTrim(const std::string& regex, const std::string& TypeName, std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it);
void TrimLiteral(const std::string& regex, const std::string& TypeName, std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it);
void EraseWhitespaceOrEmpty(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it);

#endif //LEXICALANALYSIS_LEXER_H