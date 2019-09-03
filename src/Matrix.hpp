#ifndef GUARD_MATRIX_HPP
#define GUARD_MATRIX_HPP

#include <cstddef>
#include <initializer_list>
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
    public:
        std::map<col_number, T> m_row;

    public:
        Row();
        Row(const Row& other);

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
    Matrix(const Matrix<T, rows, cols>& other);
    Matrix(std::initializer_list<std::initializer_list<T>> il);
    ~Matrix() = default;

    void clean_empty();
    template<class Op>
    void do_each_element(Op op);
    std::ostream& print_matrix(std::ostream& out) const;

    Row& operator[](const row_number& row);
    const Row& operator[](const row_number& row) const;

    Matrix<T, rows, cols> operator*(const Matrix<T, rows, cols>& other) const;
    template<class ScalarType>
    Matrix<T, rows, cols> operator*(const ScalarType& scalar) const;

    Matrix<T, rows, cols> operator+(const Matrix<T, rows, cols>& other) const;
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
Matrix<T, rows, cols>::Matrix(const Matrix<T, rows, cols>& other)
    : m_matrix(other.m_matrix)
{
}

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols>::Matrix(std::initializer_list<std::initializer_list<T>> il)
{
    row_number cur_row_index = 0;
    auto cur_row_it = il.begin();

    do
    {
        col_number cur_col_index = 0;
        auto cur_val_it = cur_row_it->begin();

        do
        {
            (*this)[cur_row_index][cur_col_index] = *cur_val_it;

            cur_val_it++;

        } while (cur_val_it != cur_row_it->end() &&  (cur_col_index++, true));

        cur_row_it++;

    } while (cur_row_it != il.end() && (cur_row_index++, true));
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
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator*(const Matrix<T, rows, cols>& other) const
{
    // TODO: Implement

    return other;
}

template<class T, restr_num rows, restr_num cols>
template<class ScalarType>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator*(const ScalarType& scalar) const
{
    Matrix<T, rows, cols> ret(*this);
    ret.do_each_element([scalar](const T& val)->T{ return val * scalar; });
    return ret;
}

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols> Matrix<T, rows, cols>::operator+(const Matrix<T, rows, cols>& other) const
{
    Matrix<T, rows, cols> ret(other);

    for (auto const& row_pair : this->m_matrix)
    {
        row_number r = row_pair.first;
        for (auto const& value_pair : row_pair.second.m_row)
        {
            col_number c = value_pair.first;
            ret[r][c] += value_pair.second;
        }
    }

    return ret;
}

template<class T, restr_num rows, restr_num cols, class ScalarType>
Matrix<T, rows, cols> operator*(const ScalarType& scalar, const Matrix<T, rows, cols>& matrix)
{
    return matrix * scalar;
}

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols>::Row::Row()
{
}

template<class T, restr_num rows, restr_num cols>
Matrix<T, rows, cols>::Row::Row(const Row& other)
    : m_row(other.m_row)
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
        } while (c != cols - 1 && (c++, true));

        out << std::endl;

    } while (r != rows - 1 && (r++, true));

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
template<class Op>
void Matrix<T, rows, cols>::do_each_element(Op op)
{
    for (auto& key_row_pair : m_matrix)
    {
        for (auto& ke_pair : key_row_pair.second.m_row)
        {
            ke_pair.second = op(ke_pair.second);
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
