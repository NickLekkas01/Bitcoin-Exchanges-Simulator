#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// tempBitcoin = MyBitcoinHashTable[int_index]->startingBitcoin;
// while(tempBitcoin->bitcoinID != atoi(token))
//     tempBitcoin = tempBitcoin->next;

void UpdateBalance(WalletNode **start, char *name, int value)
{
    WalletNode *tempWallet;
    if(strcmp((*start)->walletID, name) == 0)
        (*start)->balance += value;
    else
    {
        tempWallet = *start;
        while((tempWallet->next != NULL) && strcmp(tempWallet->next->walletID, name) != 0)
            tempWallet = tempWallet->next;
        if(tempWallet->next != NULL)
            tempWallet->next->balance += value;
        
    }
}

void InsertTreePointerNodeAtStart(TreePointerNode **start, TreeNode *treeNode)
{
    TreePointerNode *new = malloc(sizeof(TreePointerNode));
    new->treePointer = treeNode;
    new->next = (*start);
    (*start) = new;
}


void UpdateTreePointerNode(TreePointerNode **start, TreeNode *treeNode, TreeNode *newTreeNode)
{
    TreePointerNode *temp;
    if((*start) != NULL && (*start)->treePointer == treeNode)
    {
        (*start)->treePointer = newTreeNode;
    }
    else if((*start) != NULL)
    {
        temp = (*start);
        if(temp->next!=NULL && temp->next->treePointer == NULL)
        {
            printf("Found issue in line 1\n");
            fflush(stdout);
        }
        while(temp->next != NULL && temp->next->treePointer != treeNode)
        {
            temp = temp->next;
            if(temp->next!=NULL && temp->next->treePointer == NULL)
            {
                printf("Found issue in line 2\n");
                fflush(stdout);
            }
        }
        if(temp->next != NULL && temp->next->treePointer != NULL )
        {
            temp->next->treePointer = newTreeNode;
        }
        
    }
}

void FreeTreePointerNode(TreePointerNode **start, TreeNode *treeNode)
{    
    TreePointerNode *temp, *temp2;
    if((*start) != NULL && (*start)->treePointer == treeNode)
    {
        temp = (*start);
        (*start) = (*start)->next;
        free(temp);
    }
    else if((*start) != NULL)
    {
        temp = (*start);
        if(temp->next!=NULL && temp->next->treePointer == NULL)
        {
            printf("Found issue in line 3\n");
            fflush(stdout);
        }
        while(temp->next != NULL && temp->next->treePointer != treeNode)
        {
            temp = temp->next;
            if(temp->next!=NULL && temp->next->treePointer == NULL)
            {
                printf("Found issue in line 4\n");
                fflush(stdout);
            }
        }
        if(temp->next != NULL)
        {
            temp2 = temp->next;
            temp->next = temp2->next;
            free(temp2);
        }
    }
}

void FreeBitcoin(Bitcoin **start, int bitcoinID)
{
    Bitcoin *temp, *temp2;
    if((*start) == NULL)
        return;
    
    if((*start)->bitcoinID == bitcoinID)
    {
        temp = (*start);
        (*start) = (*start)->next;
        free(temp);
    }
    else
    {
        temp = (*start);
        while(temp->next != NULL && temp->next->bitcoinID != bitcoinID)
            temp = temp->next;
        if(temp->next != NULL)
        {
            temp2 = temp->next;
            temp->next = temp2->next;
            free(temp2);
        }
    }
    
}

void InsertTransactionPointerAtStart(TransactionPointerNode **start, TransactionNode *trans)
{
    TransactionPointerNode *new = malloc(sizeof(TransactionPointerNode));
    new->transPointer = trans;
    new->next = (*start);
    (*start) = new;
}

int PersonExists(Bucket *start, char *walletID, int BucketSize)
{
    Bucket *temp = start;
    while(temp != NULL)
    {
        for(int i = 0; i < BucketSize; ++i)
        {
            if(temp->Person[i] != NULL)
                if(strcmp(temp->Person[i]->walletID, walletID) == 0)
                    return 1;
        }
        temp = temp->next;     
    }
    return 0;
}

void InsertPerson(Bucket **start, char *walletID, int BucketSize)
{
    Bucket *temp;
    /*If there is a first bucket*/
    if((*start) != NULL)
    {
        /*If first bucket is full*/
        if((*start)->numPersons == BucketSize)
        {
            /*If there is no next bucket create one! */
            if((*start)->next == NULL)
            {
                (*start)->next = malloc(sizeof(Bucket));
                
                (*start)->next->Person = malloc(BucketSize*sizeof(BucketData *));
                for(int i = 0; i < BucketSize; ++i)
                {
                    (*start)->next->Person[i] = NULL;
                }
                /*Insert Person*/
                (*start)->next->Person[0] = malloc(sizeof(BucketData));            
                strcpy((*start)->next->Person[0]->walletID, walletID);
                (*start)->next->Person[0]->startingTransaction = NULL;

                /*Initialize bucketData*/
                (*start)->next->next=NULL;
                (*start)->next->numPersons = 1;
            }
            /*If there is a next Bucket*/
            else
            {
                /*Find a non-full Bucket*/
                temp = (*start);
                while(temp->next != NULL && temp->next->numPersons == BucketSize)
                    temp = temp->next;
                /*If we found one, search for an empty place and insert Person*/
                if(temp->next != NULL)
                {
                    for(int i = 0 ; i < BucketSize; ++i)
                    {
                        if(temp->next->Person[i] == NULL)
                        {
                            temp->next->Person[i] = malloc(sizeof(BucketData));
                            strcpy(temp->next->Person[i]->walletID, walletID);
                            temp->next->Person[i]->startingTransaction = NULL;
                            temp->next->numPersons++;
                            break;
                        }
                    }
                }
                else
                {
                    temp->next = malloc(sizeof(BucketData));
                    temp->next->Person = malloc(BucketSize*sizeof(BucketData *));
                    for(int i = 0; i < BucketSize; ++i)
                    {
                        temp->next->Person[i] = NULL;
                    }
                    temp->next->Person[0] = malloc(sizeof(BucketData));            
                    strcpy(temp->next->Person[0]->walletID, walletID);
                    temp->next->Person[0]->startingTransaction = NULL;

                    /*Initialize bucketData*/
                    temp->next->next=NULL;
                    temp->next->numPersons = 1;
                }
                
            }
            
        }
        /*If its not full search for an empty place and insert Person*/
        else
        {
            for(int i = 0 ; i < BucketSize; ++i)
            {
                if((*start)->Person[i] == NULL)
                {
                    (*start)->Person[i] = malloc(sizeof(BucketData));
                    strcpy((*start)->Person[i]->walletID, walletID);
                    (*start)->Person[i]->startingTransaction = NULL;
                    (*start)->numPersons++;
                    break;
                }
            }
        }
    }
    /*If there is no first bucket*/
    else
    {
        (*start) = malloc(sizeof(BucketData));
        (*start)->Person = malloc(BucketSize*sizeof(BucketData *));
        for(int i = 0; i < BucketSize; ++i)
        {
            (*start)->Person[i] = NULL;
        }
        (*start)->Person[0] = malloc(sizeof(BucketData));            
        strcpy((*start)->Person[0]->walletID, walletID);
        (*start)->Person[0]->startingTransaction = NULL;

        /*Initialize bucketData*/
        (*start)->next=NULL;
        (*start)->numPersons = 1;
    }
    
}

void InsertSenderandReceiverTransactionPointer(Bucket **SenderBucket, Bucket **ReceiverBucket, char *sender, char *receiver, TransactionNode *trans, int maxBuckets)
{
    int flag = 0;
    
    if((*SenderBucket) == NULL || (*ReceiverBucket) == NULL)
    {
        return;
    }
    for(int i = 0; i < maxBuckets; i++)
    {
        if((*SenderBucket)->Person[i] != NULL && (*SenderBucket)->Person[i]->walletID != NULL)
        {
            if(strcmp((*SenderBucket)->Person[i]->walletID, sender) == 0)
            {
                InsertTransactionPointerAtStart(&(*SenderBucket)->Person[i]->startingTransaction, trans);
                // TransactionPointerNode *new = malloc(sizeof(TransactionPointerNode));
                // new->transPointer = trans;
                // new->next = (*SenderBucket)->Person[i]->startingTransaction;
                // (*SenderBucket)->Person[i]->startingTransaction = new;
                break;
                //flag = 1;
                //break;
            }
        }
    }
    for(int i = 0; i < maxBuckets; i++)
    {
        if((*ReceiverBucket)->Person[i] != NULL && (*ReceiverBucket)->Person[i]->walletID != NULL)
        {
            if(strcmp((*ReceiverBucket)->Person[i]->walletID, receiver) == 0)
            {
                InsertTransactionPointerAtStart(&(*ReceiverBucket)->Person[i]->startingTransaction, trans);
                // TransactionPointerNode *new = malloc(sizeof(TransactionPointerNode));
                // new->transPointer = trans;
                // new->next = (*ReceiverBucket)->Person[i]->startingTransaction;
                // (*ReceiverBucket)->Person[i]->startingTransaction = new;
                return;
                //flag = 1;
                //break;
            }
        }
    }

    Bucket *tempSender, *tempReceiver;
    tempSender = *SenderBucket;
    tempReceiver = *ReceiverBucket;
    while(1)
    {
        if(tempSender->next != NULL)
        {
            for(int i = 0; i < maxBuckets; i++)
            {
                if(tempSender->next->Person[i] != NULL && tempSender->next->Person[i]->walletID != NULL)
                {
                    if(strcmp(tempSender->next->Person[i]->walletID, sender) == 0);
                    {
                        InsertTransactionPointerAtStart(&tempSender->next->Person[i]->startingTransaction, trans);
                        // TransactionPointerNode *new = malloc(sizeof(TransactionPointerNode));
                        // new->transPointer = trans;
                        // new->next = tempSender->next->Person[i]->startingTransaction;
                        // tempSender->next->Person[i]->startingTransaction = new;
                        flag = 1;
                        break;
                        //break;
                    }
                }
            }
            if(flag == 1)
                break;
            else
                tempSender = tempSender->next;
        }
    }
    flag = 0;
    while(1)
    {
        if(tempReceiver->next != NULL)
        {
            for(int i = 0; i < maxBuckets; i++)
            {
                if(tempReceiver->next->Person[i] != NULL && tempReceiver->next->Person[i]->walletID != NULL)
                {
                    if(strcmp(tempReceiver->next->Person[i]->walletID, receiver) == 0);
                    {
                        InsertTransactionPointerAtStart(&tempReceiver->next->Person[i]->startingTransaction, trans);
                        // TransactionPointerNode *new = malloc(sizeof(TransactionPointerNode));
                        // new->transPointer = trans;
                        // new->next = tempReceiver->next->Person[i]->startingTransaction;
                        // tempReceiver->next->Person[i]->startingTransaction = new;
                        //flag = 1;
                        //break;
                        return;
                    }
                }
            }
            tempReceiver = tempReceiver->next;
        }
    }
    
}

void PrintWallets(WalletHashTable *MyWalletHashTable, int maxBuckets)
{
    WalletNode *tempWallet;
    Bitcoin *tempBitcoin;
    TreePointerNode *tempTreePointer;
    for(int i = 0 ; i < maxBuckets; i++)
    {
        tempWallet = MyWalletHashTable->startingWallet[i];
        while(tempWallet != NULL)
        {
            if(tempWallet != NULL)
            {
                printf("NAME: %s\n",tempWallet->walletID);
                printf("Balance: %d\n",tempWallet->balance);
            }    
            tempBitcoin = tempWallet->startingBitcoin;
            
            while(tempBitcoin != NULL)
            {
                if(tempBitcoin != NULL)
                    printf("Bitcoin: %d\n", tempBitcoin->bitcoinID);
                tempTreePointer = tempBitcoin->startingLeafNode;
                while(tempTreePointer != NULL)
                {
                    printf("%d ",tempTreePointer->treePointer->value);
                    tempTreePointer = tempTreePointer->next;
                }
                printf("\n");
                tempBitcoin = tempBitcoin->next;
            }
            printf("\n");
            tempWallet = tempWallet->next;
        }
    }
}

TransactionNode *InsertTransactionAtStart(TransactionNode **start, WalletHashTable *MyWalletHashTable, char *id, char *sender, int sender_index, char *receiver, int receiver_index, int value, Date *date, Time *ti)
{
    /*Create transaction node */
    TransactionNode *trans = malloc(sizeof(TransactionNode));
    strcpy(trans->transaction.transactionID, id);
    trans->transaction.transactionDate.day = date->day;
    trans->transaction.transactionDate.month = date->month;
    trans->transaction.transactionDate.year = date->year;
    trans->transaction.transactionTime.hours = ti->hours;
    trans->transaction.transactionTime.minutes = ti->minutes;
    trans->startingTreePointer = NULL;
    trans->next = (*start);
    (*start) = trans;
    int sum = 0;
    WalletNode *tempWallet, *tempWallet2;
    Bitcoin *tempBitcoin, *tempBitcoin2;
    TreePointerNode *tempTreePointer;
    TreeNode *MyTreeNode;
    /*Find sender's wallet*/
    tempWallet = MyWalletHashTable->startingWallet[sender_index];
    while(tempWallet != NULL && strcmp(tempWallet->walletID, sender) != 0)
        tempWallet = tempWallet->next;
    if(tempWallet != NULL)
    {
        /*If the sender has enough money*/
        if(tempWallet->balance >= value)
        {
            tempBitcoin = tempWallet->startingBitcoin;
            while(tempBitcoin != NULL && sum < value)
            {
                tempTreePointer = tempBitcoin->startingLeafNode;
                while(tempTreePointer != NULL && sum < value)
                {
                    //myTreeNode = tempTreePointer->treePointer;
                    tempTreePointer->treePointer->leftNode = malloc(sizeof(TreeNode));
                    if(tempTreePointer->treePointer->leftNode == NULL)
                    {    exit(-1);
                    }
                    MyTreeNode = tempTreePointer->treePointer;
                    tempTreePointer->treePointer->leftNode->transNode = NULL; 
                    
                    tempTreePointer->treePointer->rightNode = malloc(sizeof(TreeNode));
                    if(tempTreePointer->treePointer->rightNode == NULL)
                    {    exit(-1);
                    }
                    tempTreePointer->treePointer->rightNode->transNode = NULL;
                    tempTreePointer->treePointer->leftNode->leftNode = NULL;
                    tempTreePointer->treePointer->leftNode->rightNode = NULL;
                    tempTreePointer->treePointer->rightNode->leftNode = NULL;
                    tempTreePointer->treePointer->rightNode->rightNode = NULL;

                    strcpy(tempTreePointer->treePointer->leftNode->walletID,receiver);
                    strcpy(tempTreePointer->treePointer->rightNode->walletID,sender);
                    
                    

                    /*Find the wallet of the receiver*/
                    tempWallet2 = MyWalletHashTable->startingWallet[receiver_index];
                    while(tempWallet2 != NULL && strcmp(tempWallet2->walletID, receiver) != 0)
                        tempWallet2 = tempWallet2->next;
                    if(tempWallet2 != NULL)
                    {
                        /*Search if he has the bitcoin*/
                        tempBitcoin2 = tempWallet2->startingBitcoin;
                        while(tempBitcoin2 != NULL && tempBitcoin2->bitcoinID != tempBitcoin->bitcoinID)
                            tempBitcoin2 = tempBitcoin2->next;
                        /*If he does insert the transaction info*/
                        if(tempBitcoin2 != NULL)
                        {
                            InsertTreePointerNodeAtStart(&tempBitcoin2->startingLeafNode, tempTreePointer->treePointer->leftNode);
                        }
                        /*Else firstly insert the bitcoin and after that the transaction info*/
                        else
                        {
                            InsertBitcoinAtStart(&tempWallet2->startingBitcoin, tempBitcoin->bitcoinID);
                            InsertTreePointerNodeAtStart(&tempWallet2->startingBitcoin->startingLeafNode, tempTreePointer->treePointer->leftNode);
                        }
                    }
                    tempTreePointer->treePointer->transNode = trans;
                    /*Insert transaction tree info to the transaction node*/
                    InsertTreePointerNodeAtStart(&trans->startingTreePointer, tempTreePointer->treePointer);
                      
                    /*If we need more than we have in this bitcoin*/
                    if((value - sum) >= tempTreePointer->treePointer->value)
                    {
                        /*Give all to receiver. Erase this money from sender balance and add to the receiver balance*/
                        tempTreePointer->treePointer->leftNode->value = tempTreePointer->treePointer->value;
                        tempTreePointer->treePointer->rightNode->value = 0;
                        UpdateBalance(&MyWalletHashTable->startingWallet[sender_index], sender, -tempTreePointer->treePointer->value);
                        UpdateBalance(&MyWalletHashTable->startingWallet[receiver_index], receiver, tempTreePointer->treePointer->value);
                        /*Remove leaf node from sender because he doesn't have that money anymore*/
                        FreeTreePointerNode(&(tempBitcoin->startingLeafNode), tempTreePointer->treePointer);
                        /*If sender do not have money anymore in this bitcoin remove this bitcoin from him*/
                        if(tempBitcoin->startingLeafNode == NULL)
                        {
                            FreeBitcoin(&tempWallet->startingBitcoin, tempBitcoin->bitcoinID);
                        }
                    }
                    /*If we have enough money in this bitcoin*/
                    else
                    {
                        /*Give to receiver the money he needs. Add these money to his(receiver) balance and erase them from senders.*/
                        tempTreePointer->treePointer->leftNode->value = (value - sum);
                        tempTreePointer->treePointer->rightNode->value = tempTreePointer->treePointer->value - (value - sum);
                        UpdateBalance(&MyWalletHashTable->startingWallet[sender_index], sender, -(value - sum));
                        UpdateBalance(&MyWalletHashTable->startingWallet[receiver_index], receiver, (value - sum));
                        /*Update senders leaf node to show the TreeNode with the rest of his money*/
                        UpdateTreePointerNode(&(tempBitcoin->startingLeafNode), tempTreePointer->treePointer, tempTreePointer->treePointer->rightNode);
                    }

                    if((value - sum) >= MyTreeNode->value)
                        sum += MyTreeNode->value;
                    else
                        sum += (value - sum); 

                    tempTreePointer = tempTreePointer->next;
                    
                }
                tempBitcoin = tempBitcoin->next;
            }

        }
        else
        {
            printf("Sender (%s) does not have enough money\n", sender);
            exit(-1);
        }
    }
    return trans;
}

void InsertBitcoinAtStart(Bitcoin **start, int bitcoinID)
{
    Bitcoin *new = malloc(sizeof(Bitcoin));
    new->bitcoinID = bitcoinID;
    new->startingLeafNode = NULL;
    new->next = *start;
    *start = new;
}

void InsertWalletAtStart(WalletNode **start, char *name)
{
    WalletNode *new = malloc(sizeof(WalletNode));
    strcpy(new->walletID, name);
    new->balance = 0;
    new->startingBitcoin = NULL;
    new->next = *start;
    *start = new;
}

int BitcoinExists(BitcoinHashTable MyBitcoinHashTable, int bitcoinID, int buckets)
{
    int index = hash_integer(bitcoinID, buckets);
    BitcoinNode *temp = MyBitcoinHashTable.startingBitcoin[index];
    while(temp != NULL)
    {
        if(temp->bitcoinID == bitcoinID)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void InsertBitcoinNodeAtStart(BitcoinNode **start, int bitcoinID, char *name, int bitcoinValue)
{
    BitcoinNode *new = malloc(sizeof(BitcoinNode));
    new->bitcoinID = bitcoinID;
    new->root = malloc(sizeof(TreeNode));
    strcpy(new->root->walletID, name);
    new->root->value = bitcoinValue;
    new->root->leftNode = NULL;
    new->root->rightNode = NULL;
    new->next = *start;
    *start = new;
}

/*online source*/
int hash_integer(int key, int buckets) 
{
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    key = key % buckets;
    return key;
}
/*online source*/

int hash_string(char *str, int buckets)
{
    unsigned long key = 5381;
    int c;

    while (c = *str++)
        key = ((key << 5) + key) + c; /* hash * 33 + c */
    unsigned int temp = (unsigned int) key;
    return (temp % buckets);
}


void NumberofBitcoinTransactions(TreeNode *temp, int *counter)
{
    if(temp->leftNode != NULL && temp->rightNode != NULL)
        *counter = *counter + 1;
    if(temp->leftNode != NULL)
        NumberofBitcoinTransactions(temp->leftNode, counter);
    if(temp->rightNode != NULL)
        NumberofBitcoinTransactions(temp->rightNode, counter);
}

void PrintTransactions(TreeNode *temp)
{
    if(temp->leftNode != NULL && temp->rightNode != NULL)
    {
        printf("%s %s %s %d %02d-%02d-%04d %02d:%02d\n", temp->transNode->transaction.transactionID, temp->rightNode->walletID, temp->leftNode->walletID, temp->leftNode->value, temp->transNode->transaction.transactionDate.day, temp->transNode->transaction.transactionDate.month, temp->transNode->transaction.transactionDate.year, temp->transNode->transaction.transactionTime.hours, temp->transNode->transaction.transactionTime.minutes);
    }
    if(temp->leftNode != NULL)
        PrintTransactions(temp->leftNode);
    if(temp->rightNode != NULL)
        PrintTransactions(temp->rightNode);
}

void FreeTree(TreeNode *temp)
{
    if(temp->leftNode != NULL)
        FreeTree(temp->leftNode);
    if(temp->rightNode != NULL)
        FreeTree(temp->rightNode);
    free(temp);
}

void readArguments(int argc, char **argv, int *bitCoinValue, int *senderHashtableNumOfEntries, int *receiverHashtableNumOfEntries, int *bucketSize, char *bitCoinBalancesFile, char *transactionFile)
{
    if(argc == 13)
    {
        for(int i = 1; i < 13; i+=2)
        {
            if(argv[i][1] == 'a')
            {
                strcpy(bitCoinBalancesFile, argv[i+1]);
            }
            else if(argv[i][1] == 't')
            {
                strcpy(transactionFile, argv[i+1]);
            }
            else if(argv[i][1] == 'v')
            {
                *bitCoinValue = atoi(argv[i+1]);
            }
            else if(argv[i][1] == 'h')
            {
                if(argv[i][2] == '1')
                {
                    *senderHashtableNumOfEntries = atoi(argv[i+1]);
                }
                else if(argv[i][2] == '2')
                {
                    *receiverHashtableNumOfEntries = atoi(argv[i+1]);
                }
            }
            else if(argv[i][1] == 'b')
            {
                *bucketSize = atoi(argv[i+1]);
            }
        }
    } 
    else
    {
        printf("Not enough input\n");
        exit(-1);
    }
}

void FreeBitcoinHashTable(BitcoinHashTable *MyBitcoinHashTable, int BitcoinHashTableBuckets)
{
    for(int i = 0; i < BitcoinHashTableBuckets; ++i)
    {
        BitcoinNode *tempB = MyBitcoinHashTable->startingBitcoin[i];
        BitcoinNode *temp2;
        while(tempB != NULL)
        {   
            temp2 = tempB;
            tempB=tempB->next;
            FreeTree(temp2->root);
            free(temp2);
        }
    }
    free(MyBitcoinHashTable->startingBitcoin); 
}

void FreeWalletHashTable(WalletHashTable *MyWalletHashTable, int WalletHashTableBuckets)
{
    for(int i = 0; i < WalletHashTableBuckets; ++i)
    {
        WalletNode *tempWallet = MyWalletHashTable->startingWallet[i];
        WalletNode *temp2;
        while(tempWallet != NULL)
        {
            temp2 = tempWallet;

            Bitcoin *tempBitcoin = temp2->startingBitcoin;
            Bitcoin *tempBit2;
            while(tempBitcoin != NULL)
            {
                tempBit2 = tempBitcoin;
                TreePointerNode *tempTreePointer = tempBit2->startingLeafNode;
                TreePointerNode *tempTreeP2;
                while(tempTreePointer != NULL)
                {
                    tempTreeP2 = tempTreePointer;
                    free(tempTreeP2);
                    tempTreePointer = tempTreePointer->next;
                }
                free(tempBit2);
                tempBitcoin = tempBitcoin->next;
            }
            free(temp2);
            tempWallet = tempWallet->next;
        }
    }
    free(MyWalletHashTable->startingWallet);
}

void FreeHashTable(MyHashTable *MyHashTable, int Buckets, int BucketSize)
{
    for(int i = 0; i < Buckets; ++i)
    {
        Bucket *tempBucket = MyHashTable->HashTable[i];
        Bucket *temp2;
        while(tempBucket != NULL)
        {
            temp2 = tempBucket;
            tempBucket = tempBucket->next;
            for(int i = 0; i < BucketSize; ++i)
            {
                if(temp2->Person[i] != NULL)
                {
                    BucketData *tempBucketData = temp2->Person[i];
                    BucketData *tempB2;
                
                    tempB2 = tempBucketData;
                    TransactionPointerNode *tempTransactionPointer = tempBucketData->startingTransaction;
                    TransactionPointerNode *tempT2;
                    while(tempTransactionPointer != NULL)
                    {
                        tempT2 = tempTransactionPointer;
                        tempTransactionPointer = tempTransactionPointer->next;
                        TransactionNode *tempTransactionNode = tempT2->transPointer;
                        TransactionNode *tempTN2; 
                        free(tempT2);
                    }
                    free(tempB2);
                }
            }
            free(temp2);
        }
    }
    free(MyHashTable->HashTable);
}

void FreeTransactions(TransactionNode *start)
{
    TransactionNode *tempTrans = start;
    TransactionNode *tempTr2;
    while(tempTrans != NULL)
    {
        tempTr2 = tempTrans;
        tempTrans = tempTrans->next;
        TreePointerNode *tempTreePointerNode = tempTr2->startingTreePointer;
        TreePointerNode *tempTP2;
        while(tempTreePointerNode != NULL)
        {
            tempTP2 = tempTreePointerNode;
            tempTreePointerNode = tempTreePointerNode->next;
            free(tempTP2);
        }
        free(tempTr2);
    }
}