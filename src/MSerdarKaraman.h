#ifndef MSERDARKARAMAN_H
#define MSERDARKARAMAN_H

#include "MapInterface.h"
#include "ZoneCounterInterface.h"
#include <iostream>
#include <memory>

/**
 * @namespace MSerdarKaraman
 * @brief Contains the concrete implementations of MapInterface and
 *        ZoneCounterInterface for the BITES interview problem.
 */
namespace MSerdarKaraman {

/**
 * @brief Concrete implementation of MapInterface backed by a flat boolean array.
 *
 * BitesMap stores a 2-D grid of cells where each cell is either a border (@c true)
 * or empty (@c false). The grid dimensions are fixed at construction time.
 *
 * @note The "Bites" prefix is used to avoid ambiguity with @c std::map.
 */
class BitesMap : public MapInterface  //I used Bites prefix for preventing confusion with std::map
{
public:

    /**
     * @brief Constructs a BitesMap with the given dimensions.
     *
     * All cells are initialised to empty (not a border).
     *
     * @param width  Number of columns in the map.
     * @param height Number of rows in the map.
     */
    BitesMap(const int width, const int height)  // There is no point of using const in here.
    {
        m_height = height;
        m_width = width;
        m_mapArray = std::unique_ptr<bool[]>(new bool[m_width * m_height]);
    }

    /**
     * @brief Deprecated – the map size cannot be changed after construction.
     * @deprecated Use the constructor to set the map dimensions instead.
     * @param width  Ignored.
     * @param height Ignored.
     */
    [[deprecated("You can't change size of map")]]
    void SetSize(const int width, const int height)
    {
        // I think size of map should be constant after creating it using constructor.
        // If its need to be changed, I need to know how its effect the current map data,
        // There should be clear requirement from software architect.
    }

    /**
     * @brief Returns the current size of the map.
     * @param[out] width  Receives the number of columns.
     * @param[out] height Receives the number of rows.
     */
    void GetSize(int &width, int
                 &height)   // I think it would be better using std:pair as a return type.
    {
        width = m_width;
        height = m_height;
    }

    /**
     * @brief Marks the cell at (x, y) as a border.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     */
    void SetBorder(const int x, const int y)
    {
        m_mapArray[y * m_width + x] = true;
    }

    /**
     * @brief Removes the border mark from the cell at (x, y).
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     */
    void ClearBorder(const int x, const int y)
    {
        m_mapArray[y * m_width + x] = false;
    }

    /**
     * @brief Checks whether the cell at (x, y) is a border.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @return @c true if the cell is a border, @c false otherwise.
     */
    bool IsBorder(const int x, const int y)
    {
        return m_mapArray[y * m_width + x];
    }

    /**
     * @brief Prints the map to standard output.
     *
     * Border cells are rendered as @c '#' and empty cells as @c '.'.
     */
    void Show()   //Also this need to be const member function.
    {
        for (int row = 0; row < m_height; row++)
        {
            for (int col = 0; col < m_width; col++)
            {
                if (IsBorder(col, row))
                {
                    std::cout << "#";
                }
                else
                {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    int m_width = 0;   ///< Number of columns in the map.
    int m_height = 0;  ///< Number of rows in the map.
    std::unique_ptr<bool[]> m_mapArray; ///< Flat array storing cell states (row-major).
};

/**
 * @brief Concrete implementation of ZoneCounterInterface using a flood-fill algorithm.
 *
 * ZoneCounter copies the map state from a MapInterface instance and uses a
 * recursive flood-fill to label each contiguous empty region with a unique
 * character, counting the total number of such regions.
 *
 * @warning The recursive flood-fill may cause a stack overflow on very large maps.
 *          This must be taken into account in safety-critical (e.g. DO-178) projects.
 */
class ZoneCounter : public ZoneCounterInterface
{
public:

    /**
     * @brief Initialises the zone counter with the given map.
     *
     * Reads the map dimensions and cell states and prepares internal storage
     * for the flood-fill algorithm. Resets any previously computed result.
     *
     * @param map Pointer to a MapInterface instance to analyse. Must not be
     *            @c nullptr.
     */
    void Init(MapInterface *map)
    {
        map->GetSize(m_width, m_height);
        isSolved = false;
        zoneNumber = 0;
        m_charArray = std::unique_ptr<char[]>(new char[m_width * m_height]);

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                if (map->IsBorder(x, y))
                {
                    m_charArray[y * m_width + x] = '#';
                }
                else
                {
                    m_charArray[y * m_width + x] = ' ';
                }
            }
        }
    };

    /**
     * @brief Counts the number of contiguous empty zones in the map.
     *
     * On the first call the flood-fill algorithm is executed and the zone
     * count is stored. Subsequent calls return the cached result without
     * recomputing.
     *
     * @return The total number of distinct contiguous empty zones found.
     */
    int Solve()   //This function have two reposibilty. Both Solving problem and returning result. Returning result should be a different function. To provent doublde solving (!) I have to add isSolved control.
    {
        if (isSolved)
        {
            return zoneNumber;
        }

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                if (isPointEmpty(x, y))
                {
                    zoneNumber++;
                    // Start filling map with char equivalent of numbers and than symbols like :;<
                    fillZone(x, y, zoneNumber + '0');
                }
            }
        }

        return zoneNumber;
    };

    /**
     * @brief Prints the labelled zone map to standard output.
     *
     * Border cells are shown as @c '#' and each empty zone is shown with a
     * unique character (starting from @c '1').
     */
    void Show() const
    {
        for (int row = 0; row < m_height; row++)
        {
            for (int col = 0; col < m_width; col++)
            {
                std::cout << m_charArray[row * m_width + col];
            }
            std::cout << std::endl;
        }
    }

private:
    /**
     * @brief Recursively fills a contiguous empty region starting at (x, y).
     *
     * Visits all four cardinal neighbours and marks each empty cell with
     * @p sign. The recursion terminates when it reaches a border cell or the
     * map boundary.
     *
     * @param x    Column index of the starting cell.
     * @param y    Row index of the starting cell.
     * @param sign Character label to assign to cells in this zone.
     */
    void fillZone(int x, int y, char sign)
    {
        bool isvalidPoint = (x < m_width && x >= 0 && y < m_height && y >= 0);
        if (isvalidPoint && isPointEmpty(x, y))
        {
            setPoint(x, y, sign);
            fillZone(x + 1, y, sign);
            fillZone(x, y + 1, sign);
            fillZone(x - 1, y, sign);
            fillZone(x, y - 1, sign);
        }
    }

    /**
     * @brief Sets the character at cell (x, y) to @p sign.
     * @param x    Column index (0-based).
     * @param y    Row index (0-based).
     * @param sign Character to store in the cell.
     */
    void setPoint(int x, int y, char sign)
    {
        bool isvalidPoint = (x < m_width && x >= 0 && y < m_height
                             && y >= 0); // Do I need to add exception in here or am I overthinking ???
        if (isvalidPoint)
        {
            m_charArray[y * m_width + x] = sign;
        }
    }

    /**
     * @brief Returns the character stored at cell (x, y).
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @return The character stored at the specified cell.
     */
    char getPoint(int x, int y) const
    {
        return m_charArray[y * m_width + x];
    }

    /**
     * @brief Checks whether the cell at (x, y) has not yet been assigned to a zone.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @return @c true if the cell is empty (space character), @c false otherwise.
     */
    bool isPointEmpty(int x, int y)
    {
        return (m_charArray[y * m_width + x] == ' ');
    }

    int m_width = 0;   ///< Number of columns in the map.
    int m_height = 0;  ///< Number of rows in the map.
    std::unique_ptr<char[]>
    m_charArray; //I think  2^8 - 1 different char is enough for filling map. (Actually 4 is already enough acording to four color map theorem)
    int zoneNumber = 0;  ///< Number of zones found so far.
    bool isSolved = false; ///< Whether Solve() has already been executed.

};

/**
 * @brief Factory function that creates and returns a new ZoneCounter instance.
 * @return A heap-allocated ZoneCounterInterface pointer to a ZoneCounter.
 */
ZoneCounterInterface *getZoneCounter()
{
    //TODO ??? I have no idea what's the purpose of this.
    return new ZoneCounter();
}

}

#endif // MSERDARKARAMAN_H
