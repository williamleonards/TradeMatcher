# TradeMatcher
A simple implementation of an order matching engine. Supports buy/sell orders with integer amounts (volume). The engine matches any incoming order with pre-existing ones, creating trades that follow the prices of pre-existing orders.

When a buy order is placed, the engine tries to match some (or all) of the amount with current pending sell orders. If there are matches, a trade occurs following the price of the pending sell order. Any untraded amount of this new order goes to the buy heap (pending buy orders).

Similarly, when a sell order is placed, the engine tries to match the current pending buy orders. Whenever a match occurs, a trade occurs following the price of the pending buy order. Any untraded amount of this new order goes to the sell heap (pending sell orders).

The engine also supports efficient deletion of a pending order.

Additional features include viewing the buy/sell heap and viewing pending orders and trade history from the perspective of a user.

The buy/sell heap is returned as a list of pairs containing the price point and its volume.
