#ifndef CHAMBER_H
#define CHAMBER_H

#include <vector>
#include <string>

class Character;
class Potion;
class Treasure;
class Enemy;

class Chamber
{
private:
    std::vector<std::pair<int,int>> coordinates; // to keep track of "dots" that belong to this chamber

public:
    bool hasPlayer;
    Chamber(std::vector<std::pair<int, int>> coordinates);
    std::pair<int, int> getRandomCoordinate() const;
    std::vector<std::pair<int, int>> getCoordinates() const;
};

#endif
