#pragma once
// ============================================================
// Atlas Marketplace Asset Import — External Store Integration
// ============================================================
//
// Provides integration with external asset marketplaces to import
// assets directly from platforms like itch.io, Unreal Marketplace,
// and Unity Asset Store into Atlas's internal format.
//
// Each marketplace has specific format requirements and metadata
// handling that must be translated into Atlas's unified asset system.
//
// See: docs/03_ASSET_SYSTEM.md

#include "AssetImporter.h"
#include "HttpClient.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace atlas::asset {

/// Supported external marketplaces
enum class MarketplaceType {
    ItchIO,          // itch.io marketplace
    UnrealEngine,    // Unreal Engine Marketplace
    UnityAssetStore, // Unity Asset Store
};

/// Metadata for marketplace assets
struct MarketplaceAssetMetadata {
    std::string id;              // Marketplace-specific asset ID
    std::string name;            // Asset name
    std::string author;          // Asset author/creator
    std::string version;         // Asset version
    std::string description;     // Asset description
    std::vector<std::string> tags; // Asset tags/categories
    MarketplaceType marketplace; // Source marketplace
};

/// Result of a marketplace asset fetch operation
struct MarketplaceFetchResult {
    bool success = false;
    std::string localPath;       // Path to downloaded asset
    MarketplaceAssetMetadata metadata;
    std::string errorMessage;    // Non-empty on failure
};

/// Options for marketplace import
struct MarketplaceImportOptions : ImportOptions {
    bool validateAssets = true;   // Verify asset integrity
    bool preserveMetadata = true; // Keep marketplace metadata
    std::string apiKey;           // API key for marketplace (if required)
    std::string downloadCache;    // Cache directory for downloads
};

/// Base class for marketplace-specific importers
class IMarketplaceImporter {
public:
    virtual ~IMarketplaceImporter() = default;
    virtual MarketplaceType GetMarketplaceType() const = 0;
    
    /// Fetch an asset from the marketplace
    virtual MarketplaceFetchResult FetchAsset(
        const std::string& assetId,
        const MarketplaceImportOptions& options) = 0;
    
    /// Import a fetched asset into Atlas format
    virtual ImportResult ImportAsset(
        const MarketplaceFetchResult& fetchResult,
        const MarketplaceImportOptions& options) = 0;
    
    /// Check if the marketplace is accessible (API key valid, etc.)
    virtual bool IsAvailable() const = 0;
    
    /// Set the HTTP client for API downloads
    virtual void SetHttpClient(IHttpClient* client) = 0;
};

/// itch.io marketplace importer
/// Supports common formats: .png, .obj, .fbx from itch.io game assets
class ItchIOImporter : public IMarketplaceImporter {
public:
    MarketplaceType GetMarketplaceType() const override;
    MarketplaceFetchResult FetchAsset(
        const std::string& assetId,
        const MarketplaceImportOptions& options) override;
    ImportResult ImportAsset(
        const MarketplaceFetchResult& fetchResult,
        const MarketplaceImportOptions& options) override;
    bool IsAvailable() const override;
    void SetHttpClient(IHttpClient* client) override;
    
private:
    bool ValidateItchAsset(const std::string& path) const;
    IHttpClient* m_httpClient = nullptr;
};

/// Unreal Engine Marketplace importer
/// Handles .uasset and .umap formats and converts to Atlas-compatible formats
class UnrealMarketplaceImporter : public IMarketplaceImporter {
public:
    MarketplaceType GetMarketplaceType() const override;
    MarketplaceFetchResult FetchAsset(
        const std::string& assetId,
        const MarketplaceImportOptions& options) override;
    ImportResult ImportAsset(
        const MarketplaceFetchResult& fetchResult,
        const MarketplaceImportOptions& options) override;
    bool IsAvailable() const override;
    void SetHttpClient(IHttpClient* client) override;
    
private:
    /// Convert Unreal .uasset to intermediate format
    bool ConvertUAsset(const std::string& inputPath, 
                       const std::string& outputPath) const;
    IHttpClient* m_httpClient = nullptr;
};

/// Unity Asset Store importer
/// Handles .unity, .prefab, and .asset formats
class UnityAssetStoreImporter : public IMarketplaceImporter {
public:
    MarketplaceType GetMarketplaceType() const override;
    MarketplaceFetchResult FetchAsset(
        const std::string& assetId,
        const MarketplaceImportOptions& options) override;
    ImportResult ImportAsset(
        const MarketplaceFetchResult& fetchResult,
        const MarketplaceImportOptions& options) override;
    bool IsAvailable() const override;
    void SetHttpClient(IHttpClient* client) override;
    
private:
    /// Convert Unity .prefab to intermediate format
    bool ConvertUnityPrefab(const std::string& inputPath,
                            const std::string& outputPath) const;
    IHttpClient* m_httpClient = nullptr;
};

/// Central registry for marketplace importers
class MarketplaceImportRegistry {
public:
    void RegisterImporter(std::unique_ptr<IMarketplaceImporter> importer);
    IMarketplaceImporter* FindImporter(MarketplaceType type) const;
    
    /// Fetch and import asset from a marketplace in one operation
    ImportResult FetchAndImport(
        MarketplaceType marketplace,
        const std::string& assetId,
        const MarketplaceImportOptions& options);
    
    std::vector<MarketplaceType> AvailableMarketplaces() const;
    size_t ImporterCount() const;
    
    /// Set the HTTP client on all registered importers
    void SetHttpClient(IHttpClient* client);
    
private:
    std::vector<std::unique_ptr<IMarketplaceImporter>> m_importers;
};

}  // namespace atlas::asset
