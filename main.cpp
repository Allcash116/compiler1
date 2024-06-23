#include "src/lexer.h"
#include "src/parcer.h"
#include "src/generate_code.h"

int main() {

    string path;
    cout << "Введите путь до файла\n";
    cin >> path;


    ifstream infile(path);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file Test.testik" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << infile.rdbuf();
    string code = buffer.str();
    infile.close();

    vector<string> lines;
    string line;


    set<string> init_vars;
    vector<string> data = {"TITLE ;KOvrigin vladislav m3o-232b-22\nINCLUDE Irvine32.inc\n\n.data\n"};
    vector<string> main = {".code\nmain PROC\n"};
    vector<string> end = {"exit\nmain ENDP\n\nEND main\n"};
    vector<string> pool;

    int i = 0;
    int loop_count = 0;
    while(i < code.length()) {
        if (code[i] != '\n') {
            line = line + code[i];
            i++;
        }
        else {
            cout << "Получена линия!";
            lines.push_back(line);
            line = "";
            i++;
        }
    }
    i = 0;
    while (i < lines.size()) {
        vector<string> tokens = lexer(lines[i]);

        if (tokens.empty()) {
            i++;
            continue;
        }

        if (tokens[0] == ".") {
            main.push_back(pool.back());
            pool.pop_back();
            i++;
            continue;
        }

        if (tokens[0] == "while") {
            tokens.erase(tokens.begin());
            main.push_back("LOOP" + to_string(loop_count) + ":\n");
            calculate(&tokens, &main);
            main.push_back("pop eax\ncmp eax, 0\njne START" + to_string(lab_count) + "\njmp END" + to_string(lab_count) + "\n");
            main.push_back("START" + to_string(lab_count) + ":\n");
            pool.push_back("jmp LOOP" + to_string(loop_count) + "\nEND" + to_string(lab_count) + ":\n");
            loop_count++;
            lab_count++;
        } else if (tokens[0] == "if") {
            bool check = false;
            int level = 0;
            for (int num = i++; num < lines.size(); num++) {
                if (lines[num] == "else") {
                    if (level == 0) {
                        tokens.erase(tokens.begin());
                        calculate(&tokens, &main);
                        main.push_back("pop eax\ncmp eax, 0\njne START" + to_string(lab_count) + "\njmp else" + to_string(lab_count) + "\n");
                        main.push_back("START" + to_string(lab_count) + ":\n");
                        pool.push_back("End" + to_string(lab_count) + ":\n");
                        lab_count++;
                        check = true;
                    }
                    level--;
                }
                else if (lines[num] == "if") {
                    level++;
                }
            }
            if (check == false) {
                tokens.erase(tokens.begin());
                calculate(&tokens, &main);
                main.push_back("pop eax\ncmp eax, 0\njne START" + to_string(lab_count) + "\njmp END" + to_string(lab_count) + "\n");
                main.push_back("START" + to_string(lab_count) + ":\n");
                pool.push_back("END" + to_string(lab_count) + ":\n");
                lab_count++;
            }
            i--;
        }else if(tokens[0] == "else") {
            main.push_back("pop eax\ncmp eax, 0\n jmp end" + to_string(lab_count) + "\n");
            main.push_back("else" + to_string(lab_count - 2) + ":\n");
            pool.push_back("End" + to_string(lab_count) + ":\n");
            lab_count++;
        }
        else if (tokens[0] == "print") {
            tokens.erase(tokens.begin());
            calculate(&tokens, &main);
            main.push_back("pop eax\ncall WriteInt\ncall Crlf\n");
        }
        else if (tokens[0] == "prints") {
            tokens.erase(tokens.begin());
            std::string result = std::accumulate(tokens.begin(), tokens.end(), std::string());

            for (int j = tokens.size() - 1; j >= 0; j--)
            {
                string word = tokens[j];
                main.push_back("push ' '\n");
                for (int p = word.length() - 1; p >= 0; p--)
                {
                    main.push_back("push " + to_string(word[p]) + "\n");
                }
            }
            main.push_back("mov ecx, " + to_string(result.length() + tokens.size()) + "\nprint_Loop" + to_string(lab_count) + ":\npop eax\ncall WriteChar\nloop print_Loop" + to_string(lab_count) + "\n");
            main.push_back("call crlf\n");
            lab_count++;
        }
        else {
            string name = tokens[0];
            tokens.erase(tokens.begin(), tokens.begin() + 2);

            if (init_vars.find(name) == init_vars.end()) {
                init_vars.insert(name);
                data.push_back(name + " dd ?\n");
            }

            calculate(&tokens, &main);
            main.push_back("pop eax\nmov " + name + ", eax\n");
        }

        i++;
        main.push_back("\n");
    }


    ofstream outfile("compiler.asm");
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file compiler.asm" << endl;
        return 1;
    }
    for (const auto& line : data) {
        outfile << line;
    }
    for (const auto& line : main) {
        outfile << line;
    }
    for (const auto& line : end) {
        outfile << line;
    }
    outfile.close();

    return 0;
}
