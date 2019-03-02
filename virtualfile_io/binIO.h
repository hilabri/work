#ifndef __BIN_IO__
#define __BIN_IO__

#include "virtIO.h"

class BinIO_t: public VirtIO_t
{
public:
	BinIO_t( const string& _fileName, const string& _mode);
	~BinIO_t() {}
    
    enum Action{
        READ, 
        WRITE
    };

    // read 
	virtual BinIO_t& operator>>(char& _buf)           { return Read( _buf); }
    virtual BinIO_t& operator>>(unsigned char& _buf)  { return Read( _buf); }
    virtual BinIO_t& operator>>(short& _buf)          { return Read( _buf); }
    virtual BinIO_t& operator>>(unsigned short& _buf) { return Read( _buf);}
    virtual BinIO_t& operator>>(int& _buf)            { return Read( _buf); }
    virtual BinIO_t& operator>>(unsigned int& _buf)   { return Read( _buf); }
    virtual BinIO_t& operator>>(long& _buf)           { return Read( _buf); }
    virtual BinIO_t& operator>>(unsigned long& _buf)  { return Read( _buf);}
    virtual BinIO_t& operator>>(float& _buf)          { return Read( _buf); }
    virtual BinIO_t& operator>>(double& _buf)         { return Read( _buf); }
    
    // write
    virtual BinIO_t& operator<<(const char& _buf)           { return Write( _buf); }
    virtual BinIO_t& operator<<(const unsigned char& _buf)  { return Write( _buf); }
    virtual BinIO_t& operator<<(const short& _buf)          { return Write( _buf); }
    virtual BinIO_t& operator<<(const unsigned short& _buf) { return Write( _buf);}
    virtual BinIO_t& operator<<(const int& _buf)            { return Write( _buf); }
    virtual BinIO_t& operator<<(const unsigned int& _buf)   { return Write( _buf); }
    virtual BinIO_t& operator<<(const long& _buf)           { return Write( _buf); }
    virtual BinIO_t& operator<<(const unsigned long& _buf)  { return Write( _buf);}
    virtual BinIO_t& operator<<(const float& _buf)          { return Write( _buf); }
    virtual BinIO_t& operator<<(const double& _buf)         { return Write( _buf); }
    
    BinIO_t& operator>>(void* _buf)   { m_buffer = _buf; m_action = READ; return *this; }
    BinIO_t& operator<<(void* _buf)   { m_buffer = _buf; m_action = WRITE; return *this; }
    void operator,(size_t _size);

private:
	template<typename T> BinIO_t& Read(T& _buf)   { return ReadInternal(&_buf, sizeof(T)); }
	template<typename T> BinIO_t& Write(T& _buf)  { return WriteInternal(&_buf, sizeof(T)); }
    BinIO_t& ReadInternal(void *_buf, size_t _size);
    BinIO_t& WriteInternal(const void* _buf, size_t _size);
    
    BinIO_t& operator= (const BinIO_t& _io); 
    BinIO_t(const BinIO_t& _io);  

private:
	void* m_buffer;
    Action m_action;

};



BinIO_t::BinIO_t( const string& _fileName, const string& _mode) : VirtIO_t(_fileName, _mode) 
{ }



void BinIO_t::operator,(size_t _size)
{
    if( m_action == READ)
    {	
        ReadInternal(m_buffer, _size);
    }
	if( m_action == WRITE)
	{
		WriteInternal(m_buffer, _size);
	}	
}



BinIO_t& BinIO_t::ReadInternal(void *_buf, size_t _size)
{
    if (!isReadingAllowed())
    {
        Error(bad_access_e);
        THROW(getError(), "can't access file for read");
    }   
    int numRead = fread(_buf, _size, 1, File());;
    if(0 == numRead)
    {
        Error(readErr_e);
        THROW(getError(), "read failed");
    }
    return *this;
}



BinIO_t& BinIO_t::WriteInternal(const void* _buf, size_t _size)
{
     if (!isWritingAllowed())
     {
        Error(bad_access_e);
        THROW(getError(), "can't access file for write");
     }   
     int numWritten = fwrite(_buf, _size, 1, File());
     if (0 == numWritten)
     {
        Error(writeErr_e); 
        THROW(getError(), "write failed");
     }   
     return *this;
}

#endif
