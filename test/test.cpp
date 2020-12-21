//
// Created by William Leonard Sumendap on 7/10/20.
//

#include "test.h"
#include "../source/TradeEngine.h"

void test::runAllTests() {
    test::testInit();
    test::testInitUser();
    test::testSingleUser();
    test::testSingleDelete();
    test::testDeleteAfterTrade();
    test::testBuyOut();
    test::testSellOut();
    test::testAll();
}

void test::testInit() {
    TradeEngine *t = new TradeEngine();
    vector<pair<int, int>> empty;
    assert(t->getPendingBuys() == empty);
    assert(t->getPendingSells() == empty);
    cout << "testInit passed" << endl;
    delete t;
}

void test::testInitUser() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    vector<Trade*> empty;
    vector<Order*> empty2;
    assert(*(t->getBuyTrades(amyID)) == empty);
    assert(*(t->getSellTrades(amyID)) == empty);
    assert(t->getPendingOrders(amyID) == empty2);
    cout << "testInitUser passed" << endl;
    delete t;
}

void test::testSingleUser() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    vector<Trade*> empty;
    assert(t->placeBuyOrder(amyID, 3, 10) == empty);
    assert(t->placeBuyOrder(amyID, 7, 5) == empty);
    assert(t->placeBuyOrder(amyID, 10, 5) == empty);
    vector<Order*> expectedOrders;
    expectedOrders.push_back(new Order(true, 10, 3, amyID, 0));
    expectedOrders.push_back(new Order(true, 5, 7, amyID, 1));
    expectedOrders.push_back(new Order(true, 5, 10, amyID, 2));
    vector<Order*> pendingOrders = t->getPendingOrders(amyID);
    sort(pendingOrders.begin(), pendingOrders.end(),
         [](Order *a, Order *b) -> bool
         {
             return a->getID() < b->getID();
         });
    for (int i = 0; i < 3; i++) {
        assert(pendingOrders[i]->equals(expectedOrders[i]));
        delete expectedOrders[i];
    }
    cout << "testSingleUser passed" << endl;
    delete t;
}

void test::testSingleDelete() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    vector<Trade*> empty;
    assert(t->placeBuyOrder(amyID, 3, 10) == empty);
    assert(t->placeBuyOrder(amyID,7, 5) == empty);
    assert(t->placeBuyOrder(amyID,12, 3) == empty);
    assert(t->placeSellOrder(amyID,15, 5) == empty);
    t->deleteOrder(amyID, 2);
    vector<Order*> expectedOrders;
    expectedOrders.push_back(new Order(true, 10, 3, amyID, 0));
    expectedOrders.push_back(new Order(true, 5, 7, amyID, 1));
    expectedOrders.push_back(new Order(false, 5, 15, amyID, 3));
    vector<Order*> pendingOrders = t->getPendingOrders(amyID);
    sort(pendingOrders.begin(), pendingOrders.end(),
         [](Order *a, Order *b) -> bool
         {
             return a->getID() < b->getID();
         });
    vector<pair<int, int>> expectedBuyHeap{{7,5}, {3,10}};
    vector<pair<int, int>> expectedSellHeap{{15, 5}};
    assert(t->getPendingBuys() == expectedBuyHeap);
    assert(t->getPendingSells() == expectedSellHeap);
    for (int i = 0; i < 3; i++) {
        assert(pendingOrders[i]->equals(expectedOrders[i]));
    }
    cout << "testSingleDelete passed" << endl;
    for (auto x : expectedOrders) {
        delete x;
    }
    delete t;
}

void test::testDeleteAfterTrade() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    int bobID = t->createUser("Bob");
    int johnID = t->createUser("John");
    vector<Trade*> empty;
    vector<Trade*> expected;
    expected.push_back(new Trade(7, 7, bobID, amyID));
    assert(t->placeSellOrder(amyID, 7, 10) == empty);
    assert(t->placeBuyOrder(bobID, 7, 7)[0]->equals(expected[0]));

    delete expected[0];
    expected.clear();
    t->deleteOrder(amyID, 0);

    assert(t->placeSellOrder(amyID, 8, 5) == empty);
    assert(t->placeSellOrder(johnID, 9, 1) == empty);
    assert(t->placeSellOrder(amyID, 9, 1) == empty);

    vector<Trade*> bobTrades = t->placeBuyOrder(bobID,10, 10);
    expected.push_back(new Trade(5, 8, bobID, amyID));
    expected.push_back(new Trade(1, 9, bobID, johnID));
    expected.push_back(new Trade(1, 9, bobID, amyID));

    for (int i = 0; i < 3; i++) {
        assert(bobTrades[i]->equals(expected[i]));
        delete expected[i];
    }

    assert(t->getPendingBuys()[0] == make_pair(10, 3));

    cout << "testDeleteAfterTrade passed" << endl;

    delete t;
}

void test::testBuyOut() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    int bobID = t->createUser("Bob");
    int johnID = t->createUser("John");
    vector<Trade*> empty;
    vector<Trade*> expected;
    assert(t->placeSellOrder(johnID, 9, 1) == empty);
    assert(t->placeSellOrder(amyID, 7, 10) == empty);
    assert(t->placeSellOrder(amyID, 8, 5) == empty);
    assert(t->placeSellOrder(amyID, 9, 1) == empty);

    vector<Trade*> bobTrades = t->placeBuyOrder(bobID,10, 20);
    expected.push_back(new Trade(10, 7, bobID, amyID));
    expected.push_back(new Trade(5, 8, bobID, amyID));
    expected.push_back(new Trade(1, 9, bobID, johnID));
    expected.push_back(new Trade(1, 9, bobID, amyID));

    for (int i = 0; i < 4; i++) {
        assert(bobTrades[i]->equals(expected[i]));
        delete expected[i];
    }

    assert(t->getPendingBuys()[0] == make_pair(10, 3));

    cout << "testBuyOut passed" << endl;

    delete t;
}

void test::testSellOut() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    int bobID = t->createUser("Bob");
    int johnID = t->createUser("John");
    vector<Trade*> empty;
    vector<Trade*> expected;
    assert(t->placeBuyOrder(amyID, 7, 10) == empty);
    assert(t->placeBuyOrder(amyID, 8, 5) == empty);
    assert(t->placeBuyOrder(johnID, 9, 1) == empty);
    assert(t->placeBuyOrder(amyID, 9, 1) == empty);

    vector<Trade*> bobTrades = t->placeSellOrder(bobID, 6, 20);
    expected.push_back(new Trade(1, 9, johnID, bobID));
    expected.push_back(new Trade(1, 9, amyID, bobID));
    expected.push_back(new Trade(5, 8, amyID, bobID));
    expected.push_back(new Trade(10, 7, amyID, bobID));

    for (int i = 0; i < 4; i++) {
        assert(bobTrades[i]->equals(expected[i]));
        delete expected[i];
    }

    assert(t->getPendingSells()[0] == make_pair(6, 3));

    cout << "testSellOut passed" << endl;

    delete t;
}

void test::testAll() {
    TradeEngine *t = new TradeEngine();
    int amyID = t->createUser("Amy");
    int bobID = t->createUser("Bob");
    int johnID = t->createUser("John");
    int samID = t->createUser("Sam");
    vector<Trade*> empty;
    vector<Trade*> expected;
    assert(t->placeBuyOrder(amyID, 7, 10) == empty);
    assert(t->placeBuyOrder(amyID, 8, 5) == empty);
    assert(t->placeBuyOrder(johnID, 9, 1) == empty);
    assert(t->placeBuyOrder(amyID, 9, 1) == empty);
    assert(t->placeBuyOrder(samID, 5, 3) == empty);

    vector<Trade*> bobTrades = t->placeSellOrder(bobID, 6, 20);
    expected.push_back(new Trade(1, 9, johnID, bobID));
    expected.push_back(new Trade(1, 9, amyID, bobID));
    expected.push_back(new Trade(5, 8, amyID, bobID));
    expected.push_back(new Trade(10, 7, amyID, bobID));

    for (int i = 0; i < 4; i++) {
        assert(bobTrades[i]->equals(expected[i]));
        delete expected[i];
    }

    assert(t->getPendingSells()[0] == make_pair(6, 3));

    t->deleteOrder(bobID, 0);

    Trade *tr = new Trade(2, 5, samID, amyID);
    assert(t->placeSellOrder(amyID, 5, 2)[0]->equals(tr));
    delete tr;

    cout << "testAll passed" << endl;

    delete t;
}