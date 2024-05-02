//
// Created by u238 on 5/22/23.
//

#ifndef SANDBOX_MAPGENERATOR_H
#define SANDBOX_MAPGENERATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

#define DEFAULT_CHUNK_SIZE 50
#define DEFAULT_MAP_SIZE 20

using namespace std;


//! MapGenerator creates tile maps by using the chunk methodology
class MapGenerator {
private:
    //! size of chunk
    int chunkSize;

    //! Tile map size in Chunks, only in one direction
    int mapSize;

    //! Arbitrary island spawn point in chunk
    int point;

    //! Tile map
    vector<vector<int>> bigMatrix;

    //! max island width
    int maxWidth;
    //! min island width
    int minWidth;
    //! min island height
    int minHeight = 5;
    //! max island height
    int maxHeight = 30;
    //! default argument for the number generator is good enough for this implementation. A different seed gen may be provided in future
    std::random_device rd;
    std::mt19937 gen;

    //! Create empty chunk
    /*!
     * Creates a chunk size matrix of zeros
     * @return matrix of 0
     */
    [[nodiscard]] vector<vector<int>> createSkyChunk() const;

    //! Create island chunk
    /*!
     * Creates a chunk size matrix with a relatively random grouping of numbers in a 2D shape of an island;
     * @return
     */
    vector<vector<int>> createIslandChunk();

    //! Create chunk matrix
    /*!
     *
     * @param islandChance
     *
     * fills the matrix with the values of chunk matrices
     * EX:
     *
     * chunk1:
     * 1 2 3
     * 6 7 9
     * 0 0 0
     *
     * chunk2:
     * 1 7 9
     * 9 9 9
     * 7 7 7
     *
     * Map:
     * 1 2 3 1 7 9
     * 6 7 9 9 9 9
     * 0 0 0 7 7 7
     */
    void generateChunks(double islandChance);

    //! saves the resulting map to a .level file
    void save(const char *filename);

public:
    [[maybe_unused]] MapGenerator(int chunkSize, int mapSize);

    //! Default constructor
    MapGenerator();

    //! method to call other methods of class
    void generateNewMap(const char *filename, double islandDensity);

    //! get island start point
    [[nodiscard]] int getPoint() const;
};


#endif //SANDBOX_MAPGENERATOR_H
