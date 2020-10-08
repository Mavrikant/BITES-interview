#include <iostream>
#include <memory>
#include "MSerdarKaraman.h"


int main() {
    using namespace MSerdarKaraman;

    constexpr int myWidth = 50;
    constexpr int myHeight = 15;

    BitesMap myMap(myWidth, myHeight);

    for (int i = 0; i < myHeight; i++) { //vertical line
        myMap.SetBorder(myWidth / 3, i);
        myMap.SetBorder(2*myWidth / 3, i);
    }

    for (int i = 0; i < myWidth; i++) { //horizontal line
        myMap.SetBorder(i, myHeight / 4);
        myMap.SetBorder(i, myHeight / 2);
        myMap.SetBorder(i, 3*myHeight / 4);
    }

    std::cout << "Empty Map\n" ;
    myMap.Show();
    std::cout << "\n\n";

    ZoneCounter myZoneCounter;
    myZoneCounter.Init(&myMap);
    myZoneCounter.Solve();

    std::cout << "Filled Map\n";
    myZoneCounter.Show();
    std::cout << "\n\n";

    std::cout << "Total number of zones:" << myZoneCounter.Solve() << "\n";
    return 0;
}
