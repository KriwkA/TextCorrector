#include "text.h"
#include <iostream>
#include <regex>
#include <exception>


Text::Text()
{

}

void Text::readWordBook(std::istream &in)
{
    std::string str;
    while(true) {
        in >> str;
        if(str == "===")
            break;
        try {
            m_wordBook.insert(str);
        } catch(const std::invalid_argument& e) {
            std::cerr << "Exception: Invalid argument: \"" << str << "\" " << e.what() << std::endl;
        } catch(const std::length_error& e) {
            std::cerr << "Exception: length error: " << e.what() << std::endl;
        }
    }
}

std::string Text::correctWord(const std::string &word)
{
    const WordList words = m_wordBook.correctTheWord(word);

    switch (words.size()) {
    case 0: return "{" + word + "?}";
    case 1: return *words.begin();
    }

    std::string result = "{";
    for(const auto& word : words) {
        result.append(word + ' ');
    }
    (*result.rbegin()) = '}';

    return result;
}

std::istream &operator >> (std::istream &in,  Text &list)
{

    char c;
    std::string line;
    std::string word;
    std::string newLine;

    while(true) {
        std::getline(std::cin, line);
        if(line == "===")
            break;

        for(char sign : line) {
            if(std::regex_match(&sign, &sign + 1, std::regex(WordBook::wordRegex))) {
                word.append(&sign, &sign  + 1);
            } else {
                if(word.size()) {
                    newLine.append(list.correctWord(word));
                    newLine.append(&sign, &sign  + 1);
                }
                word.clear();
            }
        }

        if(word.size()) {
            newLine.append(list.correctWord(word));
        }
        word.clear();

        list.push_back(newLine);
        newLine.clear();
    }

    return in;
}

std::ostream &operator << (std::ostream &out, const Text &list)
{
    for(const auto& word : list) {
        out << word << std::endl;
    }
    return out;
}
