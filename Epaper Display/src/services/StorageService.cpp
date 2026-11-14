#include "services/StorageService.h"
#include "DevBoardIO.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <stdio.h>


namespace {
    static constexpr uint32_t SD_SPI_FREQUENCY = 4000000;

    SPIClass sdSPI(HSPI);

    bool mounted = false;
}


bool StorageService::begin() {
    if (mounted) {
        return true;
    }

    // Enable power to the TF reader
    pinMode(SD_POWER, OUTPUT);
    digitalWrite(SD_POWER, HIGH);

    sdSPI.begin(SD_SPI_SCK, SD_SPI_CIPO, SD_SPI_COPI, SD_SPI_CS);
    mounted = SD.begin(SD_SPI_CS, sdSPI, SD_SPI_FREQUENCY);

    if (!mounted) {
        Serial.println("StorageService: SD mount failed.");
        return false;
    }

    Serial.println("StorageService: SD mounted successfully.");
    Serial.printf("Card size: %llu MB\n", static_cast<unsigned long long>(SD.cardSize() / (1024ULL * 1024ULL)));

    return true;
}


bool StorageService::isMounted() {
    return mounted;
}


// DEPRECATED
bool StorageService::listDirectory(const char* path, DirectoryEntry* entries, size_t capacity, size_t &count, bool &truncated) {
    return listDirectoryPage(path, entries, capacity, 0, count, truncated);
}


bool StorageService::listDirectoryPage(const char* path, DirectoryEntry* entries, size_t capacity, size_t offset, size_t &count, bool &hasMore) {
    count = 0;
    hasMore = false;

    if (!mounted || path == nullptr || entries == nullptr || capacity == 0) {
        return false;
    }

    File directory = SD.open(path);

    if (!directory || !directory.isDirectory()) {
        if (directory) {
            directory.close();
        }
        return false;
    }

    File file = directory.openNextFile();

    // Skip entries preceding the requested page
    // TODO: find a more direct way to reach the desired starting file. Current system spends effort scrolling the entire directory list
    size_t skipped = 0;
    while (file && skipped < offset) {
        file.close();
        file = directory.openNextFile();
        skipped++;
    }

    // Collect entries for this page
    while (file && count < capacity) {
        DirectoryEntry &entry = entries[count];

        const char* fullName = file.name();

        if (fullName == nullptr) {
            file.close();
            directory.close();
            return false;
        }

        const char* entryName = strchr(fullName, '/');
        entryName = (entryName != nullptr) ? entryName + 1 : fullName; // if the entryName is valid, move the memory cursor to the first symbol of the name: otherwise, give the full name

        const int written = snprintf(
            entry.name,
            sizeof(entry.name),
            "%s",
            entryName
        );

        // if the number of encoded characters is negative, there was an encoding error
        if (written < 0) {
            file.close();
            directory.close();
            return false;
        }

        entry.nameTruncated = static_cast<size_t>(written) >= sizeof(entry.name);
        entry.isDirectory = file.isDirectory();
        entry.sizeBytes = entry.isDirectory ? 0 : file.size();

        count++;

        file.close();
        file = directory.openNextFile();
    }

    // If another entry remains, another page exists
    hasMore = static_cast<bool>(file);

    if (file) {
        file.close();
    }

    directory.close();
    return true;
}