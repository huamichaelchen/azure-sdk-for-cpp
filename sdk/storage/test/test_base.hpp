// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "http/body_stream.hpp"

#include "gtest/gtest.h"

namespace Azure { namespace Storage { namespace Test {

  const std::string& StandardStorageConnectionString();
  const std::string& PremiumStorageConnectionString();
  const std::string& BlobStorageConnectionString();
  const std::string& PremiumFileConnectionString();
  const std::string& ADLSGen2ConnectionString();

  inline unsigned long long operator""_KB(unsigned long long x) { return x * 1024; }
  inline unsigned long long operator""_MB(unsigned long long x) { return x * 1024 * 1024; }
  inline unsigned long long operator""_GB(unsigned long long x) { return x * 1024 * 1024 * 1024; }
  inline unsigned long long operator""_TB(unsigned long long x)
  {
    return x * 1024 * 1024 * 1024 * 1024;
  }

  std::string RandomString();

  std::string LowercaseRandomString();

  void RandomBuffer(char* buffer, std::size_t length);

  inline std::vector<uint8_t> ReadBodyStream(std::unique_ptr<Azure::Core::Http::BodyStream>& stream)
  {
    Azure::Core::Context context;
    return Azure::Core::Http::BodyStream::ReadToEnd(context, *stream);
  }

  inline std::vector<uint8_t> ReadBodyStream(
      std::unique_ptr<Azure::Core::Http::BodyStream>&& stream)
  {
    return ReadBodyStream(stream);
  }

}}} // namespace Azure::Storage::Test
