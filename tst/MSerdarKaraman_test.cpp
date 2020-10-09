#include <gtest/gtest.h>
#include "MSerdarKaraman.h"

using namespace MSerdarKaraman;

TEST(MSerdarKaramanTest, NoBorder) {

    constexpr int myWidth = 5;
    constexpr int myHeight = 5;

    BitesMap myMap(myWidth, myHeight);

    ZoneCounter myZoneCounter;
    myZoneCounter.Init(&myMap);
    ASSERT_EQ(1, myZoneCounter.Solve());
}

TEST(MSerdarKaramanTest, SingelVerticalBorder) {

    constexpr int myWidth = 10;
    constexpr int myHeight = 15;

    BitesMap myMap(myWidth, myHeight);

    for (int i = 0; i < myHeight; i++) { //vertical line
        myMap.SetBorder(myWidth / 2, i);
    }

    ZoneCounter myZoneCounter;
    myZoneCounter.Init(&myMap);
    ASSERT_EQ(2, myZoneCounter.Solve());
}

TEST(MSerdarKaramanTest, IsBorderClearBorder) {

    constexpr int myWidth = 10;
    constexpr int myHeight = 15;

    BitesMap myMap(myWidth, myHeight);

    for (int i = 0; i < myHeight; i++) { //vertical line
        myMap.SetBorder(myWidth / 2, i);
    }

    ASSERT_FALSE(myMap.IsBorder(0,0));
    ASSERT_TRUE(myMap.IsBorder(5,0));
    myMap.ClearBorder(5,0);
    ASSERT_FALSE(myMap.IsBorder(5,0));

}

TEST(MSerdarKaramanTest, GetSize) {
    BitesMap myMap(10, 15);
    int width, height;
    myMap.GetSize(width,height);
    ASSERT_EQ(10,width);
    ASSERT_EQ(15,height);
}



TEST(MSerdarKaramanTest, 2Vertical3HorizontalBorder) {

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

    ZoneCounter myZoneCounter;
    myZoneCounter.Init(&myMap);
    ASSERT_EQ(12, myZoneCounter.Solve());
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
