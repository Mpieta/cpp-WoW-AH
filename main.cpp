#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "core/WoWAPI.h"
#include "core/AuctionHouse.h"
#include "GUI/App.h"

int main(int argc, char* argv[])
{
    wxEntry(argc, argv);
    return 0;
}