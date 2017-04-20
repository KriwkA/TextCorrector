#ifndef TEXT_H
#define TEXT_H


#include "wordbook.h"


class Text : public WordList
{
public:
    Text();

    /*
     * This function reeds the words from @in stream and put them
     * into wordbook,
     * Read continues until not meets the "===" word or streams end
     */
    void readWordBook(std::istream& in);

    /*
     * This operator reeds lines from @in stream, corrects the words
     * in which and append them in @list.
     * Read continues until not meets the "===" word or streams end
     */
    friend std::istream &operator>>(std::istream& in, Text& list);

    /*
     * This operator writes the all lines from list into @out stream
     */
    friend std::ostream &operator<<(std::ostream& out, const Text& list);

private:
    std::string correctWord(const std::string& word);
    WordBook m_wordBook;
    Text(const Text& other) = delete;
    Text(const Text&& other) = delete;
    void operator =(const Text& other) = delete;

};

#endif // TEXT_H
