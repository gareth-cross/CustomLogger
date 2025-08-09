#include "json_decoding.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

std::optional<DecodedMessage> DecodeJSON(const std::string& payload) {
  json parsed_object;
  try {
    parsed_object = json::parse(payload);
  } catch (const json::parse_error& err) {
    fmt::print("Failed decoding payload: {}\n", err.what());
    return std::nullopt;
  }
  try {
    DecodedMessage result{};
    result.verbosity = parsed_object["Verbosity"];
    result.category = parsed_object["Category"];
    result.message_body = parsed_object["MessageBody"];
    result.guid = parsed_object["GUID"];
    return result;
  } catch (const json::type_error& err) {
    fmt::print("Type error: {}\n", err.what());
    return std::nullopt;
  }
}
