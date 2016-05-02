#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <boost/variant.hpp>


namespace parser 
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    template <typename Iterator>(Iterator first, Iterator last)
    {
      using x3::lit;

      auto module_lit    = lit("module");
      auto endmodule_lit = lit("endmodule");
      auto input_lit     = lit("input");
      auto output_lit    = lit("output");
      auto wire_lit      = lit("wire");


      auto module = 
        module_lit > 
        identifier > 
        '(' > identifier_list > ')';
    }

}
