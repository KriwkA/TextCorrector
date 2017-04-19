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
    /*
     * params:
     *  @maxEditCount - the maximum count of insert/remove letter opperation that correctTheWord function can use;
     */
    explicit WordBook(int maxEditCount = 2);
    ~WordBook();

    /*
     * This functions insert word(s) into wordbook
     *
     * throws:
     *  length_error if word is empty;
     *  argument_error if word has forbidden symbol
     */
    void insert(const std::string& word);
    void insert(const WordList& words);

    // This function return count of words inside wordbook
    int size() const;

    // This function checks for enable word in wordbook
    bool hasWord(const std::string& word) const;

    // This function return all words that placed inside wordbook
    WordList words() const;

    /*
     *  This function corrects the @word
     *  return:
     *   if @word exist then function return same word
     *   otherwise function return list of all corrected variants received from that word
     *   or empty list if such words not enable
     */
    WordList correctTheWord(const std::string& word) const ;

private:
    WordBookPrivate* m_private;

    //remove copy constructor and assignment operator
    WordBook(const WordBook& other) = delete;
    void operator=(const WordBook& other) = delete;
};

#endif
