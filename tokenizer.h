#pragma once

#include <array>
#include <string>
#include <optional>
#include <vector>

using token_list = std::optional<std::vector<std::string>>;

struct token_state {
    unsigned char in_double_quotes : 1 {false}; 
    unsigned char in_single_quotes : 1 {false};
    unsigned char in_slash         : 1 {false};
};

constexpr static std::array<std::string, 12> operator_arr { "&&", "||", ">>", "<<", ">&", 
                                                            "<&", "|", "&", ";", "<", ">", "<<-"};
enum e_operator_arr : size_t {
    LOG_AND,
    LOG_OR,
    APPEND,
    L_2_REDIR,
    R_FD_REDIR,
    L_FD_REDIR,
    PIPE,
    AMPERSAND,
    SEMICOLON,
    L_REDIR,
    R_REDIR,
    WEIRD,
};


token_list tokenize_input(std::string_view line);
void print_operators(e_operator_arr idx);
