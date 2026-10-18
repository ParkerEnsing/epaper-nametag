#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include <stddef.h>
#include <stdint.h>


namespace StorageService {
    static constexpr size_t MAX_FILENAME_LENGTH = 64;

    struct DirectoryEntry {
        char name[MAX_FILENAME_LENGTH] = {};
        bool isDirectory = false;
        uint64_t sizeBytes = 0;
    };

    // Initialize and mount the microSD card.
    bool begin();

    // Returns whether mounting previously succeeded.
    bool isMounted();

    // Read directory entries into a caller-provided array.
    bool listDirectory(const char* path, DirectoryEntry* entries, size_t capacity, size_t &count, bool &truncated);
}


#endif