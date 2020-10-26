#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
global definitions
*/
#define NUM_OF_USER 100         //max number of users that can be handled
int i=0;                        //initailizing 'i' as a counter for putting data in the structure
struct bank_data_model{         //struct to store user data
  char user[80];
  unsigned int amount, account_no;
};
struct bank_data_model bank_data[10];

/*
reading data from csv and storing to the structure named "bank_data"
*/
void readFromCsv(){
  char dataToBeRead[256];

  FILE *filePointer ;
  filePointer = fopen("Files/bank_data.csv","r");

  if(filePointer == NULL)
  {
      printf("bank.csv file failed to open.");      //if file does not exists then log a message.
  }
  else
  {
      while(fgets(dataToBeRead, 256, filePointer) != NULL)      //reading each line
      {
          char *ptr = strtok(dataToBeRead, ",");

          //getting data from each line and seperating wherever ',' is encountered
          for(int count=0;ptr!=NULL;count++) {
              //switch case for distributing ',' seperated data
              switch(count){
                  case 0: strcpy(bank_data[i].user,ptr); break;
                  case 1: bank_data[i].account_no = atoi(ptr); break;
                  case 2: bank_data[i].amount= atoi(ptr); break;
              }
              ptr = strtok(NULL,",");   //pointing to next data after ','
          }
          i++;                          //incrementing 'i' to read next data
      }
      fclose(filePointer);              //closing the file
  }
}

/*
entering data for user  (undemanded feature)
*/
// void writeOnCsv(char user[],int amount, unsigned long long int account_no){
//     FILE *filePointer;
//     filePointer = fopen("Files/bank_data.csv","a");
//     fprintf(filePointer, "%s,%u,%lu", user, amount, account_no);
//     strcpy(bank_data[i].user,user);
//     bank_data[i].amount=amount;
//     bank_data[i].account_no = account_no;
//     i++;
//     fclose(filePointer);
// }

/*
fund transfer
*/
void fundTransfer(int i){

    int accn,amount;

    printf("Hi %s,\n", bank_data[i].user);
    printf("Enter the details of reciever\n");
    printf("Account number: ");
    scanf("%d", &accn);
    printf("Amount to transfer: ");
    scanf("%d", &amount);

    if(amount>bank_data[i].amount){
        printf("\nInvalid fund transfer!\nYour balance is less than %d\n",amount);
    } else {
        bank_data[i].amount -= amount;
        printf("\nFund transfer was succsessful\nYour current balance is %d\n",bank_data[i].amount);
    }
}

/*
user action
*/
void userAction(int i){
    int selection=100;

    printf("\n\n");
    while (selection){
        printf("Select an option (1,2,...) to proceed\n");
        printf("1. View balance\n");
        printf("2. Fund transfer\n");
        printf("Press 0 to logout\n");
        printf("Your Selection: ");
        scanf("%d",&selection);
        switch (selection){
            case 1:
                    printf("\n+--------------- View Balance --------------------+\n");
                    printf("Hi %s,\nYour balance is: %d\n",bank_data[i].user, bank_data[i].amount);
                    printf("+-------------------------------------------------+\n\n");
                    break;
            case 2:
                    printf("\n+--------------- Fund Transfer -------------------+\n");
                    fundTransfer(i);
                    printf("+-------------------------------------------------+\n\n");
                    break;
            case 0: printf("\nThank you for using our bank.\n\n");
                    break;
            default: printf("\nPlease enter a valid option\n\n");
        }
    }
}

/*
main function
*/
int main(int i)
{
    readFromCsv();      //reading from csv and storing to global structure
    userAction(1);      //send index to access a particular user if he/she logs in

    return 0;
}
