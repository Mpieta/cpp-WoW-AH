//
// Created by Lenovo on 14/06/2023.
//

#ifndef TRACKER_WOWAPI_H
#define TRACKER_WOWAPI_H
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <utility>

#include "item.h"

using json = nlohmann::json;



class WoWAPI {

    std::string token {};
    int realmID = 549;
    std::string base_url {};
public:
    static int sentRequests;
    WoWAPI() {};
    WoWAPI(std::string token) : token(std::move(token)) {};

    /*gets itemIDS of items with name similar to given itemName using blizzard API*/
    std::vector<int> findItemsByName(const std::string &itemName);
    Item getItemByID(int id);

    json fetchAuctionHouseCommodities();
    json fetchAuctionHouseItems();
    std::string getItemImageURL(int itemID);


private:
    static std::string  makeApiRequest(const std::string& url);
    static std::string replaceAll(const std::string& input, char character, const std::string& replacement);
    bool isSubstring(const std::string& str, const std::string& sub);
};


#endif //TRACKER_WOWAPI_H
