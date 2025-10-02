#include <sstream>
#include <regex>

#include "Lexer.h"

void SplitToLogicalLines(std::list<std::tuple<bool, std::string, std::string>>& output, std::stringstream& stream)
{
    output.clear();
    std::string LogicalLine;
    while (std::getline(stream, LogicalLine, '\n'))
    {
        if (!LogicalLine.empty() && LogicalLine.back() == '\r')
        {
            LogicalLine.pop_back();
        }
        output.emplace_back(false, LogicalLine, "" );
    }
}

void SplitToLexemes(std::list<std::tuple<bool, std::string, std::string>>& output)
{
    auto it = std::begin(output);
    while (it != std::end(output))
    {
        EraseWhitespaceOrEmpty(output, it);
        TrimComment(output, it);
        ++it;
    }

    it = std::begin(output);
    while (it != std::end(output))
    {
        EraseWhitespaceOrEmpty(output, it);
        TrimLineContinuation(output, it);
        ++it;
    }

    ProceedTrim(R"(\"([^\"]|\"\")\"c)", "Literal, CharacterLiteral" ,output, it);
    ProceedTrim(R"(\"([^\"]|\"\")*\")", "Literal, StringLiteral", output, it);
    ProceedTrim(R"(#\s*(([0-9]+/[0-9]+/[0-9]+)|([0-9]+-[0-9]+-[0-9]+))\s+((([0-9]+:[0-9]+(:[0-9]+)?\s*(AM|PM)?))|([0-9]+\s*(AM|PM)))\s*#)",
            "Literal, DateLiteral, DateValue and TimeValue", output, it);
    ProceedTrim(R"(#\s*(([0-9]+\/[0-9]+\/[0-9]+)|([0-9]+-[0-9]+-[0-9]+))\s*#)",
            "Literal, DateLiteral, DateValue", output, it);
    ProceedTrim(R"(#\s*((([0-9]+:[0-9]+(:[0-9]+)?\s*(AM|PM)?))|([0-9]+\s*(AM|PM)))\s*#)",
            "Literal, DateLiteral, TimeValue", output, it);
    ProceedTrim(R"(\b\s*(True|False)\s*\b)", "Literal, BooleanLiteral", output, it);
    ProceedTrim(R"(\b\s*Nothing\s*\b)", "Literal, Nothing", output, it);
    ProceedTrim(R"(\b\s*(AddHandler|AddressOf|Alias|And|AndAlso|As|Boolean|ByRef|Byte|ByVal|Call|Case|Catch|CBool|CByte|CChar|CDate|CDbl|CDec|Char|CInt|Class|CLng|CObj|Const|Continue|CSByte|CShort|CSng|CStr|CType|CUInt|CULng|CUShort|Date|Decimal|Declare|Default|Delegate|Dim|DirectCast|Do|Double|Each|Else|ElseIf|End|EndIf|Enum|Erase|Error|Event|Exit|False|Finally|For|Friend|Function|Get|GetType|GetXmlNamespace|Global|GoSub|GoTo|Handles|If|Implements|Imports|In|Inherits|Integer|Interface|Is|IsNot|Let|Lib|Like|Long|Loop|Me|Mod|Module|MustInherit|MustOverride|MyBase|MyClass|Namespace|Narrowing|New|Next|Not|Nothing|NotInheritable|NotOverridable|Object|Of|On|Operator|Option|Optional|Or|OrElse|Overloads|Overridable|Overrides|ParamArray|Partial|Private|Property|Protected|Public|RaiseEvent|ReadOnly|ReDim|REM|RemoveHandler|Resume|Return|SByte|Select|Set|Shadows|Shared|Short|Single|Static|Step|Stop|String|Structure|Sub|SyncLock|Then|Throw|To|True|Try|TryCast|TypeOf|UInteger|ULong|UShort|Using|Variant|Wend|When|While|Widening|With|WithEvents|WriteOnly|Xor)\s*\b)",
            "Keyword", output, it);
    ProceedTrim(R"(\b\s*([A-Za-z_][A-Za-z0-9_]*)\s*\b)",
        "Identifier", output, it);
    ProceedTrim(R"(\b\s*((([0-9]+\.[0-9]+(E(\+|-)?[0-9]+)?)|(\.[0-9]+(E(\+|-)?[0-9]+)?)|([0-9]+(E(\+|-)?[0-9]+)))(F|R|D|!|#|@)?)|([0-9]+(F|R|D|!|#|@))\s*\b)",
        "Literal, FloatingPointLiteral", output, it);
    ProceedTrim(R"(\b\s*(&H[A-F0-9]+(S|US|I|UI|L|UL)?)\s*\b)", "Literal, IntegerLiteral, HexLiteral and IntegerTypeCharacter?", output, it);
    ProceedTrim(R"(\b\s*(&O[0-7]+(S|US|I|UI|L|UL)?)\s*\b)", "Literal, IntegerLiteral, OctalLiteral and IntegerTypeCharacter?", output, it);
    ProceedTrim(R"(\b\s*([0-9]+(S|US|I|UI|L|UL)?)\s*\b)", "Literal, IntegerLiteral, IntLiteral and IntegralTypeCharacter?", output, it);
    ProceedTrim(R"(\s*(\(|\)|\{|\}|!|#|,|\.|:|\?)\s*)", "Separator", output, it);
    ProceedTrim(R"(\s*(\&|\*|\+|-|\/|\\\\|\^|<|=|>)\s*)", "Operator", output, it);

    it = std::begin(output);
    while (it != std::end(output))
    {
        EraseWhitespaceOrEmpty(output, it);
        ++it;
    }
}

void ProceedTrim(const std::string& regex, const std::string& TypeName, std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it)
{
    it = std::begin(output);
    while (it != std::end(output))
    {
        EraseWhitespaceOrEmpty(output, it);
        TrimLiteral(regex, TypeName, output, it);
        ++it;
    }
}

void TrimComment(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it)
{
    std::string& LogicalLineElement = std::get<1>(*it);
    std::string Comment;

    const size_t pos1 = LogicalLineElement.find('\'');
    const size_t pos2 = LogicalLineElement.find("REM");

    const size_t pos = std::min(pos1, pos2);
    if (pos != std::string::npos)
    {
        Comment = LogicalLineElement.substr(pos);
        LogicalLineElement = LogicalLineElement.substr(0, pos);
    }

    if (!Comment.empty())
    {
        ++it;
        output.insert(it, { true, Comment, "Comment"});
        --it;
    }
}

void TrimLineContinuation(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it)
{
    if (std::get<0>(*it) == true)
        return;

    const std::regex compare("(\\s_\\s*)$");
    std::string& LogicalLineElements = std::get<1>(*it);
    const auto lexeme = std::sregex_iterator(LogicalLineElements.begin(), LogicalLineElements.end(), compare);
    const std::string& LineContinuation = lexeme->str();

    if (!LineContinuation.empty())
    {
        LogicalLineElements = LogicalLineElements.substr(0, lexeme->position());
        ++it;
        output.insert(it, { true, LineContinuation, "LineContinuation" });
        --it;
    }
}


void TrimLiteral(const std::string& regex, const std::string& TypeName, std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it)
{
    if (std::get<0>(*it) == true)
        return;

    const std::regex compare(regex);
    std::string& LogicalLineElements = std::get<1>(*it);
    const auto lexemeBegin = std::sregex_iterator(LogicalLineElements.begin(), LogicalLineElements.end(), compare);
    const auto lexemeEnd = std::sregex_iterator();

    if (lexemeBegin != lexemeEnd)
    {
        const std::string& Literal = lexemeBegin->str();

        const std::string& PrevLogicalLineElements = LogicalLineElements.substr(0, lexemeBegin->position());
        const std::string& NextLogicalLineElements = LogicalLineElements.substr(lexemeBegin->position()+ Literal.size());

        if (!Literal.empty())
        {
            LogicalLineElements = PrevLogicalLineElements;
            ++it;
            output.insert(it, { true, Literal, TypeName });
            if (!NextLogicalLineElements.empty())
            {
                output.insert(it, { false, NextLogicalLineElements, ""});
                --it;
            }
            --it;
        }
    }
}

void EraseWhitespaceOrEmpty(std::list<std::tuple<bool, std::string, std::string>>& output, std::list<std::tuple<bool, std::string, std::string>>::iterator& it)
{
    if (std::get<1>(*it).empty() || std::regex_match(std::get<1>(*it), std::regex(R"(^(\s*)$)")))
    {
        ++it;
        const auto it2 = std::prev(it);
        output.erase(it2);
    }
}