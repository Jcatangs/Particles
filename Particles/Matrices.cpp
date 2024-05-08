#include "Matrices.h"
//#include <vector>
//#include <iomanip>
//#include <iostream>


namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.resize(rows, std::vector<double>(cols));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
                a.at(i).at(j) = 0;
        }
    }

    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            throw std::runtime_error("Error: dimensions must agree");
        }

        Matrix solution(a.getRows(), a.getCols());

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                solution(i, j) = a(i, j) + b(i, j);
            }
        }

        return solution;
    }


    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getRows())
        {
            throw std::runtime_error("Error: dimensions must agree");
        }

        Matrix solutions(a.getRows(), b.getCols());

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int k = 0; k < b.getCols(); k++)
            {
                for (int j = 0; j < a.getCols(); j++)
                {
                    solutions(i, k) += a(i, j) * b(j, k);
                }
            }
        }
        return solutions;
    }

    bool operator==(const Matrix& a, const Matrix& b) //FIXME //FIXME //FIXME //FIXME
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) return false;

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001) return false;
            }
        }

        return true;
    }


    bool operator!=(const Matrix& a, const Matrix& b)//FIXME //FIXME //FIXME //FIXME
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) return true;

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001) return true;
            }
        }

        return false;
    }

    ///Separate columns by ' ' and rows by '\n'
    std::ostream& operator<<(std::ostream& os, const Matrix& a)
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                os << std::setfill(' ') << std::setw(9) << a(i, j) << " ";
            }
            os << '\n';
        }

        return os;
    }
}