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

void WordBook::remove(const std::string &word)
{
    m_private->erase(word);
}

int WordBook::size() const
{
    return m_private->size();
}

bool WordBook::hasWord(const std::string &word) const
{
    return m_private->count(word);
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

WordList WordBookPrivate::words() const
{
    WordList result;
    for(const auto& word : *this)
        result.push_back(word);
    return result;
}

WordList WordBookPrivate::correctTheWord(const std::string &str) const
{
    WordList result;
    for(const auto& word : *this) {
        if(canCorrect(word, str, m_maxEditCount)) {
            result.push_back(word);
        }
    }
    return result;
}

bool WordBookPrivate::canCorrect(const std::string &base, const std::string &wrong, int editCount)
{
    return canCorrect(base.cbegin(), base.cend(), wrong.cbegin(), wrong.cend(), editCount);
}

bool WordBookPrivate::canCorrect(strciter bBegin, strciter bEnd, strciter wBegin, strciter wEnd, int editCount, WordBookPrivate::Operation prevOperator)
{
    if(editCount < 0)
        return false;

    if(bBegin == bEnd && wBegin == wEnd)
        return true;

    if(bBegin != bEnd && wBegin != wEnd)
        return (*bBegin == *wBegin ? EQUALS : REPLACE) || INSERT || REMOVE;

    if(bBegin == bEnd)
        return REMOVE;

    return INSERT;
}
