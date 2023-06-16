
#include "WoWAPI.h"
int WoWAPI::sentRequests = 0;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string WoWAPI::makeApiRequest(const std::string &url){
    CURL* curl = curl_easy_init();
        if (curl) {
            sentRequests++;
            std::string response;

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
            return response;
        }
        return "";
}

std::vector<int> WoWAPI::findItemsByName(const std::string &itemName) {
    std::vector<int> ids;
    int pagenum = 1;

    std::string baseURL = "https://eu.api.blizzard.com/data/wow/search/item?namespace=static-eu&name.en_GB=";
    std::string newName = WoWAPI::replaceAll(itemName, ' ', "%20");
    std::string page_string = "&orderby=id&_pageSize=1000&_page=" + std::to_string(pagenum);
    std::string request = baseURL + newName + page_string + "&access_token=" + this->token;
    std::string response = WoWAPI::makeApiRequest(request);
    if (!response.empty()) {
        json jsonResponse = json::parse(response);
        for (auto result: jsonResponse["results"]) {
            int id = result["data"]["id"];
            std::string name = result["data"]["name"]["en_GB"];
            if (WoWAPI::isSubstring(name, itemName)) {
                ids.push_back(id);
            }
        }
        while(jsonResponse["pageCount"] > jsonResponse["page"]) {
            pagenum++;
            page_string = "&orderby=id&_pageSize=1000&_page=" + std::to_string(pagenum);
            request = baseURL + newName + page_string + "&access_token=" + this->token;
            response = WoWAPI::makeApiRequest(request);
            if (!response.empty()) {
                jsonResponse = json::parse(response);
                for (auto result: jsonResponse["results"]) {
                    int id = result["data"]["id"];
                    std::string name = result["data"]["name"]["en_GB"];
                    if (WoWAPI::isSubstring(name, itemName)) {
                        ids.push_back(id);
                    }
                }
            }
        }
        return ids;
    }
}

std::string WoWAPI::replaceAll(const std::string& input, char character, const std::string& replacement) {
    std::string result = input;
    size_t pos = result.find(character);
    while (pos != std::string::npos) {
        result.replace(pos, 1, replacement);
        pos = result.find(character, pos + replacement.length());
    }

    return result;
}

bool WoWAPI::isSubstring(const std::string& str, const std::string& sub) {
    std::string lowerStr = str;
    std::string lowerSub = sub;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    std::transform(lowerSub.begin(), lowerSub.end(), lowerSub.begin(), ::tolower);

    return lowerStr.find(lowerSub) != std::string::npos;
}

json WoWAPI::fetchAuctionHouseCommodities() {
    std::string base = "https://eu.api.blizzard.com/data/wow/auctions/commodities";
    std::string request = base + "?namespace=dynamic-eu&locale=en_GB&" + "access_token=" + this->token;

    std::string response = WoWAPI::makeApiRequest(request);
    if(!response.empty()){
        json jsonResponse = json::parse(response);
        return jsonResponse;
    }
    return {};
}

json WoWAPI::fetchAuctionHouseItems() {
    std::string base = "https://eu.api.blizzard.com/data/wow/connected-realm/";
    std::string locale = "/auctions?namespace=dynamic-eu&locale=en_GB";
    std::string request = base + std::to_string(this->realmID) + locale + "&access_token=" + this->token;

    std::string response = WoWAPI::makeApiRequest(request);
    if(!response.empty()){
        json jsonResponse = json::parse(response);
        return jsonResponse;
    }
    return {};
}

std::string WoWAPI::getItemImageURL(int itemID) {
    std::string base ="https://eu.api.blizzard.com/data/wow/media/item/";
    std::string locale = "?namespace=static-eu&locale=en_GB";
    std::string request = base + std::to_string(itemID) + locale + "&access_token=" + this->token;

    std::string response = WoWAPI::makeApiRequest(request);
    if(!response.empty()){
        json jsonResponse = json::parse(response);
        return jsonResponse["assets"][0]["value"];
    }
    return {};
}

Item WoWAPI::getItemByID(int id) {
    std::string base = "https://eu.api.blizzard.com/data/wow/item/";
    std::string locale = "?namespace=static-eu&locale=en_GB";
    std::string request = base + std::to_string(id) + locale + "&access_token=" + this->token;

    std::string response = WoWAPI::makeApiRequest(request);
    if(!response.empty()){

        json jsonResponse = json::parse(response);
        std::string name = jsonResponse["name"];
        std::string desc;
        if(jsonResponse.contains("preview_item")){
            if(jsonResponse["preview_item"].contains("spells")){
                desc=jsonResponse["preview_item"]["spells"][0]["description"];
            }
            else if(jsonResponse.contains("modified_crafting")){
                desc=jsonResponse["modified_crafting"]["description"];
            }
        }
        std::string imageURL;

        return {id,name,desc,imageURL};
    }
}
