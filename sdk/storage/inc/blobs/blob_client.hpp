// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "blob_options.hpp"
#include "common/storage_credential.hpp"
#include "common/storage_url_builder.hpp"
#include "internal/protocol/blob_rest_client.hpp"

#include <map>
#include <memory>
#include <string>

namespace Azure { namespace Storage { namespace Blobs {

  struct BlobDownloadInfo
  {
    std::string ETag;
    std::string LastModified;
    int64_t ContentLength = 0;
    BlobHttpHeaders HttpHeaders;
    std::map<std::string, std::string> Metadata;
    Blobs::BlobType BlobType = Blobs::BlobType::Unknown;
    Azure::Core::Nullable<bool> ServerEncrypted;
    Azure::Core::Nullable<std::string> EncryptionKeySHA256;
  };

  class BlockBlobClient;
  class AppendBlobClient;
  class PageBlobClient;

  /**
   * @brief The BlobClient allows you to manipulate Azure Storage blobs.
   */
  class BlobClient {
  public:
    /**
     * @brief Initialize a new instance of BlobClient.
     *
     * @param connectionString A connection string includes the authentication information required
     * for your application to access data in an Azure Storage account at runtime.
     * @param containerName The name of the container containing this blob.
     * @param blobName The name of this blob.
     * @param options Optional client options that define the transport pipeline policies for
     * authentication, retries, etc., that are applied to every request.
     * @return A new BlobClient instance.
     */
    static BlobClient CreateFromConnectionString(
        const std::string& connectionString,
        const std::string& containerName,
        const std::string& blobName,
        const BlobClientOptions& options = BlobClientOptions());

    /**
     * @brief Initialize a new instance of BlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob.
     * @param credential The shared key credential used to sign
     * requests.
     * @param options Optional client options that define the transport pipeline
     * policies for authentication, retries, etc., that are applied to every request.
     */
    explicit BlobClient(
        const std::string& blobUri,
        std::shared_ptr<SharedKeyCredential> credential,
        const BlobClientOptions& options = BlobClientOptions());

    /**
     * @brief Initialize a new instance of BlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob.
     * @param credential The token credential used to sign requests.
     * @param options Optional client options that define the transport pipeline policies for
     * authentication, retries, etc., that are applied to every request.
     */
    explicit BlobClient(
        const std::string& blobUri,
        std::shared_ptr<TokenCredential> credential,
        const BlobClientOptions& options = BlobClientOptions());

    /**
     * @brief Initialize a new instance of BlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob, and possibly also a SAS token.
     * @param options Optional client
     * options that define the transport pipeline policies for authentication, retries, etc., that
     * are applied to every request.
     */
    explicit BlobClient(
        const std::string& blobUri,
        const BlobClientOptions& options = BlobClientOptions());

    /**
     * @brief Creates a new BlockBlobClient object with the same uri as this BlobClient. The
     * new BlockBlobClient uses the same request policy pipeline as this BlobClient.
     *
     *
     * @return A new BlockBlobClient instance.
     */
    BlockBlobClient GetBlockBlobClient() const;

    /**
     * @brief Creates a new AppendBlobClient object with the same uri as this BlobClient.
     * The new AppendBlobClient uses the same request policy pipeline as this BlobClient.
     *
     * @return A new AppendBlobClient instance.
     */
    AppendBlobClient GetAppendBlobClient() const;

    /**
     * @brief Creates a new PageBlobClient object with the same uri as this BlobClient.
     * The new PageBlobClient uses the same request policy pipeline as this BlobClient.
     *
     * @return A new PageBlobClient instance.
     */
    PageBlobClient GetPageBlobClient() const;

    /**
     * @brief Gets the blob's primary uri endpoint.
     *
     * @return The blob's
     * primary uri endpoint.
     */
    std::string GetUri() const { return m_blobUrl.ToString(); }

    /**
     * @brief Initializes a new instance of the BlobClient class with an identical uri
     * source but the specified snapshot timestamp.
     *
     * @param snapshot The snapshot
     * identifier.
     * @return A new BlobClient instance.
     * @remarks Pass empty string to remove the snapshot returning the base blob.
     */
    BlobClient WithSnapshot(const std::string& snapshot) const;

    /**
     * @brief Returns all user-defined metadata, standard HTTP properties, and system
     * properties for the blob. It does not return the content of the blob.
     *
     * @param
     * options Optional parameters to execute this function.
     * @return A BlobProperties
     * describing the blob's properties.
     */
    BlobProperties GetProperties(
        const GetBlobPropertiesOptions& options = GetBlobPropertiesOptions()) const;

    /**
     * @brief Sets system properties on the blob.
     *
     * @param httpHeaders The standard HTTP header system properties to set.
     * @param options Optional parameters to execute this function.
     * @return A SetBlobHttpHeadersResponse describing the updated blob.
     */
    SetBlobHttpHeadersResponse SetHttpHeaders(
        BlobHttpHeaders httpHeaders,
        const SetBlobHttpHeadersOptions& options = SetBlobHttpHeadersOptions()) const;

    /**
     * @brief Sets user-defined metadata for the specified blob as one or more name-value
     * pairs.
     *
     * @param metadata Custom metadata to set for this blob.
     * @param
     * options Optional parameters to execute this function.
     * @return A SetBlobMetadataResponse describing the updated blob.
     */
    SetBlobMetadataResponse SetMetadata(
        std::map<std::string, std::string> metadata,
        const SetBlobMetadataOptions& options = SetBlobMetadataOptions()) const;

    /**
     * @brief Sets the tier on a blob. The operation is allowed on a page blob in a premium
     * storage account and on a block blob in a blob storage or general purpose v2 account.
     *
     * @param Tier Indicates the tier to be set on the blob.
     * @param options Optional
     * parameters to execute this function.
     * @return A SetAccessTierResponse on successfully setting the tier.
     */
    SetAccessTierResponse SetAccessTier(
        AccessTier Tier,
        const SetAccessTierOptions& options = SetAccessTierOptions()) const;

    /**
     * @brief Copies data at from the source to this blob.
     *
     * @param sourceUri
     * Specifies the uri of the source blob. The value may a uri of up to 2 KB in length that
     * specifies a blob. A source blob in the same storage account can be authenticated via Shared
     * Key. However, if the source is a blob in another account, the source blob must either be
     * public or must be authenticated via a shared access signature. If the source blob is public,
     * no authentication is required to perform the copy operation.
     * @param options Optional parameters to execute this function.
     * @return A BlobCopyInfo describing the state of the copy operation.
     */
    BlobCopyInfo StartCopyFromUri(
        const std::string& sourceUri,
        const StartCopyFromUriOptions& options = StartCopyFromUriOptions()) const;

    /**
     * @brief Aborts a pending StartCopyFromUri operation, and leaves this blob with zero
     * length and full metadata.
     *
     * @param copyId ID of the copy operation to abort.
     * @param options Optional parameters to execute this function.
     * @return A AbortCopyBlobResponse on successfully aborting.
     */
    AbortCopyBlobResponse AbortCopyFromUri(
        const std::string& copyId,
        const AbortCopyFromUriOptions& options = AbortCopyFromUriOptions()) const;

    /**
     * @brief Downloads a blob or a blob range from the service, including its metadata and
     * properties.
     *
     * @param options Optional parameters to execute this function.
     * @return A BlobDownloadResponse describing the downloaded blob.
     * BlobDownloadResponse.BodyStream contains the blob's data.
     */
    BlobDownloadResponse Download(const DownloadBlobOptions& options = DownloadBlobOptions()) const;

    /**
     * @brief Downloads a blob or a blob range from the service to a memory buffer using parallel
     * requests.
     *
     * @param buffer A memory buffer to write the blob content to.
     * @param bufferSize Size of the memory buffer. Size must be larger or equal to size of the blob
     * or blob range.
     * @param options Optional parameters to execute this function.
     * @return A
     * BlobDownloadInfo describing the downloaded blob.
     */
    BlobDownloadInfo DownloadToBuffer(
        uint8_t* buffer,
        std::size_t bufferSize,
        const DownloadBlobToBufferOptions& options = DownloadBlobToBufferOptions()) const;

    /**
     * @brief Downloads a blob or a blob range from the service to a file using parallel
     * requests.
     *
     * @param file A file path to write the downloaded content to.
     * @param options Optional parameters to execute this function.
     * @return A
     * BlobDownloadInfo describing the downloaded blob.
     */
    BlobDownloadInfo DownloadToFile(
        const std::string& file,
        const DownloadBlobToFileOptions& options = DownloadBlobToFileOptions()) const;

    /**
     * @brief Creates a read-only snapshot of a blob.
     *
     * @param options Optional
     * parameters to execute this function.
     * @return A BlobSnapshotInfo describing the new
     * blob snapshot.
     */
    BlobSnapshotInfo CreateSnapshot(
        const CreateSnapshotOptions& options = CreateSnapshotOptions()) const;

    /**
     * @brief Marks the specified blob or snapshot for deletion. The blob is later deleted
     * during garbage collection. Note that in order to delete a blob, you must delete all of its
     * snapshots. You can delete both at the same time using DeleteBlobOptions.DeleteSnapshots.
     *
     * @param options Optional parameters to execute this function.
     * @return A DeleteBlobResponse on successfully deleting.
     */
    DeleteBlobResponse Delete(const DeleteBlobOptions& options = DeleteBlobOptions()) const;

    /**
     * @brief Restores the contents and metadata of a soft deleted blob and any associated
     * soft deleted snapshots.
     *
     * @param options Optional parameters to execute this
     * function.
     * @return A UndeleteBlobResponse on successfully deleting.
     */
    UndeleteBlobResponse Undelete(const UndeleteBlobOptions& options = UndeleteBlobOptions()) const;

  protected:
    UrlBuilder m_blobUrl;
    std::shared_ptr<Azure::Core::Http::HttpPipeline> m_pipeline;

  private:
    explicit BlobClient(
        UrlBuilder blobUri,
        std::shared_ptr<Azure::Core::Http::HttpPipeline> pipeline)
        : m_blobUrl(std::move(blobUri)), m_pipeline(std::move(pipeline))
    {
    }

    friend class BlobContainerClient;
  };
}}} // namespace Azure::Storage::Blobs
