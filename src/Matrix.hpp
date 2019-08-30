#ifndef GUARD_MATRIX_HPP
#define GUARD_MATRIX_HPP

#include <cstddef>
#include <map>
#include <ostream>
#include <string>

#include "RestrictedNum.hpp"

template<class T, restr_num rows, restr_num cols>
class Matrix
{
private:
    typedef RestrictedNum<0, rows - 1> row_number;
    typedef RestrictedNum<0, cols - 1> col_number;

private:
    class Row
    {
    private:
        std::map<col_number, T> m_row;

    public:
        Row();

        T& operator[](const col_number& col);
        const T& operator[](const col_number& row) const;

        void clean_empty();
    };

private:
    typedef std::map<row_number, Row> matrix_type;

private:
    matrix_type m_matrix;

    const static Row default_row;
    const static T default_t;

public:
    Matrix();
    ~Matrix() = default;

    void clean_empty();
    std::ostream& print_matrix(std::ostream& out) const;

    Row& operator[](const row_number& row);
    const Row& operator[](const row_number& row) const;

    Matrix<T, rows, cols> operator*(const Matrix<T, rows, cols>& other);
    template<class ScalarType>
    Matrix<T, rows, cols> operator*(ScalarType scalar);
};

template<class T, restr_num rows, restr_num cols>
const typename Matrix<T, rows, cols>::Row Matrix<T, rows, cols>::default_row;

template<class T, restr_num rows, restr_num cols>
const T Matrix<T, rows, cols>::default_t = T();

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols>::Matrix()
{
}

template<class T, restr_num rows, restr_num cols>
typename Matrix<T, rows, cols>::Row& Matrix<T, rows, cols>::operator[](const row_number& row)
{
    return m_matrix[row];
}

template<class T, restr_num rows, restr_num cols>
const typename Matrix<T, rows, cols>::Row& Matrix<T, rows, cols>::operator[](const row_number& row) const
{ 
    auto temp = m_matrix.find(row);
    if (temp != m_matrix.end())
    {
        return temp->second;
    }
    else
    {
        return default_row;
    }
}

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols>::Row::Row()
{
}

template<class T, restr_num rows, restr_num cols>
T& Matrix<T, rows, cols>::Row::operator[](const col_number& row)
{
    return m_row[row];
}

template<class T, restr_num rows, restr_num cols>
const T& Matrix<T, rows, cols>::Row::operator[](const col_number& row) const
{
    auto temp = m_row.find(row);
    if (temp != m_row.end())
    {
        return temp->second;
    }
    else
    {
        return default_t;
    }
}

template<class T, restr_num rows, restr_num cols>
std::ostream& Matrix<T, rows, cols>::print_matrix(std::ostream& out) const
{
    // TODO: This will fail if the number of rows or columns is zero

    row_number r = 0;
    do
    {
        col_number c = 0;
        do
        {
            out << (*this)[r][c] << " ";
        } while (c != cols - 1 && (c++ == c || true));

        out << std::endl;

    // TODO: Maybe there's a better way
    } while (r != rows - 1 && (r++ == r || true));

    return out;
}

template<class T, restr_num rows, restr_num cols>
void Matrix<T, rows, cols>::clean_empty()
{
    // TODO: Using something like a list comprehension may be better

    // Delete all the rows that evaluate equal to the default row

    auto it = m_matrix.begin();
    while (it != m_matrix.end())
    {
        if (*it == default_row)
        {
            it = m_matrix.erase(it);
        }
        else
        {
            it->clean_empty();
        }
    }
}

template<class T, restr_num rows, restr_num cols>
void Matrix<T, rows, cols>::Row::clean_empty()
{
    auto it = m_row.begin();
    while (it != m_row.end())
    {
        if (*it == default_t)
        {
            it = m_row.erase(it);
        }
    }
}

#endif
