#ifndef __PIXEL_H__
#define __PIXEL_H__

template<class T>
class PixelRGB
{
	public:
		PixelRGB() { }
        PixelRGB(const T _red, const T _green, const T _blue);
        //~PixelRGB();
	/*	
		PixelRGB(const   PixelRGB& _rgb);
		PixelRGB& operator=(const   PixelRGB& _rgb);
	*/	
		T GetRed()const;
		T GetBlue()const;
		T GetGreen()const;
		
		void SetPixel(const T _red, const T _green, const T _blue);
		
	private:
		void SetRed(const T _red);
		void SetGreen(const T _green);
		void SetBlue(const T _blue);
	private:
		T m_red;
		T m_green;
		T m_blue;
};

template<class T>
PixelRGB<T>::PixelRGB(const T _red, const T _green, const T _blue): m_red(_red), m_green(_green), m_blue(_blue) {}

template<class T>
T PixelRGB<T>::GetRed()const {return m_red;}

template<class T>
T PixelRGB<T>::GetGreen()const {return m_green;}

template<class T>
T PixelRGB<T>::GetBlue()const {return m_blue;}

template<class T>
void PixelRGB<T>::SetPixel(const T _red, const T _green, const T _blue)
{
	m_red = _red;
	m_green = _green;
	m_blue = _blue;
}

#endif
