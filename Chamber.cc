#include "Chamber.h"

using namespace std;

Chamber::Chamber(vector<pair<int, int>> coordinates) 
    : hasPlayer(false), coordinates(coordinates) {}

pair<int, int> Chamber::getRandomCoordinate() const {
    int randomIndex = rand() % coordinates.size();
    return coordinates[randomIndex];
}

vector<pair<int, int>> Chamber::getCoordinates() const {
    return coordinates;
}
