#include <boost/spirit/include/qi.hpp>

#include "test_helpers.hpp"

namespace qi = boost::spirit::qi;

// very simple obj file reader (no normals, no texture coordinates)
std::list<std::list<std::size_t> > obj_faces(std::istream & in)
{
    // disable skipping of whitespace
    in.unsetf(std::ios::skipws);

    // wrap istream into iterator
    boost::spirit::istream_iterator first(in);
    boost::spirit::istream_iterator last;

    // use iterator to parse stream
    std::list<std::list<std::size_t> > result;
    bool r =
        qi::phrase_parse(
            first, last,
            // parser
            (
                // skip all lines not starting with "f"
                *qi::omit[
                    (qi::char_ - "f")
                    >> *(qi::char_ - qi::eol) >> qi::eol
                ]
                // parse all faces
                >> *("f" >> qi::repeat(3)[qi::int_] >> qi::eol)
                // skip remaining lines
                >> *qi::omit[qi::char_]
            ),
            // skipper
            qi::space - qi::eol,
            result);

    // fail if we did not get a full match
    if (!r || first != last) {
        std::string rest(first, last);
        throw std::runtime_error(
            "Syntax error while parsing\nStopped at:\n" + rest);
    };
    return result;
}
