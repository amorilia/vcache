// helper functions so sets and lists can be printed

#include <boost/foreach.hpp>
#include <iostream>
#include <list>
#include <set>

#include "vcache/mesh.hpp"

namespace std
{

template <typename T>
ostream & operator<<(ostream & stream, set<T> const & data)
{
    stream << "std::set( ";
    BOOST_FOREACH(T const & elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

template <typename T>
ostream & operator<<(ostream & stream, boost::shared_ptr<T> const & data)
{
    stream << "boost::shared_ptr( & ";
    if (data) {
        stream << *data << " ";
    } else {
        stream << "NULL ";
    }
    stream << ")";
    return stream;
};

template <typename T>
ostream & operator<<(ostream & stream, boost::weak_ptr<T> const & data)
{
    stream << "boost::weak_ptr( & ";
    boost::shared_ptr<T> result = data.lock();
    if (result) {
        stream << *result << " ";
    } else {
        stream << "NULL ";
    }
    stream << ")";
    return stream;
};

ostream & operator<<(ostream & stream, MFace const & mface)
{
    stream << "MFace( ";
    stream << mface.mv0->vertex << " " << mface.mv1->vertex << " " << mface.mv2->vertex;
    stream << ")";
    return stream;
};

template <typename T>
ostream & operator<<(ostream & stream, list<T> const & data)
{
    stream << "std::list( ";
    BOOST_FOREACH(T const & elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

// lock all elements of a set of weak_ptr's (this is needed before
// checking equality)
template <typename T>
set<boost::shared_ptr<T> > lock_set(set<boost::weak_ptr<T> > const & data)
{
    set<boost::shared_ptr<T> > result;
    BOOST_FOREACH(boost::weak_ptr<T> const & elem, data) {
        result.insert(elem.lock());
    }
    return result;
}

}
