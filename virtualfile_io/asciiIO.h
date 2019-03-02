#ifndef __ASCII_IO__
#define __ASCII_IO__

#include "virtIO.h"


class AsciiIO_t: public VirtIO_t
{
public:
    AsciiIO_t( const string& _fileName, const string& _mode);
    ~AsciiIO_t() {}
    
    // Read 
	virtual AsciiIO_t& operator>>(char& _buf)           { return Read( _buf, "%c"); }
    virtual AsciiIO_t& operator>>(unsigned char& _buf)  { return Read( _buf, "%c"); }
    virtual AsciiIO_t& operator>>(short& _buf)          { return Read( _buf, "%hd"); }
    virtual AsciiIO_t& operator>>(unsigned short& _buf) { return Read( _buf, "%hu");}
    virtual AsciiIO_t& operator>>(int& _buf)            { return Read( _buf, "%d"); }
    virtual AsciiIO_t& operator>>(unsigned int& _buf)   { return Read( _buf, "%u"); }
    virtual AsciiIO_t& operator>>(long& _buf)           { return Read( _buf, "%ld"); }
    virtual AsciiIO_t& operator>>(unsigned long& _buf)  { return Read( _buf, "%lu");}
    virtual AsciiIO_t& operator>>(float& _buf)          { return Read( _buf, "%f"); }
    virtual AsciiIO_t& operator>>(double& _buf)         { return Read( _buf, "%f"); }
    
    // Write
    virtual AsciiIO_t& operator<<(const char& _buf)           { return Write( _buf, "%c"); }
    virtual AsciiIO_t& operator<<(const unsigned char& _buf)  { return Write( _buf, "%c"); }
    virtual AsciiIO_t& operator<<(const short& _buf)          { return Write( _buf, "%hd"); }
    virtual AsciiIO_t& operator<<(const unsigned short& _buf) { return Write( _buf, "%hu");}
    virtual AsciiIO_t& operator<<(const int& _buf)            { return Write( _buf, "%d"); }
    virtual AsciiIO_t& operator<<(const unsigned int& _buf)   { return Write( _buf, "%u"); }
    virtual AsciiIO_t& operator<<(const long& _buf)           { return Write( _buf, "%ld"); }
    virtual AsciiIO_t& operator<<(const unsigned long& _buf)  { return Write( _buf, "%lu");}
    virtual AsciiIO_t& operator<<(const float& _buf)          { return Write( _buf, "%f"); }
    virtual AsciiIO_t& operator<<(const double& _buf)         { return Write( _buf, "%f"); }
    
private:
	template<typename T> AsciiIO_t&  Read(T& _buf, const char* _format);
	template<typename T> AsciiIO_t&  Write(T& _buf, const char* _format);
    
    AsciiIO_t& operator= (const AsciiIO_t& _io); 
    AsciiIO_t(const AsciiIO_t& _io);  
};

AsciiIO_t::AsciiIO_t( const string& _fileName, const string& _mode) : VirtIO_t(_fileName, _mode) 
{}


template<typename T>
AsciiIO_t& AsciiIO_t::Read(T& _buf, const char* _format)
{
    if (!isReadingAllowed()) 
    {
        Error(bad_access_e);
        THROW(getError(), "can't access file for read");
    }   
    int numRead = fscanf(File(), _format , &_buf);
    if(0 == numRead)
    {
        Error(readErr_e);
        THROW(getError(), "read failed");
    }
    return *this;
}


template<typename T>
AsciiIO_t& AsciiIO_t::Write(T& _buf, const char* _format)
{
     if (!isWritingAllowed())
     {
        Error(bad_access_e);
        THROW(getError(), "can't access file for write");
     }   
     int numWritten = fprintf(File(), _format, _buf);
     if (numWritten < 0)
     {
        Error(writeErr_e); 
        THROW(getError(), "write failed");
     }
    return *this;  
}



#endif
