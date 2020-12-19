//
// Created by William Leonard Sumendap on 7/10/20.
//

#ifndef TRADEMATCHER_ORDER_H
#define TRADEMATCHER_ORDER_H

using namespace std;

#include<vector>
#include<string>
#include<list>
#include<unordered_map>
#include<map>

class User;

class Order {
public:
    Order(bool type_, int amt_, int price_, int issuerID_, int id_);
    bool getType();
    int getID();
    int getIssuerID();
    int getAmt();
    void setAmt(int amt_);
    int getPrice();
    bool checkValid();
    void setInvalid();
    bool equals(Order *other);
    void print();
private:
    bool type; // true = buy, false = sell
    int id;
    int issuerID;
    int amt;
    int price;
    bool valid;
};


#endif //TRADEMATCHER_ORDER_H
