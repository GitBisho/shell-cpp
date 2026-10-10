#include <iostream>
#include <vector>
#include <optional>

#include "tokenizer.h"


//Aided with Claude
//:Find best case match
int check_operator(std::string_view line, size_t idx, size_t& len_out, bool& is_redirect) {
    len_out = 0;
    int best_idx{-1};

    for(size_t j{}; j < operator_arr.size(); j++) {
        std::string_view elem = operator_arr[j];

        if(elem.length() > len_out && line.compare(idx, elem.length(), elem) == 0) {
            best_idx = j;
            len_out = elem.length(); 
        } 
    }    
    if(best_idx >= 0 && (operator_arr[best_idx] == "<" || operator_arr[best_idx] == ">")) {
        is_redirect = true;
    }


    return best_idx;
}

void print_token_list(std::vector<std::string>& list) {
    for(std::string token : list) {
        std::cout << token << ' ';
    }    
    std::cout << '\n';
}

token_list tokenize_input(std::string_view line) {
    token_state state{};
    std::string temp_token{};
    std::vector<std::string> list{};
    
    auto end_token = [&temp_token, &list]() {
        if(temp_token.empty()) { return; }
        temp_token.insert(temp_token.begin(), '[');
        temp_token.push_back(']');
        list.push_back(temp_token);
        temp_token.erase(temp_token.begin(), temp_token.end());
    };

    for(size_t i{0}; i < line.length(); ++i) {
        if(state.in_double_quotes) {
            if(line[i] == '\\') {
                if(i + 1 >= line.length()) {
                    std::cout << "ERR trailing backslash";
                    return std::nullopt;
                }
                temp_token.push_back(line[i + 1]);
                i++; 
                continue; 
            }
            if(line[i] == '"') {
                state.in_double_quotes = false;
                end_token();
                continue;
            }
            temp_token.push_back(line[i]);
            continue;
        }
        if(state.in_single_quotes) {
            if(line[i] == '\'') {
                state.in_single_quotes = false;
                end_token();
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
        if(state.in_comment) {
            if(line[i] == '\n') { 
                state.in_comment = false;
            }
            continue; 
        }

        switch(line[i]) {
            case '"':
                state.in_double_quotes = true;
                break;
            case '\'':
                state.in_single_quotes = true;
                break;
            case '\\': 
                state.in_slash = true;
                break;
            case '#':
                state.in_comment = true;
                break;
            case ' ':
                end_token();
                break; 
            default:
                //Out var for checking best case len
                size_t len_out{};
                bool is_redirect{false};
                int op_idx = check_operator(line, i, len_out, is_redirect); 
                
                if(op_idx != -1) {
                    if(is_redirect) {
                        temp_token.insert(temp_token.begin(), '{');
                        temp_token.push_back('}');

                        list.push_back(temp_token);
                        temp_token.erase(temp_token.begin(), temp_token.end());
                    }
                    else { end_token(); }
                    std::string_view operator_view = operator_arr[op_idx];
                    for(size_t op_length{}; op_length < len_out; op_length++) {
                        temp_token.push_back(operator_view[op_length]);
                    }
                    end_token();
                    i += len_out - 1;
                    continue;
                }


                temp_token.push_back(line[i]);
                break;
        }
    }
    end_token();


    //Leftover state
    if(state.in_double_quotes || state.in_single_quotes) {
        std::cout << "ERR unterminated quote";
        return std::nullopt;
    }
    
    print_token_list(list);

    return list; //NVRO 
}
