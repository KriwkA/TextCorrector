#ifndef WORKBOOK_PRIVATE_H
#define WORKBOOK_PRIVATE_H

#include <list>
#include <string>
#include <map>
#include <set>

class WordBook;
class WordBookPrivate;

typedef std::list<std::string> WordList;
typedef  std::string::const_iterator strciter;

class Node
{
enum Operation {Equals = 0, Remove = 1, Insert = 2, Replace = 4};
#define CORRECT_PARAMS begin ,end, editCount, prevOperation, dp
#define CORRECT_PUSH(correct_func) pushListToList(result, correct_func(CORRECT_PARAMS))
#define CORRECT_EQUALS CORRECT_PUSH(correctEquals)
#define CORRECT_REMOVE CORRECT_PUSH(correctRemove)
#define CORRECT_INSERT CORRECT_PUSH(correctInsert)

struct DP {
    typedef std::pair<strciter, Operation> Pair;

    std::map< int, std::set<Pair>> dp;
    bool selected;

    DP() : selected(false) {}

    bool check(strciter begin, int editCount, Operation oper) {
        return dp.count(editCount) && dp.at(editCount).count(std::make_pair(begin, oper));
    }

    void set(strciter begin, int editCount, Operation oper) {
        dp[editCount].insert(std::make_pair(begin, oper));
    }
};

friend class WordBookPrivate;

    Node();
    explicit Node(char letter, const Node* parent = nullptr);
    ~Node();

    bool insert(strciter begin, strciter end);
    bool hasWord(strciter begin, strciter end) const;
    WordList words() const;

    WordList correctTheWord(const std::string& word, int maxEditCount) const;
    WordList correctTheWord(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;
    WordList correctInsert(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;
    WordList correctRemove(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;
    WordList correctEquals(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;
    WordList correctForChilds(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;
    WordList correctForChildsAndEndNode(strciter begin, strciter end, int editCount, Operation prevOperation, std::map<const Node*, DP>& dp) const;

    std::string getWord() const;
    void pushIfNotSelect(WordList& list,  std::map<const Node*, DP>& dp) const;
    static void pushListToList(WordList& target, const WordList& data);
    inline bool isEndNode() const { return m_parent && m_letter == '\0'; }


    bool m_wordEnd;
    char m_letter;
    const Node* m_parent;
    std::map<char, Node*> m_childs;


};

class WordBookPrivate
{
    friend class WordBook;    
    explicit WordBookPrivate(int maxEditCount);

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
