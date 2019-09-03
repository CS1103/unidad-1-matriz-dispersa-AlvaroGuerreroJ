#include <iostream>

#include "Matrix.hpp"
#include "RestrictedNum.hpp"

using std::cout;
using std::endl;

int main()
{
    using number_0_10 = RestrictedNum<0, 10>;
    number_0_10 n0 = 0;
    cout << "n0: " << n0 << endl;;
    n0 = 7;
    cout << "n0: " << n0 << endl;;

    number_0_10 n1 = 10;
    cout << "n1: " << n0 << endl;;
    n1 = 2.5;
    cout << "n1: " << n0 << endl;;

    // number_0_10 n2 = 11;
    // cout << "n2: " << n2;
    // n2 = "hello";

    Matrix<double, 10, 15> md;

    cout << endl << endl;
    md.print_matrix(cout);

    for (unsigned int i = 0; i < 10; i++)
    {
        for (unsigned int j = 0; j < 15; j++)
        md[i][j] = i + j;
    }
    cout << endl << endl;

    md.print_matrix(cout);
    cout << endl << endl;

    (md * 1.5).print_matrix(cout);
    cout << endl << endl;

    md = 2 * md;
    md.print_matrix(cout);
    cout << endl << endl;

    md = md * 1.4;
    md.print_matrix(cout);
    cout << endl << endl;

    Matrix<int, 3, 3> mi0 {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };

    mi0.print_matrix(cout);
    cout << endl << endl;

    Matrix<int, 3, 3> mi1 = {
        {-9, -8, -7},
        {-6, -5, -4},
        {-3, -2, -1}
    };

    mi1.print_matrix(cout);
    cout << endl << endl;

    Matrix<int, 3, 3> mi2 = mi0 + mi1;

    mi2.print_matrix(cout);
    cout << endl << endl;
}
