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


bool StorageService::listDirectory(const char* path, DirectoryEntry* entries, size_t capacity, size_t &count, bool &truncated) {
    count = 0;
    truncated = false;

    if (!mounted || path == nullptr || (entries == nullptr && capacity > 0)) {
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

    while (file) {
        if (count >= capacity) {
            truncated = true;
            file.close();
            break;
        }

        DirectoryEntry &entry = entries[count];

        // Extract the filename from the returned path.
        const char* fullName = file.name();
        const char* entryName = strrchr(fullName, '/');

        if (entryName != nullptr) {
            entryName++;
        }
        else {
            entryName = fullName;
        }

        snprintf(
            entry.name,
            sizeof(entry.name),
            "%s",
            entryName
        );

        entry.isDirectory = file.isDirectory();
        entry.sizeBytes = entry.isDirectory ? 0 : file.size();

        count++;

        file.close();
        file = directory.openNextFile();
    }

    directory.close();

    return true;
}