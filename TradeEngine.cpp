//
// Created by William Leonard Sumendap on 7/10/20.
//

#include "TradeEngine.h"

TradeEngine::TradeEngine() {
    nextUserID = 0;
}
TradeEngine::~TradeEngine() {
    for (auto p : users) {
        delete p.second;
    }
    for (auto p : sellHeap) {
        list<Order*> *lst = p.second->second;
        for (Order *ord : *lst) {
            delete ord;
        }
        delete p.second;
    }
}
int TradeEngine::createUser(string name) {
    User *user = new User(nextUserID, name);
    users[nextUserID] = user;
    if (nextUserID == (1 << 31) - 1) {
        cout << "User limit reached" << endl;
        return -1;
    }
    nextUserID++;
    return nextUserID - 1;
}
vector<Trade*> TradeEngine::placeBuyOrder(int issuerID, int price, int amt) {
    User *user = users[issuerID];
    vector<Trade*> ans;
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return ans;
    }
    int remaining = amt;
    // generate trades when appropriate
    for (auto itr = sellHeap.begin(); itr != sellHeap.end(); itr++) {
        int currPrice = itr->first;
        if (currPrice > price || remaining <= 0) break;
        int amtLeft = itr->second->first;
        list<Order*> *lst = itr->second->second;
        while (remaining > 0 && !lst->empty()) {
            Order *frst = lst->front();
            if (!frst->checkValid()) { // if order is stale
                lst->pop_front();
                delete frst;
                continue;
            }
            int currAmt = frst->getAmt();
            int sellerID = frst->getIssuerID();
            User *seller = users[sellerID];
            if (remaining < currAmt) { // current order not finished
                Trade *trade  = new Trade(remaining, currPrice, issuerID, sellerID);
                ans.push_back(trade);
                // update buyer's annd seller's finished orders
                user->getBought()->push_back(trade);
                seller->getSold()->push_back(trade);
                // update frst order amount
                frst->setAmt(currAmt - remaining);
                amtLeft -= remaining;
                remaining = 0;
            } else { // current order finished
                Trade *trade = new Trade(currAmt, currPrice, issuerID, sellerID);
                ans.push_back(trade);
                // update buyer's and seller's finished orders
                user->getBought()->push_back(trade);
                seller->getSold()->push_back(trade);
                lst->pop_front();
                // update seller's orders
                seller->getOrders()->erase(frst->getID());
                delete frst;
                remaining -= currAmt;
                amtLeft -= currAmt;
            }
        }
        itr->second->first = amtLeft;
    }
    if (remaining > 0) { // put surplus amount on buy heap
        pair<int, list<Order*>*> *p = buyHeap[price];
        Order *leftover = user->issueOrder(true, price, remaining);
        if (p != NULL) {
            p->first += remaining;
            p->second->push_back(leftover);
        } else {
            list<Order*> *lst = new list<Order*>();
            lst->push_back(leftover);
            buyHeap[price] = new pair<int, list<Order*>*>(remaining, lst);
        }
    }
    return ans;
}
vector<Trade*> TradeEngine::placeSellOrder(int issuerID, int price, int amt) {
    User *user = users[issuerID];
    vector<Trade*> ans;
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return ans;
    }
    int remaining = amt;
    // generate trades when appropriate
    for (auto itr = buyHeap.rbegin(); itr != buyHeap.rend(); itr++) {
        int currPrice = itr->first;
        if (currPrice < price || remaining <= 0) break;
        int amtLeft = itr->second->first;
        list<Order*> *lst = itr->second->second;
        while (remaining > 0 && !lst->empty()) {
            Order *frst = lst->front();
            if (!frst->checkValid()) { // if order is stale
                lst->pop_front();
                delete frst;
                continue;
            }
            int currAmt = frst->getAmt();
            int buyerID = frst->getIssuerID();
            User *buyer = users[buyerID];
            if (remaining < currAmt) { // current order not finished
                Trade *trade  = new Trade(remaining, currPrice, buyerID, issuerID);
                ans.push_back(trade);
                // update buyer's and seller's finished orders
                buyer->getBought()->push_back(trade);
                user->getSold()->push_back(trade);
                // update frst order amount
                frst->setAmt(currAmt - remaining);
                amtLeft -= remaining;
                remaining = 0;
            } else { // current order finished
                Trade *trade = new Trade(currAmt, currPrice, buyerID, issuerID);
                ans.push_back(trade);
                // update buyer's and seller's finished orders
                buyer->getBought()->push_back(trade);
                user->getSold()->push_back(trade);
                lst->pop_front();
                // update buyer's orders
                buyer->getOrders()->erase(frst->getID());
                delete frst;
                remaining -= currAmt;
                amtLeft -= currAmt;
            }
        }
        itr->second->first = amtLeft;
    }
    if (remaining > 0) { // put surplus amount on sell heap
        pair<int, list<Order*>*> *p = sellHeap[price];
        Order *leftover = user->issueOrder(false, price, remaining);
        if (p != NULL) {
            p->first += remaining;
            p->second->push_back(leftover);
        } else {
            list<Order*> *lst = new list<Order*>();
            lst->push_back(leftover);
            sellHeap[price] = new pair<int, list<Order*>*>(remaining, lst);
        }
    }
    return ans;
}
void TradeEngine::deleteOrder(int issuerID, int orderID) { //lazy deletion
    User *user = users[issuerID];
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return;
    }
    unordered_map<int, Order*> *userOrders = (user->getOrders());
    Order *order = userOrders->at(orderID);
    if (order == NULL) {
        cout << "Order ID not known" << endl;
        return;
    }
    int price = order->getPrice();
    pair<int, list<Order*>*> *p = order->getType() ? buyHeap[price] : sellHeap[price];
    if (p == NULL) {
        cout << "Heap incorrectly configured!, type = " << order->getType() << endl;
        return;
    }
    p->first -= order->getAmt();
    order->setInvalid();
    userOrders->erase(orderID); // CHECK THE SYNTAX!!!
}
vector<pair<int, int>> TradeEngine::getPendingBuys() {
    vector<pair<int, int>> v;
    for (auto itr = buyHeap.rbegin(); itr != buyHeap.rend(); itr++) {
        int price = itr->first;
        int vol = itr->second->first;
        if (vol != 0) {
            v.push_back(pair<int, int>(price, vol));
        }
    }
    return v;
}
vector<pair<int, int>> TradeEngine::getPendingSells() {
    vector<pair<int, int>> v;
    for (auto itr = sellHeap.begin(); itr != sellHeap.end(); itr++) {
        int price = itr->first;
        int vol = itr->second->first;
        if (vol != 0) {
            v.push_back(pair<int, int>(price, vol));
        }
    }
    return v;
}
vector<Order*> TradeEngine::getPendingOrders(int userID) {
    User *user = users[userID];
    vector<Order*> ans;
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return ans;
    }
    auto orders = user->getOrders();
    for (auto itr = orders->begin(); itr != orders->end(); itr++) {
        ans.push_back(itr->second);
    }
    return ans;
}
vector<Trade*> TradeEngine::getBuyTrades(int userID) {
    User *user = users[userID];
    vector<Trade*> ans;
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return ans;
    }
    return *(user->getBought());
}
vector<Trade*> TradeEngine::getSellTrades(int userID) {
    User *user = users[userID];
    vector<Trade*> ans;
    if (user == NULL) {
        cout << "User ID not known" << endl;
        return ans;
    }
    return *(user->getSold());
}
