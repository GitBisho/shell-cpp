#pragma once
#include <iostream>
#include <assert.h>
#include <optional>
#include <array>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using pipeline_list = std::vector<std::string>;
using token_list = std::vector<pipeline_list>; 

struct token_state {
    unsigned char in_double_quotes : 1 {false}; 
    unsigned char in_single_quotes : 1 {false};
    unsigned char in_slash         : 1 {false};
    unsigned char in_comment       : 1 {false};
    unsigned char in_pipe          : 1 {false};
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

class op_stat {
public:
    e_operator_arr op{};
    std::string target{};
    int fd{};
    bool fd_target{};
    op_stat() = delete;
    op_stat(e_operator_arr new_op, std::string_view file, int file_desc = -1) : //Error prone
        op { new_op }, target { file.data() }
    {
        if(file_desc == -1) {
            switch(op) {
                case L_2_REDIR:
                case L_REDIR:
                case WEIRD:
                case L_FD_REDIR:
                    fd = 0;
                    break;
                case APPEND:
                case R_FD_REDIR:
                case R_REDIR:
                    fd = 1;
                    break;
                default:
                    fd = -1;
                    break;
            }
        }
        else {
            fd = file_desc;
        }

        try {
            std::stoi(target);
            fd_target = true;
        }
        catch(std::invalid_argument const& e) {
            assert("Failed to create op_stat object");
        }
    } 
};

inline std::ostream& operator<<(std::ostream& out, op_stat op) {
    out << "redir fd=" << op.fd << " op=" << operator_arr[op.op] << " target=" << op.target << '\n';
    return out;
}

std::optional<token_list> tokenize_input(std::string_view line);
void print_operators(e_operator_arr idx);
