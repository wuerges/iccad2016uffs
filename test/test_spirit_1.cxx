#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace client
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    ///////////////////////////////////////////////////////////////////////////
    //  Our number list parser
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    int parse_number(Iterator first, Iterator last)
    {
        using x3::int_;
        using x3::phrase_parse;
        using ascii::space;
        using x3::parse;

        int value;
        bool r = phrase_parse(
            first,                          // Start Iterator
            last,                           // End Iterator
            int_,
            space,                          // The Skip-Parser
            value
        );
        return value;
    }
}

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::string str = "12345";
    if (client::parse_number(str.begin(), str.end()) == 12345)
    {
      return 0;
    }
    else 
    {
      return -1;
    }
}
