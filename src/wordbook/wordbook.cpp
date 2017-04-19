#include "wordbook_p.h"
#include "wordbook.h"

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

}

void WordBook::insert(const WordList &word)
{

}

void WordBook::remove(const std::string &word)
{

}

int WordBook::size() const
{
    return 0;
}

bool WordBook::hasWord(const std::string &word) const
{
    return false;
}

WordList WordBook::words() const
{
    return WordList();
}

WordList WordBook::correctTheWord(const std::string &str) const
{
    return WordList();
}
