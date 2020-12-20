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

    /* Place a new buy order at price `price` with amount (volume) of `amt`
     * returns a vector of Trades that occur when this order is placed.
     * A trade occurs when the incoming order wants to buy at a price higher than pending sell orders,
     * at a price specified on each of the sell order.*/
    vector<Trade*> placeBuyOrder(int issuerID, int price, int amt);

    /* Place a new sell order at price `price` with amount (volume) of `amt`
     * returns a vector of Trades that occur when this order is placed.
     * A trade occurs when the incoming order wants to sell at a price lower than pending buy orders,
     * at a price specified on each of the buy order.*/
    vector<Trade*> placeSellOrder(int issuerID, int price, int amt);

    void deleteOrder(int issuerID, int orderID);

    /* Get the volume of orders on the buy tree at all price points (in descending order).
     * Returns a vector of (price point, volume of orders at that price). */
    vector<pair<int, int>> getPendingBuys();

    /* Get the volume of orders on the sell tree at all price points (in ascending order).
     * Returns a vector of (price point, volume of orders at that price). */
    vector<pair<int, int>> getPendingSells();

    // Returns a vector of orders that are pending from the user with id `userID`
    vector<Order*> getPendingOrders(int userID);

    // Gets the buy history of the user with id `userID`. Returns a vector of trades involving the user as the buyer.
    vector<Trade*> getBuyTrades(int userID);

    // Gets the sell history of the user with id `userID`. Returns a vector of trades involving the user as the seller.
    vector<Trade*> getSellTrades(int userID);
private:
    unordered_map<int, User*> users;
    int nextUserID;

    // Ordered map from a price point to a pair of (volume of sell orders, list of sell orders) at that price
    map<int, pair<int, list<Order*>*>*> sellHeap;

    // Ordered map from a price point to a pair of (volume of buy orders, list of buy orders) at that price
    map<int, pair<int, list<Order*>*>*> buyHeap;
};


#endif //TRADEMATCHER_TRADEENGINE_H
