#pragma once

#undef _DLL

#include <string>
#include <stdint.h>
#include <cstddef>
#include <string>

#include "zip.h"

#include "ship/resource/File.h"
#include "ship/resource/Resource.h"
#include "ship/resource/archive/Archive.h"

#ifdef __WIIU__
#ifndef WIIU_DIAGNOSTICS
#define WIIU_DIAGNOSTICS 1
#endif
#endif

namespace Ship {
struct File;

class O2rArchive final : virtual public Archive {
  public:
    O2rArchive(const std::string& archivePath);
    ~O2rArchive();

    bool Open();
    bool Close();
    bool WriteFile(const std::string& filename, const std::vector<uint8_t>& data);

    std::shared_ptr<File> LoadFile(const std::string& filePath);
    std::shared_ptr<File> LoadFile(uint64_t hash);

#ifdef __WIIU__
    static void GetStats(uint32_t& loads, uint64_t& compressedBytes, uint64_t& microseconds);
    // Reads one entry's archive bytes into the shared O2R byte cache without invoking a resource
    // loader or factory. The caller supplies the remaining scene-prefetch budget; the operation
    // stores either an inflated payload or compressed bytes and returns the cache payload size.
    bool PrefetchFile(const std::string& filePath, size_t remainingBudget, size_t& storedBytes);
    bool GetEntryInfo(const std::string& filePath, size_t& compressedSize, size_t& uncompressedSize) const;
    static size_t GetScenePrefetchBytes();
#if WIIU_DIAGNOSTICS
    struct HitchArchiveStats {
        char name[32]{};
        uint64_t readMicroseconds = 0;
    };

    struct HitchStats {
        uint32_t loads = 0;
        uint64_t compressedBytes = 0;
        uint64_t readMicroseconds = 0;
        uint32_t archiveCount = 0;
        HitchArchiveStats topArchives[3]{};
    };

    static HitchStats GetHitchStats();
#endif
#endif

  private:
    zip_t* mZipArchive;
};
} // namespace Ship
