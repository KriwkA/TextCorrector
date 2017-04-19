#include "text.h"
#include <iostream>
#include <string>


Text::Text()
{

}

std::istream &operator >> (std::istream &in,  Text &list)
{
    std::string newWord;

    while(true) {
        in >> newWord;
        if(newWord == "===")
            break;
        list.push_back(newWord);
    }
    return in;
}

std::ostream &operator << (std::ostream &out, const Text &list)
{
    for(const auto& word : list) {
        out << word << ' ';
    }
    return out;
}
