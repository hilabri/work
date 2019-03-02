#include "big_number.h"

#define MIN(A,B) (A < B ? A : B)
/*------------------------------------------------------------------------------------------*/

BigNumber::BigNumber()
{
    isPositive = true;
}
/*------------------------------------------------------------------------------------------*/

BigNumber::BigNumber(int num)
{
    isPositive = (0 <= num);
    if (!isPositive) num *= -1;
    number = to_string(num);
}
/*------------------------------------------------------------------------------------------*/

BigNumber::BigNumber(const char *str)
{
    int i;
    isPositive = (str[0] != '-');
    number = isPositive ? string(str): string(str+1);
    /* validate string is numeric, otherwise init to 0 */
    for (i = 0; i< number.length(); i++)
    {
        if (!isdigit(number[i]))
        {
            number = "0";
            return;
        }
    }
    if (0 == number.compare("0"))
    {
        isPositive = true;
    }
    deleteLeadingZeros();
}
/*------------------------------------------------------------------------------------------*/

BigNumber::BigNumber(const BigNumber &bn)
{
    number = bn.getNumber();
    isPositive = bn.getIsPositive();
}
/*------------------------------------------------------------------------------------------*/

BigNumber& BigNumber::operator=(const BigNumber& bn)
{
    if(this != &bn)  
    {
        number = bn.getNumber();
        isPositive = bn.getIsPositive();
    }
    return *this; 
} 
/*------------------------------------------------------------------------------------------*/

BigNumber& BigNumber::operator=(int num)
{
    isPositive = (0 <= num);
    if (!isPositive) num *= -1;
    number = to_string(num);
    return *this;
}

/* I/O operators: */
/*------------------------------------------------------------------------------------------*/

ostream& operator<<(ostream& os, const BigNumber& bn)
{
    const char sign = bn.getIsPositive() ? '+' : '-';
    os << sign << bn.getNumber();
    return os;
}

/*------------------------------------------------------------------------------------------*/

istream& operator<<(istream& is, BigNumber& bn)
{
    string tmp;
    is >> tmp;
    bn.setNumber(tmp);
    bn.setIsPositive(true);
    return is;
}

/* comparison operators */
/*------------------------------------------------------------------------------------------*/

bool BigNumber::operator>(const BigNumber& bn) const
{
    const string &bnNum = bn.getNumber();
    int res;

    if (isPositive != bn.getIsPositive())
    {
        return isPositive; /* return true if 'this' is positive and bn is negative */
    }

    int len = number.length();
    if (len != bnNum.length()) 
    {
        return isPositive ? (len > bnNum.length()) : (len < bnNum.length());
    }

    res = number.compare(bnNum);
    return isPositive ? (0 < res) : (0 > res);
}
/*------------------------------------------------------------------------------------------*/

bool BigNumber::operator<(const BigNumber& bn) const
{
    return (bn > *this);
}
/*------------------------------------------------------------------------------------------*/

bool BigNumber::operator==(const BigNumber& bn) const
{
    const int len = number.length();
    const string &bnNum = bn.getNumber();

    if ((len == bnNum.length()) && (isPositive == bn.getIsPositive()))
    {
        return (0 == bnNum.compare(number));
    }
    return false;
}
/*------------------------------------------------------------------------------------------*/

BigNumber& BigNumber::operator+(const BigNumber& bn)
{
    const string& num1 = bn.getNumber().length() >= number.length() ? bn.getNumber() : number;
    const string& num2 = bn.getNumber().length() >= number.length() ? number : bn.getNumber(); 
    int i1 = num1.length()-1;
    int i2 = num2.length()-1;
    string tmp;
    unsigned short carry = 0;
    int i, sum;

    if (!isPositive || !bn.getIsPositive())
    { 
        cout << "No change was made, negative numbers not supported" << endl;
        return *this; 
    } 
    
    for (i = MIN(i1,i2); i >= 0; i--) /* iterate over common indexes */
    {
        sum = num1[i1] - '0' + num2[i2] - '0' + carry;
        tmp.insert(tmp.begin(), (sum % 10) + '0');
        carry = sum / 10;
        i1--;
        i2--;
    }
    while (0 <= i1) /* iterate over remaining digits of longer number */
    {
        sum = num1[i1] - '0' + carry;
        tmp.insert(tmp.begin(), (sum % 10) + '0');
        carry = sum / 10;
        i1--;
    }
    if (0 < carry) /* add carry if exists */
    {
        tmp.insert(tmp.begin(), '1');
    }
    
    number = tmp;
    isPositive = true;
    return *this;
}

/*------------------------------------------------------------------------------------------*/
BigNumber& BigNumber::operator-(const BigNumber& bn)
{
    const string& num1 = (bn < *this) ? number : bn.getNumber();
    const string& num2 = (bn < *this) ? bn.getNumber() : number;
    int i1 = num1.length()-1;
    int i2 = num2.length()-1;
    const bool resPositive = (bn < *this || bn == *this); 
    string tmp;
    unsigned short carry = 0;
    int i, sum;

    if (!isPositive || !bn.getIsPositive()) 
    {
        cout << "No change was made, negative numbers not supported" << endl;
        return *this;
    }
 
    for (i = MIN(i1,i2); i >= 0; i--) /* iterate over common indexes */
    {
        sum = (num1[i1] - '0') - (num2[i2] - '0') - carry;
        carry = 0;
        if (sum < 0) 
        {
            carry = 1;
            sum += 10;
        }
        tmp.insert(tmp.begin(), sum + '0');
        i1--;
        i2--;
    }
    while (0 <= i1) /* iterate over remaining digits of longer number */
    {
        sum = (num1[i1] - '0') - carry;
        carry = 0;
        if (sum < 0) 
        {
            carry = 1;
            sum += 10;
        }
        tmp.insert(tmp.begin(), sum  + '0');
        i1--;
    }
    number = tmp;
    deleteLeadingZeros();
    isPositive = resPositive;
    return *this; 
}

