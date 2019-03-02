#ifndef __VIRT_IO__
#define __VIRT_IO__

#include "stdio.h"
#include "exception.h"
#include <string>

using namespace std;

class VirtIO_t
{
public:
	VirtIO_t();
	VirtIO_t(const string& _fileName, const string& _mode); 
	virtual ~VirtIO_t();  
    
    void Open(const string& _fileName, const string& _mode);

    enum ErrorStatus {
		ok_e,
		cant_open_file_e, 
		bad_access_e,
		writeErr_e,
    	readErr_e
	};
    
    const string& getPath() const              { return m_filePath; }
    long int getPos()const                     { return ftell(m_fp); }
    void setPos(size_t _pos)                   { fseek(m_fp, _pos, SEEK_SET); }
    
    // read 
	virtual VirtIO_t& operator>>(char& _Buf)=0;
    virtual VirtIO_t& operator>>(unsigned char& _Buf)=0;
    virtual VirtIO_t& operator>>(short& _buf)=0;
    virtual VirtIO_t& operator>>(unsigned short& _buf)=0;
    virtual VirtIO_t& operator>>(int& _buf)=0;
    virtual VirtIO_t& operator>>(unsigned int& _buf)=0;
    virtual VirtIO_t& operator>>(long& _buf)=0;
    virtual VirtIO_t& operator>>(unsigned long& _buf)=0;
    virtual VirtIO_t& operator>>(float& _buf)=0;
    virtual VirtIO_t& operator>>(double& _buf)=0;
    
    // write
    virtual VirtIO_t& operator<<(const char& _Buf)=0;
    virtual VirtIO_t& operator<<(const unsigned char& _Buf)=0;
    virtual VirtIO_t& operator<<(const short& _Buf)=0;
    virtual VirtIO_t& operator<<(const unsigned short& _Buf)=0;
    virtual VirtIO_t& operator<<(const int& _Buf)=0;
    virtual VirtIO_t& operator<<(const unsigned int& _buf)=0;
    virtual VirtIO_t& operator<<(const long& _Buf)=0;
    virtual VirtIO_t& operator<<(const unsigned long& _Buf)=0;
    virtual VirtIO_t& operator<<(const float& _Buf)=0;
    virtual VirtIO_t& operator<<(const double& _Buf)=0;


protected:
    bool IsOpen() const                 { return m_isOpen; }
    void Error(ErrorStatus _error)      { m_error = _error; }
	FILE* File() const                  { return m_fp; } 

    ErrorStatus getError() const        { return m_error; } 
    bool isReadingAllowed() const       { return (m_isOpen && m_mode != "w" && m_mode != "a"); }
    bool isWritingAllowed() const       { return (m_isOpen && m_mode != "r"); }

private:
	ErrorStatus m_error;
	FILE* m_fp;
    bool m_isOpen; 
	
    string m_filePath;
	string m_mode;

private:
    void Close();
    
    VirtIO_t& operator=(const VirtIO_t& _io);
    VirtIO_t(const VirtIO_t& _io);
};



VirtIO_t::VirtIO_t() 
    : m_error(ok_e)
    , m_isOpen(false)
    , m_fp(0)
{}



VirtIO_t::VirtIO_t(const string& _fileName, const string& _mode) 
	: m_error(ok_e)
    , m_isOpen(false) 
	, m_filePath(_fileName)
	,m_mode(_mode)
{
    Open(_fileName, _mode);   
}



VirtIO_t::~VirtIO_t()  
{
    if(m_isOpen)
    {
        Close();
    }
}



void VirtIO_t::Open(const string& _fileName, const string& _mode)
{
    if(m_isOpen)
	{
		m_error = cant_open_file_e;
        THROW(getError(), "file already open");
	}
    m_fp = fopen(_fileName.c_str(), _mode.c_str());
    if(!m_fp) 
	{
		m_error = cant_open_file_e;
        THROW(getError(), "open file failed");
	}
    m_isOpen = true;
}



void VirtIO_t::Close()
{
    if (EOF == fclose(m_fp))
    {
        m_error = bad_access_e;
        THROW(getError(), "close file failed");    
    } 
    m_isOpen = false;
}




#endif
