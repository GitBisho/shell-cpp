#include <string>
#include <iostream>
#include <vector>

struct token_state {
    unsigned char in_double_quotes : 1 {false}; //Might cause errors later with line grouping with single quotes
    unsigned char in_single_quotes : 1 {false};
    unsigned char in_slash         : 1 {false};
};

int parse_input(std::string_view line) {
    token_state state{};
    std::vector<char> temp_token{};
    std::vector<std::vector<char>> list{};

    for(size_t i{1}; i < line.length(); ++i) {
        if(state.in_double_quotes) {
            if(line[i] == '\\') {
                temp_token.push_back(line[i + 1]);
                i++; 
                continue; 
            }
            if(line[i] == '"') {
                state.in_double_quotes = false;
                temp_token.push_back(']');
                list.push_back(temp_token);
                temp_token.erase(temp_token.begin(), temp_token.end());
                continue;
            }
            temp_token.push_back(line[i]);
            continue;
        }
        if(state.in_single_quotes) {
            if(line[i] == '\'') {
                state.in_single_quotes = false;
                temp_token.push_back(']');
                list.push_back(temp_token);
                temp_token.erase(temp_token.begin(), temp_token.end());
                continue;
            }
            temp_token.push_back(line[i]);
            continue;
        }
        if(state.in_slash) {
            temp_token.push_back(line[i]);
            state.in_slash = false;
            continue;
        } 
        switch(line[i]) {
            case '"':
                temp_token.push_back('[');
                state.in_double_quotes = true;
                break;
            case '\'':
                temp_token.push_back('['); 
                state.in_single_quotes = true;
                break;
            case '\\': 
                state.in_slash = true;
                break;
            case ' ':
                if(temp_token.empty()) break;
                temp_token.push_back(']');
                temp_token.insert(temp_token.begin(), '[');

                list.push_back(temp_token);
                temp_token.erase(temp_token.begin(), temp_token.end());
                break;
            default:
                temp_token.push_back(line[i]);
                break;
        }
    }

    //Leftover state
    if(state.in_double_quotes || state.in_single_quotes) {
        std::cout << "ERR unterminated quote";
        return 1;
    }
    std::string ending{};
    for(auto i : list) {
        std::string result(i.begin(), i.end());
        result.append(" ");
        ending.append(result);
    }
    if(ending.back() == ' ') {
        ending.pop_back();
    }
    std::cout << ending;
    return list.size();
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        line.append(" ");
        line.insert(0, " ");
        parse_input(line); 
        if (line.empty()) continue;
    }
}
