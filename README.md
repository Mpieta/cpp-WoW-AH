### General App Info
C++ Application that fetches data from World of Warcraft API to show information
about current auctions on the Auction House.

Currently, the App only works with EU auction house
but that can be easily changed, simply change all "eu" to "us" (or other region) and "GB" to "US"
(or other language)
in request strings.

The app does not list item auctions (only consumables/enchants/materials etc. so all commodities)
as the item auction house API request is/was temporarily 
disabled by the game producer while developing the app

API does not support queries for individual items, therefore the entire
region's Auction House data needs to be fetched in one query, 
which may take a few seconds.

Auctions in the app might differ slightly compared to auctions
you can see in game due to data in API being updated only once an hour

### Usage

The app requires a valid API token in **API_token.txt** text file,
if valid token is not provided the app will exit  after
trying to send any request.

[Here](https://develop.battle.net/documentation/guides/getting-started) is how you can get your own API access token

To use the app first use "Fetch/Update Data" button to fetch EU auction house data, and then type the name
(or part of the name) of an item you want to look up.




![results of "potion" query](example.png)
<p align="center">
<sup>results of "potion" search</sup>
</p>

### Required libraries

The app uses following libraries:
* cURL (8.1.2)
* nlohmann_json (3.11.2)
* wxWidgets (3.2.2.1#3)