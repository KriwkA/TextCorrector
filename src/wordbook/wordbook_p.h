#ifndef WORKBOOK_PRIVATE_H
#define WORKBOOK_PRIVATE_H

#include <set>
#include <list>
#include <string>

class WordBook;

typedef std::list<std::string> WordList;
typedef  std::string::const_iterator strciter;

class WordBookPrivate : public std::set<std::string>
{
    enum Operation {Equals = 0, Delete, Insert, Replace};
#define EQUALS  (canCorrect(bBegin + 1, bEnd, wBegin + 1, wEnd, editCount, Equals))
#define REPLACE (canCorrect(bBegin + 1, bEnd, wBegin + 1, wEnd, editCount - 2, Replace))
#define REMOVE (prevOperator != Delete && canCorrect(bBegin, bEnd, wBegin + 1, wEnd, editCount - 1, Delete))
#define INSERT (prevOperator != Insert && canCorrect(bBegin + 1, bEnd, wBegin, wEnd, editCount - 1, Insert))

    friend class WordBook;

    const int m_maxEditCount;
    inline explicit WordBookPrivate(int maxEditCount) : m_maxEditCount(maxEditCount) {}

    WordList words() const;
    WordList correctTheWord(const std::string &str) const;
    static bool canCorrect(const std::string &base, const std::string &wrong, int editCount);
    static bool canCorrect(strciter bBegin, strciter bEnd, strciter wBegin, strciter wEnd, int editCount, Operation prevOperator = Equals);



};

#endif
