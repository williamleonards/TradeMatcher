//
// Created by William Leonard Sumendap on 7/10/20.
//

#ifndef TRADEMATCHER_TRADEENGINE_H
#define TRADEMATCHER_TRADEENGINE_H

using namespace std;

#include <iostream>
#include<vector>
#include<string>
#include<list>
#include<unordered_map>
#include<map>

#include "Order.h"
#include "Trade.h"
#include "User.h"

class TradeEngine {
public:
    TradeEngine();
    ~TradeEngine();
    int createUser(string name);
    vector<Trade*> placeBuyOrder(int issuerID, int price, int amt);
    vector<Trade*> placeSellOrder(int issuerID, int price, int amt);
    void deleteOrder(int issuerID, int orderID);
    vector<pair<int, int>> getPendingBuys();
    vector<pair<int, int>> getPendingSells();
    vector<Order*> getPendingOrders(int userID);
    vector<Trade*> getBuyTrades(int userID);
    vector<Trade*> getSellTrades(int userID);
private:
    unordered_map<int, User*> users;
    int nextUserID;
    map<int, pair<int, list<Order*>*>*> sellHeap;
    map<int, pair<int, list<Order*>*>*> buyHeap;
};


#endif //TRADEMATCHER_TRADEENGINE_H
