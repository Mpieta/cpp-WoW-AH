//
// Created by Lenovo on 15/06/2023.
//

#ifndef TRACKER_AUCTIONENTRY_H
#define TRACKER_AUCTIONENTRY_H

#include <string>


class AuctionEntry {
public:
    int itemID;
    int quantity;
    long unsigned int unitPrice;
    std::string timeLeft;

public:
    AuctionEntry(int id, int quant, long unsigned int up, std::string time) : itemID(id), quantity(quant), unitPrice(up), timeLeft(time) {};

    bool operator<(const AuctionEntry& other) const {
        return unitPrice < other.unitPrice;
    }

    [[nodiscard]] std::string getPriceString() const{
        long unsigned int silver = unitPrice/100;
        long unsigned int copper = unitPrice%100;
        long unsigned int gold = silver/100;
        silver %= 100;

        return std::to_string(gold) + " G  " + std::to_string(silver) + " S  " + std::to_string(copper) + " C";

    }
};


#endif //TRACKER_AUCTIONENTRY_H
