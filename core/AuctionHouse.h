#ifndef TRACKER_AUCTIONHOUSE_H
#define TRACKER_AUCTIONHOUSE_H


#include <unordered_map>
#include <string>
//#include <thread>

#include "WoWAPI.h"
#include "AuctionEntry.h"

using json = nlohmann::json;
class AuctionHouse {
public:
    WoWAPI api {};
    std::unordered_map<int, std::vector<AuctionEntry> > commodityAuctions {}; //itemID -> vector<AuctionEntry>
    std::unordered_map<int, Item> queriedItems; //map of items queried so far

    bool isFetched = false;

public:
    AuctionHouse() : api(), commodityAuctions(), queriedItems() {};

    AuctionHouse(const std::string& token);

    std::vector<AuctionEntry> query(const std::string& itemName);

    Item idToItem(int id);

    void fetchData();



    [[nodiscard]] std::string getIMGbyID(int id);

private:
    void fillItemImage(int id);
};


#endif //TRACKER_AUCTIONHOUSE_H
