
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/*
CDR line:

IMSI
MSISDN
IMEI
operatorName
operatorMCC             
callType
callDate
callTime
partyMSISDN
partyOperatot
callDuration
Download
Upload
*/

char* OBN[] = {"Yairjun","bari","Janiv","Asaf","Michael","Avital","Hila","Liran","Eli","Yaron"};
char* tuple = "90210";
char* callType[] = {"MOC","MTC","SMS-MO","SMS-MT","GPRS"};


int main()
{
	int i, j, k;
	char fileName[50]; 
	char baseName[50]; 
	char newName[50];
	long unsigned int number;
	long unsigned int number2;
	long unsigned int duration;
	FILE* filePtr = NULL;
	srand(time(0)); /* rand() */
	
	for(k=0; k<1; k++)
	{    
	    for( j =0; j <4; j++)
	    {
	        sprintf(baseName, "%dCDRfile%d.cdr", k, j);
	        sprintf(fileName, "/tmp/%dCDRfile%d.cdr", k, j);
	        filePtr = fopen(fileName, "w");
	        
	        for(i = 0; i < 10000 ; ++i)
	        {
		        number = (rand() %  99999999999) + 10000000; /* imsi */
		        
		        fprintf(filePtr,"%lu|",number);
		        
		        number = (rand() %  99999999999) + 10000000; /* msisdn */
		        
		        fprintf(filePtr,"%lu|",number);
		        
		        number = (rand() %  99999999999) + 10000000; /* imei */
		        
		        fprintf(filePtr,"%lu|",number);
		        
		        fprintf(filePtr,"%s|",OBN[rand() % 10]); /*OBN */
		        
		        number2 = (rand() %  9999) + 1000; /* mcc */
		        fprintf(filePtr,"%lu|",number2);
		        
		        number = rand() % 5;
		        fprintf(filePtr,"%s|",callType[number]); /* callType */
		        
		        fprintf(filePtr,"06/11/1993|"); /* call Time */
		        fprintf(filePtr,"04:20:00|"); /* call Time */
		        
		        number2 = (rand() %  99999999999) + 10000000; /* party msisdn */
		        
		        fprintf(filePtr,"%lu|",number2);
		        
		        /* if internet - no need for party obn */
		        if(!strcmp(callType[number], callType[4]))
		        {
			        fprintf(filePtr,"----|"); /* party OBN */
		        }
		        else
		        {
			        fprintf(filePtr,"%s|",OBN[rand() % 10]); /* party OBN */
		        }
		        /* if call - duration */		
		        if(!strcmp(callType[number], callType[0]) || !strcmp(callType[number], callType[1]))
		        {
			        duration = rand() %  1000; /* duration */
			        fprintf(filePtr,"%lu|",duration);
		        }
		        else
		        {
			        duration = 0;
			        fprintf(filePtr,"%lu|",duration);
		        }
		        /* if internet - add download + upload */
		        if(!strcmp(callType[number], callType[4]))
		        {
			        duration = rand() %  1000; /* download */
			        fprintf(filePtr,"%lu|",duration);
			        duration = rand() %  10; /* upload */
			        fprintf(filePtr,"%lu|",duration);
		        }
		        else
		        {
			        duration = 0;
			        fprintf(filePtr,"%lu|",duration);
			        fprintf(filePtr,"%lu|",duration);
		        }
                fprintf(filePtr,"\n");
	        }
        	fclose(filePtr);
        	/* move to input dir */
        	strcpy(newName, "/tmp/cdr/");
        	strcat(newName, baseName);
        	printf("renaming %s=>%s\n", fileName, newName);
        	if (0 != rename(fileName, newName)) perror("Rename");
        }
        sleep(1);
    }
	return 0;
}













