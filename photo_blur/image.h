#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "filter.h"
#include "pixel.h"
#include <fstream>
#include <iostream>

#define PIC_TYPE_LENGTH 10

template<class T, class W>
class Image_t 
{
	public:
		Image_t(const char* _inputFile);
		~Image_t();
/*		
		Image_t(const Image_t& _img);
		Image_t& operator=(const Image_t& _img);
*/		
		void ApplyFilter(const Filter_t<W>& _filter); 
		void Save(const char* _outputFileName) const;
		
	private:
		void ReadFromPhoto(const char *_inputFile);
        PixelRGB<T> ApplyFilterForPixel(const Filter_t<W> &_filter, int _column, int _row);
        bool IsOutOfBounds(int _column, int _row, int steps) const;

		Matrix_t<PixelRGB<T> >* m_imageMatrix;
        char m_picType[PIC_TYPE_LENGTH];
		int m_range;
        int m_columns;
        int m_rows;
};

/*-----------------------------------------------------*/

template<class T, class W>
void Image_t<T,W>::ReadFromPhoto(const char* _inputFile)
{
    std::ifstream pic;    /* ifstream  ios::in default mode parameter */
	pic.open(_inputFile);
	if (!pic.is_open()) 
    {
        throw std::runtime_error("open photo fail");
    }
	pic >> m_picType >> m_rows >> m_columns >> m_range;  
    m_imageMatrix = new Matrix_t<PixelRGB<T> >(m_columns, m_rows);
    if(!m_imageMatrix)
    {
        throw std::runtime_error("new image allocation failed");
    }
    
    PixelRGB<T> pix;
    T red, green, blue;
    PixelRGB<T>* itr = &(m_imageMatrix->ItrBegin());
    while(pic >> red >> green >> blue)
	{
        pix.SetPixel(red, green, blue);
        *itr = pix; /* add to  image matrix */
        itr = &(m_imageMatrix->ItrNext());
    }
    pic.close();
}

template<class T, class W>
Image_t<T,W>::Image_t(const char *_inputFile)
{
    ReadFromPhoto(_inputFile);
}

template<class T, class W>
Image_t<T,W>::~Image_t()
{
    delete m_imageMatrix;
}

/* ---Whith iterotors-----
 *
template<class T, class W>
void Image_t<T,W>::Save(const char* _outputFileName) const
{
    std::ofstream pic;
	pic.open(_outputFileName);
	if (!pic.is_open()) 
    {
        throw std::runtime_error("open photo fail");
    }
    pic << m_picType << std::endl << m_columns << " " << m_rows << std::endl << m_range << std::endl;
    PixelRGB<T>* itr = &m_imageMatrix->ItrBegin();
    while (true)
    {
        pic << itr->GetRed() << " " << itr->GetGreen() << " " << itr->GetBlue() << " ";
        if (itr == &(m_imageMatrix->ItrEnd())) break;
        itr = &m_imageMatrix->ItrNext();
    } 
    pic.close();
}
*/

template<class T, class W>
void Image_t<T,W>::Save(const char* _outputFileName) const
{
    std::ofstream pic;    /* ofstream  ios::out default mode parameter */
	pic.open(_outputFileName);
	if (!pic.is_open()) 
    {
        throw std::runtime_error("open photo fail");
    }
    pic << m_picType << std::endl << m_rows << " " << m_columns << std::endl << m_range << std::endl;
    PixelRGB<T> pix;
    for (int i = 0; i < m_columns; i++)
    {
        for (int j = 0; j < m_rows; j++)
        {
            pix = m_imageMatrix->GetValue(i, j);
            pic << pix.GetRed() << " " << pix.GetGreen() << " " << pix.GetBlue() << " ";
        }
    }
    pic.close();
}

template<class T, class W>
bool Image_t<T,W>::IsOutOfBounds(int _column, int _row, int _steps) const
{
    if (m_imageMatrix->GetNumRows() <= (_row + _steps))
    {
        return true;
    }
    if (m_imageMatrix->GetNumColumns() <= (_column + _steps)) 
    {
        return true;
    }
    return ((_column - _steps < 0) || (_row - _steps < 0));
}

template<class T, class W>
PixelRGB<T> Image_t<T,W>::ApplyFilterForPixel(const Filter_t<W> &_filter, int _column, int _row)
{
    W red = 0;
    W green = 0;
    W blue = 0;
    const int size = _filter.GetSize();
    if (IsOutOfBounds(_column, _row, size/2))
    {
        return m_imageMatrix->GetValue(_column, _row); /* Return self if out of bounds, TODO: improve? */
    }
    const int col = _column - size/2;
    const int row = _row - size/2;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            const PixelRGB<T>& pix = m_imageMatrix->GetValue(col + i, row + j);
            W weight = _filter.GetWeight(i,j);
            red += weight * pix.GetRed();
            green += weight * pix.GetGreen();
            blue += weight * pix.GetBlue();
        }
    }
    return PixelRGB<T>(red, green, blue); 
}

template<class T, class W>
void Image_t<T,W>::ApplyFilter(const Filter_t<W>& _filter)
{
    Matrix_t<PixelRGB<T> > *outputMatrix = new Matrix_t<PixelRGB<T> >(m_columns, m_rows);
    if (!outputMatrix)
    {
        throw std::runtime_error("apply filter - allocation failed");
    }
    PixelRGB<T> pix;
    for (int i = 0; i < m_columns; i++)
    {
        for (int j = 0; j < m_rows; j++)
        {       
            pix = ApplyFilterForPixel(_filter, i, j);
            outputMatrix->SetValue(pix, i, j);
        }
    }
    delete m_imageMatrix;
    m_imageMatrix = outputMatrix;
}

#endif
