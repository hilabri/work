#include <exception>
#include <string>
#include <sstream>
#include <iostream>

#define THROW(E,M) TExceptionThrow(E, M, __FILE__, __LINE__);

template<typename T>  // exception type
class TException_t {
public:
	TException_t(const T& _nested, const char* _message, const char* _file, size_t _line)
	: m_nested(_nested)
	, m_msg(_message)
	, m_file(_file)
	, m_line(_line) { }
	
	std::string what() const
    {
		std::stringstream s; 
        s << std::endl << "Error: " << m_msg << ' ' << " at line: " << m_line << " in file: " << m_file << std::endl;
		return s.str();
	}
	
private:
	T m_nested; 
	std::string m_msg;
	std::string m_file; 
	size_t m_line;
	
	
};

    
template<typename T>  
void TExceptionThrow(const T& _nested, const char* _message, const char* _file, size_t _line)
{
	throw TException_t<T>(_nested,  _message, _file, _line);
}


