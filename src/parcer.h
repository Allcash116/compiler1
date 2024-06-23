#pragma once
#include "lexer.h"
// Node class for the expression tree
class Node {
public:
    string value;
    int level;
    string nodetype;
    int parent = -1;
    int left = -1;
    int right = -1;

    Node(const string token, int levell) : value(token), level(levell) {
        nodetype = findType(token);
    }

    string findType(const string& token) {
        static set<string> operators = {"+", "-", "*", "/", ">", "<", "%", "|", "&", ">=", "<=", "==", "!="};
        if (operators.find(token) != operators.end()) {
            return "Operator";
        }
        else {
            return "Value";
        }
    }
};

// Tree class to represent the expression
class Tree {
public:
    vector<Node> tree;
    int root = -1;

    void pull(const vector<string>* tokens) {
        int len;
        int prev_node;
        int new_node;

        int level_count = 0;
        for (const auto& token : *tokens) {
            if (token == "(") {
                level_count++;
                continue;
            }
            if (token == ")") {
                level_count--;
                continue;
            }
            Node node(token, level_count);

            tree.push_back(node);

            len = tree.size();
            new_node = len - 1;

            if (len == 1) {
                root = new_node;
                continue;
            }
            prev_node = len - 2;

            if (len == 2) {
                root = new_node;
                tree[new_node].right = prev_node;
                tree[prev_node].parent = new_node;
                continue;
            }
            if (tree[prev_node].nodetype == "Operator") {
                tree[prev_node].left = tree[prev_node].right;
                tree[prev_node].right = new_node;
                tree[new_node].parent = prev_node;
            } else {
                while (tree[prev_node].level > tree[new_node].level) {
                    if (tree[prev_node].parent != -1) {
                        prev_node = tree[prev_node].parent;
                    } else {
                        break;
                    }
                }
                if (tree[prev_node].nodetype != "Operator") {
                    prev_node = tree[prev_node].parent;
                }
                if (tree[prev_node].level > tree[new_node].level || (tree[prev_node].level == tree[new_node].level && !(!(tree[prev_node].value == "*" || tree[prev_node].value == "/" || tree[prev_node].value == "%" || tree[prev_node].value == "&") && (tree[new_node].value == "*" || tree[new_node].value == "/" || tree[new_node].value == "%" || tree[new_node].value == "&")))) {
                    if (tree[prev_node].parent != -1) {
                        int parent = tree[prev_node].parent;
                        tree[parent].right = new_node;
                        tree[new_node].parent = parent;
                        tree[prev_node].parent = new_node;
                        tree[new_node].right = prev_node;

                    } else {
                        root = new_node;
                        tree[new_node].right = prev_node;
                        tree[prev_node].parent = new_node;
                    }
                } else {
                    int right = tree[prev_node].right;
                    tree[right].parent = new_node;
                    tree[new_node].right = tree[prev_node].right;
                    tree[prev_node].right = new_node;
                    tree[new_node].parent = prev_node;
                }
            }
        }
    }
};

int lab_count = 0;
