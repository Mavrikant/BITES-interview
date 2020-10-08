#include <iostream>
#include <memory>
#include "MapInterface.h"
#include "ZoneCounterInterface.h"

namespace MSerdarKaraman {
    class Map : public MapInterface {//Using map name creates confusion with std::map
    public:

        Map(const int width, const int height) {// There is no point of using const in here.
            m_height = height;
            m_width = width;
            m_mapArray = std::unique_ptr<bool[]>(new bool[m_width * m_height]);
        }

        [[deprecated("You can't change size of map")]]
        void SetSize(const int width, const int height) {
            // Bence SetSize'ın kaldırılıp bunun constructor haline getirilmesi lazım.
            // Bir harita oluşturulurken size'ı bellidir ve sonradan içerisi dolu iken değiştirmemeli.
        }

        // Returns size of map to solve.
        void GetSize(int &width, int &height) { // Bence bu fonksiyonun dönüş değeri std::pair olsaydı daha uygun olabilirdi.
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
        // Ekrana yazdırmaktansa std::ostream'a yazdırsaydık gelecekte dosyaya yazmak için de kullanabilirdik.
        // Interface'de dönüş değeri void olduğu için değiştirmiyorum.
        void Show() {
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

        ~Map() {
            //TODO ?
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
                        m_charArray[y * m_width + x]=borderSign;
                    }
                    else {
                        m_charArray[y * m_width + x]=' ';
                    }
                }
            }
        };

        // Recursive algoritma bu problem için çok uygun ama safety-critical yazılımlar için sorun çıkarabilir.
        // Çok derine inen bir recurive çağrı stack ve RAM'i patlatabileceği için DO-178 geregi recurive çağrıların
        // sınırlandırılmış olması gerekiyor.
        void fillZone(int x, int y, char sign){

            if(x>=m_width || x<0 || y>=m_height || y<0){
                return;
            }
            if(isPointEmpty(x,y))
            {
                setPoint(x, y, sign);
                fillZone(x + 1, y, sign);
                fillZone(x, y + 1, sign);
                fillZone(x - 1, y, sign);
                fillZone(x, y - 1, sign);
            }
        }

        inline void setPoint(int x, int y,char sign){
            m_charArray[y * m_width + x] = sign;
        }

        inline char getPoint(int x, int y){
            return m_charArray[y * m_width + x];
        }
        inline bool isPointEmpty(int x,int y){
            return (m_charArray[y * m_width + x] == ' ');
        }

        // Counts zones in provided map, and return result.
        int Solve() {
            if(isSolved){
                return zoneNumber;
            }

            for (int y = 0; y < m_height; y++) {
                for (int x = 0; x < m_width; x++) {
                    if (isPointEmpty(x, y)) {
                        zoneNumber++;
                        // 1'den 9'a kadar rakamları yazıyor. Sonra :;< gibi işaretlere başlıyor.
                        // Haritayı görselleştirmek ve debug için alanları farklı işaretler ile boyamak güzel bir çözüm bence.
                        fillZone(x,y,zoneNumber + '0');
                    }
                }
            }

            return zoneNumber;
        };

        // Ekrana yazdırmaktansa std::ostream'a yazdırsaydık gelecekte dosyaya yazmak için de kullanabilirdik.
        // Interface'de dönüş değeri void olduğu için değiştirmiyorum.
        void Show() {
            for (int row = 0; row < m_height; row++) {
                for (int col = 0; col < m_width; col++) {
                    std::cout << m_charArray[row * m_width + col];
                }
                std::cout << std::endl;
            }
        }

    private:
        const char borderSign = '#';
        int m_width = 0;
        int m_height = 0;
        std::unique_ptr<char[]> m_charArray; //Çok yer kaplamasın diye burayı char array tanımladım. 2^8 - 1 farklı karakter alan işaretlemek için yeterli bence.
        int zoneNumber=0;
        bool isSolved=false;
    };

//    ZoneCounterInterface * getZoneCounter() {
//
//        //TODO
//
//        return new ZoneCounter(...);
//    }
}




int main() {
    using namespace MSerdarKaraman;

    const int myWidth = 50;
    const int myHeight = 15;

    Map myMap(myWidth, myHeight);

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
