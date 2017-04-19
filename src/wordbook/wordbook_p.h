#ifndef WORKBOOK_PRIVATE_H
#define WORKBOOK_PRIVATE_H

#include <list>
#include <string>
#include <algorithm>
#include <regex>

class WordBook;

class WordBookPrivate
{
    friend class WordBook;

    const int m_maxEditCount;
    inline explicit WordBookPrivate(int maxEditCount) : m_maxEditCount(maxEditCount) {}
};

#endif
