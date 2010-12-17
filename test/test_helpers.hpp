// helper functions so sets and lists can be printed

#include <boost/foreach.hpp>
#include <iostream>
#include <list>
#include <set>

namespace std
{

ostream & operator<<(ostream & stream, set<int> const & data)
{
    stream << "std::set( ";
    BOOST_FOREACH(int elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

ostream & operator<<(ostream & stream, list<int> const & data)
{
    stream << "std::list( ";
    BOOST_FOREACH(int elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

}
