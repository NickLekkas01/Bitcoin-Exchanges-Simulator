#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"


int main(int argc, char *argv[])
{
    char bitCoinBalancesFile[50], transactionFile[50];
    int bitCoinValue, senderHashtableNumOfEntries, receiverHashtableNumOfEntries, bucketSize;
    printf("ARGC: %d\n",argc);
    readArguments(argc, argv, &bitCoinValue, &senderHashtableNumOfEntries, &receiverHashtableNumOfEntries, &bucketSize, bitCoinBalancesFile, transactionFile);

    FILE *fp1, *fp2;
    
    fp1 = fopen(bitCoinBalancesFile, "r");
    if(fp1 == NULL)
        exit(-1);
    
    fp2 = fopen(transactionFile, "r");
    if(fp2 == NULL)
        exit(-1);
    
    char *buffer = NULL;
    size_t size = 0;
    char *token;

    BitcoinHashTable MyBitcoinHashTable;
    int BitcoinHashTableBuckets = senderHashtableNumOfEntries;
    int int_index, string_index;
    MyBitcoinHashTable.startingBitcoin = malloc(BitcoinHashTableBuckets * sizeof(BitcoinNode *));
    for(int i = 0; i < BitcoinHashTableBuckets; ++i)
        MyBitcoinHashTable.startingBitcoin[i] = NULL;

    WalletHashTable MyWalletHashTable;
    int WalletHashTableBuckets = senderHashtableNumOfEntries;
    MyWalletHashTable.startingWallet = malloc(WalletHashTableBuckets*sizeof(WalletNode));
    for(int i = 0; i < WalletHashTableBuckets; ++i)
        MyWalletHashTable.startingWallet[i] = NULL;
    
    char name[50];
    int flag = 0;
    int flag2 = 0;
    
    TreePointerNode *tempTreePointerNode = malloc(sizeof(TreePointerNode));
    while(getline(&buffer, &size, fp1) != -1)
    {
        printf("%s",buffer);
        token = strtok(buffer, " ");
        strcpy(name, token);
        if(name[strlen(name)-1] == '\n')
            name[strlen(name)-1] = '\0';
        string_index = hash_string(name, WalletHashTableBuckets);
        
        InsertWalletAtStart(&MyWalletHashTable.startingWallet[string_index], name);
        //printf("NAME: %s\n",token);
        while(token != NULL)
        {
            token = strtok(NULL, " ");
            
            if( token == NULL || strcmp(token, " ") == 0 || (strcmp(token, "") == 0))
            {printf("\n\n\n(%s)\n\n\n",name);    break;
            }
            if(BitcoinExists(MyBitcoinHashTable, atoi(token), BitcoinHashTableBuckets) == 1)
            {
                printf("(%d) Bitcoin should not exists more than once. Exiting program\n",atoi(token));
                exit(-1);
            }
            int_index = hash_integer(atoi(token), BitcoinHashTableBuckets);
            InsertBitcoinNodeAtStart(&MyBitcoinHashTable.startingBitcoin[int_index], atoi(token), name, bitCoinValue);
            
            printf("Name: (%s) Bitcoin %d inserted\n",name , atoi(token));
            InsertBitcoinAtStart(&MyWalletHashTable.startingWallet[string_index]->startingBitcoin, atoi(token));
            
            InsertTreePointerNodeAtStart(&MyWalletHashTable.startingWallet[string_index]->startingBitcoin->startingLeafNode, MyBitcoinHashTable.startingBitcoin[int_index]->root);
            
            UpdateBalance(&MyWalletHashTable.startingWallet[string_index], name, bitCoinValue);
            
            //printf("(%s) ",token);
        }
        //printf("\n");
    }
    
    printf("\n");
    
    MyHashTable SenderHashTable, ReceiverHashTable;
    TransactionNode *start = NULL;
    int BucketSize = ((bucketSize - sizeof(Bucket *)) - sizeof(int)) / sizeof(BucketData); 
    
    SenderHashTable.HashTable = malloc(senderHashtableNumOfEntries * sizeof(Bucket *));
    for(int i = 0; i < senderHashtableNumOfEntries; i++)
    {
        SenderHashTable.HashTable[i] = NULL;
    }
    
    ReceiverHashTable.HashTable = malloc(receiverHashtableNumOfEntries * sizeof(Bucket *));
    for(int i = 0; i < receiverHashtableNumOfEntries; i++)
    {
        ReceiverHashTable.HashTable[i] = NULL;
    }
    int sender_index, receiver_index;
    char id[50], sender[50], receiver[50];
    int value;
    TransactionNode *tempTransaction;
    Date *date = malloc(sizeof(Date));;
    Time *ti = malloc(sizeof(Time));
    Date *date2 = malloc(sizeof(Date));;
    Time *ti2 = malloc(sizeof(Time));
    int lastid;
    Date lastdate;
    Time lasttime;
    char lastid_string[50];
    lastdate.day = 0;
    lastdate.month = 0;
    lastdate.year = 0;
    lasttime.hours = 0;
    lasttime.minutes = 0;
    while(getline(&buffer, &size, fp2) != -1)
    {
        printf("%s",buffer);
        token = strtok(buffer, " ");
        //if(token[strlen(token)-1] == '\n')
        //    token[strlen(token)-1] = '\0';
        strcpy(id, token);
        //printf("ID: %s\n",token);
        token = strtok(NULL, " ");
        sender_index = hash_string(token, senderHashtableNumOfEntries);
        strcpy(sender, token);
        //printf("Sender: %s\n",token);
        token = strtok(NULL, " ");
        receiver_index = hash_string(token, receiverHashtableNumOfEntries);
        strcpy(receiver, token);
        
        if(strcmp(sender, receiver) == 0)
            continue;
        
        
        //printf("Receiver: %s\n",token);
        token = strtok(NULL, " ");
        value = atoi(token);
        //printf("Money to give: %d\n",value);
        
        token = strtok(NULL, "-");
        date->day = atoi(token);
        //printf("Day: %d\n",atoi(token));
        token = strtok(NULL, "-");
        date->month = atoi(token);
        //printf("Month: %d\n",atoi(token));
        token = strtok(NULL, " ");
        date->year = atoi(token);
        //printf("Year: %d\n",atoi(token));
        
        token = strtok(NULL, ":");
        ti->hours = atoi(token);;
        //printf("Hours: %d\n",atoi(token));
        token = strtok(NULL, " ");
        // if( token[strlen(token)-1] == '\n')
        //     token[strlen(token)-1] = '\0';
        ti->minutes = atoi(token);
        //printf("Minutes: %d\n",atoi(token));

        if(PersonExists(SenderHashTable.HashTable[sender_index], sender, BucketSize) == 0)
            InsertPerson(&SenderHashTable.HashTable[sender_index], sender, BucketSize);
        if(PersonExists(ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize) == 0)
            InsertPerson(&ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize);

        tempTransaction = InsertTransactionAtStart(&start, &MyWalletHashTable, id, sender, sender_index, receiver, receiver_index, value, date, ti);
        InsertSenderandReceiverTransactionPointer(&SenderHashTable.HashTable[sender_index], &ReceiverHashTable.HashTable[receiver_index], sender, receiver, tempTransaction, BucketSize);
        flag = 0;
        if(date->year == lastdate.year)
        {
            if(date->month == lastdate.month)
            {
                if(date->day == lastdate.day)
                {
                    if(ti->hours == lasttime.hours)
                    {
                        if(ti->minutes > lasttime.minutes)
                            flag = 1;
                    }
                    else if(ti->hours > lasttime.hours)
                        flag = 1;
                }
                else if(date->day > lastdate.day)
                    flag = 1;
            }
            else if(date->month > lastdate.month)
                flag = 1;
        }
        else if(date->year > lastdate.year)
            flag = 1;

        if(flag == 1)
        {
            lastdate.day = date->day;
            lastdate.month = date->month;
            lastdate.year = date->year;
            lasttime.hours = ti->hours;
            lasttime.minutes = ti->minutes;
        }
        flag = 0;
        
        //printf("\n");
    }
    PrintWallets(&MyWalletHashTable, WalletHashTableBuckets);
    fflush(stdout);
    time_t t = time(NULL);
    struct tm tm ;
    FILE *fp;
    char *line;
    size_t s=0;
    flag = 0;
    flag2 = 0;
    while(getline(&buffer, &size, stdin) != -1)
    {  
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, " ");
        if(strcmp(token, "/requestTransaction") == 0 || strcmp(buffer, "/requestTransaction") == 0)
        {
            token = strtok(NULL, " ");
            sender_index = hash_string(token, senderHashtableNumOfEntries);
            strcpy(sender, token);
            printf("(%s)\n",sender);

            token = strtok(NULL, " ");
            
            receiver_index = hash_string(token, receiverHashtableNumOfEntries);
            strcpy(receiver, token);  
            printf("(%s)\n",receiver);

            if(strcmp(sender, receiver) == 0)
                continue;

            
            token = strtok(NULL, " ");
            value = atoi(token);
            
            token = strtok(NULL, "-");            
            if(token == NULL || strcmp(buffer, "")==0)
            {
                tm = *localtime(&t);
                date->day = tm.tm_mday;
                date->month = tm.tm_mon + 1;
                date->year = tm.tm_year + 1900;
                ti->hours = tm.tm_hour;
                ti->minutes = tm.tm_min;
            }
            else
            {
                date->day = atoi(token);
                token = strtok(NULL, "-");
                date->month = atoi(token);
                token = strtok(NULL, " ");
                date->year = atoi(token);
        
                token = strtok(NULL, ":");
                ti->hours = atoi(token);;
                token = strtok(NULL, " ");
                ti->minutes = atoi(token);
            }
            
                    
            
            if(date->year == lastdate.year)
            {
                if(date->month == lastdate.month)
                {
                    if(date->day == lastdate.day)
                    {
                        if(ti->hours == lasttime.hours)
                        {
                            if(ti->minutes <= lasttime.minutes)
                                continue;
                        }
                        else if(ti->hours < lasttime.hours)
                            continue;
                    }
                    else if(date->day < lastdate.day)
                        continue;
                }
                else if(date->month < lastdate.month)
                    continue;
            }
            else if(date->year < lastdate.year)
                continue;
            
            if(PersonExists(SenderHashTable.HashTable[sender_index], sender, BucketSize) == 0)
                InsertPerson(&SenderHashTable.HashTable[sender_index], sender, BucketSize);
            if(PersonExists(ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize) == 0)
                InsertPerson(&ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize);
                
            sprintf(lastid_string, "%d",++lastid);
            tempTransaction = InsertTransactionAtStart(&start, &MyWalletHashTable, lastid_string, sender, sender_index, receiver, receiver_index, value, date, ti);
            InsertSenderandReceiverTransactionPointer(&SenderHashTable.HashTable[sender_index], &ReceiverHashTable.HashTable[receiver_index], sender, receiver, tempTransaction, BucketSize);
            
            lastdate.day = date->day;
            lastdate.month = date->month;
            lastdate.year = date->year;
            lasttime.hours = ti->hours;
            lasttime.minutes = ti->minutes;

        }
        else if(strcmp(token, "/requestTransactions") == 0 || strcmp(buffer, "/requestTransactions") == 0)
        {
            flag = 0;
            flag2 = 0;
            while(1)
            {
               
                token = strtok(NULL, " ");
                if(token == NULL || strcmp(token, "")==0)
                    break;
                sender_index = hash_string(token, senderHashtableNumOfEntries);
                strcpy(sender, token);
                printf("(%s)\n",sender);

                token = strtok(NULL, " ");
                if(token == NULL)
                {
                    printf("file(%s)\n",sender);
                    /*String sender will have the name of the file*/
                    fp = fopen(sender, "r");
                    if(fp == NULL)
                        exit(-1);
                    
                    flag = 1;
                    getline(&line, &s, fp);
                    if(line[strlen(line)-1] == '\n')
                        line[strlen(line)-1] = '\0';
                    printf("%s\n", line);
                    token = strtok(line, " ");
                    strcpy(sender, token);
                    printf("(%s)\n",sender);
                    sender_index = hash_string(sender, senderHashtableNumOfEntries);

                    token = strtok(NULL, " ");
                }

                strcpy(receiver, token);  
                receiver_index = hash_string(receiver, receiverHashtableNumOfEntries);
                printf("(%s)\n",receiver);

                if(strcmp(sender, receiver) == 0)
                    continue;

                
                token = strtok(NULL, " ");
                value = atoi(token);
                
                token = strtok(NULL, "-");            
                if(token == NULL || strcmp(buffer, "")==0)
                {
                    tm = *localtime(&t);
                    date->day = tm.tm_mday;
                    date->month = tm.tm_mon + 1;
                    date->year = tm.tm_year + 1900;
                    ti->hours = tm.tm_hour;
                    ti->minutes = tm.tm_min;
                }
                else
                {
                    date->day = atoi(token);
                    token = strtok(NULL, "-");
                    date->month = atoi(token);
                    token = strtok(NULL, " ");
                    date->year = atoi(token);
            
                    token = strtok(NULL, ":");
                    ti->hours = atoi(token);;
                    token = strtok(NULL, ";");
                    ti->minutes = atoi(token);
                }
                
                
                if(date->year == lastdate.year)
                {
                    if(date->month == lastdate.month)
                    {
                        if(date->day == lastdate.day)
                        {
                            if(ti->hours == lasttime.hours)
                            {
                                if(ti->minutes <= lasttime.minutes)
                                    continue;
                            }
                            else if(ti->hours < lasttime.hours)
                                continue;
                        }
                        else if(date->day < lastdate.day)
                            continue;
                    }
                    else if(date->month < lastdate.month)
                        continue;
                }
                else if(date->year < lastdate.year)
                    continue;
                
                if(PersonExists(SenderHashTable.HashTable[sender_index], sender, BucketSize) == 0)
                    InsertPerson(&SenderHashTable.HashTable[sender_index], sender, BucketSize);
                if(PersonExists(ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize) == 0)
                    InsertPerson(&ReceiverHashTable.HashTable[receiver_index], receiver, BucketSize);
                
                sprintf(lastid_string, "%d",++lastid);
                tempTransaction = InsertTransactionAtStart(&start, &MyWalletHashTable, lastid_string, sender, sender_index, receiver, receiver_index, value, date, ti);
                InsertSenderandReceiverTransactionPointer(&SenderHashTable.HashTable[sender_index], &ReceiverHashTable.HashTable[receiver_index], sender, receiver, tempTransaction, BucketSize);
                
                lastdate.day = date->day;
                lastdate.month = date->month;
                lastdate.year = date->year;
                lasttime.hours = ti->hours;
                lasttime.minutes = ti->minutes;
            }
            /*If I opened a file, close it */
            if(flag == 1) 
                fclose(fp);
        }
        else if(strcmp(token, "/findEarnings") == 0 || strcmp(buffer, "/findEarnings") == 0)
        {
            int sum = 0;
            int local_sum = 0;
            flag = 0;
            token = strtok(NULL, " ");
            strcpy(name, token);
            if(name[strlen(name)-1] == '\n')
                name[strlen(name)-1] = '\0';
            token = strtok(NULL, ":");
            if(token == NULL)
            {
                string_index = hash_string(name, receiverHashtableNumOfEntries);
                Bucket *tempBucket = ReceiverHashTable.HashTable[string_index];
                while(tempBucket != NULL)
                {
                    for(int i = 0; i < BucketSize; ++i)
                    {
                        if(tempBucket->Person[i] != NULL && strcmp(tempBucket->Person[i]->walletID, name) == 0)
                        {
                            TransactionPointerNode *tempTransactionPointer = tempBucket->Person[i]->startingTransaction;
                            while(tempTransactionPointer != NULL)
                            {
                                if(tempTransactionPointer->transPointer != NULL && tempTransactionPointer->transPointer->startingTreePointer != NULL && strcmp(tempTransactionPointer->transPointer->startingTreePointer->treePointer->leftNode->walletID, name)==0)
                                {
                                    TreePointerNode *tempTreePointer = tempTransactionPointer->transPointer->startingTreePointer;
                                    local_sum = 0;
                                    while(tempTreePointer != NULL)
                                    {
                                        sum += tempTreePointer->treePointer->leftNode->value;
                                        printf("%s %s %s %d %02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionID, tempTreePointer->treePointer->rightNode->walletID, tempTreePointer->treePointer->leftNode->walletID, tempTreePointer->treePointer->leftNode->value, tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                        tempTreePointer = tempTreePointer->next;
                                    }
                                    printf("%s %s %s ", tempTransactionPointer->transPointer->transaction.transactionID, tempTransactionPointer->transPointer->startingTreePointer->treePointer->rightNode->walletID, tempTransactionPointer->transPointer->startingTreePointer->treePointer->leftNode->walletID); 
                                    printf("%d ", local_sum);
                                    printf("%02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                    sum += local_sum;
                                    
                                }
                                tempTransactionPointer = tempTransactionPointer->next;
                            }
                            break;
                            flag = 1;
                        }
                    }
                    if(flag == 1)
                        break;
                    tempBucket = tempBucket->next;
                }
                printf("Total Earnings: %d\n",sum);                
            }
            else
            {
                ti->hours = atoi(token);
                token = strtok(NULL, " ");
                ti->minutes = atoi(token);
                token = strtok(NULL, "-");
                date->day = atoi(token);
                token = strtok(NULL, "-");
                date->month = atoi(token);
                token = strtok(NULL, " ");
                date->year = atoi(token);

                token = strtok(NULL, ":");
                if(token == NULL)
                    continue;
                else
                {                    
                    ti2->hours = atoi(token);
                    token = strtok(NULL, " ");
                    ti2->minutes = atoi(token);
                    token = strtok(NULL, "-");
                    date2->day = atoi(token);
                    token = strtok(NULL, "-");
                    date2->month = atoi(token);
                    token = strtok(NULL, " ");
                    date2->year = atoi(token);
                }

                string_index = hash_string(name, receiverHashtableNumOfEntries);
                Bucket *tempBucket = ReceiverHashTable.HashTable[string_index];
                while(tempBucket != NULL)
                {
                    for(int i = 0; i < BucketSize; ++i)
                    {
                        if(tempBucket->Person[i] != NULL && strcmp(tempBucket->Person[i]->walletID, name) == 0)
                        {
                            TransactionPointerNode *tempTransactionPointer = tempBucket->Person[i]->startingTransaction;
                            while(tempTransactionPointer != NULL)
                            {
                                if(strcmp(tempTransactionPointer->transPointer->startingTreePointer->treePointer->leftNode->walletID, name)==0)
                                {
                                    TreePointerNode *tempTreePointer = tempTransactionPointer->transPointer->startingTreePointer;
                                    local_sum = 0;
                                    while(tempTreePointer != NULL)
                                    {
                                        local_sum += tempTreePointer->treePointer->leftNode->value;
                                        tempTreePointer = tempTreePointer->next;
                                    }
                                    sum += local_sum;
                                
                                    /*check if it is bigger than time1, year1*/
                                    if(date->year == tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        if(date->month == tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            if(date->day == tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            { 
                                                if(ti->hours == tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    if(ti->minutes > tempTransactionPointer->transPointer->transaction.transactionTime.minutes)
                                                    {
                                                        break;
                                                    }
                                                }
                                                else if(ti->hours > tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    break;
                                                }
                                            }
                                            else if(date->day > tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            {
                                                break;
                                            }
                                        }
                                        else if(date->month > tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            break;
                                        }
                                    }
                                    else if(date->year > tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        break;
                                    }
                                    /*check if it is smaller than time2, year2*/
                                    if(date2->year == tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        if(date2->month == tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            if(date2->day == tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            { 
                                                if(ti2->hours == tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    if(ti2->minutes < tempTransactionPointer->transPointer->transaction.transactionTime.minutes)
                                                    {
                                                        break;
                                                    }
                                                }
                                                else if(ti2->hours < tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    break;
                                                }
                                            }
                                            else if(date2->day < tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            {
                                                break;
                                            }
                                        }
                                        else if(date2->month < tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            break;
                                        }
                                    }
                                    else if(date2->year < tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        break;
                                    }

                                    printf("%s %s %s %d %02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionID, tempTreePointer->treePointer->rightNode->walletID, tempTreePointer->treePointer->leftNode->walletID, local_sum, tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                }
                                tempTransaction = tempTransaction->next;
                            }
                            break;
                            flag = 1;
                        }
                    }
                    if(flag == 1)
                        break;
                    tempBucket = tempBucket->next;
                }
                printf("Total Earnings: %d\n",sum);                
                
            }
            

        }
        else if(strcmp(token, "/findPayments") == 0 || strcmp(buffer, "/findPayments") == 0)
        {
            int sum = 0;
            int local_sum;
            flag = 0;
            token = strtok(NULL, " ");
            strcpy(name, token);
            if(name[strlen(name)-1] == '\n')
                name[strlen(name)-1] = '\0';
            token = strtok(NULL, ":");
            if(token == NULL)
            {
                string_index = hash_string(name, senderHashtableNumOfEntries);
                Bucket *tempBucket = SenderHashTable.HashTable[string_index];
                while(tempBucket != NULL)
                {
                    for(int i = 0; i < BucketSize; ++i)
                    {
                        if(tempBucket->Person[i] != NULL && strcmp(tempBucket->Person[i]->walletID, name) == 0)
                        {
                            TransactionPointerNode *tempTransactionPointer = tempBucket->Person[i]->startingTransaction;
                            while(tempTransactionPointer != NULL)
                            {
                                if(tempTransactionPointer->transPointer != NULL && tempTransactionPointer->transPointer->startingTreePointer != NULL && strcmp(tempTransactionPointer->transPointer->startingTreePointer->treePointer->walletID, name)==0)
                                {
                                    TreePointerNode *tempTreePointer = tempTransactionPointer->transPointer->startingTreePointer;
                                    local_sum = 0;
                                    while(tempTreePointer != NULL)
                                    {
                                        sum += tempTreePointer->treePointer->leftNode->value;
                                        printf("%s %s %s %d %02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionID, tempTreePointer->treePointer->rightNode->walletID, tempTreePointer->treePointer->leftNode->walletID, tempTreePointer->treePointer->leftNode->value, tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                        tempTreePointer = tempTreePointer->next;
                                    }
                                    printf("%s %s %s ", tempTransactionPointer->transPointer->transaction.transactionID, tempTransactionPointer->transPointer->startingTreePointer->treePointer->rightNode->walletID, tempTransactionPointer->transPointer->startingTreePointer->treePointer->leftNode->walletID); 
                                    printf("%d ", local_sum);
                                    printf("%02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                    sum += local_sum;
                                }
                                tempTransactionPointer = tempTransactionPointer->next;
                            }
                            break;
                            flag = 1;
                        }
                    }
                    if(flag == 1)
                        break;
                    tempBucket = tempBucket->next;
                }
                printf("Total Payments: %d\n",sum);
            }
            else
            {
                ti->hours = atoi(token);
                token = strtok(NULL, " ");
                ti->minutes = atoi(token);
                token = strtok(NULL, "-");
                date->day = atoi(token);
                token = strtok(NULL, "-");
                date->month = atoi(token);
                token = strtok(NULL, " ");
                date->year = atoi(token);

                token = strtok(NULL, ":");
                if(token == NULL)
                    continue;
                else
                {                    
                    ti2->hours = atoi(token);
                    token = strtok(NULL, " ");
                    ti2->minutes = atoi(token);
                    token = strtok(NULL, "-");
                    date2->day = atoi(token);
                    token = strtok(NULL, "-");
                    date2->month = atoi(token);
                    token = strtok(NULL, " ");
                    date2->year = atoi(token);
                }

                string_index = hash_string(name, senderHashtableNumOfEntries);
                Bucket *tempBucket = SenderHashTable.HashTable[string_index];
                while(tempBucket != NULL)
                {
                    for(int i = 0; i < BucketSize; ++i)
                    {
                        if(tempBucket->Person[i] != NULL && strcmp(tempBucket->Person[i]->walletID, name) == 0)
                        {
                            TransactionPointerNode *tempTransactionPointer = tempBucket->Person[i]->startingTransaction;
                            while(tempTransactionPointer != NULL)
                            {
                                if(strcmp(tempTransactionPointer->transPointer->startingTreePointer->treePointer->walletID, name)==0)
                                {
                                    TreePointerNode *tempTreePointer = tempTransactionPointer->transPointer->startingTreePointer;
                                    local_sum = 0;
                                    while(tempTreePointer != NULL)
                                    {
                                        local_sum += tempTreePointer->treePointer->leftNode->value;
                                        tempTreePointer = tempTreePointer->next;
                                    }
                                    sum += local_sum;
                                
                                    /*check if it is bigger than time1, year1*/
                                    if(date->year == tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        if(date->month == tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            if(date->day == tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            { 
                                                if(ti->hours == tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    if(ti->minutes > tempTransactionPointer->transPointer->transaction.transactionTime.minutes)
                                                    {
                                                        break;
                                                    }
                                                }
                                                else if(ti->hours > tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    break;
                                                }
                                            }
                                            else if(date->day > tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            {
                                                break;
                                            }
                                        }
                                        else if(date->month > tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            break;
                                        }
                                    }
                                    else if(date->year > tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        break;
                                    }
                                    /*check if it is smaller than time2, year2*/
                                    if(date2->year == tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        if(date2->month == tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            if(date2->day == tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            { 
                                                if(ti2->hours == tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    if(ti2->minutes < tempTransactionPointer->transPointer->transaction.transactionTime.minutes)
                                                    {
                                                        break;
                                                    }
                                                }
                                                else if(ti2->hours < tempTransactionPointer->transPointer->transaction.transactionTime.hours)
                                                {
                                                    break;
                                                }
                                            }
                                            else if(date2->day < tempTransactionPointer->transPointer->transaction.transactionDate.day)
                                            {
                                                break;
                                            }
                                        }
                                        else if(date2->month < tempTransactionPointer->transPointer->transaction.transactionDate.month)
                                        {
                                            break;
                                        }
                                    }
                                    else if(date2->year < tempTransactionPointer->transPointer->transaction.transactionDate.year)
                                    {
                                        break;
                                    }

                                    printf("%s %s %s %d %02d-%02d-%04d %02d:%02d\n", tempTransactionPointer->transPointer->transaction.transactionID, tempTreePointer->treePointer->rightNode->walletID, tempTreePointer->treePointer->leftNode->walletID, local_sum, tempTransactionPointer->transPointer->transaction.transactionDate.day, tempTransactionPointer->transPointer->transaction.transactionDate.month, tempTransactionPointer->transPointer->transaction.transactionDate.year, tempTransactionPointer->transPointer->transaction.transactionTime.hours, tempTransactionPointer->transPointer->transaction.transactionTime.minutes);
                                    
                                }
                                tempTransaction = tempTransaction->next;
                            }
                            break;
                            flag = 1;
                        }
                    }
                    if(flag == 1)
                        break;
                    tempBucket = tempBucket->next;
                }
                printf("Total Payments: %d\n",sum);                
            }
            
        }
        else if(strcmp(token, "/walletStatus") == 0 || strcmp(buffer, "/walletStatus") == 0)
        {
            token = strtok(NULL, " ");
            strcpy(name, token);
            if(name[strlen(name)-1] == '\n')
                name[strlen(name)-1] = '\0';
            string_index = hash_string(name, WalletHashTableBuckets);
            WalletNode *tempWallet = MyWalletHashTable.startingWallet[string_index];
            while(tempWallet != NULL)
            {
                if(strcmp(tempWallet->walletID, name)==0)
                {
                    printf("Balance: %d\n",tempWallet->balance);
                }
                tempWallet = tempWallet->next;
            }
            

        }
        else if(strcmp(token, "/bitCoinStatus") == 0 || strcmp(buffer, "/bitCoinStatus") == 0)
        {
            int counter = 0;
            token = strtok(NULL, " ");
            printf("%d\n",atoi(token));
            int_index = hash_integer(atoi(token), BitcoinHashTableBuckets);
            BitcoinNode *temp = MyBitcoinHashTable.startingBitcoin[int_index];
            TreeNode *tempTreeNode;
            while(temp != NULL)
            {
                if(temp->bitcoinID == atoi(token))
                {
                    printf("Starting Value: %d\n",temp->root->value);
                    NumberofBitcoinTransactions(temp->root, &counter);
                    printf("Number of Transactions:%d\n",counter);
                    tempTreeNode = temp->root;
                    while(tempTreeNode->rightNode != NULL)
                        tempTreeNode = tempTreeNode->rightNode;
                    printf("Unspent money %d\n",tempTreeNode->value);
                    break;
                }
                temp = temp->next;
            }            
            
        }
        else if(strcmp(token, "/traceCoin") == 0 || strcmp(buffer, "/traceCoin") == 0)
        {
            token = strtok(NULL, " ");
            int_index = hash_integer(atoi(token), BitcoinHashTableBuckets);
            BitcoinNode *tempBit = MyBitcoinHashTable.startingBitcoin[int_index];
            while(tempBit != NULL)
            {
                if(tempBit->bitcoinID == atoi(token))
                {
                    PrintTransactions(tempBit->root);
                    break;
                }
                tempBit = tempBit->next;
            }
            
        }
        else if(strcmp(token, "/exit") == 0 || strcmp(buffer, "/exit") == 0)
        {
            FreeBitcoinHashTable(&MyBitcoinHashTable, BitcoinHashTableBuckets);
            FreeWalletHashTable(&MyWalletHashTable, WalletHashTableBuckets);
            FreeHashTable(&SenderHashTable, senderHashtableNumOfEntries, BucketSize);
            FreeHashTable(&ReceiverHashTable, receiverHashtableNumOfEntries, BucketSize);
            FreeTransactions(start); 
            /*Free data of Main*/    
            free(ti2);
            free(ti);
            free(date);
            free(date2);
            return 0;
            
        }
        //PrintWallets(&MyWalletHashTable, WalletHashTableBuckets);

        //printf("%s\n",buffer);
    }
    
}