#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip> 

using namespace std;

// Grammar rule and Parse Tree node 
struct Rule {
    string lhs;               
    vector<string> rhs;       
};

struct TreeNode {
    string name;
    vector<TreeNode*> children;
    TreeNode(string n) : name(n) {}
};

// Global maps for parsing tables
map<int, Rule> grammar;                                
map<int, map<string, string>> actionTable;             
map<int, map<string, string>> gotoTable;               

// Load data from text files
void loadGrammar(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int ruleNo;
        string lhs, arrow, token;
        Rule rule;
        ss >> ruleNo >> lhs >> arrow; 
        rule.lhs = lhs;
        while (ss >> token) rule.rhs.push_back(token);
        grammar[ruleNo] = rule;
    }
    file.close();
}

void loadActionTable(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    string line;
    getline(file, line);
    stringstream headerStream(line);
    string headerToken;
    vector<string> headers;
    headerStream >> headerToken; 
    while (headerStream >> headerToken) headers.push_back(headerToken); 

    while (getline(file, line)) {
        stringstream ss(line);
        int state;
        string action;
        ss >> state; 
        for (const string& h : headers) {
            ss >> action;
            actionTable[state][h] = action; 
        }
    }
    file.close();
}

void loadGotoTable(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    string line;
    getline(file, line);
    stringstream headerStream(line);
    string headerToken;
    vector<string> headers;
    headerStream >> headerToken; 
    while (headerStream >> headerToken) headers.push_back(headerToken); 

    while (getline(file, line)) {
        stringstream ss(line);
        int state;
        string gotoState;
        ss >> state; 
        for (const string& h : headers) {
            ss >> gotoState;
            gotoTable[state][h] = gotoState; 
        }
    }
    file.close();
}

// Recursive function to print the tree paths
void printTree(TreeNode* node, string currentPath, ofstream& out) {
    if (!node) return;
    string path = currentPath + "/" + node->name;
    out << path << endl; 
    for (TreeNode* child : node->children) {
        printTree(child, path, out);
    }
}

// Core LR Parsing Logic
void parseInput(const string& inputFile, const string& outputFile) {
    ifstream file(inputFile);
    if (!file.is_open()) return;

    vector<string> tokens;
    string token;
    while (file >> token) tokens.push_back(token);
    file.close();

    ofstream out(outputFile);
    out << left << setw(41) << "Stack" << setw(41) << "Input" << "Action" << endl;
    out << "--------------------------------------------------------------------------------------------------------\n";

    vector<string> stack;
    stack.push_back("0"); 
    vector<TreeNode*> treeStack; 

    int tokenIndex = 0;
    bool isError = false;

    while (tokenIndex < tokens.size()) {
        int currentState = stoi(stack.back());
        string currentToken = tokens[tokenIndex];
        
        // unknown token error'u terminalde uyarı verip parsing processi durduruyor
        if (actionTable[currentState].find(currentToken) == actionTable[currentState].end()) {
            cout << "ERROR: Unknown token '" << currentToken << "' detected in " << inputFile << ". Parsing stopped." << endl;
            isError = true; 
            break; 
        }

        string action = actionTable[currentState][currentToken];

        // tokenı alıp tableda ona göre action yazıyoruz
        string stackStr = "";
        for (string s : stack) stackStr += s;

        string inputStr = "";
        for (int i = tokenIndex; i < tokens.size(); i++) {
            inputStr += tokens[i] + (i == tokens.size()-1 ? "" : " ");
        }

        string displayAction = "";

        // syntax error terminalde yazıp processi durdurma
        if (action == "" || action == "-") { 
            displayAction = "SYNTAX ERROR";
            out << left << setw(41) << stackStr << setw(41) << inputStr << displayAction << endl;
            cout << "ERROR: Syntax error at token '" << currentToken << "' in " << inputFile << ". Parsing stopped." << endl;
            isError = true; 
            break; 
        }
        else if (action[0] == 's') { 
            string nextState = action.substr(1);
            displayAction = "Shift " + nextState;
            
            treeStack.push_back(new TreeNode(currentToken));
            stack.push_back(currentToken);
            stack.push_back(nextState);
            tokenIndex++;
        }
        else if (action[0] == 'r') { 
            int ruleNo = stoi(action.substr(1));
            Rule rule = grammar[ruleNo];
            int rhsSize = rule.rhs.size();

            vector<TreeNode*> children;
            for (int i = 0; i < rhsSize; i++) {
                children.push_back(treeStack.back());
                treeStack.pop_back();
            }
            
            TreeNode* parentNode = new TreeNode(rule.lhs);
            for (int i = rhsSize - 1; i >= 0; i--) {
                parentNode->children.push_back(children[i]);
            }
            treeStack.push_back(parentNode);

            for (int i = 0; i < rhsSize * 2; i++) stack.pop_back();

            int topState = stoi(stack.back());
            string gotoState = gotoTable[topState][rule.lhs];

            displayAction = "Reduce " + to_string(ruleNo) + " (GOTO [" + to_string(topState) + ", " + rule.lhs + "])";

            stack.push_back(rule.lhs);
            stack.push_back(gotoState);
        }
        else if (action == "accept") { 
            displayAction = "Accept";
            out << left << setw(41) << stackStr << setw(41) << inputStr << displayAction << endl;
            break; 
        }

        out << left << setw(41) << stackStr << setw(41) << inputStr << displayAction << endl;
    }

    out << "--------------------------------------------------------------------------------------------------------\n";
    out << "Parse tree:\n";
    
    // parsing tree
    if (isError) {
        if (!treeStack.empty()) {
            printTree(treeStack.back(), "", out);
        }
    } else {
        for (TreeNode* node : treeStack) printTree(node, "", out);
    }
    
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: ./LR-parser <input.txt> <output.txt>" << endl;
        return 1;
    }

    loadGrammar("Grammar.txt");
    loadActionTable("ActionTable.txt");
    loadGotoTable("GotoTable.txt");

    parseInput(argv[1], argv[2]);
    return 0;
}