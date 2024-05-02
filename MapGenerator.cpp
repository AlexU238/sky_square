//
// Created by u238 on 5/22/23.
//

#include "MapGenerator.h"

//default argument for the number generator is good enough for this implementation. A different seed gen may be provided in future
[[maybe_unused]] MapGenerator::MapGenerator(int chunkSize, int mapSize) : chunkSize(chunkSize), mapSize(mapSize) {
    if (chunkSize <= 20) {
        chunkSize += 20;
    }
    bigMatrix.resize(chunkSize * mapSize, std::vector<int>(chunkSize * mapSize));
    point = static_cast<int>(((sqrt(2) * chunkSize) * 0.15) /
                             sqrt(2)); // island generation point relative to chunk size
    maxWidth = chunkSize - 9;
    minWidth = point - 3;
}

MapGenerator::MapGenerator() {
    chunkSize = DEFAULT_CHUNK_SIZE;
    mapSize = DEFAULT_MAP_SIZE;
    bigMatrix.resize(chunkSize * mapSize, std::vector<int>(chunkSize * mapSize));
    point = static_cast<int>(((sqrt(2) * chunkSize) * 0.15) /
                             sqrt(2)); // island generation point relative to chunk size
    maxWidth = chunkSize - 9;
    minWidth = point - 3;
}

vector<vector<int>> MapGenerator::createSkyChunk() const {
    vector<vector<int>> chunk(chunkSize, std::vector<int>(chunkSize));
    for (int i = 0; i < chunkSize; i++) {
        chunk[i].assign(chunkSize, 0);
    }
    return chunk;
}

vector<vector<int>> MapGenerator::createIslandChunk() {
    vector<vector<int>> chunk(chunkSize, std::vector<int>(chunkSize));
    for (int i = 0; i < chunkSize; i++) {
        chunk[i].assign(chunkSize, 0);
    }
    std::uniform_int_distribution<int> width(minWidth, maxWidth);
    std::uniform_int_distribution<int> height(minHeight, maxHeight);
    std::uniform_int_distribution<int> dec(0, 2);
    int stW = width(gen);
    int enW = width(gen);
    if (stW > enW) {
        int tmp = enW;
        enW = stW;
        stW = tmp;
    }

    int enH = height(gen);
    for (int j = point; j <= enH; j++) {
        for (int i = stW; i < enW + 1; i++) {
            if (j == point) {
                chunk[j][i] = 1;
            } else chunk[j][i] = 2;
        }
        stW = stW + dec(gen);
        enW = enW - dec(gen);
        if (stW >= enW) break;
    }
    return chunk;
}

void MapGenerator::generateChunks(double islandChance) {
    cout << "Generating chunks..." << endl;
    uniform_real_distribution<> chance(0.0, 1.0);
    int posX = 0; //vertical in matrix
    int posY = 0; //horizontal in matrix
    vector<vector<int>> chunk = createIslandChunk();

    for (int j = 0; j < chunkSize; j++) {
        for (int k = 0; k < chunkSize; k++) {
            bigMatrix[posX + j][posY + k] = chunk[j][k];
        }
    }
    posY += chunkSize;
    //get a sky or island chunk based on chance
    for (int i = 1; i < mapSize * mapSize; i++) {
        if (chance(gen) < islandChance) {
            chunk = createIslandChunk();
        } else {
            chunk = createSkyChunk();
        }

        // Add the sub-matrix to the big matrix
        for (int j = 0; j < chunkSize; j++) {
            for (int k = 0; k < chunkSize; k++) {
                bigMatrix[posX + j][posY + k] = chunk[j][k];
            }
        }
        posY += chunkSize;
        if (posY == mapSize * chunkSize) {
            posY = 0;
            posX += chunkSize;
        }
    }
    cout << "Chunks generated successfully." << endl;
}

void MapGenerator::save(const char *filename) {
    cout << "Creating level..." << endl;
    int size = chunkSize * mapSize;
    ofstream outputFile(filename);
    if (outputFile.is_open()) {
        cout << "Level file opened successfully." << endl;
        cout << "Writing to file..." << endl;
        outputFile << size << " " << size << endl;
        outputFile << 0 << " " << 0 << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                outputFile << bigMatrix[i][j] << " ";
            }
            outputFile << endl;
        }
        cout << "Writing finished." << endl;
        outputFile.close();
        cout << "Level file saved successfully." << endl;
    } else {
        cout << "Failed to open the level file." << endl;
    }
    cout << "Level created successfully." << endl;
}

void MapGenerator::generateNewMap(const char *filename, double islandDensity) {
    generateChunks(islandDensity);
    save(filename);
}

int MapGenerator::getPoint() const {
    return point;
}








