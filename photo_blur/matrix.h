#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>

#include <iostream>

template<class T>
class Matrix_t 
{
	public:
		Matrix_t(int columns, int _rows);
		//~Matrix_t();
		
		void SetAllValues(T _value);
		void SetValue(T _value, int columns, int _rows);
		void SetMatrixSize(int columns, int _rows);
		
        const T& GetValue(int columns, int _rows) const;
		int GetNumRows() const;
		int GetNumColumns() const;

        /* Iterator functions */
        T& ItrBegin();
        T& ItrNext();
        T& ItrEnd();

    private:
		typedef std::vector<std::vector<T> > Matrix;
        Matrix m_matrix;
		int m_rows;
    	int m_columns;
        
        int i;
        typename std::vector<T>::iterator rowItr;
        typename std::vector<std::vector<T> >::iterator columItr;
};

/* ---------------------------------------------------*/
template<class T>
Matrix_t<T>::Matrix_t(int _columns, int _rows): m_matrix(Matrix(_columns, std::vector<T>(_rows))), m_rows(_rows), m_columns(_columns)
{
}

template<class T>
void Matrix_t<T>::SetMatrixSize(int _columns, int _rows)
{
	m_matrix.resize(_columns);
	for(int j = 0; j < m_columns; j++) 
	{
    	m_matrix[j].resize( _rows);
    }
}

template<class T>
void Matrix_t<T>::SetValue(T _value, int _columns, int _rows)
{
	 m_matrix[_columns][_rows] = _value;
}


template<class T>
const T& Matrix_t<T>::GetValue(int _columns, int _rows) const
{
	return (m_matrix[_columns])[_rows];
}

template<class T>
void Matrix_t<T>::SetAllValues(T _value)
{
	for(int j = 0; j < m_columns; j++) 
	{
        for(int i = 0; i < m_rows; i++) 
        {
            SetValue(_value,j,i);
        }	
    }
}

template<class T>
int Matrix_t<T>::GetNumRows() const
{
	return m_rows;
}

template<class T>
int Matrix_t<T>::GetNumColumns() const
{
	return m_columns;
}

template<class T>
T& Matrix_t<T>::ItrBegin()
{
    columItr = m_matrix.begin();
    i = 0;
    rowItr = m_matrix[i].begin();
    return (*rowItr);
}

template<class T>
T& Matrix_t<T>::ItrNext()
{
    if(++rowItr != m_matrix[i].end())
    {
        return (*rowItr);
    }
    else 
    {
        if(++columItr != m_matrix.end())
        {
            i++;
            rowItr = m_matrix[i].begin();
            return (*rowItr);
        }
    }
    return ItrEnd();
}

template<class T>
T& Matrix_t<T>::ItrEnd()
{
    return (*m_matrix[m_columns-1].rbegin());
}

#endif
