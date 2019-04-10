# Bitcoin-Exchanges-Simulator
A simulator of a system in which users send money from bitcoins with each other

Compile:
make

Running:
./bitcoin -a bitCoinBalancesFile –t transactionsFile -v bitCoinValue –h1 senderHashtableNumOfEntries –h2  receiverHashtableNumOfEntries –b bucketSize

Functions

/requestTransaction senderWalletID receiverWalletID amount date time
Create a transaction. Sender sends ammount money to receiver at date time 

/requestTransactions  senderWalletID receiverWalletID amount date time;                       
                      senderWalletID2 receiverWalletID2 amount2 date2 time2;
                       ...                       
                      senderWalletIDn receiverWalletIDn amountn daten timen;
Create many transactions                     
                     
/requestTransactions inputFile
Create many transactions from file

/findEarnings walletID [time1][year1][time2][year2]
Find earnings of this wallet between this time

/findPayments walletID [time1][year1][time2][year2]
Find payments of this wallet between this time

/walletStatus walletID
Current value of this wallet

/bitCoinStatus bitCoinID
Value of Bitcoin, number of transactions happened, unspent money

/traceCoin bitCoinID
Transactions happened in this bitcoin
