#ifndef WORKBOOK_PRIVATE_H
#define WORKBOOK_PRIVATE_H

#include <list>
#include <string>
#include <map>
#include <set>
#include <memory>

class WordBook;

typedef std::list<std::string> WordList;
typedef  std::string::const_iterator strciter;

class Node
{
public:
    Node() : Node('\0') {}

    explicit Node(char letter, const Node* parent = nullptr)
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

    void pushIfNotSelect(WordList& list,  std::set<const Node*>& selectedWords) const
    {
        if(selectedWords.count(this))
            return;
        selectedWords.insert(this);
        list.push_back(getWord());
    }

    WordList correctTheWord(const std::string& word, int maxEditCount) const
    {
        WordList result;

        for(const auto& child : m_childs) {
            const Node* node = child.second;
            pushListToList(result, node->correctTheWord(word.cbegin(), word.cend(), maxEditCount));
        }
        return result;
    }

    WordList correctForChilds(strciter begin, strciter end, int editCount, Operation prevOperation, std::set<const Node*>& selectedWords) const
    {
        WordList result;
        if(m_childs.size()) {
            for(const auto& child : m_childs) {
                const Node* node = child.second;
                pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, selectedWords));
            }
        }
        return result;
    }

    WordList correctForChildsAndEndNode(strciter begin, strciter end, int editCount, Operation prevOperation, std::set<const Node*>& selectedWords) const
    {
        WordList result;

        if(m_childs.size()) {
            for(const auto& child : m_childs) {
                const Node* node = child.second;
                pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, selectedWords));
            }
        } else {
            Node* node = new Node('\0', this);
            pushListToList(result, node->correctTheWord(begin, end, editCount, prevOperation, selectedWords));
            delete node;
        }

        return result;
    }

    WordList correctInsert(strciter begin, strciter end, int editCount, Operation prevOperation, std::set<const Node*>& selectedWords) const
    {
        if(prevOperation == Insert)
            return WordList();
        return correctForChildsAndEndNode(begin, end, editCount - 1, Insert, selectedWords);
    }

    WordList correctRemove(strciter begin, strciter end, int editCount, Operation prevOperation, std::set<const Node*>& selectedWords) const
    {
        if(prevOperation == Remove)
            return WordList();
        return this->correctTheWord(begin + 1, end, editCount - 1, Remove, selectedWords);
    }

    WordList correctEquals(strciter begin, strciter end, int editCount, Operation prevOperation, std::set<const Node*>& selectedWords) const
    {
        return correctForChildsAndEndNode(begin + 1, end, editCount, Equals, selectedWords);
    }


#define CORRECT_PARAMS begin ,end, editCount, prevOperation, selectedWords

    WordList correctTheWord(strciter begin, strciter end, int editCount, Operation prevOperation = Equals, std::set<const Node*>& selectedWords = std::set<const Node*>()) const
    {
        if(editCount < 0)
            return WordList();

        WordList result;

        if(begin == end && isEndNode()) {
            m_parent->pushIfNotSelect(result, selectedWords);
            return result;
        }

        if(begin == end && m_parent->m_wordEnd) {
            m_parent->pushIfNotSelect(result, selectedWords);
        }


        if(!(begin == end || isEndNode())) {
            if(*begin == m_letter)
                pushListToList(result, correctEquals(CORRECT_PARAMS));
            pushListToList(result, correctInsert(CORRECT_PARAMS));
            pushListToList(result, correctRemove(CORRECT_PARAMS));

        } else if(isEndNode()) {
            pushListToList(result, correctRemove(CORRECT_PARAMS));
        } else {
            pushListToList(result, correctInsert(CORRECT_PARAMS));
        }

        return result;
    }

    bool isEndNode() const { return m_parent && m_letter == '\0'; }


private:
    bool m_wordEnd;
    char m_letter;
    const Node* m_parent;
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
