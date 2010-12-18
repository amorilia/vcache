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

}

// convert raw array to list
template <typename T, int n, int m>
std::list<std::list<T> > array_to_list(T raw_data[n][m])
{
    std::list<std::list<T> > data;
    for (int i = 0; i < n; ++i) {
        std::list<T> row;
        for (int j = 0; j < m; ++j) {
            row.push_back(raw_data[i][j]);
        };
        data.push_back(row);
    };
    return data;
}
