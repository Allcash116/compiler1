#include "lexer.h"

void push_node(const Tree* tree, int new_node, vector<string>* main) {
    if (tree->tree[new_node].nodetype != "Operator") {
        main->push_back("push " + tree->tree[new_node].value + "\n");
    }
    else {
        push_node(tree, tree->tree[new_node].right, main);
        push_node(tree, tree->tree[new_node].left, main);

        string operation;
        if (tree->tree[new_node].value == "+") {
            operation = "add";
        } else if (tree->tree[new_node].value == "-") {
            operation = "sub";
        } else if (tree->tree[new_node].value == "*") {
            operation = "imul";
        } else if (tree->tree[new_node].value == "|") {
            operation = "or";
        } else if (tree->tree[new_node].value == "&") {
            operation = "and";
        } else if (tree->tree[new_node].value == ">" || tree->tree[new_node].value == "<" || tree->tree[new_node].value == ">=" || tree->tree[new_node].value == "<=" || tree->tree[new_node].value == "==" || tree->tree[new_node].value == "!=") {
            operation = "cmp";
        } else if (tree->tree[new_node].value == "%" || tree->tree[new_node].value == "/") {
            operation = "idiv";
        }

        main->push_back("pop eax\n");

        if (operation == "idiv") {
            main->push_back("cdq\n");
        }

        main->push_back("pop ebx\n");
        main->push_back(operation + " ");

        if (operation == "idiv") {
            main->push_back("ebx\n");
            if (tree->tree[new_node].value == "%") {
                main->push_back("push edx\n");
            } else {
                main->push_back("push eax\n");
            }
        } else {
            main->push_back("eax, ebx\n");
            if (operation == "cmp") {
                string jump;
                if (tree->tree[new_node].value == ">") {
                    jump = "ja";
                } else if (tree->tree[new_node].value == "<") {
                    jump = "jl";
                } else if (tree->tree[new_node].value == "==") {
                    jump = "je";
                } else if (tree->tree[new_node].value == ">=") {
                    jump = "jge";
                } else if (tree->tree[new_node].value == "<=") {
                    jump = "jle";
                } else if (tree->tree[new_node].value == "!=") {
                    jump = "jne";
                } else {
                    jump = "jmp";
                }
                main->push_back(jump + " pos" + to_string(lab_count) + "\n");
                main->push_back("push 0\n");
                main->push_back("jmp neg" + to_string(lab_count) + "\n");
                main->push_back("pos" + to_string(lab_count) + ":\n");
                main->push_back("push 1\n");
                main->push_back("neg" + to_string(lab_count) + ":\n");
                lab_count++;
            } else {
                main->push_back("push eax\n");
            }
        }
    }
}

void tree_to_asm(const Tree* tree, vector<string>* main) {
    push_node(tree, tree->root, main);
}

void calculate(const vector<string>* tokens, vector<string>* main) {
    Tree tree;
    tree.pull(tokens);
    tree_to_asm(&tree, main);
}



