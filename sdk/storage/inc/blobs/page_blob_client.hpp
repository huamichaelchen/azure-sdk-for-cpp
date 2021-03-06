// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "blob_options.hpp"
#include "blobs/blob_client.hpp"
#include "common/storage_credential.hpp"
#include "internal/protocol/blob_rest_client.hpp"

#include <string>

namespace Azure { namespace Storage { namespace Blobs {

  struct PageRange
  {
    int64_t Offset;
    int64_t Length;
  };

  struct PageRangesInfo
  {
    std::string RequestId;
    std::string Date;
    std::string Version;
    Azure::Core::Nullable<std::string> ClientRequestId;
    std::string ETag;
    std::string LastModified;
    int64_t BlobContentLength = 0;
    std::vector<PageRange> PageRanges;
    std::vector<PageRange> ClearRanges;
  };

  /**
   * The PageBlobClient allows you to manipulate Azure Storage page blobs.
   *
   * Page blobs are a collection of 512-byte pages optimized for random read and write operations.
   * To create a page blob, you initialize the page blob and specify the maximum size the page blob
   * will grow. To add or update the contents of a page blob, you write a page or pages by
   * specifying an offset and a range that align to 512-byte page boundaries. Writes to page blobs
   * happen in-place and are immediately committed to the blob.
   */
  class PageBlobClient : public BlobClient {
  public:
    /**
     * @brief Initialize a new instance of PageBlobClient.
     *
     * @param connectionString A connection string includes the authentication information required
     * for your application to access data in an Azure Storage account at runtime.
     * @param containerName The name of the container containing this blob.
     * @param blobName The name of this blob.
     * @param options Optional client options that define the transport pipeline policies for
     * authentication, retries, etc., that are applied to every request.
     * @return A new PageBlobClient instance.
     */
    static PageBlobClient CreateFromConnectionString(
        const std::string& connectionString,
        const std::string& containerName,
        const std::string& blobName,
        const PageBlobClientOptions& options = PageBlobClientOptions());

    /**
     * @brief Initialize a new instance of PageBlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob.
     * @param credential The shared key credential used to sign
     * requests.
     * @param options Optional client options that define the transport pipeline
     * policies for authentication, retries, etc., that are applied to every request.
     */
    explicit PageBlobClient(
        const std::string& blobUri,
        std::shared_ptr<SharedKeyCredential> credential,
        const PageBlobClientOptions& options = PageBlobClientOptions());

    /**
     * @brief Initialize a new instance of PageBlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob.
     * @param credential The token credential used to sign requests.
     * @param options Optional client options that define the transport pipeline policies for
     * authentication, retries, etc., that are applied to every request.
     */
    explicit PageBlobClient(
        const std::string& blobUri,
        std::shared_ptr<TokenCredential> credential,
        const PageBlobClientOptions& options = PageBlobClientOptions());

    /**
     * @brief Initialize a new instance of PageBlobClient.
     *
     * @param blobUri A uri
     * referencing the blob that includes the name of the account, the name of the container, and
     * the name of the blob, and possibly also a SAS token.
     * @param options Optional client
     * options that define the transport pipeline policies for authentication, retries, etc., that
     * are applied to every request.
     */
    explicit PageBlobClient(
        const std::string& blobUri,
        const PageBlobClientOptions& options = PageBlobClientOptions());

    /**
     * @brief Initializes a new instance of the PageBlobClient class with an identical uri
     * source but the specified snapshot timestamp.
     *
     * @param snapshot The snapshot
     * identifier.
     * @return A new PageBlobClient instance.
     * @remarks Pass empty string to remove the snapshot returning the base blob.
     */
    PageBlobClient WithSnapshot(const std::string& snapshot) const;

    /**
     * @brief Creates a new page blob of the specified size. The content of any  existing
     * blob is overwritten with the newly initialized page blob.
     *
     * @param
     * blobContentLength Specifies the maximum size for the page blob. The size must be aligned to a
     * 512-byte boundary.
     * @param options Optional parameters to execute this function.
     * @return A BlobContentInfo describing the newly created page blob.
     */
    BlobContentInfo Create(
        int64_t blobContentLength,
        const CreatePageBlobOptions& options = CreatePageBlobOptions());

    /**
     * @brief Writes content to a range of pages in a page blob, starting at offset.
     *
     * @param content A BodyStream containing the content of the pages to upload.
     * @param offset Specifies the starting offset for the content to be written as a page. Given
     * that pages must be aligned with 512-byte boundaries, the start offset must be a modulus of
     * 512.
     * @param options Optional parameters to execute this function.
     * @return A
     * PageInfo describing the state of the updated pages.
     */
    PageInfo UploadPages(
        Azure::Core::Http::BodyStream& content,
        int64_t offset,
        const UploadPagesOptions& options = UploadPagesOptions());

    /**
     * @brief Writes a range of pages to a page blob where the contents are read from a
     * uri.
     *
     * @param sourceUri Specifies the uri of the source blob. The value may be a
     * uri of up to 2 KB in length that specifies a blob. The source blob must either be public or
     * must be authenticated via a shared access signature. If the source blob is public, no
     * authentication is required to perform the operation.
     * @param sourceOffset Only upload a
     * part of the blob in the sourceUri from the specified offset.
     * @param sourceLength Only
     * upload specified length of the blob in the sourceUri.
     * @param destinationOffset
     * Specifies the starting offset for the content to be written. Given that pages must be aligned
     * with 512-byte boundaries, the start offset must be a modulus of 512.
     * @param options
     * Optional parameters to execute this function.
     * @return A PageInfo describing the state
     * of the updated pages.
     */
    PageInfo UploadPagesFromUri(
        std::string sourceUri,
        int64_t sourceOffset,
        int64_t sourceLength,
        int64_t destinationoffset,
        const UploadPagesFromUriOptions& options = UploadPagesFromUriOptions());

    /**
     * @brief Clears one or more pages from the page blob, as specificed by offset and length.
     *
     * @param offset Specifies the starting offset for the content to be cleared. Given that pages
     * must be aligned with 512-byte boundaries, the start offset must be a modulus of 512.
     * @param length Specifies the length of the content to be cleared.
     * @param options Optional parameters to execute this function.
     * @return A PageInfo describing the state of the updated pages.
     */
    PageInfo ClearPages(
        int64_t offset,
        int64_t length,
        const ClearPagesOptions& options = ClearPagesOptions());

    /**
     * @brief Resizes the page blob to the specified size (which must be a multiple of 512). If the
     * specified value is less than the current size of the blob, then all pages above the specified
     * value are cleared.
     *
     * @param blobContentLength Specifies the maximum size for the page blob. The size must be
     * aligned to a 512-byte boundary.
     * @param options Optional parameters to execute this function.
     * @return A PageBlobInfo describing the resized page blob.
     */
    PageBlobInfo Resize(
        int64_t blobContentLength,
        const ResizePageBlobOptions& options = ResizePageBlobOptions());

    /**
     * @brief Returns the list of valid page ranges for a page blob or snapshot of a page blob.
     *
     * @param options Optional parameters to execute this function.
     * @return A PageRangesInfo describing the valid page ranges for this blob.
     */
    PageRangesInfo GetPageRanges(const GetPageRangesOptions& options = GetPageRangesOptions());

    /**
     * @brief Starts copying a snapshot of the sourceUri page blob to this page blob. The snapshot
     * is copied such that only the differential changes between the previously copied snapshot
     * are transferred to the destination. The copied snapshots are complete copies of the original
     * snapshot and can be read or copied from as usual.
     *
     * @param sourceUri Specifies the to the source page blob as a uri up to 2 KB in length. The
     * source blob must either be public or must be authenticated via a shared access signature.
     * @param options Optional parameters to execute this function.
     * @return A BlobCopyInfo describing the state of the copy operation.
     */
    BlobCopyInfo StartCopyIncremental(
        const std::string& sourceUri,
        const IncrementalCopyPageBlobOptions& options = IncrementalCopyPageBlobOptions());

  private:
    explicit PageBlobClient(BlobClient blobClient);
    friend class BlobClient;
  };

}}} // namespace Azure::Storage::Blobs
