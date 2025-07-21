#pragma once

#include <stdexcept>
#include <string>

// Define a custom exception so that we don't need to include json library everywhere.
class DecodingError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

// Result of decoding the JSON data.
struct DecodedMessage {
  std::string verbosity;
  std::string category;
  std::string message_body;
  std::string guid;
  bool is_server;
};

// Decode from JSON into `DecodedMessage`. Throws `DecodingError` on failure.
DecodedMessage DecodeJSON(const std::string& payload);
