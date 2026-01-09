#include "analyzer.h"
#include <fstream>        // To open file (std::ifstream)
#include <sstream>        // For string manipulation
#include <iostream>       // For printing errors (we use it to make sure that there is no error in our code)
#include <algorithm>      // for using sort algorithm (std::sort)

// Students may use ANY data structure internally
// =============================================
// 2361862 - TAREQ IYAD MOHAMMED AKRAM SHWAIKA
// 2361790 - YOUSEF SALAMA
// 2477317 - WALID M W KOBAHALABI
// =============================================

//===============================================================
// Implemented on VS Code By: 2477317 - WALID M W KOBAHALABI
//===============================================================

// we use this function as a helper function to ensure that ingestFile & ingestStdin uses the same logic
// we rewrite this block structure after understanding the HackerRank submission structure
    
// TODO:
    // - open file
    // - skip header
    // - skip malformed rows
    // - extract PickupZoneID and pickup hour
    // - aggregate counts

static void processLine(
    std::string& line,
    std::unordered_map<std::string, long long>& zoneCounts,
    std::unordered_map<std::string, std::unordered_map<int, long long>>& hourlyCounts
) {
    if (line.empty()) return;

    // handle the line endings
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    if (line.empty()) return;

    // Split by comma the entire line first to count the columns
    std::vector<std::string> columns;
    columns.reserve(6);
    size_t start = 0;
    size_t pos;
    while ((pos = line.find(',', start)) != std::string::npos) {
        columns.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    columns.push_back(line.substr(start));

    // for dirty data check we must have 6 columns (TripID, PickupZoneID, DropoffZoneID, PickupDateTime, DistanceKm, FareAmount)
    if (columns.size() < 6) return;
    const std::string& pickupZone = columns[1];
    const std::string& timeStr    = columns[3];
    if (pickupZone.empty()) return;                 // Skip empty zones

    // extract hour in format ("YYYY-MM-DD HH:MM")
    size_t spacePos = timeStr.find(' ');
    if (spacePos == std::string::npos || spacePos + 2 >= timeStr.size()) return;
    try {
        int hour = std::stoi(timeStr.substr(spacePos + 1, 2));
        if (hour < 0 || hour > 23) return;

        // - aggregate counts
        zoneCounts[pickupZone]++;
        hourlyCounts[pickupZone][hour]++;
    }
    // as dirty data test if (std::stoi) crash / substring fails there will be an error
    // so we will move to the next line without crashing the program
    catch (...) {
        return;
    }
}

void TripAnalyzer::ingestFile(const std::string& csvPath) {
    std::ifstream file(csvPath);        // open the file stream
    if (!file.is_open()) return;        // if file cant be open it return to satisfy empty file test (category A)

    std::string line;
    bool skipHeader = true;             // Flag to skip header row

    while (std::getline(file, line)) {
        if (skipHeader) {               // Skip the first line Header row
            skipHeader = false;
            continue;
        }
        processLine(line, m_zoneCounts, m_hourlyCounts);
    }
}

// we add this block for HackerRank requirements
void TripAnalyzer::ingestStdin() {
    std::ios_base::sync_with_stdio(false);    // used for sync off for faster standard I/O
    std::cin.tie(nullptr);

    std::string line;
    bool skipHeader = true;

    while (std::getline(std::cin, line)) {
        if (skipHeader) {
            skipHeader = false;
            continue;
        }
        processLine(line, m_zoneCounts, m_hourlyCounts);
    }
}

//========================================================================
// Implemented on VS Code By: 2361862 - TAREQ IYAD MOHAMMED AKRAM SHWAIKA
//========================================================================

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    // TODO:
    // - sort by count desc, zone asc

    // As we cant sort the map we need to copy all stuff to a vector

    std::vector<ZoneCount> result;
    result.reserve(m_zoneCounts.size());                // using reserve(m_zoneCounts.size()) to pre allocate memorey

    for (const auto& pair : m_zoneCounts) {
        result.push_back({pair.first, pair.second});
    }

    // Then sorting the vector by count (descending) and then ZoneID (ascending)
    // we use a Lambda function for custom comparator where count (Desc) -> ZoneID (Asc)

    std::sort(result.begin(), result.end(), [](const ZoneCount& a, const ZoneCount& b) {
        if (a.count != b.count) {
            return a.count > b.count;                   // where the higher count will come first
        }
        return a.zone < b.zone;                         // where its alphabetical order for (Tie-Breaker)
    });

    // - return first k

    if ((size_t)k < result.size()) {
        result.resize(k);
    }
    return result;                                       // Return Top K
}

//===============================================================
// Implemented on VS Code By: 2361790 - YOUSEF SALAMA
//===============================================================


std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    // TODO:
    // - sort by count desc, zone asc, hour asc

    std::vector<SlotCount> result;

    // First we should flatten the hourly data to iterate through every zone and every hour from 0 to 23 

    for (const auto& zPair : m_hourlyCounts) {
        const std::string& zone = zPair.first;
        for (const auto& hPair : zPair.second) {
             result.push_back({zone, hPair.first, hPair.second});
        }
    }

    // Then sorting by count (descending) then ZoneID (ascending) and the hour (ascending)

    std::sort(result.begin(), result.end(), [](const SlotCount& a, const SlotCount& b) {
        if (a.count != b.count) {
            return a.count > b.count;           // first sort count DESC
        }
        if (a.zone != b.zone) {
            return a.zone < b.zone;             // Then sort ZoneID ASC
        }
        return a.hour < b.hour;                 // then sort hour ASC
    });

    // - return first k

    if ((size_t)k < result.size()) {
        result.resize(k);
    }
    return result;                              // Return Top K
}
