
//  Copyright (c) Herb Sutter
//  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


//===========================================================================
//  Lexer
//===========================================================================

#ifndef __CPP2_LEX
#define __CPP2_LEX

#include "load.h"
#include <map>


namespace cpp2 {

//-----------------------------------------------------------------------
// 
//  lexeme: represents the type of a token
// 
//  The symbols that C++ uses for bitwise operators (and sometimes also
//  other meanings) are intentionally not named with "Bit". For example,
//  this uses Caret instead of BitXor. This is because in Cpp2 the
//  built-in types use the intrinsic functions (e.g. bitxor(a,b))
//  instead of the operator notation (e.g., a ^ b). Note that for
//  compatibility with existing C++ experession template libraries we
//  may still offer the operator notation for overloadable operators.
// 
//-----------------------------------------------------------------------
//

enum class lexeme : std::int8_t {
    DivideEq,
    Divide,
    LeftShiftEq,
    LeftShift,
    Spaceship,
    LessEq,
    Less,
    RightShiftEq,
    RightShift,
    GreaterEq,
    Greater,
    PlusPlus,
    PlusEq,
    Plus,
    MinusMinus,
    MinusEq,
    Arrow,
    Minus,
    LogicalOrEq,
    LogicalOr,
    PipeEq,
    Pipe,
    LogicalAndEq,
    LogicalAnd,
    AmpersandEq,
    Ampersand,
    MultiplyEq,
    Multiply,
    ModuloEq,
    Modulo,
    CaretEq,
    Caret,
    TildeEq,
    Tilde,
    EqualComparison,
    Assignment,
    NotEqualComparison,
    Not,
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    Colon,
    Semicolon,
    Comma,
    Dot,
    Dollar,
    FloatLiteral,
    BinaryLiteral,
    DecimalLiteral,
    HexadecimalLiteral,
    StringLiteral,
    CharacterLiteral,
    Keyword,
    Identifier
};

//  TODO: Remove the duplication above/below with a variadic macro,
//        but for now it's simpler just to write it out

template<typename T>
    requires std::is_same_v<T, std::string>
auto as(lexeme l)
{
    switch (l) {
    break;case lexeme::DivideEq:            return "DivideEq";
    break;case lexeme::Divide:              return "Divide";
    break;case lexeme::LeftShiftEq:         return "LeftShiftEq";
    break;case lexeme::LeftShift:           return "LeftShift";
    break;case lexeme::Spaceship:           return "Spaceship";
    break;case lexeme::LessEq:              return "LessEq";
    break;case lexeme::Less:                return "Less";
    break;case lexeme::RightShiftEq:        return "RightShiftEq";
    break;case lexeme::RightShift:          return "RightShift";
    break;case lexeme::GreaterEq:           return "GreaterEq";
    break;case lexeme::Greater:             return "Greater";
    break;case lexeme::PlusPlus:            return "PlusPlus";
    break;case lexeme::PlusEq:              return "PlusEq";
    break;case lexeme::Plus:                return "Plus";
    break;case lexeme::MinusMinus:          return "MinusMinus";
    break;case lexeme::MinusEq:             return "MinusEq";
    break;case lexeme::Arrow:               return "Arrow";
    break;case lexeme::Minus:               return "Minus";
    break;case lexeme::LogicalOrEq:         return "LogicalOrEq";
    break;case lexeme::LogicalOr:           return "LogicalOr";
    break;case lexeme::PipeEq:              return "PipeEq";
    break;case lexeme::Pipe:                return "Pipe";
    break;case lexeme::LogicalAndEq:        return "LogicalAndEq";
    break;case lexeme::LogicalAnd:          return "LogicalAnd";
    break;case lexeme::AmpersandEq:         return "AmpersandEq";
    break;case lexeme::Ampersand:           return "Ampersand";
    break;case lexeme::MultiplyEq:          return "MultiplyEq";
    break;case lexeme::Multiply:            return "Multiply";
    break;case lexeme::ModuloEq:            return "ModuloEq";
    break;case lexeme::Modulo:              return "Modulo";
    break;case lexeme::CaretEq:             return "CaretEq";
    break;case lexeme::Caret:               return "Caret";
    break;case lexeme::TildeEq:             return "TildeEq";
    break;case lexeme::Tilde:               return "Tilde";
    break;case lexeme::EqualComparison:     return "EqualComparison";
    break;case lexeme::Assignment:          return "Assignment";
    break;case lexeme::NotEqualComparison:  return "NotEqualComparison";
    break;case lexeme::Not:                 return "Not";
    break;case lexeme::LeftBrace:           return "LeftBrace";
    break;case lexeme::RightBrace:          return "RightBrace";
    break;case lexeme::LeftParen:           return "LeftParen";
    break;case lexeme::RightParen:          return "RightParen";
    break;case lexeme::LeftBracket:         return "LeftBracket";
    break;case lexeme::RightBracket:        return "RightBracket";
    break;case lexeme::Colon:               return "Colon";
    break;case lexeme::Semicolon:           return "Semicolon";
    break;case lexeme::Comma:               return "Comma";
    break;case lexeme::Dot:                 return "Dot";
    break;case lexeme::Dollar:              return "Dollar";
    break;case lexeme::FloatLiteral:        return "FloatLiteral";
    break;case lexeme::BinaryLiteral:       return "BinaryLiteral";
    break;case lexeme::DecimalLiteral:      return "DecimalLiteral";
    break;case lexeme::HexadecimalLiteral:  return "HexadecimalLiteral";
    break;case lexeme::StringLiteral:       return "StringLiteral";
    break;case lexeme::CharacterLiteral:    return "CharacterLiteral";
    break;case lexeme::Keyword:             return "Keyword";
    break;case lexeme::Identifier:          return "Identifier";
    break;default:                          return "INTERNAL-ERROR";
    }
};


//-----------------------------------------------------------------------
// 
//  token: represents a single token
// 
//     Note: by reference, thge test into the program's source lines
// 
//-----------------------------------------------------------------------
//
class token 
{
public:
    token( 
        char const*     start, 
        auto            count, 
        source_position pos,
        lexeme          type
    )
      : start   {start}
      , count   {int16_t(count)}
      , pos     {pos  }
      , lex_type{type }
    {
    }

    operator std::string_view() const 
    { 
        return {start, (unsigned)count}; 
    }

    auto operator== (std::string_view s) const 
    { 
        return s ==  this->operator std::string_view(); 
    }

    auto to_string( bool text_only = false ) const -> std::string 
    { 
        auto text = std::string{start, (unsigned)count};
        if (text_only) {
            return text;
        }
        else {
            return as<std::string>(lex_type) + std::string(": ") + text;
        }
    }

    friend auto& operator<< (auto& o, token const& t) 
    { 
        return o << std::string_view(t); 
    }

    auto position() const { return pos;      }
    auto type    () const { return lex_type; }

private:
    //  Store (char*,count) because it's smaller than a string_view
    //
    char const*     start;
    int16_t         count;
    source_position pos;
    lexeme          lex_type;
};

static_assert (CHAR_BIT == 8);
//static_assert (sizeof(token) <= 16);    // token fits in 128 bits


//-----------------------------------------------------------------------
//  lex: Tokenize a single line while maintaining inter-line state
//
//  line                    the line to be tokenized
//  lineno                  the current line number
//  in_comment              are we currently in a comment
//  current_comment         the current partial comment
//  current_comment_start   the current comment's start position
//  tokens                  the token list to add to
//  comments                the comment token list to add to
//  errors                  the error message list to use for reporting problems
//
auto lex_line(
    std::string const&      line,
    int const               lineno,
    bool&                   in_comment,
    std::string&            current_comment,
    source_position&        current_comment_start,
    std::vector<token>&     tokens,
    std::vector<comment>&   comments,
    std::vector<error>&     errors
)
    -> bool
{
    auto original_size = std::ssize(tokens);

    auto i = colno_t{0};

    auto peek  = [&](int num) 
    { 
        return (i+num < ssize(line)) ? line[i+num] : '\0';
    };

    auto store = [&](int16_t num, lexeme type) 
    {
        tokens.push_back({
            &line[i],
            num,
            {lineno, i+1},
            type
            });
        i += num-1;
    };

    //-----------------------------------------------------
    //  These functions return the length of sequence if
    //  present at the current location, else 0

    //G escape-sequence:
    //G     \ { any member of the basic source character set except newline }
    //G
    auto peek_is_simple_escape_sequence = [&](int offset) { 
        if (peek(offset) && peek(offset) == '\\' && peek(1+offset)) { return 2; }
        return 0;
    };

    //G hexadecimal-escape-sequence: \x {hexadecimal-digit}+
    //G
    auto peek_is_hexadecimal_escape_sequence = [&](int offset)
    {
        if (peek(offset) && peek(offset) == '\\' && 
            peek(1+offset) == 'x' && 
            is_hexadecimal_digit(peek(2+offset))) 
        {
            auto j = 3;
            while (peek(j+offset) && is_hexadecimal_digit(peek(j+offset))) { ++j; }
            return j;
        }
        return 0;
    };

    //G universal-character-name:
    //G     \u { hexadecimal-digit }4 { hexadecimal-digit }4?
    //G
    auto peek_is_universal_character_name = [&](colno_t offset) 
    { 
        if (peek(offset) && peek(offset) == '\\' && peek(1+offset) == 'u') {
            auto j = 2;
            while (j <= 9 && is_hexadecimal_digit(peek(j+offset))) { ++j; }
            if (j ==  6 || j == 10) { return j; }
            errors.emplace_back(
                source_position{lineno, i+offset}, 
                "invalid universal character name (\\u must"
                    " be followed by 4 or 8 hexadecimal digits)"
            );
        }
        return 0;
    };

    //G escape-sequence:
    //G     hexadecimal-escape-sequence
    //G     simple-escape-sequence
    //G
    auto peek_is_escape_sequence = [&](int offset) 
    { 
        if (auto h = peek_is_hexadecimal_escape_sequence(offset)) { return h; }
        return peek_is_simple_escape_sequence(offset);
    };

    //G s-char:
    //G     universal-character-name
    //G     escape-sequence
    //G     basic-s-char
    //G
    //G basic-s-char: 
    //G     any member of the basic source character set except " \ or new-line
    //G
    //G c-char:
    //G     universal-character-name
    //G     escape-sequence
    //G     basic-c-char
    //G
    //G basic-c-char: 
    //G     any member of the basic source character set except ' \ or new-line
    //G
    auto peek_is_sc_char = [&](int offset, char quote) 
    { 
        if      (auto u = peek_is_universal_character_name(offset))             { return u; }
        else if (auto e = peek_is_escape_sequence(offset))                      { return e; }
        else if (peek(offset) && peek(offset) != quote && peek(offset) != '\\') { return 1; }
        return 0;
    };

    //G keyword:
    //G     any C++20-and-Cpp2 keyword
    //G     any other C++20 keyword
    //GT     import-keyword
    //GT     module-keyword
    //GT     export-keyword
    //G
    auto peek_is_keyword = [&]() 
    { 
        //  Cpp2 has a smaller set of the C++ globally reserved keywords, but we continue to
        //  reserve all the ones C++ has both for compatibility and to not give up a keyword
        //  Cpp2 also adds a couple, notably "is" and "as" (reserved only in Cpp2 code)
        const auto keys = std::regex(
            "^alignas|^alignof|^asm|^as|^auto|"
            "^bool|^break|"
            "^case|^catch|^char|^char16_�t|^char32_�t|^char8_�t|^class|^co_�await|^co_�return|"
            "^co_�yield|^concept|^const|^const_�cast|^consteval|^constexpr|^constinit|^continue|"
            "^decltype|^default|^delete|^double|^do|^dynamic_�cast|"
            "^else|^enum|^explicit|^export|^extern|"
            "^false|^float|^for|^friend|"
            "^goto|"
            "^if|^inline|^int|^is|"
            "^long|"
            "^mutable|"
            "^namespace|^new|^noexcept|^nullptr|"
            "^operator|"
            "^private|^protected|^public|"
            "^register|^reinterpret_�cast|^requires|^return|"
            "^short|^signed|^sizeof|^static|^static_�assert|^static_�cast|^struct|^switch|"
            "^template|^this|^thread_�local|^throw|^true|^try|^typedef|^typeid|^typename|"
            "^union|^unsigned|^using|"
            "^virtual|^void|^volatile|"
            "^wchar_�t|^while"
        );

        std::cmatch m;
        if (std::regex_search(&line[i], m, keys)) {
            assert (m.position(0) == 0);
            return (int)(m[0].length());
        }
        return 0;
    };

    //
    //-----------------------------------------------------

    for ( ; i < ssize(line); ++i) 
    {
        auto peek1 = peek(1);
        auto peek2 = peek(2);

        //G encoding-prefix: one of
        //G     u8 u
        //G
        auto is_encoding_prefix_and = [&](char next) { 
            if (line[i] == next)                        { return 1; }
            else if (line[i] == 'u') {
                if (peek1 == next)                      { return 2; }
                else if (peek1 == '8' && peek2 == next) { return 3; }
            }
            return 0;
        };

        //  If we're currently in a multiline comment,
        //  the only thing to look for is the */ comment end
        //
        if (in_comment) {
            switch (line[i]) {
            //  */ comment end
            break;case '*':
                if (peek1 == '/') {
                    current_comment += "*/";
                    comments.push_back({
                        current_comment_start,
                        current_comment
                        });
                    in_comment = false;
                    ++i;
                }
            break;default:
                current_comment += line[i];
            }
        }

        //  Otherwise, we will be at the start of a token, a comment, or whitespace
        //
        else {
            //G token:
            //G     identifier
            //G     keyword 
            //G     literal
            //G     operator-or-punctuator
            //G
            //G operator-or-punctuator: one of

            switch (line[i]) {

            //  :: .* ->* ? aren't currently used in Cpp2, and aren't needed

            //  (we do need all the overloadable operators for C++ compat,
            //  even if we may not keep their meanings for built-in types)

            //      /* and // comment starts
            //G     /= /
            break;case '/': 
                if (peek1 == '*') {
                    current_comment = "/*";
                    current_comment_start = source_position(lineno, i);
                    in_comment = true;
                    ++i;
                }
                else if (peek1 == '/') {
                    comments.push_back({
                        {lineno, i},
                        std::string(&line[i], std::size(line) - i)
                        });
                    in_comment = false;
                    goto END;
                }
                else if (peek1 == '=') {
                    store(2, lexeme::DivideEq);
                }
                else {
                    store(1, lexeme::Divide);
                }

            //G     <<= << <=> <= <
            break;case '<':
                if (peek1 == '<') { 
                    if (peek2 == '=') { store(3, lexeme::LeftShiftEq); }
                    else { store(2, lexeme::LeftShift); }
                }
                else if (peek1 == '=') { 
                    if (peek2 == '>') { store(3, lexeme::Spaceship); }
                    else { store(2, lexeme::LessEq); }
                }
                else { store(1, lexeme::Less); }

            //G     >>= >> >= >
            break;case '>':
                if (peek1 == '>') { 
                    if (peek2 == '=') { store(3, lexeme::RightShiftEq); }
                    else { store(2, lexeme::RightShift); }
                }
                else if (peek1 == '=') { store(2, lexeme::GreaterEq); }
                else { store(1, lexeme::Greater); }

            //G     ++ += +
            break;case '+':
                if (peek1 == '+') { store(2, lexeme::PlusPlus); }
                else if (peek1 == '=') { store(2, lexeme::PlusEq); }
                else { store(1, lexeme::Plus); }

            //G     -- -= -> -
            break;case '-':
                if (peek1 == '-') { store(2, lexeme::MinusMinus); }
                else if (peek1 == '=') { store(2, lexeme::MinusEq); }
                else if (peek1 == '>') { store(2, lexeme::Arrow); }
                else { store(1, lexeme::Minus); }

            //G     ||= || |= |
            break;case '|':
                if (peek1 == '|') { 
                    if (peek2 == '=') { store(3, lexeme::LogicalOrEq); }
                    else { store(2, lexeme::LogicalOr); }
                }
                else if (peek1 == '=') { store(2, lexeme::PipeEq); }
                else { store(1, lexeme::Pipe); }

            //G     &&= && &= &
            break;case '&': 
                if (peek1 == '&') { 
                    if (peek2 == '=') { store(3, lexeme::LogicalAndEq); }
                    else { store(2, lexeme::LogicalAnd); }
                }
                else if (peek1 == '=') { store(2, lexeme::AmpersandEq); }
                else { store(1, lexeme::Ampersand); }

            //  Next, all the other operators that have a compound assignment form

            //G     *= *
            break;case '*': 
                if (peek1 == '=') { store(2, lexeme::MultiplyEq); }
                else { store(1, lexeme::Multiply); }

            //G     %= %
            break;case '%': 
                if (peek1 == '=') { store(2, lexeme::ModuloEq); }
                else { store(1, lexeme::Modulo); }

            //G     ^= ^
            break;case '^': 
                if (peek1 == '=') { store(2, lexeme::CaretEq); }
                else { store(1, lexeme::Caret); }

            //G     ~= ~
            break;case '~': 
                if (peek1 == '=') { store(2, lexeme::TildeEq); }
                else { store(1, lexeme::Tilde); }

            //G     == =
            break;case '=': 
                if (peek1 == '=') { store(2, lexeme::EqualComparison); }
                else { store(1, lexeme::Assignment); }

            //G     != !
            break;case '!': 
                if (peek1 == '=') { store(2, lexeme::NotEqualComparison); }
                else { store(1, lexeme::Not); }

            //  All the other single-character tokens

            //G     { } ( ) [ ] : ; , . $
            //G
            break;case '{': 
                store(1, lexeme::LeftBrace);

            break;case '}': 
                store(1, lexeme::RightBrace);

            break;case '(': 
                store(1, lexeme::LeftParen);

            break;case ')':
                store(1, lexeme::RightParen);

            break;case '[': 
                store(1, lexeme::LeftBracket);

            break;case ']':
                store(1, lexeme::RightBracket);

            break;case ':':
                store(1, lexeme::Colon);

            break;case ';':
                store(1, lexeme::Semicolon);

            break;case ',':
                store(1, lexeme::Comma);

            break;case '.':
                store(1, lexeme::Dot);

            break;case '$':
                store(1, lexeme::Dollar);

            //G literal:
            //G     integer-literal
            //G     character-literal
            //GT     floating-point-literal
            //G     string-literal
            //GT     boolean-literal
            //GT     pointer-literal
            //GT     user-defined-literal
            //G
            //G integer-literal:
            //G     binary-literal
            //G     hexadecimal-literal
            //G     decimal-literal
            //G
            //G binary-literal:
            //G     0b binary-digit { ' | binary-digit }+
            //G
            //G hexadecimal-literal:
            //G     0x hexadecimal-digit { ' | hexadecimal-digit }+
            //G
            break;case '0': {
                auto j = 3;
                if (peek1 == 'b') {
                    if (is_binary_digit(peek2)) {
                        while (is_separator_or(is_binary_digit,peek(j))) { ++j; }
                        store(j, lexeme::BinaryLiteral);
                    }
                    else {
                        errors.emplace_back(
                            source_position{lineno, i}, 
                            "binary literal cannot be empty (0b must be followed by binary digits)"
                        );
                        ++i;
                    }
                }
                else if (peek1 == 'B') {
                    errors.emplace_back(
                        source_position{lineno, i}, 
                        "0B is invalid - did you mean 0b to introduce a binary literal?"
                    );
                    ++i;
                }
                else if (peek1 == 'x') {
                    if (is_hexadecimal_digit(peek2)) {
                        while (is_separator_or(is_hexadecimal_digit,peek(j))) { ++j; }
                        store(j, lexeme::HexadecimalLiteral);
                    }
                    else {
                        errors.emplace_back(
                            source_position{lineno, i}, 
                            "hexadecimal literal cannot be empty (0x must be followed by hexadecimal digits)"
                        );
                        ++i;
                    }
                }
                else if (peek1 == 'X') {
                    errors.emplace_back(
                        source_position{lineno, i}, 
                        "0X is invalid - did you mean 0x to introduce a hexadecimal literal?"
                    );
                    ++i;
                }
            }

            break;default:

                //G decimal-literal:
                //G     nonzero-digit { ' | digit }*
                //G     
                //G floating-point-literal:
                //G     nonzero-digit { ' | digit }* . { ' | digit }* 
                //G     
                if (is_digit(line[i])) {
                    auto j = 1;
                    while (is_separator_or(is_digit,peek(j))) { ++j; }
                    if (peek(j) != '.') {
                        store(j, lexeme::DecimalLiteral);
                    }
                    else {
                        ++j;
                        while (is_separator_or(is_digit,peek(j))) { ++j; }
                        store(j, lexeme::FloatLiteral);
                    }
                }

                //G string-literal: { encoding-prefix }? " { s-char }* "
                //G
                else if (auto j = is_encoding_prefix_and('\"')) {
                    while (auto len = peek_is_sc_char(j, '\"')) { j += len; }
                    if (peek(j) != '\"') { 
                        errors.emplace_back(
                            source_position{lineno, i},
                            "string literal \"" + std::string(&line[i+1],j) 
                                + "\" is missing its closing \""
                        );
                    }
                    ++i;
                    store(j-1, lexeme::StringLiteral);
                    ++i;
                }

                //G character-literal: { encoding-prefix }? ' { c-char }* '
                //G
                else if (auto j = is_encoding_prefix_and('\'')) {
                    auto len = peek_is_sc_char(j, '\'');
                    if (len > 0) { 
                        j += len; 
                        if (peek(j) != '\'') { 
                            errors.emplace_back(
                                source_position{lineno, i}, 
                                "character literal '" + std::string(&line[i+1],j) 
                                    + "' is missing its closing '"
                            ); 
                        }
                        ++i;
                        store(j-1, lexeme::CharacterLiteral);
                        ++i;
                    }
                    else {
                        errors.emplace_back(
                            source_position{lineno, i}, 
                            "character literal is empty"
                        );
                    }
                }

                //  Keyword
                //
                else if (auto j = peek_is_keyword()) {
                    store(j, lexeme::Keyword);
                }

                //  Identifier
                //
                else if (auto j = starts_with_identifier({&line[i], std::size(line)-i})) {
                    store(j, lexeme::Identifier);
                }

                //  Anything else should be whitespace
                //
                else if (!isspace(line[i])) {
                    errors.emplace_back(
                        source_position{lineno, i}, 
                        std::string("unexpected text '") + line[i] + "'"
                    );
                }
            }
        }
    }

END:
    if (in_comment) {
        current_comment += "\n";
    }

    assert (std::ssize(tokens) >= original_size);
    return std::ssize(tokens) != original_size;
}


//-----------------------------------------------------------------------
// 
//  tokens: a map of the tokens of a source file
// 
//-----------------------------------------------------------------------
//

class tokens 
{
    std::vector<error>& errors;

    //  All non-comment tokens go here, which will be parsed in the parser
    std::map<lineno_t, std::vector<token>> grammar_map;

    //  All comment tokens go here, which are applied in the lexer
    // 
    //  We could put all the tokens in the same map, but that would mean the
    //  parsing logic would have to remember to skip comments everywhere...
    //  simpler to keep comments separate, at the smaller cost of traversing
    //  a second token stream when lowering to C++ to re-interleave comments
    std::vector<comment> comments;

public:
    //-----------------------------------------------------------------------
    //  Constructor
    // 
    //  errors      error list
    //
    tokens(
        std::vector<error>& errors
    )
        : errors{ errors }
    {
    }


    //-----------------------------------------------------------------------
    //  lex: Tokenize the Cpp2 lines
    //
    //  lines       tagged source lines
    //
    auto lex( 
        std::vector<source_line> const& lines
    )
        -> void
    {
        auto in_comment = false;
        assert (std::ssize(lines) > 0);
        auto line = std::begin(lines)+1;
        while (line != std::end(lines)) {

            //  Skip over non-Cpp2 lines
            if (line->cat != source_line::category::cpp2) {
                ++line;
                continue;
            }

            //  At this point, we're at the first line of a Cpp2 code section

            //  Create new map entry for the section starting at this line,
            //  and populate its tokens with the tokens in this section
            auto lineno = std::distance(std::begin(lines), line);
            auto& entry = grammar_map[lineno];
            auto current_comment = std::string{};
            auto current_comment_start = source_position{};

            for ( 
                ; 
                line != std::end(lines) && line->cat == source_line::category::cpp2; 
                ++line, ++lineno
                )
            {
                lex_line(
                    line->text, lineno,
                    in_comment, current_comment, current_comment_start,
                    entry, comments, errors
                );
            }

        }
    }

        
    //-----------------------------------------------------------------------
    //  get_map: Access the token map
    //
    auto get_map() const -> auto const& 
    {
        return grammar_map;
    }


    //-----------------------------------------------------------------------
    //  get_comments: Access the comment list
    //
    auto get_comments() const -> auto const& 
    {
        return comments;
    }


    //-----------------------------------------------------------------------
    //  debug_print
    //
    auto debug_print(std::ostream& o) const -> void 
    {
        for (auto const& [lineno, entry] : grammar_map) {

            o << "--- Section starting at line " << lineno << "\n";
            for (auto const& token : entry) {
                o << "    " << token << " (" << token.position().lineno
                    << "," << token.position().colno << ") " 
                    << as<std::string>(token.type()) << "\n";
            }

        }

        o << "--- Comments\n";
        for (auto const& [pos, text] : comments) {
            o << "    (" << pos.lineno
                << "," << pos.colno << ") " 
                << text << "\n";
        }

    }

};

}

#endif