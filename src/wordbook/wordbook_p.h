#ifndef WORKBOOK_PRIVATE_H
#define WORKBOOK_PRIVATE_H

#include <list>
#include <string>
#include <map>
#include <set>

class WordBook;

typedef std::list<std::string> WordList;
typedef  std::string::const_iterator strciter;

class Node
{
public:
    Node() : Node('\0') {}

    explicit Node(char letter, Node* parent = nullptr)
        : m_letter(letter)
        , m_wordEnd(false)
        , m_parent(parent)
    {
    }

    ~Node() {
        for(auto& node : m_childs)
            delete node.second;
    }

    bool insert(strciter begin, strciter end)
    {
       if(begin == end) {
          if(m_letter == '\0' || m_wordEnd)
            return false;
          return m_wordEnd = true;
       }

       if(!m_childs.count(*begin)) {
           m_childs[*begin] = new Node(*begin, this);
       }

       return m_childs[*begin]->insert(begin + 1, end);
    }

    bool hasWord(strciter begin, strciter end) const
    {
        if(begin == end) {
             return m_wordEnd;
        }

        if(!m_childs.count(*begin)) {
            return false;
        }

        return m_childs.at(*begin)->hasWord(begin + 1, end);
    }

    std::string getWord() const {
        if(!m_parent)
            return "";

        return m_parent->getWord() + m_letter;
    }

    static void pushListToList(WordList& target, const WordList& data) {
        for(const auto& word : data)
            target.push_back(word);
    }

    WordList words() const
    {
        WordList result;
        if(m_wordEnd)
            result.push_back(getWord());

        for(const auto& child : m_childs) {
            pushListToList(result, child.second->words());
        }
        return result;
    }

    enum Operation {Equals = 0, Remove, Insert, Replace};

    mutable std::set<const Node*> m_selectedWords;

    void pushIfNotSelect(WordList& list) const
    {
        if(m_selectedWords.count(this))
            return;
        m_selectedWords.insert(this);
        list.push_back(getWord());
    }

    WordList correctTheWord(const std::string& word, int maxEditCount) const
    {
        WordList result;
        m_selectedWords.clear();
        for(const auto& child : m_childs) {
            Node* node = child.second;
            pushListToList(result, node->correctTheWord(word.cbegin(), word.cend(), maxEditCount));
        }
        return result;
    }

    WordList correctTheWord(strciter begin, strciter end, int editCount, Operation prevOperation = Equals) const
    {
        WordList result;

        if(editCount < 0)
            return result;

        if(m_wordEnd) {
            if(begin == end && prevOperation == Remove) {
//                pushIfNotSelect(result);
            } else if(begin + 1 == end) {
                if(m_letter == *begin)
                    pushIfNotSelect(result);
            }
        }

        if(prevOperation != Insert) {
            for(const auto& child : m_childs) {
                pushListToList(result, child.second->correctTheWord(begin, end, editCount - 1, Insert));
            }
        }

        if(begin != end) {
            for(const auto& child : m_childs) {
                Node* node = child.second;
                if(*begin == m_letter)
                    pushListToList(result, node->correctTheWord(begin + 1, end, editCount, Equals));
            }

            if(prevOperation != Remove)
                pushListToList(result, this->correctTheWord(begin + 1, end, editCount - 1, Remove));
        }

        return result;
    }

private:
    bool m_wordEnd;
    char m_letter;
    Node* m_parent;
    std::map<char, Node*> m_childs;
};

class WordBookPrivate
{

    friend class WordBook;    

    inline explicit WordBookPrivate(int maxEditCount)
        : m_maxEditCount(maxEditCount)
        , m_size(0)
        , m_words(new Node())
    {}

    ~WordBookPrivate();

    void insert(const std::string& word);
    inline bool hasWord(const std::string& word) const { return m_words->hasWord(word.cbegin(), word.cend()); }
    WordList words() const;

    int m_size;
    inline int size() const { return m_size; }

    inline WordList correctTheWord(const std::string &word) const { return m_words->correctTheWord(word, m_maxEditCount); }

    const int m_maxEditCount;
    Node* m_words;
};

#endif
