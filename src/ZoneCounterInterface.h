#ifndef HELLO_WORLD_ZONECOUNTERINTERFACE_H
#define HELLO_WORLD_ZONECOUNTERINTERFACE_H
#include "MapInterface.h"

/**
 * @brief Abstract interface for counting contiguous empty zones in a map.
 *
 * ZoneCounterInterface defines the contract for algorithms that count the
 * number of distinct contiguous regions of empty cells in a MapInterface
 * instance. Cells that are not borders are considered empty and connected
 * cells form a single zone.
 */
class ZoneCounterInterface
{
public:
    virtual ~ZoneCounterInterface() {}

    /**
     * @brief Initialises the zone counter with the given map.
     *
     * Reads the map dimensions and cell states from @p map and prepares
     * the internal state for a subsequent call to Solve().
     *
     * @param map Pointer to a MapInterface instance to analyse. Must not be
     *            @c nullptr.
     */
    virtual void Init(MapInterface *map) = 0;

    /**
     * @brief Counts the number of contiguous empty zones in the map.
     *
     * Performs the zone-counting algorithm on the map supplied to Init().
     * Calling Solve() multiple times returns the same result without
     * recomputing.
     *
     * @return The total number of distinct contiguous empty zones found.
     */
    virtual int Solve() = 0;
};


#endif //HELLO_WORLD_ZONECOUNTERINTERFACE_H
