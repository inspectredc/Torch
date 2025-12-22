#pragma once

#include "factories/BaseFactory.h"
#include "utils/TextureUtils.h"
#include <types/RawBuffer.h>
#include <unordered_map>
#include <string>
#include <vector>

namespace BK64 {

typedef struct FrameHeader {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    int16_t chunkCount;
    int16_t unkA;
    int16_t unkC;
    int16_t unkE;
    int16_t unk10;
    int16_t unk12;
} FrameHeader; // size = 0x14

class SpriteData : public IParsedData {
public:
    int16_t mFrameCount;
    int16_t mFormatCode;
    std::vector<FrameHeader> mFrameHeaders;
    std::vector<std::pair<int16_t, int16_t>> mPositions;

    SpriteData(int16_t frameCount, int16_t formatCode, std::vector<FrameHeader> frameHeaders, std::vector<std::pair<int16_t, int16_t>> positions) : mFrameCount(frameCount), mFormatCode(formatCode), mFrameHeaders(std::move(frameHeaders)), mPositions(std::move(positions)) {}
};

class SpriteHeaderExporter : public BaseExporter {
    ExportResult Export(std::ostream& write, std::shared_ptr<IParsedData> raw, std::string& entryName, YAML::Node& node, std::string* replacement) override;
};

class SpriteBinaryExporter : public BaseExporter {
    ExportResult Export(std::ostream& write, std::shared_ptr<IParsedData> raw, std::string& entryName, YAML::Node& node, std::string* replacement) override;
};

class SpriteCodeExporter : public BaseExporter {
    ExportResult Export(std::ostream& write, std::shared_ptr<IParsedData> raw, std::string& entryName, YAML::Node& node, std::string* replacement) override;
};

class SpriteModdingExporter : public BaseExporter {
    ExportResult Export(std::ostream& write, std::shared_ptr<IParsedData> raw, std::string& entryName, YAML::Node& node, std::string* replacement) override;
};

class SpriteFactory : public BaseFactory {
public:
    std::optional<std::shared_ptr<IParsedData>> parse(std::vector<uint8_t>& buffer, YAML::Node& data) override;
    inline std::unordered_map<ExportType, std::shared_ptr<BaseExporter>> GetExporters() override {
        return {
            REGISTER(Header, SpriteHeaderExporter)
            REGISTER(Binary, SpriteBinaryExporter)
            REGISTER(Code, SpriteCodeExporter)
            REGISTER(Modding, SpriteModdingExporter)
        };
    }

    bool HasModdedDependencies() override { return true; }
    bool SupportModdedAssets() override { return true; }
};

} // namespace BK64
