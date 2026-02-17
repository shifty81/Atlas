#include "MarketplaceImporter.h"
#include "AssetImporter.h"
#include "../sim/StateHasher.h"
#include "../core/Logger.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace atlas::asset {

// ---------------------------------------------------------------------------
// Helper functions
// ---------------------------------------------------------------------------

static bool FileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

static std::string GetFileExtension(const std::string& path) {
    auto ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return ext;
}

static std::string GetDownloadCacheDir(const MarketplaceImportOptions& options) {
    if (!options.downloadCache.empty()) {
        return options.downloadCache;
    }
    return std::filesystem::temp_directory_path().string() + "/atlas_marketplace_cache";
}

// ---------------------------------------------------------------------------
// MarketplaceImportRegistry
// ---------------------------------------------------------------------------

void MarketplaceImportRegistry::RegisterImporter(std::unique_ptr<IMarketplaceImporter> importer) {
    if (importer) {
        m_importers.push_back(std::move(importer));
    }
}

IMarketplaceImporter* MarketplaceImportRegistry::FindImporter(MarketplaceType type) const {
    for (const auto& imp : m_importers) {
        if (imp->GetMarketplaceType() == type) {
            return imp.get();
        }
    }
    return nullptr;
}

ImportResult MarketplaceImportRegistry::FetchAndImport(
    MarketplaceType marketplace,
    const std::string& assetId,
    const MarketplaceImportOptions& options) {
    
    IMarketplaceImporter* importer = FindImporter(marketplace);
    if (!importer) {
        ImportResult result;
        result.errorMessage = "No importer registered for the specified marketplace";
        return result;
    }
    
    if (!importer->IsAvailable()) {
        ImportResult result;
        result.errorMessage = "Marketplace importer is not available (check API keys/connectivity)";
        return result;
    }
    
    // Fetch the asset from the marketplace
    MarketplaceFetchResult fetchResult = importer->FetchAsset(assetId, options);
    if (!fetchResult.success) {
        ImportResult result;
        result.errorMessage = "Failed to fetch asset: " + fetchResult.errorMessage;
        return result;
    }
    
    // Import the fetched asset into Atlas format
    return importer->ImportAsset(fetchResult, options);
}

std::vector<MarketplaceType> MarketplaceImportRegistry::AvailableMarketplaces() const {
    std::vector<MarketplaceType> types;
    for (const auto& imp : m_importers) {
        if (imp->IsAvailable()) {
            types.push_back(imp->GetMarketplaceType());
        }
    }
    return types;
}

size_t MarketplaceImportRegistry::ImporterCount() const {
    return m_importers.size();
}

// ---------------------------------------------------------------------------
// ItchIOImporter
// ---------------------------------------------------------------------------

MarketplaceType ItchIOImporter::GetMarketplaceType() const {
    return MarketplaceType::ItchIO;
}

MarketplaceFetchResult ItchIOImporter::FetchAsset(
    const std::string& assetId,
    const MarketplaceImportOptions& options) {
    
    MarketplaceFetchResult result;
    result.metadata.marketplace = MarketplaceType::ItchIO;
    result.metadata.id = assetId;
    
    // NOTE: In a real implementation, this would use itch.io's API
    // to download the asset. For now, this is a placeholder that
    // assumes the asset is already downloaded locally.
    
    std::string cacheDir = GetDownloadCacheDir(options);
    std::filesystem::create_directories(cacheDir);
    
    std::string localPath = cacheDir + "/" + assetId;
    
    // Check if asset exists locally in cache
    if (!FileExists(localPath)) {
        result.errorMessage = "Asset not found in cache (marketplace download not yet implemented): " + assetId;
        return result;
    }
    
    if (!ValidateItchAsset(localPath)) {
        result.errorMessage = "Asset validation failed for: " + assetId;
        return result;
    }
    
    result.success = true;
    result.localPath = localPath;
    result.metadata.name = assetId;
    return result;
}

ImportResult ItchIOImporter::ImportAsset(
    const MarketplaceFetchResult& fetchResult,
    const MarketplaceImportOptions& options) {
    
    if (!fetchResult.success) {
        ImportResult result;
        result.errorMessage = "Cannot import failed fetch result";
        return result;
    }
    
    // Use the standard asset importer based on file extension
    AssetImportRegistry standardRegistry;
    
    // Register standard importers
    standardRegistry.RegisterImporter(std::make_unique<MeshImporter>(ImportFormat::OBJ));
    standardRegistry.RegisterImporter(std::make_unique<MeshImporter>(ImportFormat::FBX));
    standardRegistry.RegisterImporter(std::make_unique<TextureImporter>(ImportFormat::PNG));
    standardRegistry.RegisterImporter(std::make_unique<AudioImporter>(ImportFormat::WAV));
    standardRegistry.RegisterImporter(std::make_unique<AudioImporter>(ImportFormat::OGG));
    
    ImportOptions importOpts;
    importOpts.outputDirectory = options.outputDirectory;
    importOpts.generateMipmaps = options.generateMipmaps;
    importOpts.compressTextures = options.compressTextures;
    
    return standardRegistry.Import(fetchResult.localPath, importOpts);
}

bool ItchIOImporter::IsAvailable() const {
    // itch.io doesn't require API keys for public assets
    return true;
}

bool ItchIOImporter::ValidateItchAsset(const std::string& path) const {
    // Basic validation: check if file exists and has recognized extension
    if (!FileExists(path)) {
        return false;
    }
    
    std::string ext = GetFileExtension(path);
    return ext == ".png" || ext == ".obj" || ext == ".fbx" || 
           ext == ".wav" || ext == ".ogg" || ext == ".ttf";
}

// ---------------------------------------------------------------------------
// UnrealMarketplaceImporter
// ---------------------------------------------------------------------------

MarketplaceType UnrealMarketplaceImporter::GetMarketplaceType() const {
    return MarketplaceType::UnrealEngine;
}

MarketplaceFetchResult UnrealMarketplaceImporter::FetchAsset(
    const std::string& assetId,
    const MarketplaceImportOptions& options) {
    
    MarketplaceFetchResult result;
    result.metadata.marketplace = MarketplaceType::UnrealEngine;
    result.metadata.id = assetId;
    
    // NOTE: In a real implementation, this would use Epic Games Store API
    // to download the asset. For now, this is a placeholder.
    
    std::string cacheDir = GetDownloadCacheDir(options);
    std::filesystem::create_directories(cacheDir);
    
    std::string localPath = cacheDir + "/" + assetId + ".uasset";
    
    if (!FileExists(localPath)) {
        result.errorMessage = "Unreal asset not found in cache (marketplace download not yet implemented): " + assetId;
        return result;
    }
    
    result.success = true;
    result.localPath = localPath;
    result.metadata.name = assetId;
    return result;
}

ImportResult UnrealMarketplaceImporter::ImportAsset(
    const MarketplaceFetchResult& fetchResult,
    const MarketplaceImportOptions& options) {
    
    if (!fetchResult.success) {
        ImportResult result;
        result.errorMessage = "Cannot import failed fetch result";
        return result;
    }
    
    ImportResult result;
    
    // Convert .uasset to intermediate format (FBX or similar)
    std::string intermediatePath = fetchResult.localPath + ".fbx";
    
    if (!ConvertUAsset(fetchResult.localPath, intermediatePath)) {
        result.errorMessage = "Failed to convert Unreal asset to intermediate format";
        return result;
    }
    
    // Import the intermediate format using standard importer
    AssetImportRegistry standardRegistry;
    standardRegistry.RegisterImporter(std::make_unique<MeshImporter>(ImportFormat::FBX));
    
    ImportOptions importOpts;
    importOpts.outputDirectory = options.outputDirectory;
    
    return standardRegistry.Import(intermediatePath, importOpts);
}

bool UnrealMarketplaceImporter::IsAvailable() const {
    // Would check for Epic Games Store API credentials
    return false;  // Not available without API integration
}

bool UnrealMarketplaceImporter::ConvertUAsset(
    const std::string& inputPath,
    const std::string& outputPath) const {
    
    // NOTE: Real implementation would use Unreal Engine's asset export tools
    // or a third-party .uasset parser. This is a placeholder.
    
    if (!FileExists(inputPath)) {
        return false;
    }
    
    // Placeholder: just copy the file
    // In reality, we'd parse the .uasset binary format and extract mesh data
    try {
        std::filesystem::copy_file(inputPath, outputPath, 
                                   std::filesystem::copy_options::overwrite_existing);
        Logger::Info("Successfully converted Unreal asset: " + inputPath);
        return true;
    } catch (const std::exception& e) {
        Logger::Error("Failed to convert Unreal asset '" + inputPath + "': " + e.what());
        return false;
    }
}

// ---------------------------------------------------------------------------
// UnityAssetStoreImporter
// ---------------------------------------------------------------------------

MarketplaceType UnityAssetStoreImporter::GetMarketplaceType() const {
    return MarketplaceType::UnityAssetStore;
}

MarketplaceFetchResult UnityAssetStoreImporter::FetchAsset(
    const std::string& assetId,
    const MarketplaceImportOptions& options) {
    
    MarketplaceFetchResult result;
    result.metadata.marketplace = MarketplaceType::UnityAssetStore;
    result.metadata.id = assetId;
    
    // NOTE: In a real implementation, this would use Unity Asset Store API
    // to download the asset. For now, this is a placeholder.
    
    std::string cacheDir = GetDownloadCacheDir(options);
    std::filesystem::create_directories(cacheDir);
    
    std::string localPath = cacheDir + "/" + assetId + ".prefab";
    
    if (!FileExists(localPath)) {
        result.errorMessage = "Unity asset not found in cache (marketplace download not yet implemented): " + assetId;
        return result;
    }
    
    result.success = true;
    result.localPath = localPath;
    result.metadata.name = assetId;
    return result;
}

ImportResult UnityAssetStoreImporter::ImportAsset(
    const MarketplaceFetchResult& fetchResult,
    const MarketplaceImportOptions& options) {
    
    if (!fetchResult.success) {
        ImportResult result;
        result.errorMessage = "Cannot import failed fetch result";
        return result;
    }
    
    ImportResult result;
    
    // Convert Unity .prefab to intermediate format
    std::string intermediatePath = fetchResult.localPath + ".obj";
    
    if (!ConvertUnityPrefab(fetchResult.localPath, intermediatePath)) {
        result.errorMessage = "Failed to convert Unity asset to intermediate format";
        return result;
    }
    
    // Import the intermediate format using standard importer
    AssetImportRegistry standardRegistry;
    standardRegistry.RegisterImporter(std::make_unique<MeshImporter>(ImportFormat::OBJ));
    
    ImportOptions importOpts;
    importOpts.outputDirectory = options.outputDirectory;
    
    return standardRegistry.Import(intermediatePath, importOpts);
}

bool UnityAssetStoreImporter::IsAvailable() const {
    // Would check for Unity Asset Store API credentials
    return false;  // Not available without API integration
}

bool UnityAssetStoreImporter::ConvertUnityPrefab(
    const std::string& inputPath,
    const std::string& outputPath) const {
    
    // NOTE: Real implementation would parse Unity's YAML-based .prefab format
    // and extract mesh/material data. This is a placeholder.
    
    if (!FileExists(inputPath)) {
        return false;
    }
    
    // Placeholder: just copy the file
    // In reality, we'd parse the .prefab YAML and extract geometry
    try {
        std::filesystem::copy_file(inputPath, outputPath,
                                   std::filesystem::copy_options::overwrite_existing);
        Logger::Info("Successfully converted Unity prefab: " + inputPath);
        return true;
    } catch (const std::exception& e) {
        Logger::Error("Failed to convert Unity prefab '" + inputPath + "': " + e.what());
        return false;
    }
}

}  // namespace atlas::asset
