//
// Created by Lenovo on 15/06/2023.
//

#ifndef TRACKER_ITEM_H
#define TRACKER_ITEM_H

#endif //TRACKER_ITEM_H
#include <string>
#include <utility>
#include <iostream>

class Item {
    int itemID;
    std::string itemName;
    std::string itemDesc;
    std::string imageURL;

public:
    Item() : itemID(0), itemName(""), itemDesc(""), imageURL("") {}

    Item(int id, std::string name, std::string desc, std::string img)
            : itemID(id), itemName(name), itemDesc(desc), imageURL(img) {};

    Item(const Item& other)
            : itemID(other.itemID), itemName(other.itemName), itemDesc(other.itemDesc), imageURL(other.imageURL) {};

    Item(Item&& other) noexcept
            : itemID(other.itemID), itemName(std::move(other.itemName)), itemDesc(std::move(other.itemDesc)), imageURL(std::move(other.imageURL)) {};

    Item& operator=(const Item& other) {
        if (this != &other) {
            itemID = other.itemID;
            itemName = other.itemName;
            itemDesc = other.itemDesc;
            imageURL = other.imageURL;
        }
        return *this;
    }

    [[nodiscard]] std::string getName() const { return this->itemName;};

    [[nodiscard]] std::string getIMG() const {return this->imageURL;};

    void setIMG(std::string url) { this->imageURL = std::move(url);};

    [[nodiscard]] std::string getDesc() const {return this->itemDesc;};

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << "Item ID: " << item.itemID << std::endl;
        os << "Item Name: " << item.itemName << std::endl;
        os << "Item Description: " << item.itemDesc << std::endl;
        os << "Image URL: " << item.imageURL << std::endl;
        return os;
    }
};