#include "big_number.h"
#include <iostream>
using namespace std;

#define MAX_LEN 256

int main()
{
    int cont = 1;           // trigger to stop loop 
    unsigned int option;
    char str[MAX_LEN];
    int num = 0;
    BigNumber bn1(num);
    BigNumber bn2(num);
    unsigned int res;
    
    while (cont) {
        cout << "---------------------------" << endl;
        cout << "Choose option: " << endl;
        cout << "1: Test CTOR from int " << endl;
        cout << "2: Test CTOR from string " << endl;
        cout << "3: Assign BN from int [ BN = int ] " << endl;
        cout << "4: Assign BN from BigNumber [ BN = &bn ] " << endl;
        cout << "5: +  operator " << endl;
        cout << "6: -  operator " << endl;
        cout << "7: == operator [ BN == &bn ]" << endl;
        cout << "8: >  operator [ BN  > &bn ]" << endl;
        cout << "9: <  operator [ BN  < &bn ]" << endl;
        cout << "---------------------------" << endl;
    
        cin >>  option;

        switch (option) {
            case 1:
            {
                cout << "Enter a number?\n";
                cin >> num;
                BigNumber bnTest(num);
                cout << "Test number = " << bnTest << endl;
                break;
            }
            case 2:
            {
                cout << "Enter a number?\n";
                cin >> str;
                BigNumber bnTest(str);
                cout << "Test number = " << bnTest << endl;       
                break;
            }
            case 3:
                cout << "Enter a number?\n";
                cin >> num;
                bn1 = num;
                cout << "BN = " << bn1 << endl;
                break;
            case 4:
                cout << "Enter a number?\n";
                cin >> str;
                bn2 = str;
                bn1 = bn2;
                cout << "BN = " << bn1 << endl;
                break;
            case 5:
                cout << "Please enter two positive numbers: " << endl;
                cin >> str;
                bn1 = str;
                cin >> str;
                bn2 = str;
                cout << "Big numbers addition = " << bn1+bn2 << endl;
                break;
            case 6:
                cout << "Please enter two positive numbers: " << endl;
                cin >> str;
                bn1 = str;
                cin >> str;
                bn2 = str;
                cout << "Big numbers subtraction = " << bn1-bn2 << endl;
                break;
            case 7:
                cout << "Enter a number a compare to BN " << bn1 << endl;
                cin >> str;
                bn2 = str;
                res = (bn2 == bn1);
                if (res) { cout << "numbers are equal\n";}
                else { cout << "numbers are different\n" ;}
                break;
            case 8:
                cout << "Enter a number to compare to BN " << bn1 << endl;
                cin >> str;
                bn2 = str;
                res = (bn1 > bn2);
                if (res) { cout << bn1 << " is bigger than " << bn2 << endl; }
                else { cout << bn1 << " not bigger than " << bn2 << endl; }
                break;
            case 9:
                cout << "Enter a number to compare to BN " << bn1 << endl;
                cin >> str;
                bn2 = str;
                res = (bn1 < bn2);
                if (res) { cout << bn1 << " is smaller than " << bn2 << endl; }
                else { cout << bn1 << " not smaller than " << bn2 << endl; }
                break;
            default: cont = 0;  break;
        }
    }

    return 0;
}
