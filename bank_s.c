/*
  @author: Team 8 - Abhijeet,Anurag,Gourab,Nandini,Utkarsh,Yukta
  @language: C
*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<math.h>
#include<time.h>

int no_of_records(const char*);                 //To find no. of users in bank system
void scan(int);                                 //To read login credential file
void user_name_validation(int);                 //To find if user-name exist or not
void user_pwd_validation(int,int,int);          //To validate customer user-name and password
void admin_pwd_validation(int,int);             //To validate admin user-name and password
void scan_monthly_report();                     //To read Monthly unautorised access file
void userAction(int,int);                       //Customer Functionality
void fundTransfer(int,int);                     //Amount transfer(bank transaction)
void transaction_details(int,float,int);    	//To store transaction details in file
void transaction_history(int,int);              //To read transaction details
void admin_authorities(int);                    //Admin Functionality
void disp_blocked_users(int);                   //To display blocked user(admin action)
void unblock_user(int);                         //To unblock user(admin action)
void login_attempt(int,int);                    //To store unauthorised access in file
void write_to_file(int,int,int);                //To overwrite to login crediential file
void tostring(char[],int);

struct transaction                              //To store transaction details from file to structure
{
    int to_account;                             //To which account amount is transferred
    float amt_transferred;                      //Amount transferred
    float curr_bal;                             //Available balance after transfer
    char tr_date[20];                           //Date of transaction
    char tr_time[20];                           //Time of transaction
}*t;

struct bank                                     //To store bank details from file to structure
{
    int acc;                                    //User Account Number
    char name[20];                              //User-name
    char pwd[20];                               //Password
    float bal;                                  //Account Balance
    int role;                                   //Check if admin or user
    int l_count;                                //Store login count
    char dateStr[9];                            //Date of unauthorised access
    char timeStr[9];                            //Date of unauthorised access
}*s;

struct report                                   //To store monthly report from file to structure
{
    int no;                                     //Account Number of unauthorised User
    char u_name[20];                            //User-name of blocked user
    int attempts;                               //No of attempts done for login
    char date[20];                              //Date of unauthorised access
    char time[20];                              //Date of unauthorised access
}*r;

/*  THIS FUNCTION TAKES IN FILENAME IN PARAMETER AND RETURNS TOTAL COUNT OF ENTRIES IN THE SPECIFIED FILE
    f   -FILE POINTER
    chr -USED TO STORE EACH CHARACTER OF A FILE
    n   -STORES COUNT OF ENTRIES
 */
int no_of_records(const char *filename)
{
    FILE *f;
    char chr;
    int n=0;

    f=fopen(filename,"r");
    for (chr=getc(f);chr!=EOF;chr=getc(f))
    {
        if (chr=='\n')
        {
            n=n+1;
        }
    }
    fclose(f);
    return n;
}

/*  THIS FUNCTION TAKES IN NUMBER OF ENTRIES TO PICK AND STORES BANK DETAILS AS A STRUCTURE ARRAY DEFINED IN GLOBAL SCOPE
    f - FILE POINTER
*/
void scan(int n)
{
    FILE *f;
    int i;

    f=fopen("bdata.txt","r");
    for(i=0;i<n;i++)
    {
        fscanf(f,"%d %s %s %f %d %d",&s[i].acc,&s[i].name,&s[i].pwd,&s[i].bal,&s[i].role,&s[i].l_count);
    }
    fclose(f);
}

/*  THIS FUNCTION TAKES IN NUMBER OF ENTRIES AUTHENTICATES USER AND AUTHORIZES USER AS ADMIN OR USER ROLE BY CALLING SEPERATE PASSWORD 
    VALIDATION FUNTIONS OF EACH ROLE 
    i       -   TO RUN A LOOP
    r_flag  -   ROLE FLAG STORES ROLE 1 FOR ADMIN 2 FOR USER
    f_count -   STORES FAILURE COUNT
    flag    -   CHECKS IF USERNAME CORRECT OR NOT
    tmp     -   STORES INDEX OF ENTRY IN ARRAY
    uname   -   STORE USENAME FROM INPUT
*/
void user_name_validation(int n)
{
    int i,r_flag=0,f_count,flag=0,tmp=0;
    char uname[20];

    do
    {
        printf("\n\tENTER LOGIN NAME: ");
        scanf("%s",&uname);

        if(strcmpi(uname,"exit")==0)
        {
            exit(0);
        }

        for(i=0;i<n;i++)
        {
            if(strcmp(s[i].name,uname)==0)
            {
                tmp=i;
                flag=1;
                if(s[i].role==2)
                {
                    r_flag=1;
                    f_count=s[i].l_count;
                }
            }
        }

        if(flag==0)
        {
            printf("\n+------------------------ INCORRECT USER NAME ----------------------------+\n");
            printf("\n\tNO RECORDS FOUND FOR THE USER NAME ENTERED\n");
            printf("\n+------------------------------------------------------------------------+\n");
        }
    }
    while(flag==0);

    if(r_flag==1)
    {
        user_pwd_validation(tmp,f_count,n);    //password validation for user
    }

    if(r_flag==0)
    {
        admin_pwd_validation(tmp,n);           //password validation for admin
    }
}

/*  THIS FUNCTION TAKES INDEX OF ENTRY OF THE CURRENT USER, FAILURE COUNT AND TOTAL NUMBER OF ENTRIES IN ARRAY,
    CHECKS IF USER IS BLOCKED AND IF PASSWORD IS CORRECT OR NOT,
    AND MAINTAINS LOGS IN A FILE WHICH IS USED LATER,
    AFTER VALIDATING SEND THE CONTROL TO USER MENU
    rem     - STORES REMAINING ATTEMPTS
    flag    - SIGNIFIES IF VALID PASSWORD ENTERED OR NOT 
    count   - STORES CURRENT FAILURE COUNT
    upwd    - STORES PASSWORD GIVEN AS INPUT
 */
void user_pwd_validation(int i,int f_count,int n)
{
    int rem,flag=0,count=0,p=0;
    char upwd[20],ch,password[55];

    if(f_count<3)
    {
        rem=2;
        do
        {
            printf("\n\tENTER PASSWORD : ");
            p=0;
            do
            {
                password[p]=getch();
                if(password[p]!='\r')
                {
                    printf("*");
                }
                p++;
            }
            while(password[p-1]!='\r');
            password[p-1]='\0';
            strcpy(upwd,password);

            if(strcmp(upwd,s[i].pwd)==0)
            {
                system("@cls||clear");
                printf("\n+------------------------ WELCOME %s ----------------------------+\n",s[i].name);
                userAction(i,n);
                flag=1;
                write_to_file(i,n,0);
            }

            if(flag==0)
            {
                system("@cls||clear");
                printf("\n+------------------------ INCORRECT PASSWORD ----------------------------+\n");
                printf("\n\t\t\t REMANING ATTEMPTS: %d\n",rem);
                printf("\n+------------------------------------------------------------------------+\n");
                rem--;
                count++;
                write_to_file(i,n,count);        //update no of login attempts made by an user
                login_attempt(i,count);        //write to monthly_report file
            }

            if(count==3)
            {
                system("@cls||clear");
                printf("\n+------------------------ INCORRECT PASSWORD ----------------------------+\n");
                printf("\n\tMAXIMUM LOGIN ATTEMPT REACHED : YOUR ACCOUNT HAS BEEN BLOCKED\n");
                printf("\n+------------------------------------------------------------------------+\n");
                write_to_file(i,n,count);        //update no of login attempts made by an user
                user_name_validation(n);         //write to monthly_report file
            }
        }
        while(count>=1 && count<=2);
    }
    else
    {
        system("@cls||clear");
        printf("\n+--------------------- ACCOUNT BLOCKED -------------------------+\n");
        printf("\n\tYOUR ACCOUNT WAS BLOCKED : REACH TO YOUR NEAREST BANK\n");
        printf("\n+---------------------------------------------------------------+\n");
        user_name_validation(n);
    }
}
/*  THIS FUNCTION TAKES INDEX OF ENTRY OF THE CURRENT USER AND TOTAL NUMBER OF ENTRIES IN ARRAY,
    CHECKS IF ADMIN PASSWORD IS CORRECT OR NOT,
    AFTER VALIDATING SEND THE CONTROL TO ADMIN MENU
    flag2    - SIGNIFIES IF VALID PASSWORD ENTERED OR NOT 
    upwd    - STORES PASSWORD GIVEN AS INPUT
 */
void admin_pwd_validation(int i,int n)
{
    int flag2=0,p=0;
    char upwd[20],password[55];

    do
    {
        printf("\n\tENTER PASSWORD: ");
        p=0;
        do
        {
            password[p]=getch();
            if(password[p]!='\r')
            {
                printf("*");
            }
            p++;
        }
        while(password[p-1]!='\r');
        password[p-1]='\0';
        strcpy(upwd,password);

        if(strcmp(upwd,s[i].pwd)==0)
        {
            system("@cls||clear");
            printf("\n+--------------------------------- WELCOME ADMIN ----------------------------------+\n\n");
            admin_authorities(n);
            flag2=1;
        }

        if(flag2==0)
        {
            system("@cls||clear");
            printf("\n+--------------- ERROR -------------------+\n");
            printf("\n\tINVALID ADMIN PASSWORD\n");
            printf("\n+--------------- ERROR -------------------+\n");
        }
    }
    while(flag2==0);
}


/*  THIS FUNCTION IS CALLED THROUGH ADMIN MENU RETURNS AUTHENTICATION LOGS
    f   - FILE POINTER POINTS TO MONTHLY LOGS FILE
    n   - STORE NUMBER OF ENTRIES IN MONTHLY LOGS 
    j   - TO RUN A LOOP
    i   - TO RUN A LOOP
    r   - DYNAMICALLY INITIALIZES REPORT STRUCTURE ARRAY
 */
void scan_monthly_report()
{
    FILE *f;
    int n=0,j=0,i=0;

    n=no_of_records("monthly_report.txt");

    r=(struct report*)malloc(n*sizeof(struct report));

    f=fopen("monthly_report.txt","r");
    for(i=0;i<n;i++)
    {
        fscanf(f,"%d %s %d %s %s",&r[i].no,&r[i].u_name,&r[i].attempts,&r[i].date,&r[i].time);
    }
    fclose(f);

    printf("\n+--------------------------- RECENT UNAUTHORISED ACCESS ---------------------------+\n\n");
    printf("\n\tACCOUNT NO\tUSER NAME\tATTEMPTS\t  DATE\t\t  TIME\n\n");

    if(i<5)
    {
        for(j=(n-1);j>=0;j--)
        {
            if(strlen(r[j].u_name)>7)
            {
                printf("\t%d\t\t%s\t%4d\t\t%s\t%s\n",r[j].no,r[j].u_name,r[j].attempts,r[j].date,r[j].time);
            }
            else
            {
                printf("\t%d\t\t%s\t\t%4d\t\t%s\t%s\n",r[j].no,r[j].u_name,r[j].attempts,r[j].date,r[j].time);
            }
        }
        printf("\n+----------------------------------------------------------------------------------+\n\n");
    }
    else
    {
        for(j=(n-1);j>(n-6);j--)
        {
            if(strlen(r[j].u_name)>7)
            {
                printf("\t%d\t\t%s\t%4d\t\t%s\t%s\n",r[j].no,r[j].u_name,r[j].attempts,r[j].date,r[j].time);
            }
            else
            {
                printf("\t%d\t\t%s\t\t%4d\t\t%s\t%s\n",r[j].no,r[j].u_name,r[j].attempts,r[j].date,r[j].time);
            }
        }
        printf("\n+----------------------------------------------------------------------------------+\n\n");
    }
}

/*  DISPLAYS USER MENU AND CALLS RELEVANT FUNCTIONS
    selection   - STORES CHOICE AS INTEGER
    choice      - STORES CHOICE AS CHARACTER
 */
void userAction(int i,int n)
{
    int selection;
    char choice;

    printf("\n");
    do{
        printf("\n+---------- SELECT AN OPTION TO PROCEED ----------+\n\n");
        printf("\t1. VIEW BALANCE\n");
        printf("\t2. FUND TRANSFER\n");
        printf("\t3. TRANSATION HISTORY\n");
        printf("\t4. LOGOUT\n");
        printf("\n+-------------------------------------------------+\n\n");

        printf("\tYOUR SELECTION: ");
        scanf(" %s",&choice);
        selection=(int)choice;
        system("@cls||clear");

        switch(selection)
        {
            case 49:
                    printf("\n+----------------- BALANCE -----------------------+\n");
                    printf("\n\tACCOUNT NO.\tBALANCE\n\n");
                    printf("\t%d\t\t%0.2f\n",s[i].acc,s[i].bal);
                    printf("\n+-------------------------------------------------+\n\n");
                    break;
            case 50:
                    printf("\n+------------------- FUND TRANSFER -----------------------+\n");
                    fundTransfer(i,n);
                    break;
            case 51:
                    transaction_history(i,n);
                    break;
            case 52:
                    system("@cls||clear");
                    printf("\n+------------------------------------------------------+\n");
                    printf("\n\tThank You %s for using our bank.\n\t\t!! Have a Nice Day !!\n",s[i].name);
                    printf("\n+------------------------------------------------------+\n");
                    user_name_validation(n);
                    break;
            default:
                    printf("\n+--------------- ERROR -------------------+\n");
                    printf("\n\tPLEASE ENTER A VALID OPTION\n");
                    printf("\n+--------------- ERROR -------------------+\n");
        }
    }
    while(1);
}

/*  THIS FUNCTION TAKES INDEX OF CURRENT USER AND TOTAL NUMBER OF ENTRIES AS PARAMETER
    TAKES ACCOUNT NUMBER AS INPUT IF USER WANT TO SEND MONEY AND VALIDATES ACCOUNT NUMBER
    AFTER SUCCESSFUL TRANSACTION SENDS CONTROL BACK TO USER MENU
    accn    - STORES INPUTED ACCOUNT NUMBER OF RECIEVER
    count   - STORES NUMBER OF DIGITS IN ACCOUNT NUMBER
    tmp     - USED TO EXTRACT NUMBER OF DIGITS
    amou    - STORES THE AMOUNT TO BE TRANSFERRED
 */
void fundTransfer(int i,int n)
{
    int accn,count=0,tmp=0;
    float amount;

    if(s[i].bal<=1)
    {
        printf("\n\tYOU DO NOT HAVE SUFFICIENT FUND TO TRANSFER\n");
        printf("\n+---------------------------------------------------------+\n");
        userAction(i,n);
    }

    printf("\n\tHi %s, please enter the details of receiver\n",s[i].name);

    printf("\n\tENTER ACCOUNT NUMBER OF RECEIVER: ");
    scanf("%d",&accn);
    tmp=accn;
    //count=(tmp==0)?1:log10(tmp)+1;
    while(tmp!=0)
    {
        tmp /= 10;     // n = n/10
        count++;
    }

    if(count!=5 || accn==0)
    {
        printf("\n+--------------- ERROR -------------------+\n");
        printf("\n\tINVALID ACCOUNT NUMBER\n");
        printf("\n+--------------- ERROR -------------------+\n");
        userAction(i,n);
    }
    else
    {
        do
        {
            printf("\n\tENTER AMOUNT TO TRANSFER: ");
            if(scanf("%c",&amount)<=0 && amount-fabs(amount)!=0)
            {
                printf("\nINVALID AMOUNT ENTERED: ENTER AGAIN");
            }
            else
            {
                scanf("%f",&amount);
            }

            if(amount<=0)
            {
                printf("\n+--------------- INVALID AMOUNT TRANSFER -------------------+\n");
                printf("\n\tRs.%0.2f AMOUNT CANNOT BE TRSNSFERED\n\tTRY AGAIN",amount);
                printf("\n+-----------------------------------------------------------+\n");
            }
        }
        while(amount<1);
        system("@cls||clear");

        if(s[i].bal<amount)
        {
            printf("\n+--------------- ERROR -------------------+\n");
            printf("\n\tINVALID FUND TRANSFER!\n\tYOUR BANALCE IS LESS THAN %0.2f\n",amount);
            printf("\n+--------------- ERROR -------------------+\n");
            userAction(i,n);
        }
        else
        {
            s[i].bal=s[i].bal-amount;
            printf("\n+--------------- ACCOUNT DETAILS -------------------+\n");
            printf("\n\tYour Account Balance is Rs. %0.2f\n",s[i].bal);
            printf("\n+---------------------------------------------------+\n");
            transaction_details(accn,amount,i);
            write_to_file(i,n,0);
        }
    }
}

/*  
    THIS FUNCTION TAKES ACCOUNT NUMBER OF RECIEVER , AMOUNT AND INDEX OF SENDER AND WRITES TRANSACTION DETAILS TO FILE
    fptr        - FILE POINTER POINTING TO TRANSACTION DETAILS FILE
    acc_no      - STORES THE ACCOUNT NUMBER OF SENDER
    time        - STORES CURRENT TIME
    date        - STORES CURRENT DATE
    tmp         - STORES CONVERTED ACCOUNT NUMBER FROM INTEGER TO STRING
    filename    - STORES THE FILENAME IN WHICH TRANSACTION TO BE SAVED
*/
void transaction_details(int accn,float amount,int i)
{
    FILE *fptr;

    int acc_no;
    char time[20],date[20],tmp[10],filename[30];

    acc_no=s[i].acc;
    //itoa(acc_no,tmp,10);
    tostring(tmp,acc_no);

    _strdate(date);
    _strtime(time);

    strcpy(filename,s[i].name);
    strcat(filename,"_");
    strcat(filename,tmp);
    strcat(filename,".txt");

    fptr=fopen(filename,"a+");

    fprintf(fptr,"%d %0.2f %0.2f %s %s\n",accn,amount,s[i].bal,date,time);

    fclose(fptr);
}

/*  THIS FUNCTION TAKES INDEX OF CURRENT USER AND TOTAL NUMBER OF USERS AS PARAMETERS, READS TRANSACTION HISTORY AND DISPLAYS IT
    f           -  FILE POINTER POINTS TO TRANSACTIONS FILE OF USER
    filename    -  STORES THE FILENAME 
    tmp         -  STORES CONVERTED ACCOUNT NUMBER FROM INTEGER TO STRING
    n           -  STORES NUMBER OF RECORDS
    j           -  TO RUN A LOOP
    acc_no      -  STORES ACCOUNT NUMBER OF USER
    t           -  INITIALIZES TRANSACTION STRUCTURE DYNAMICALLY
*/
void transaction_history(int i,int q)
{
    FILE *f;
    char chr,filename[30],tmp[10];
    int n=0,j=0,acc_no;

    acc_no=s[i].acc;
    //itoa(acc_no,tmp,10);
    tostring(tmp,acc_no);

    strcpy(filename,s[i].name);
    strcat(filename,"_");
    strcat(filename,tmp);
    strcat(filename,".txt");

    f=fopen(filename,"r");
    if(f!=NULL)
    {
        n=no_of_records(filename);

        if(n==0)
        {
            printf("\n+--------------- INVALID REQUEST -------------------+\n");
            printf("\n\tNO TRANSACTIONS MADE YET\n");
            printf("\n+---------------------------------------------------+\n");
            userAction(i,q);
        }
    }
    else
    {
        printf("\n+--------------- INVALID REQUEST -------------------+\n");
        printf("\n\tNO TRANSACTIONS MADE YET\n");
        printf("\n+---------------------------------------------------+\n");
        userAction(i,q);
    }
    fclose(f);

    t=(struct transaction*)malloc(n*sizeof(struct transaction));

    f=fopen(filename,"r");
    printf("\n+---------------------------------- TRANSACTION HISTORY ------------------------------------------+\n");
    printf("\n\tTRANSFERRED TO\t  AMOUNT TRANSFERRED\tCURRENT BALANCE\t\t  DATE\t\t  TIME\n\n");
    for(j=0;j<n;j++)
    {
        fscanf(f,"%d %f %f %s %s",&t[j].to_account,&t[j].amt_transferred,&t[j].curr_bal,&t[j].tr_date,&t[j].tr_time);
        printf("\t%d\t\t\t%0.2f\t\t%0.2f\t\t%s\t%s\n",t[j].to_account,t[j].amt_transferred,t[j].curr_bal,t[j].tr_date,t[j].tr_time);
    }
    printf("\n+-------------------------------------------------------------------------------------------------+\n");
    fclose(f);
}

/*  THIS FUNCTION TAKES TOTAL NUMBER OF ENTRIES IN BANK DETAILS FILE IN PARAMETERS, DISPLAYS ADMIN MENU AND CALLS RELEVENT FUNCTIONS
    i           - USED TO RUN A LOOP
    selection   - STORE ENTERED CHOICE AS A NUMBER
    t           - STORES NUMBER OF ENTRIES IN MONTHLY REPORT
    choice      - STORES ENTERED CHOICE AS A CHARACTER
*/
void admin_authorities(int n)
{
    int i,selection,t=0;
    char choice;

    scan_monthly_report();

    printf("\n");
    do{
        printf("\n+------------ SELECT AN OPTION TO PROCEED -----------+\n\n");
        printf("\t1. VIEW BLOCKED USER\n");
        printf("\t2. UNBLOCK USER\n");
        printf("\t3. VIEW MONTHLY REPORT OF UNAUTHORISED ACCESS\n");
        printf("\t4. LOGOUT\n");
        printf("\n+----------------------------------------------------+\n\n");

        printf("\tYOUR SELECTION: ");
        scanf(" %s",&choice);
        selection=(int)choice;
        system("@cls||clear");

        switch(selection)
        {
            case 49:
                    printf("\n+--------------- LIST OF BLOCKED USER --------------------+\n");
                    disp_blocked_users(n);
                    printf("\n+------------------------------------------------------+\n\n");
                    break;
            case 50:
                    unblock_user(n);
                    break;
            case 51:
                    printf("\n+---------------------------------- MONTHLY REPORT ----------------------------------+\n");
                    printf("\n\tACCOUNT NO.\tUSER NAME\tATTEMPTS\t  DATE\t\t  TIME\n\n");
                    t=no_of_records("monthly_report.txt");
                    for(i=0;i<t;i++)
                    {
                        if(strlen(r[i].u_name)>7)
                        {
                            printf("\t%d\t\t%s\t%4d\t\t%s\t%s\n",r[i].no,r[i].u_name,r[i].attempts,r[i].date,r[i].time);
                        }
                        else
                        {
                            printf("\t%d\t\t%s\t\t%4d\t\t%s\t%s\n",r[i].no,r[i].u_name,r[i].attempts,r[i].date,r[i].time);
                        }
                    }
                    printf("\n+------------------------------------------------------------------------------------+\n\n");
                    break;
            case 52:
                    system("@cls||clear");
                    printf("\n+----------------------- !! HAVE A NICE DAY !! ----------------------------+\n\n");
                    printf("\t\t\tADMIN LOGOUT SUCESSFULL\n");
                    printf("\n+------------------------------------------------------------------------+\n\n");
                    user_name_validation(n);
                    break;
            default:
                    printf("\n+--------------- ERROR -------------------+\n");
                    printf("\n\tPLEASE ENTER A VALID OPTION\n");
                    printf("\n+--------------- ERROR -------------------+\n");
        }
    }
    while(1);
}

/*  THIS FUNCTION TAKES TOTAL NUMBER OF ENTRIES IN BANK DETAILS FILE IN PARAMETERS, CHECKS ROLE AND LOGIN FAILURE COUNT 
    AND DISPLAYS BLOCKED USERS
    i       - USED TO RUN A LOOP
    flag    - SHOWES IF BLOCKED ACCOUNT FOUND OR NOT 
*/
void disp_blocked_users(int n)
{
	int i,flag=0;

    for(i=0;i<n;i++)
    {
        if((s[i].role==2) && (s[i].l_count==3))
        {
            if(flag==0)
            {
                printf("\n\n\tACCOUNT NO.\tUSER NAME\tATTEMPTS\n\n");
            }
            if(strlen(s[i].name)>7)
            {
                printf("\t%d\t\t%s\t%4d\n",s[i].acc,s[i].name,s[i].l_count);
            }
            else
            {
                printf("\t%d\t\t%s\t\t%4d\n",s[i].acc,s[i].name,s[i].l_count);
            }
            flag=1;
        }
    }

    if(flag==0)
    {
        printf("\n\t\tNo BLOCKED USER FOUND\n");
    }
}

/*  THIS FUNCTION TAKES TOTAL NUMBER OF ENTRIES IN BANK DETAILS FILE IN PARAMETERS, CHECKS IF USER BLOCKED, IF BLOCKED, INPUTS ACCOUNT NUMBER AND UNBLOCKS IF VALID
    i       - USED TO RUN A LOOP
    acc_no  - USED TO INPUT ACCOUNT NUMBER OF BLOCKED USER
    flag    - SIGNIFIES IF ANY BLOCKED USER FOUND
    flag1   - SIGNIFIES IF ENTERED ACCOUNT NUMBER MATCHED OR NOT
*/
void unblock_user(int n)
{
    int i,acc_no,flag=0,flag1=0;

    for(i=0;i<n;i++)
    {
        if(s[i].role==2 && s[i].l_count==3)
        {
            flag=1;
            break;
        }
    }

    if(flag==0)
    {
        printf("\n+--------------- INVALID REQUEST -------------------+\n");
        printf("\n\tNo BLOCKED USER FOUND\n");
        printf("\n+---------------------------------------------------+\n");
        flag1=1;
    }

    if(flag==1)
    {
        printf("\n+--------------------------- ENTER DETAILS TO UNBLOCK ---------------------------+\n");
        printf("\n\tENTER ACCOUNT NUMBER TO UNBLOCK: ");
        scanf("%d",&acc_no);
        printf("\n+--------------------------------------------------------------------------------+\n");

        for(i=0;i<n;i++)
        {
            if(s[i].acc==acc_no)
            {
                flag1=1;
                s[i].l_count=0;
                printf("\n+--------------- UNBLOCKED USER -------------------+\n");
                printf("\n\tUSER NAME\tACCOUNT NO.\tSTATUS\n\n");
                printf("\t%s\t\t%d\t\tUNBLOCKED\n",s[i].name,s[i].acc);
                printf("\n+--------------------------------------------------+\n");
                write_to_file(i,n,0);
            }
        }
    }

    if(flag1==0)
    {
        printf("\n+--------------- ERROR -------------------+\n");
        printf("\n\tINVALID ACCOUNT NUMBER\n");
        printf("\n+--------------- ERROR -------------------+\n");
    }
}

/* THIS FUNCTION TAKES INDEX OF ENTRY AND NUMBER OF FAILED ATTEMPTS AS PARAMETERS AND WRITES IT TO MONTHLY REPORT FILE */
void login_attempt(int i,int count)
{
    FILE *fptr;

    fptr = fopen("monthly_report.txt","a+");

    s[i].l_count=count;

    _strdate(s[i].dateStr);
    _strtime(s[i].timeStr);
    fprintf(fptr,"%d %s %d %s %s\n",s[i].acc,s[i].name,s[i].l_count,s[i].dateStr,s[i].timeStr);

    fclose(fptr);
}

/* THIS FUNCTION TAKES INDEX OF ENTRY, TOTAL NUMBER OF ENTRIES AND NUMBER OF FAILED ATTEMPT AS PARAMETERS AND WRITES IT TO BANK DETAILS FILE TO BLOCK THEIR ACCESS */
void write_to_file(int i,int n,int count)
{
    FILE *fptr;
    int j;

    //open for writing
    fptr = fopen("bdata.txt","w");

    if (fptr==NULL)
    {
        printf("\n+--------------- ERROR -------------------+\n");
        printf("\n\tFILE DOES NOT EXIST\n");
        printf("\n+--------------- ERROR -------------------+\n");
        return;
    }

    s[i].l_count=count;

    for(j=0;j<n;j++)
    {
        fprintf(fptr,"%d %s %s %0.2f %d %d\n",s[j].acc,s[j].name,s[j].pwd,s[j].bal,s[j].role,s[j].l_count);
    }
    fclose(fptr);
}

/* THIS FUNCTION TAKES STRING POINTER AND INTEGER NUMBER AND CONVERTS THAT NUMBER TO STRING*/
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

int main()
{
    int n=0;

    n=no_of_records("bdata.txt");                                   //No. of records available in the file

    s=(struct bank*)malloc(n*sizeof(struct bank));                  //dymanic memory allocation

    scan(n);

    printf("\n+--------------------------------------- WELCOME ---------------------------------------+\n");                                   //read the file and storing it in an structure array

    user_name_validation(n);                                        //user-name validation

    return 0;
}

