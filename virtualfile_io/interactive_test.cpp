#include "virtIO.h"
#include "asciiIO.h"
#include "binIO.h"
#include <iostream>
#include <typeinfo>
#include <string.h>

using namespace std;

enum ReadWrite
{
    READ, 
    WRITE
};

template<typename T>
void Test(VirtIO_t* virtIO, ReadWrite _readWrite)
{
    T var;
	switch(_readWrite)
	{
        case READ:
            try 
            {
    	    	(*virtIO) >> var;
                cout << var << endl; 
            } 
            catch (TException_t<VirtIO_t::ErrorStatus> ex)  
            {
                cout << ex.what();
            } 
	        break;
        case WRITE:
            try 
            {
	    	    cout << "Enter input: ";
	    	    cin >> var;
    		    (*virtIO) << var; 
            } 
            catch (TException_t<VirtIO_t::ErrorStatus> ex)  
            {
                cout << ex.what();
            } 
            break;
	  	 default: 
            break;
	}
}

/*---------------------------------------------------------*/

void TestGeneric(VirtIO_t* virtIO, ReadWrite _readWrite) 
{
    char testBuf[256];
    BinIO_t *binIO = (BinIO_t *)virtIO;
    switch(_readWrite)
	{
        case READ:
            try 
            {
	            size_t length;
	    	    cout << "Enter length: ";
                cin >> length;
                (*binIO) >> testBuf, length;
                cout << testBuf;
            } 
            catch (TException_t<VirtIO_t::ErrorStatus> ex)  
            {
                cout << ex.what();
            } 
	        break;
        case WRITE:
            try {
	    	    cout << "Enter input: ";
                cin >> testBuf;
                (*binIO) << testBuf, strlen(testBuf);
            } 
            catch (TException_t<VirtIO_t::ErrorStatus> ex) 
            {
                cout << ex.what();
            } 
            break;
	  	 default: 
            break;
	}
}
/*---------------------------------------------------------*/
void ChooseType(VirtIO_t* virtIO, ReadWrite _readWrite)
{
	unsigned int option;
    const bool isBin = typeid(*virtIO) == typeid(BinIO_t);

	cout << "Choose variable type: " << endl;
	cout << "[1] char" << endl;
	cout << "[2] unsigned char" << endl;
	cout << "[3] short" << endl;
	cout << "[4] unsigned short " << endl;
	cout << "[5] int " << endl;
	cout << "[6] unsigned int " << endl;
	cout << "[7] long " << endl;
	cout << "[8] unsigned long " << endl;
	cout << "[9] float " << endl;
	cout << "[10] double " << endl;
    
    if (isBin)
    {
	    cout << "[11] void* " << endl;
    }
	
    cin >>  option;
    switch (option) 
    {
		case 1: 
            Test<char>(virtIO, _readWrite); 
            break;
		case 2:
            Test<unsigned char>(virtIO,  _readWrite);
            break;
        case 3:
            Test<short>(virtIO, _readWrite);
            break;
        case 4:
            Test<unsigned short>(virtIO, _readWrite);
            break;
        case 5:
            Test<int>(virtIO, _readWrite);
            break;
        case 6:
            Test<unsigned int>(virtIO, _readWrite);
            break;
        case 7:
            Test<long>(virtIO, _readWrite);
             break;
        case 8:
            Test<unsigned long>(virtIO, _readWrite);
             break;
        case 9:
            Test<float>(virtIO, _readWrite);
             break;
        case 10:
            Test<double>(virtIO, _readWrite);
            break;
        case 11:
            if (isBin)
            {
                TestGeneric(virtIO, _readWrite);
            }
            break;
		default:
            break;
	}
}

/*---------------------------------------------------------*/
void ChooseOp(VirtIO_t* virtIO)
{
    int cont = 1;
    char option;
    size_t pos;
 
    while (cont)
    {
        cout << endl << "[<] <<(write) | [>] >>(read) | [s] seek position | [c] curr position : ";
       	cin >>  option;
        switch (option) 
        {
			case '<':
                ChooseType(virtIO, WRITE);
                break;
			case '>':
                ChooseType(virtIO, READ);
                break;         
			case 's':
                cout << "choose position: ";  
                cin >> pos;              
                virtIO->setPos(pos);
                break;
			case 'c':
                cout << "position is: " << virtIO->getPos() << endl;
                break;         
			default:
                cout << endl << "invalid option: " << option << endl; 
                cont = 0;
                break;
        }
    }
}

/*---------------------------------------------------------*/

int main() 
{
	int cont = 1;
	unsigned int option;
	VirtIO_t* virtIO;
    char fileType;
    string path;
    string access;
    
	while (cont) 
    {
		cout << endl;
        cout << "Test of Binary/Ascii? [b/a]: ";
		cin >> fileType;
		cout << "Enter Path and Access: ";
        cin >> path >> access;
        cout << "Path is: " << path << ", Access is: " << access << endl;

        switch (fileType) {
			case 'a':
                try 
                {
				    virtIO = new AsciiIO_t(path, access);
	                ChooseOp(virtIO);
                    delete virtIO;
                } 
                catch (TException_t<VirtIO_t::ErrorStatus> ex) 
                {
                    cout << ex.what();
                } 
                break;
			case 'b':
                try 
                {
 				    virtIO = new BinIO_t(path, access);
                    ChooseOp(virtIO);
	                delete virtIO;
                } 
                catch (TException_t<VirtIO_t::ErrorStatus> ex)  
                {
                    cout << ex.what();
                } 
                break;
			default:
                cout << "invalid file type - exiting program" << endl; 
                cont = 0;	
                break;
		}
    }
	return 0;
}


