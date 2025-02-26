#include "Scanner.hpp"

const std::unordered_map<std::string, TokenType> Star::Scanner::s_Keywords =
{
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::MY_FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::MY_TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

Star::Scanner::Scanner(const std::string& source)
    : m_Source(source)
{

}

std::vector<Star::Token> Star::Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        m_Start = m_Current;
        ScanToken();
    }

    m_Tokens.emplace_back(Token(TokenType::MY_EOF, "", nullptr, m_Line));
    return m_Tokens;
}

bool Star::Scanner::IsAtEnd()
{
    return m_Current >= m_Source.size();
}

bool Star::Scanner::IsAlpha(char c)
{
    bool condition = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    return condition;
}

bool Star::Scanner::IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Star::Scanner::IsAlphaNumeric(char c)
{
    return IsAlpha(c) || IsDigit(c);
}

bool Star::Scanner::Match(char expected)
{
    if(IsAtEnd() || m_Source.at(m_Current) != expected)
    {
        return false;
    }
    else
    {
        m_Current++;
        return true;
    }
}

void Star::Scanner::ScanToken()
{
    char c = Advance();
    switch (c)
    {
        case '(':
        {
            AddToken(TokenType::LEFT_PAREN);
            break;
        }
        case ')':
        {
            AddToken(TokenType::RIGHT_PAREN);
            break; 
        }
        case '{':
        {
            AddToken(TokenType::LEFT_BRACE);
            break;
        }
        case '}':
        {
            AddToken(TokenType::RIGHT_BRACE);
            break;
        }
        case ',':
        {
            AddToken(TokenType::COMMA);
            break;
        }
        case '.':
        { 
            AddToken(TokenType::DOT);
            break;
        }
        case '-':
        {
            AddToken(TokenType::MINUS);
            break;
        }
        case '+':
        {
            AddToken(TokenType::PLUS);
            break;
        }
        case ';':
        {
            AddToken(TokenType::SEMICOLON);
            break;
        }
        case '*':
        {
            AddToken(TokenType::STAR);
            break;
        }
        case '[':
        {
            AddToken(TokenType::LEFT_BRACKET);
            break;
        }
        case ']':
        {
            AddToken(TokenType::RIGHT_BRACKET);
            break;
        }
        case '!':
        { 
            AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break; 
        }
        case '=':
        {
            AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break; 
        }
        case '<':
        {
            AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        }
        case '>':
        {
            AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        }
        case '/':
        {
            if(Match('/'))
            {
                while(Peek() != '\n' && !IsAtEnd())
                {
                    Advance();
                }
            }
            else
            {
                AddToken(TokenType::SLASH);
            }
            break; 
        }
        case ' ':
        case '\r':
        case '\t':
        {
            break;
        }
        case '\n':
        { 
            break;
        }
        case '"': 
        { 
            break; 
        }
        default:
        {
            if(IsDigit(c))
            {
                Number();
            }
            else if(IsAlpha(c))
            {
                Identifier();
            }
            else
            {
                Debug::Error(m_Line, "Unexpected character.");
            }
            break;
        }
    }
}

char Star::Scanner::Advance()
{
    m_Current++;
    return m_Source.at(m_Current - 1);
}

void Star::Scanner::AddToken(TokenType type)
{
    AddToken(type, nullptr);
}

void Star::Scanner::AddToken(TokenType type, std::any literal)
{
    std::string text = m_Source.substr(m_Start, m_Current - m_Start);
    m_Tokens.emplace_back(Token(type, text, literal, m_Line));
}

char Star::Scanner::Peek()
{
    if (IsAtEnd())
    {
        return '\0';
    }
    else
    {
        return m_Source.at(m_Current);
    }
    
}

char Star::Scanner::PeekNext()
{
    if (m_Current + 1 >= m_Source.length())
    {
        return '\0';
    }

    return m_Source.at(m_Current + 1);
}

void Star::Scanner::String()
{
    while(Peek() != '"' && !IsAtEnd())
    {
        if(Peek() == '\n')
        {
            m_Line++;
        }
        Advance();
    }

    if(IsAtEnd())
    {
        Debug::Error(m_Line, "Unterminated string.");
        return;
    }

    Advance();

    std::string m_Value = m_Source.substr(m_Start + 1, m_Current - m_Start + 1);
    AddToken(TokenType::STRING, m_Value);
}

void Star::Scanner::Number()
{
    while(IsDigit(Peek()))
    {
        Advance();
    }
    if(Peek() == '.' && IsDigit(PeekNext()))
    {
        Advance();
        while(IsDigit(Peek()))
        {
            Advance();
        }
    }

    std::string m_Value(m_Source.substr(m_Start, m_Current - m_Start));
    AddToken(TokenType::NUMBER, m_Value);
}

void Star::Scanner::Identifier()
{
    while(IsAlphaNumeric(Peek()))
    {
        Advance();
    }
    std::string text = m_Source.substr(m_Start, m_Current - m_Start);
    auto keyword = s_Keywords.find(text);
    if (keyword != s_Keywords.end())
    {
        AddToken(keyword->second);
    }
    else
    {
        AddToken(TokenType::IDENTIFIER);
    }
}