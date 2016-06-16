#include <verilog_model.hpp>

#include <boost/spirit/include/classic.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace verilog {

namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    template <typename Iterator>
    int parse_verilog(verilog::ast::Verilog &v, 
        Iterator first, Iterator last)
    {
      using x3::lit;
      using x3::char_;
      using x3::_attr;
      using x3::string;
      using x3::phrase_parse;
      using ascii::space;

      auto module_lit    = lit("module");
      auto endmodule_lit = lit("endmodule");
      auto input_lit     = lit("input");
      auto output_lit    = lit("output");
      auto wire_lit      = lit("wire");

      typedef std::vector<std::string> string_vector; 

	  // auto print_debug = [&](auto &c){ std::cout << "DEBUG: " << _attr(c) << std::endl; };
      
      auto identifier 
        = x3::rule<class identifier, std::string>{}
        = (char_("a-zA-Z_") > *char_("a-zA-Z0-9_")) | string("1'b0") | string("1'b1");
      auto identifier_list = identifier % ',';

      auto add_inputs = [&](auto &c){ v.add_inputs(_attr(c)); };
      auto input_decl
        = input_lit > identifier_list[add_inputs] > ';';

      auto add_outputs = [&](auto &c){ v.add_outputs(_attr(c)); };
      auto output_decl 
        = output_lit > identifier_list[add_outputs] > ';';

      auto add_wires = [&](auto &c){ v.add_wires(_attr(c)); };
      auto wire_decl 
        = wire_lit > identifier_list[add_wires] > ';';

      std::string op;
      
      auto set_function_op = [&](auto &c){ op = _attr(c); };
      auto function_name 
        = x3::rule<class function_name, std::string>{}
        = string("and")   
        | string("nand") 
        | string("or")   
        | string("nor")  
        | string("xor")  
        | string("xnor") 
        | string("not")  
        | string("buf");
      auto function_name_op = function_name[set_function_op];

      auto add_function = [&](auto &c){ v.add_function(op, _attr(c)); };
      auto function_decl 
        = function_name_op > 
        '(' > identifier_list[add_function] > ')' > ';';

      auto module_body 
        = +(input_decl 
          | output_decl 
          | wire_decl 
          | function_decl
          );

      
      auto add_ports = [&](auto &c){ v.add_ports(_attr(c)); };
      
      auto module 
        = module_lit 
          > identifier 
	>> -( '(' >> -identifier_list[add_ports] >> ')' ) > ';'
          >> module_body 
          >> endmodule_lit;
	
	  auto cpp_style_comment = "//" > *(~char_('\n')) > char_('\n');
	  auto rest_of_cpp_comment = *(~char_('\n')) > char_('\n');
	  auto rest_of_c_comment = *(~char_('*') > ~char_('/')) > "*/";
	  
	  auto comment = +space | (char_('/') >> ((char_('/') >> rest_of_cpp_comment) | (char_('*') > rest_of_c_comment)));

      phrase_parse(first, last,
          module >> x3::eoi, comment);

      if (first != last) {
        std::cout << "ERROR: PARSE FAILED" << std::endl;
        return -1;
      }

      return 0;
    }

    int parse_verilog_file(verilog::ast::Verilog &v, char* filename) {
      std::ifstream input(filename);
      input.unsetf(std::ios::skipws);
      boost::spirit::istream_iterator begin(input);
      boost::spirit::istream_iterator end;
      return parse_verilog(v, begin, end);
    }



}

}
