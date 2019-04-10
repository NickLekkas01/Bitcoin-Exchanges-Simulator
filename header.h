typedef struct date
{
    int day;
    int month;
    int year;
}Date;

typedef struct time
{
    int hours;
    int minutes;
}Time;

typedef struct transactionNode TransactionNode;

typedef struct transaction
{
    char transactionID[50];
    // char senderWalletID[50];
    // char receiverWalletID[50];
    // int value;
    Date transactionDate;
    Time transactionTime;
}Transaction;

/*Node of the tree with the bitcoin's transactions*/
typedef struct treenode
{
    char walletID[50];
    int value;
    TransactionNode *transNode;
    struct treenode *leftNode;
    struct treenode *rightNode;
}TreeNode;

/* A list of pointers pointing to the bitcoins trees which were present to a transaction*/
typedef struct treePointerNode
{
   TreeNode *treePointer;
   struct treePointerNode *next;  
}TreePointerNode;

/*A list with nodes showing the data each transaction has and the starting pointer for the TreePointer list above*/
typedef struct transactionNode
{
    Transaction transaction;
    TreePointerNode *startingTreePointer;
    struct transactionNode *next;
}TransactionNode;

/*A list of pointers pointing to the transaction list*/
typedef struct transactionPointerNode
{
    TransactionNode *transPointer;
    struct transactionPointerNode *next;
}TransactionPointerNode;

/*Data in Bucket(walletID and the start of the list with the transactionPointers)*/
typedef struct bucketdata
{
    char walletID[50];
    TransactionPointerNode *startingTransaction;
}BucketData;  

/*A bucket with its data and the pointer to the next bucket*/
typedef struct bucket
{
    BucketData **Person;
    int numPersons;
    struct bucket *next; 
}Bucket;

/*A HashTable with Buckets as data*/
typedef struct senderhashtable
{
    Bucket **HashTable;
}MyHashTable;

typedef struct bitcoin
{
    int bitcoinID;
    //int value;
    TreePointerNode *startingLeafNode;
    struct bitcoin *next;
}Bitcoin;

typedef struct wallet
{
    char walletID[50];
    int balance;
    Bitcoin *startingBitcoin;
    struct wallet *next;
}WalletNode;

typedef struct walletHashTable
{
    WalletNode **startingWallet;
}WalletHashTable;

typedef struct bitcoinNode
{
    int bitcoinID;
    TreeNode *root;
    struct bitcoinNode *next;
}BitcoinNode;

typedef struct bitcoinHashTable
{
    BitcoinNode **startingBitcoin;
}BitcoinHashTable;

void InsertSenderandReceiverTransactionPointer(Bucket **SenderBucket, Bucket **ReceiverBucket, char *sender, char *receiver, TransactionNode *trans, int maxBuckets);
int PersonExists(Bucket *start, char *walletID, int BucketSize);
void InsertPerson(Bucket **start, char *walletID, int BucketSize);
void UpdateBalance(WalletNode **start, char *name, int value);
void InsertTreePointerNodeAtStart(TreePointerNode **start, TreeNode *treeNode);
void UpdateTreePointerNode(TreePointerNode **start, TreeNode *treeNode, TreeNode *newTreeNode);
void FreeTreePointerNode(TreePointerNode **start, TreeNode *treeNode);
void FreeBitcoin(Bitcoin **start, int bitcoinID);
void PrintWallets(WalletHashTable *MyWalletHashTable, int maxBuckets);
TransactionNode *InsertTransactionAtStart(TransactionNode **start, WalletHashTable *MyWalletHashTable, char *id, char *sender, int sender_index, char *receiver, int receiver_index, int value, Date *date, Time *ti);
void InsertTransactionPointerAtStart(TransactionPointerNode **start, TransactionNode *trans);
void InsertBitcoinAtStart(Bitcoin **start, int bitcoinID);
void InsertWalletAtStart(WalletNode **start, char *name);
int BitcoinExists(BitcoinHashTable MyBitcoinHashTable, int bitcoinID, int buckets);
void InsertBitcoinNodeAtStart(BitcoinNode **start, int bitcoinID, char *name, int bitcoindValue);
int hash_integer(int key, int buckets);
int hash_string(char *str, int buckets);
void readArguments(int argc, char **argv, int *bitCoinValue, int *senderHashtableNumOfEntries, int *receiverHashtableNumOfEntries, int *bucketSize, char *bitCoinBalancesFile, char *transactionFile);
void NumberofBitcoinTransactions(TreeNode *temp, int *counter);
void PrintTransactions(TreeNode *temp);
void FreeTree(TreeNode *temp);
void FreeBitcoinHashTable(BitcoinHashTable *MyBitcoinHashTable, int BitcoinHashTableBuckets);
void FreeWalletHashTable(WalletHashTable *MyWalletHashTable, int WalletHashTableBuckets);
void FreeHashTable(MyHashTable *MyHashTable, int Buckets, int BucketSize);
void FreeTransactions(TransactionNode *start);