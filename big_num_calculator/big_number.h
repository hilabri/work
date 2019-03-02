#include <string>
#include <iostream>

using namespace std;

class BigNumber 
{
    public:
        // CTOR's 
        BigNumber();
        BigNumber(const char *str);
        BigNumber(int num);
        BigNumber(const BigNumber& bn);
        
        // DTOR
        ~BigNumber() {};

        // assignment operators
        BigNumber& operator=(const BigNumber& bn);
        BigNumber& operator=(int num);
        
        // comparison operators
        bool operator==(const BigNumber& bn) const;
        bool operator>(const BigNumber& bn) const;
        bool operator<(const BigNumber& bn) const;
       
        // arithmetic operators
        BigNumber& operator+(const BigNumber& bn);
        BigNumber& operator-(const BigNumber& bn);
        
        // help functions
        inline bool getIsPositive() const;
        inline void setIsPositive(bool positive);
        inline const string& getNumber() const;
        inline void setNumber(const string &str);
    private:
        string number;
        bool isPositive;
        inline void deleteLeadingZeros();
};

// I/O operator
ostream& operator<<(ostream& os, const BigNumber& bn);
istream& operator<<(istream& is, const BigNumber& bn);

/* Inline functions: */
inline const string& BigNumber::getNumber() const { return number; }
inline bool BigNumber::getIsPositive() const { return isPositive; }
inline void BigNumber::setIsPositive(bool positive) { isPositive = positive; }
inline void BigNumber::setNumber(const string& str) { number = str; }
inline void BigNumber::deleteLeadingZeros() { number.erase(0, min(number.find_first_not_of('0'), number.size()-1)); }
