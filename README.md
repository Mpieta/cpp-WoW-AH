### General App Info
C++ Application that fetches data from World of Warcraft API to show information
about current auctions on the Auction House.

Currently, the App only works with EU auction house
but that can be easily changed, simply change all "eu" to "us" and "GB" to "US"
in request strings.

The app does not list item auctions (only consumables/enchants/materials etc. so all commodities)
as the item auction house API request is/was temporarily 
disabled by the game producer while developing the app

API does not support queries for individual items, therefore the entire
region's Auction House data needs to be fetched in one query, 
which may take a few seconds.

Auctions in the app might differ slightly compared to auctions
you can see in game dut to data in API being updated only once an hour

### Usage

The app requires a valid API token in **API_token.txt** text file,
if valid token is not provided the app will most likely crash after
trying to send any request.

https://develop.battle.net/documentation/guides/getting-started

### Required libraries

The app uses following libraries:
* cURL (8.1.2)
* nlohmann_json (3.11.2)
* wxWidgets (3.2.2.1#3)

To build the app you can simply clone the repository and build using 
existing CMakeLists.txt