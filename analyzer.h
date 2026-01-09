#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// we use <unordered_map> Hash Maps because finding zone or adding anything to the zone will takes O(1) time
// Insertion & search operations take O(1) time on avg allows us to process millions of rows in O(N) linear time
// So we use Hash Table for speed and efficient hashing where <map> was rejected because O(log N) is slower for this huuge data

struct ZoneCount {
    std::string zone;
    long long count;
};

struct SlotCount {
    std::string zone;
    int hour;              // 0–23
    long long count;
};

class TripAnalyzer {
public:
    // Parse Trips.csv, skip dirty rows, never crash
    void ingestFile(const std::string& csvPath);

    // We add this function for HackerRank submission requirements
    void ingestStdin();

    // Top K zones: count desc, zone asc
    std::vector<ZoneCount> topZones(int k = 10) const;

    // Top K slots: count desc, zone asc, hour asc
    std::vector<SlotCount> topBusySlots(int k = 10) const;

//===============================================================================
// Implemented on VS Code By: TAREQ IYAD MOHAMMED AKRAM SHWAIKA & YOUSEF SALAMA
//===============================================================================

// Adding 2 private variables (m_zoneCounts) & (m_hourlyCounts)
private:
    // Mapping the ZoneID to Total Trips (ZoneID (string), Total Count (long long))
    std::unordered_map<std::string, long long> m_zoneCounts;

    // Mapping the ZoneID to a map of Hours to count
    // we choose this structure to ensures only valid hours extracted from the CSV are stored
    std::unordered_map<std::string, std::unordered_map<int, long long>> m_hourlyCounts;
};

