#include "wordbook_p.h"
#include "wordbook.h"

#include <algorithm>
#include <exception>
#include <regex>

const char* WordBook::wordRegex = "^[A-za-z]+$";

WordBook::WordBook(int maxEditCount)
    : m_private(new WordBookPrivate(maxEditCount))   
{
}

WordBook::~WordBook()
{
    delete m_private;
}

WordBook::WordBook(WordBook &&other)
{
    m_private = other.m_private;
    other.m_private = new WordBookPrivate(m_private->m_maxEditCount);
}

void WordBook::insert(const std::string &word)
{
    static const std::regex wordRegexpr(wordRegex);
    if(!word.empty()) {
        if(std::regex_match(word, wordRegexpr)) {
            m_private->insert(word);
            return;
        }
        throw std::invalid_argument("word has forbidden symbol");
        return;
    }
    throw std::length_error("word musn`t be empty");
}

void WordBook::insert(const WordList &words)
{
    for(const auto& word : words)
        insert(word);
}

void WordBook::remove(const std::string &word)
{
    m_private->remove(word);
}

int WordBook::size() const
{
    return m_private->size();
}

bool WordBook::hasWord(const std::string &word) const
{
    return m_private->hasWord(word);
}

WordList WordBook::words() const
{
    return m_private->words();
}

WordList WordBook::correctTheWord(const std::string &word) const
{
    if(hasWord(word))
        return WordList({word});
    return m_private->correctTheWord(word);
}

//PRIVATE

WordBookPrivate::WordBookPrivate(int maxEditCount)
    : m_maxEditCount(maxEditCount)
    , m_size(0)
    , m_words(new Node())
{
}

WordBookPrivate::~WordBookPrivate()
{
    delete m_words;
}

void WordBookPrivate::insert(const std::string &word)
{
    if(m_words->insert(word.cbegin(), word.cend()))
        m_size++;
}

void WordBookPrivate::remove(const std::string &word)
{
    if(m_words->remove(word.cbegin(), word.cend()))
        m_size--;
}

WordList WordBookPrivate::words() const
{
    return m_words->words();
}

Node::Node() : Node('\0') {}

Node::Node(char letter, const Node *parent)
    : m_letter(letter)
    , m_wordEnd(false)
    , m_parent(parent)
{
}

Node::~Node() {
    for(auto& node : m_childs)
        delete node.second;
}

bool Node::insert(const strciter &begin, const strciter &end)
{
    if(begin != end) {
        if(m_childs.count(*begin)) {
            return m_childs[*begin]->insert(begin + 1, end);
        }
        m_childs[*begin] = new Node(*begin, this);
        return m_childs[*begin]->insert(begin + 1, end);
    }

    if(!(m_letter == '\0' || m_wordEnd))
        return m_wordEnd = true;

    return false;
}

bool Node::remove(const strciter &begin, const strciter &end)
{
    if(begin != end) {
        if(m_childs.count(*begin)) {
            return m_childs[*begin]->remove(begin + 1, end);
        }
        return false;

    }

    if(!(m_letter == '\0' || !m_wordEnd)) {
        m_wordEnd = false;
        return true;
    }

    return false;
}

bool Node::hasWord(const strciter& begin, const strciter& end) const
{
    if(begin != end) {
        if(m_childs.count(*begin)) {
            return m_childs.at(*begin)->hasWord(begin + 1, end);
        }
        return false;
    }
    return m_wordEnd;
}

WordList Node::words() const
{
    WordList result;

    if(m_wordEnd) {
        result.push_back(getWord());
    }

    for(const auto& child : m_childs) {
        pushListToList(result, child.second->words());
    }

    return result;
}

WordList Node::correctTheWord(const std::string &word, int maxEditCount) const
{
    WordList result;
    auto dp = std::map<const Node*, NodeChecker>();
    for(const auto& child : m_childs) {
        const Node* node = child.second;
        pushListToList(result, node->correctTheWord(word.cbegin(), word.cend(), maxEditCount, Equals, dp));
    }
    return result;
}

WordList Node::correctTheWord(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    auto& currentdp = dp[this];

    static int i = 0;

    if(currentdp.check(begin, editCount, prevOperation)) {
        return WordList();
    }

    WordList result;

    const bool strEnd = begin == end;

    if(!(strEnd && isEndNode())) {

        if(strEnd) {
            m_parent->pushIfNotSelect(result, dp);
        }

        CORRECT_EQUALS;
        if(editCount) {
            CORRECT_REMOVE;
            CORRECT_INSERT;
        }

        if(!isEndNode())
            currentdp.set(begin, editCount, prevOperation);

        return result;
    }    

    m_parent->pushIfNotSelect(result, dp);
    return result;
}

WordList Node::correctInsert(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    if(prevOperation == Insert || isEndNode())
        return WordList();
    return correctForChildsAndEndNode(begin, end, editCount - 1, Insert, dp);
}

WordList Node::correctRemove(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    if(prevOperation == Remove || begin == end)
        return WordList();
    return this->correctTheWord(begin + 1, end, editCount - 1, Remove, dp);
}

WordList Node::correctEquals(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    if(begin == end || *begin != m_letter)
        return WordList();
    return correctForChildsAndEndNode(begin + 1, end, editCount, Equals, dp);
}

WordList Node::correctForChilds(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    WordList result;
    if(m_childs.size()) {
        for(const auto& child : m_childs) {
            const Node* node = child.second;
            pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, dp));
        }
    }
    return result;
}

WordList Node::correctForChildsAndEndNode(const strciter &begin, const strciter &end, int editCount, Node::Operation prevOperation,  std::map<const Node*, NodeChecker>& dp) const
{
    WordList result;

    if(m_childs.size()) {
        for(const auto& child : m_childs) {
            const Node* node = child.second;
            pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, dp));
        }
    } else {
        Node* node = new Node('\0', this);
        pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, dp));
        delete node;
    }

    return result;
}

std::string Node::getWord() const {
    if(!m_parent)
        return "";
    return m_parent->getWord() + m_letter;
}

void Node::pushIfNotSelect(WordList &list, std::map<const Node*, NodeChecker>& dp) const
{
    auto& currentdp = dp[this];
    if(currentdp.selected || !this->m_wordEnd)
        return;
    currentdp.selected = true;
    list.push_back(getWord());
}

void Node::pushListToList(WordList &target, const WordList &data) {
    for(const auto& word : data)
        target.push_back(word);
}

