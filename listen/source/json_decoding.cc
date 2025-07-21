#include "json_decoding.h"

#include <fmt/format.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

DecodedMessage DecodeJSON(const std::string& payload) {
  json parsed_object;
  try {
    parsed_object = json::parse(payload);
  } catch (const json::parse_error& err) {
    throw DecodingError(err.what());
  }
  // Extract values and call the print method.
  DecodedMessage result;
  try {
    result.verbosity = parsed_object["Verbosity"];
    result.category = parsed_object["Category"];
    result.message_body = parsed_object["MessageBody"];
    result.guid = parsed_object["GUID"];
    if (parsed_object.contains("IsServer")) {
      result.is_server = parsed_object["IsServer"];
    } else {
      result.is_server = false;
    }
  } catch (const json::type_error& err) {
    throw DecodingError(err.what());
  }
  return result;
}
