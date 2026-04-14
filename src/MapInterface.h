#ifndef HELLO_WORLD_MAPINTERFACE_H
#define HELLO_WORLD_MAPINTERFACE_H

/**
 * @brief Abstract interface for a 2D grid map used in zone counting.
 *
 * MapInterface defines the operations required to manage a rectangular grid
 * where each cell can either be a border or an empty cell. Implementations
 * must provide concrete definitions for all pure virtual methods.
 */
class MapInterface
{
public:
    virtual ~MapInterface() {}

    /**
     * @brief Creates a map of the given size.
     * @param width  Number of columns in the map.
     * @param height Number of rows in the map.
     */
    virtual void SetSize(const int width, const int height) = 0;

    /**
     * @brief Returns the current size of the map.
     * @param[out] width  Receives the number of columns.
     * @param[out] height Receives the number of rows.
     */
    virtual void GetSize(int &width, int &height) = 0;

    /**
     * @brief Marks the cell at (x, y) as a border.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     */
    virtual void SetBorder(const int x, const int y) = 0;

    /**
     * @brief Removes the border mark from the cell at (x, y).
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     */
    virtual void ClearBorder(const int x, const int y) = 0;

    /**
     * @brief Checks whether the cell at (x, y) is a border.
     * @param x Column index (0-based).
     * @param y Row index (0-based).
     * @return @c true if the cell is a border, @c false otherwise.
     */
    virtual bool IsBorder(const int x, const int y) = 0;

    /**
     * @brief Prints the map contents to standard output.
     */
    virtual void Show() = 0;
};


#endif //HELLO_WORLD_MAPINTERFACE_H
