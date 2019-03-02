#include <iostream>
#include "page.h"
#include "pool.h"

using namespace std;

#define TEXT_SIZE 256
#define PAGE_CAPACITY 8

void Test(memManager_t* mnPts)
{
	int cont = 1;			// trigger to stop loop	
	unsigned int option;
    unsigned int res;
    unsigned int size;
    unsigned int pos;
    char src[TEXT_SIZE];
    char dest[TEXT_SIZE];
    int srcInt;
    int destInt;
    
	while (cont) {
		cout << "------------------------------" << endl;
		cout << "Choose option: " << endl;
		cout << "1: write to current pos" << endl;
		cout << "2: read from current pos" << endl;
		cout << "3: write to pos " << endl;
		cout << "4: read from pos " << endl;
		cout << "5: is empty " << endl;
		cout << "6: get actual size " << endl;
		cout << "7: get position " << endl;
		cout << "8: set position " << endl;
		cout << "9: write int " << endl;
		cout << "10: read int " << endl;
		//cout << "11: Pool: get default size  " << endl;
		//cout << "12: Pool: set default size  " << endl;
		//cout << "13: Page: get capacity  " << endl;
		//cout << "14: Page: is full?  " << endl;
		cout << "Any another number - stop " << endl;
		cout << "------------------------------" << endl;
		
		cin >>  option;

        switch (option) {
			case 1:
                cout << "Enter text: \n";
                cin >> src;
                size = strlen(src);
                res = mnPts->write(src, size);
                cout << res << " bytes were written" << endl;
                break;
			case 2:
                cout << "Enter size to read? \n";
                cin >> size;
                res = mnPts->read(dest, size);
                cout << res << " bytes were read" << endl;
                cout << "Text is: " << dest << endl;
                memset(dest, 0, TEXT_SIZE);
                break;
			case 3:
                cout << "Enter text: \n";
                cin >> src;
                cout << "Enter postion: \n";
                cin >> pos;
                size = strlen(src);
                res = mnPts->write(src, size, pos);
                cout << res << " bytes were written" << endl;
                break;
			case 4:
                cout << "Enter size to read? \n";
                cin >> size;
                cout << "Enter postion: \n";
                cin >> pos;
                res = mnPts->read(dest, size, pos);
                cout << res << " bytes were read" << endl;
                cout << "Text is: " << dest << endl;
                memset(dest, 0, TEXT_SIZE);
                break;
			case 5:
                if (mnPts->isEmpty()) { cout << "page is empty" ;}
                else { cout << "page not empty\n" ;}
                break;
			case 6:
                cout << "The page actual size is: " << mnPts->getActualSize() <<endl;
                break;
			case 7:
                cout << "The page position is: " << mnPts->getPosition() <<endl;
                break;
			case 8:
                cout << "Enter position :\n";
                cin >> pos;
                res =  mnPts->setPosition(pos);
                if(!res) { cout << " Position is invalid\n";}
                break;
			case 9:
                cout << "Enter number:\n";
                cin >> srcInt;
                cout << "Enter postion:\n";
                cin >> pos;
                res = mnPts->write(&srcInt, sizeof(int), pos);
                cout << res << " bytes were written" << endl;
                break;
			case 10:
                cout << "Enter postion:\n";
                cin >> pos;
                res = mnPts->read(&destInt, sizeof(int), pos);
                cout << res << " bytes were read" << endl;
                cout << "Number is: " << destInt << endl;
                destInt = 0; 
                break;
/*
			case 11:
                cout << "The page capacity is: " << page.getCapacity() << endl;
                break;
			case 12:
                if (page.isFull()) { cout << "page is full" ;}
                else { cout << "page not full\n" ;}
                break;
            case 13:
                cout << "The default size is: " << pool.getDefaultSize() <<endl;
                break;
			case 14:
                cout << "Enter default size: \n";
                cin >> size;
                pool.setDefaultSize(size);
                break;
*/
			default: cont = 0;	break;
		}
	}
}


int main()
{
	int cont = 1;			// trigger to stop loop	
	unsigned int option;
	memManager_t* mnPts;
	
	while (cont) {
		cout << "------------------------------" << endl;
		cout << "1: Page" << endl;
		cout << "2: Pool" << endl;
		cout << "------------------------------" << endl;
		cin >>  option;
        
        switch (option) {
			case 1:
				mnPts = new memPage_t(PAGE_CAPACITY);
				Test(mnPts);
				delete mnPts;
                break;
			case 2:
    			mnPts = new memPool_t;
    			Test(mnPts); 
				delete mnPts;
                break;

			default: cont = 0;	break;
		}
	}

	return 0;
}


