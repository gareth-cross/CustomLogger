#pragma once
#include <optional>
#include <string>

// Result of decoding the JSON data.
struct DecodedMessage {
  std::string verbosity;
  std::string category;
  std::string message_body;
  std::string guid;
  bool is_server;
};

// Decode from JSON into `DecodedMessage`.
std::optional<DecodedMessage> DecodeJSON(const std::string& payload);
