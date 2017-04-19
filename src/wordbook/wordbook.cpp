#include "wordbook_p.h"
#include "wordbook.h"

#include <algorithm>
#include <exception>
#include <regex>

WordBook::WordBook(int maxEditCount)
    : m_private(new WordBookPrivate(maxEditCount))   
{

}

WordBook::~WordBook()
{
    delete m_private;
}

void WordBook::insert(const std::string &word)
{
    if(word.empty())
        throw std::length_error("word musn`t be empty");

    static const std::regex wordRegexpr("^[A-za-z]+$");

    if(!std::regex_match(word, wordRegexpr))
        throw std::invalid_argument("word has forbidden symbol");

    m_private->insert(word);
}

void WordBook::insert(const WordList &words)
{
    for(const auto& word : words)
        insert(word);
}

int WordBook::size() const
{
    return m_private->size();
}

bool WordBook::hasWord(const std::string &word) const
{
    return m_private->hasWord(word);
}

WordList WordBook::words() const
{
    return m_private->words();
}

WordList WordBook::correctTheWord(const std::string &str) const
{
    if(hasWord(str))
        return WordList({str});
    return m_private->correctTheWord(str);
}

//PRIVATE

WordBookPrivate::~WordBookPrivate()
{
    delete m_words;
}

void WordBookPrivate::insert(const std::string &word)
{
    if(m_words->insert(word.cbegin(), word.cend()))
        m_size++;
}




WordList WordBookPrivate::words() const
{
    return m_words->words();
}

