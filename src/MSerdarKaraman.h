#ifndef MSERDARKARAMAN_H
#define MSERDARKARAMAN_H

#include "MapInterface.h"
#include "ZoneCounterInterface.h"
#include <iostream>
#include <memory>

namespace MSerdarKaraman {
    class BitesMap : public MapInterface {//I used Bites prefix for preventing confusion with std::map
    public:

        BitesMap(const int width, const int height) {// There is no point of using const in here.
            m_height = height;
            m_width = width;
            m_mapArray = std::unique_ptr<bool[]>(new bool[m_width * m_height]);
        }

        [[deprecated("You can't change size of map")]]
        void SetSize(const int width, const int height) {
            // I think size of map should be constant after creating it using constructor.
            // If its need to be changed, I need to know how its effect the current map data,
            // There should be clear requirement from software architect.
        }

        // Returns size of map to solve.
        void GetSize(int &width, int &height) { // I think it would be better using std:pair as a return type.
            width = m_width;
            height = m_height;
        }

        // Sets border at given point.
        void SetBorder(const int x, const int y) {
            m_mapArray[y * m_width + x] = true;
        }

        // Clears border at given point.
        void ClearBorder(const int x, const int y) {
            m_mapArray[y * m_width + x] = false;
        }

        // Checks if there is a border at given point.
        bool IsBorder(const int x, const int y) {
            return m_mapArray[y * m_width + x];
        }

        // Show map contents.
        // If we change return type to std::ostream, we can use it for writing file or any other stream object.
        void Show() { //Also this need to be const member function.
            for (int row = 0; row < m_height; row++) {
                for (int col = 0; col < m_width; col++) {
                    if (IsBorder(col, row)) {
                        std::cout << "#";
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << std::endl;
            }
        }

    private:
        int m_width = 0;
        int m_height = 0;
        std::unique_ptr<bool[]> m_mapArray;
    };

    class ZoneCounter : public ZoneCounterInterface {
    public:

        // Feeds map instance into solution class, and initialize.
        void Init(MapInterface *map) {
            map->GetSize(m_width, m_height);
            isSolved=false;
            zoneNumber=0;
            m_charArray = std::unique_ptr<char[]>(new char[m_width * m_height]);

            for (int y = 0; y < m_height; y++) {
                for (int x = 0; x < m_width; x++) {
                    if (map->IsBorder(x, y)) {
                        m_charArray[y * m_width + x] = '#';
                    }
                    else {
                        m_charArray[y * m_width + x] = ' ';
                    }
                }
            }
        };

        // Recursive algorithm perfectly fits for this problem BUT it have a risk of stack overflow
        // That why its must be bounded in DO-178 projects.
        void fillZone(int x, int y, char sign){

            bool isvalidPoint = (x<m_width && x>=0 && y<m_height && y>=0);

            if(isvalidPoint && isPointEmpty(x,y))
            {
                setPoint(x, y, sign);
                fillZone(x + 1, y, sign);
                fillZone(x, y + 1, sign);
                fillZone(x - 1, y, sign);
                fillZone(x, y - 1, sign);
            }
        }

        void setPoint(int x, int y,char sign){
            bool isvalidPoint = (x<m_width && x>=0 && y<m_height && y>=0); // Do I need to add exception in here or am I overthinking ???
            if(isvalidPoint){
                m_charArray[y * m_width + x] = sign;
            }
        }

        char getPoint(int x, int y) const {
            return m_charArray[y * m_width + x];
        }
        bool isPointEmpty(int x,int y){
            return (m_charArray[y * m_width + x] == ' ');
        }

        // Counts zones in provided map, and return result.
        int Solve() { //This function have two reposibilty. Both Solving problem and returning result. Returning result should be a different function. To provent doublde solving (!) I have to add isSolved control.
            if(isSolved){
                return zoneNumber;
            }

            for (int y = 0; y < m_height; y++) {
                for (int x = 0; x < m_width; x++) {
                    if (isPointEmpty(x, y)) {
                        zoneNumber++;
                        // Start filling map with char equivalent of numbers and than symbols like :;<
                        fillZone(x,y,zoneNumber + '0');
                    }
                }
            }

            return zoneNumber;
        };

        // If we change return type to std::ostream, we can use it for writing file or any other stream object.
        void Show() const {
            for (int row = 0; row < m_height; row++) {
                for (int col = 0; col < m_width; col++) {
                    std::cout << m_charArray[row * m_width + col];
                }
                std::cout << std::endl;
            }
        }

    private:
        int m_width = 0;
        int m_height = 0;
        std::unique_ptr<char[]> m_charArray; //I think  2^8 - 1 different char is enough for filling map. (Actually 4 is already enough acording to four color map theorem)
        int zoneNumber=0;
        bool isSolved=false;
    };

    ZoneCounterInterface * getZoneCounter() {
        //TODO ??? I have no idea what's the purpose of this.
        return new ZoneCounter();
    }

}

#endif // MSERDARKARAMAN_H
