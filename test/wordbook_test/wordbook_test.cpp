#define BOOST_TEST_MODULE WordList test
#include <boost/test/included/unit_test.hpp>

#include "wordbook.h"
#include <list>
#include <string>
#include <algorithm>


bool contains(const WordList& list, const std::string& word) {
    return std::find(list.cbegin(), list.cend(), word) != list.end();
}

bool contains(const WordList& list, const WordList& words) {
    for(const auto& word : words) {
        if(!contains(list, word))
            return false;
    }
    return true;
}

bool contains(const WordList& list, const std::initializer_list<std::string>& words) {
    return contains(list, WordList(words));
}


BOOST_AUTO_TEST_CASE( insert_test )
{
    WordBook list;
    BOOST_CHECK_NO_THROW(list.insert("rain"));
    BOOST_CHECK_THROW(list.insert("spain,"), std::invalid_argument);
    BOOST_CHECK_THROW(list.insert(""), std::length_error);
}


BOOST_AUTO_TEST_CASE( size_test )
{
    WordBook list;
    BOOST_CHECK_EQUAL(list.size(), 0);
    list.insert("rain");
    BOOST_CHECK_EQUAL(list.size(), 1);
    list.insert("rain");
    BOOST_CHECK_EQUAL(list.size(), 1);
    list.insert("rai");
    BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE( hasWord_test )
{
    WordBook list;

    BOOST_CHECK(!list.hasWord("test"));
    list.insert("test");
    BOOST_CHECK(list.hasWord("test"));

    BOOST_CHECK(!list.hasWord("testt"));
    list.insert("testt");
    BOOST_CHECK(list.hasWord("testt"));

    WordList words({"one", "two", "three", "four", "five"});

    list.insert(words);
}


BOOST_AUTO_TEST_CASE( words_test )
{
    WordBook list;

    WordList words({"one", "two", "three", "four", "five"});

    list.insert(words);
    WordList wordsFromList = list.words();

    BOOST_CHECK_EQUAL(list.size(), 5);
    BOOST_CHECK(contains(wordsFromList, words));    
}

BOOST_AUTO_TEST_CASE( correctTheWord_test )
{
    WordBook list;
    list.insert("rain");
    list.insert("spain");
    list.insert("plain");
    list.insert("plaint");
    list.insert("pain");
    list.insert("main");
    list.insert("mainly");
    list.insert("the");
    list.insert("in");
    list.insert("on");
    list.insert("fall");
    list.insert("falls");
    list.insert("his");
    list.insert("was");

    const auto hte = list.correctTheWord("hte");
    const auto rame = list.correctTheWord("rame");
    const auto in = list.correctTheWord("in");
    const auto pain = list.correctTheWord("pain");
    const auto fells = list.correctTheWord("fells");
    const auto mainy = list.correctTheWord("mainy");
    const auto oon = list.correctTheWord("oon");
    const auto teh = list.correctTheWord("teh");
    const auto lain = list.correctTheWord("lain");
    const auto was = list.correctTheWord("was");
    const auto hints = list.correctTheWord("hints");
    const auto pliant = list.correctTheWord("pliant");

    BOOST_CHECK(hte.size() == 1 && contains(hte, "the"));
    BOOST_CHECK(rame.size() == 0);
    BOOST_CHECK(in.size() == 1 && contains(in, "in"));
    BOOST_CHECK(pain.size() == 1 && contains(pain, "pain"));
    BOOST_CHECK(fells.size() == 1 && contains(fells, "falls"));
    BOOST_CHECK(mainy.size() == 2 && contains(mainy, {"main", "mainly"}));
    BOOST_CHECK(oon.size() == 1 && contains(oon, "on"));
    BOOST_CHECK(teh.size() == 1 && contains(teh, "the"));
    BOOST_CHECK(lain.size() == 5 && contains(lain, {"main", "pain", "plain", "plaint", "rain"}));
    BOOST_CHECK(was.size() == 1 && contains(was, "was"));
    BOOST_CHECK(hints.size() == 0);
    BOOST_CHECK(pliant.size() == 1 && contains(pliant, "plaint"));

}
