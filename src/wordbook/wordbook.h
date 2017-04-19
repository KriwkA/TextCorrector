#ifndef WORKBOOK_H
#define WORKBOOK_H

namespace std {
typedef basic_string<char, char_traits<char>, allocator<char> > string;
template<class _Ty, class _Alloc = allocator<_Ty> > class list;
}

typedef std::list<std::string> WordList;

class WordBookPrivate;

class WordBook
{
public:
    explicit WordBook(int maxEditCount = 2);
    ~WordBook();

    void insert(const std::string& word);
    void insert(const WordList& words);

    void remove(const std::string& word);

    int size() const;
    bool hasWord(const std::string& word) const;
    WordList words() const;

    WordList correctTheWord(const std::string& str) const;

private:
    WordBookPrivate* m_private;
};

#endif
