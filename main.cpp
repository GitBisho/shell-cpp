#include <string>
#include <iostream>
#include <bit>
#include <vector>

struct token_state {
    unsigned char in_double_quotes : 1 {false}; //Might cause errors later with line grouping with single quotes
    unsigned char in_single_quotes : 1 {false};
    unsigned char in_slash         : 1 {false};
};

struct token {
    std::string symbol{};
};

std::ostream& operator<<(std::ostream& out, std::vector<token> tok) {
    for(token i : tok) {
        std::cout << i.symbol << '\n';
    }
    return out;
}

bool has_flag(token_state &state) {
    auto i = std::bit_cast<char>(state);
    if(i == true) {
        return true;
    }
    return false;
}

int parse_input(std::string_view line) {
    token_state state{};
    int token_count{};
    size_t pos{0};
    std::vector<token> symbol_list{};

    for(size_t i{1}; i < line.length(); ++i) {
        switch(line[i]) {
            case '"':
                state.in_double_quotes = true;
                break;
            case '\'': //State priority mess maybe
                if(state.in_single_quotes == true) {
                    symbol_list.emplace_back( token{ .symbol{line.substr(pos, i)} } );
                    state.in_single_quotes = false;
                    break;
                }
                state.in_single_quotes = true;
                pos = i;
                break;
            case '\\':
                state.in_slash = true;
                break;
            case ' ':
                if(!has_flag(state) && line[i-1] != ' ') { //Possible out of range array index at i=0
                    token_count++;
                    if(pos < i) { symbol_list.emplace_back(
                                    token{ .symbol{ line.substr(pos + 1, i - pos - 1) }}
                                    ); 
                        pos = i; 
                    }
                }
        }
    }        
    std::cout << "symbol_list: \n";
    std::cout << symbol_list;
    return token_count;
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        line.append(" ");
        line.insert(0, " ");
        std::cout << parse_input(line) << '\n';
        if (line.empty()) continue;
    }
}
