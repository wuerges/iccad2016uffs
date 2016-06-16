#pragma once

#include <verilog_model.hpp>

namespace verilog {

namespace parser
{
    template <typename Iterator>
    int parse_verilog(verilog::ast::Verilog &v, 
        Iterator first, Iterator last);
    int parse_verilog_file(verilog::ast::Verilog &v, char* filename);
}
}
