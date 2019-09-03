#ifndef GUARD_RESTRICTEDNUM_HPP
#define GUARD_RESTRICTEDNUM_HPP

#include <ostream>
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
public:
    static const RestrictedNum<min, max> min_value;
    static const RestrictedNum<min, max> max_value;

private:
    restr_num m_num;

private:
    template<class NumType>
    restr_num convert_to_restricted(const NumType& other);

public:
    template<class NumType>
    RestrictedNum(NumType num);
    RestrictedNum(const RestrictedNum<min, max>& other);

    ~RestrictedNum() = default;

    template<class NumType>
    RestrictedNum<min, max>& operator=(const NumType& other);

    template<restr_num fmin, restr_num fmax>
    friend std::ostream& operator<<(std::ostream& out, RestrictedNum<fmin, fmax> rnum);

    bool operator==(const RestrictedNum<min, max> other) const;
    bool operator!=(const RestrictedNum<min, max> other) const;
    bool operator<(const RestrictedNum<min, max> other) const;
    bool operator>(const RestrictedNum<min, max> other) const;

    RestrictedNum<min, max>& operator++();
    RestrictedNum<min, max> operator++(int);
};


template<restr_num min, restr_num max>
const RestrictedNum<min, max> RestrictedNum<min, max>::min_value = min;
template<restr_num min, restr_num max>
const RestrictedNum<min, max> RestrictedNum<min, max>::max_value = max;

template<restr_num min, restr_num max>
template<class NumType>
RestrictedNum<min, max>::RestrictedNum(NumType num)
    : m_num(convert_to_restricted(num))
{
}

template<restr_num min, restr_num max>
RestrictedNum<min, max>::RestrictedNum(const RestrictedNum<min, max>& other)
    : m_num(other.m_num)
{
}

template<restr_num min, restr_num max>
template<class NumType>
RestrictedNum<min, max>& RestrictedNum<min, max>::operator=(const NumType& other)
{
    m_num = convert_to_restricted(other);
    return *this;
}

template<restr_num min, restr_num max>
template<class NumType>
restr_num RestrictedNum<min, max>::convert_to_restricted(const NumType& other)
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

template<restr_num min, restr_num max>
bool RestrictedNum<min, max>::operator==(const RestrictedNum<min, max> other) const
{
    return this->m_num == other.m_num;
}

template<restr_num min, restr_num max>
bool RestrictedNum<min, max>::operator!=(const RestrictedNum<min, max> other) const
{
    return this->m_num != other.m_num;
}

template<restr_num min, restr_num max>
bool RestrictedNum<min, max>::operator<(const RestrictedNum<min, max> other) const
{
    return this->m_num < other.m_num;
}

template<restr_num min, restr_num max>
bool RestrictedNum<min, max>::operator>(const RestrictedNum<min, max> other) const
{
    return this->m_num > other.m_num;
}

template<restr_num min, restr_num max>
RestrictedNum<min, max>& RestrictedNum<min, max>::operator++()
{
    (*this) = m_num + 1;
    return *this;
}

template<restr_num min, restr_num max>
RestrictedNum<min, max> RestrictedNum<min, max>::operator++(int)
{
    RestrictedNum<min, max> ret(this->m_num);
    *this = m_num + 1;
    return ret;
}

template<restr_num min, restr_num max>
std::ostream& operator<<(std::ostream& out, RestrictedNum<min, max> rnum)
{
    out << rnum.m_num;
    return out;
}

#endif
