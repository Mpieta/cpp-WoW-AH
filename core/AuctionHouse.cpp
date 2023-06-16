#include "AuctionHouse.h"

AuctionHouse::AuctionHouse(const std::string& token) : api(token){}

std::vector<AuctionEntry> AuctionHouse::query(const std::string& itemName) {
    std::vector<int> ids = this->api.findItemsByName(itemName);
    std::vector<AuctionEntry> results;
    for(const int& id : ids){
        if(this->commodityAuctions.contains(id)){
            if(!this->queriedItems.contains(id)){
                Item i = this->api.getItemByID(id);
                this->queriedItems[id] = i;
            }
            results.insert(results.end(), this->commodityAuctions[id].begin(), this->commodityAuctions[id].end());
        }
    }
    return results;
}

Item AuctionHouse::idToItem(int id) {
    if(this->queriedItems.contains(id)){
        return this->queriedItems[id];
    }
    throw std::exception("Item not found");
}

void AuctionHouse::fetchData() {
    json commoditiesJson = this->api.fetchAuctionHouseCommodities();
    for (auto result: commoditiesJson["auctions"]) {
        int itemID = result["item"]["id"];
        int quantity = result["quantity"];
        long unsigned int unit_price = result["unit_price"];
        std::string time = result["time_left"];
        AuctionEntry ae(itemID, quantity, unit_price, time);
        this->commodityAuctions[itemID].push_back(ae);
    }

    for (auto &pair: commodityAuctions) {
        std::vector<AuctionEntry> &entries = pair.second;
        std::sort(entries.begin(), entries.end());
    }

    this->isFetched = true;

}

void AuctionHouse::fillItemImage(int id) {
    if(queriedItems.contains(id)){
        if(queriedItems[id].getIMG().empty()){
            std::string url = api.getItemImageURL(id);
            queriedItems[id].setIMG(url);
        }
    }
}

std::string AuctionHouse::getIMGbyID(int id) {
    if(queriedItems.contains(id)){
        if(queriedItems[id].getIMG().empty()){
            fillItemImage(id);
        }

        return queriedItems[id].getIMG();
    }

    return {};

}
