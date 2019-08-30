#ifndef GUARD_RESTRICTEDNUM_HPP
#define GUARD_RESTRICTEDNUM_HPP

#include <stdexcept>
#include <sstream>

typedef size_t restr_num;

/**
 * RestrictedNum is a class that stores a number that must be in the range
 * [min, max] (both inclusive)
 */
template<restr_num min, restr_num max>
class RestrictedNum
{
private:
    restr_num m_num;

private:
    template<class NumType>
    RestrictedNum<min, max> convert_to_restricted(const NumType& other);

public:
    template<class NumType>
    RestrictedNum(NumType num);

    ~RestrictedNum() = default;

    template<class NumType>
    RestrictedNum<min, max>& operator=(const NumType& other);
};

template<restr_num min, restr_num max>
template<class NumType>
RestrictedNum<min, max>::RestrictedNum(NumType num)
    : m_num(convert_to_restricted(num))
{
}

template<restr_num min, restr_num max>
template<class NumType>
RestrictedNum<min, max>& RestrictedNum<min, max>::operator=(const NumType& other)
{
    m_num = convert_to_restricted(other);
}

template<restr_num min, restr_num max>
template<class NumType>
RestrictedNum<min, max> RestrictedNum<min, max>::convert_to_restricted(const NumType& other)
{
    restr_num temp = other;
    if (min <= temp && temp <= max)
    {
        return temp;
    }
    else
    {
        std::ostringstream oss;
        oss << other << " casted to " << temp << " is not in the range ["
            << min << ", " << max << "]";
        throw std::domain_error(oss.str());
    }
}

#endif
