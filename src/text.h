#ifndef TEXT_H
#define TEXT_H

#include <deque>

class Text : public std::deque<std::string>
{
public:
    Text();

    friend std::istream &operator>>(std::istream& in, Text& list);
    friend std::ostream &operator<<(std::ostream& out, const Text& list);
};

#endif // TEXT_H
