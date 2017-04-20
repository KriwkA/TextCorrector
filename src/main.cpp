#include <iostream>
#include "text.h"

int main(int argc, char *argv[])
{
    try {
        Text text;
        text.readWordBook(std::cin);
        std::cin >> text;
        std::cout << text << std ::endl;
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        std::cerr << "Application can`t continue working normaly" << std::endl;
        return -1;
    } catch(...) {
        std::cerr << "Undefined exception" << std::endl;
        std::cerr << "Application can`t continue working normaly" << std::endl;
        return -1;
    }

    return 0;
}
