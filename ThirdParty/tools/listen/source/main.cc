#include <fmt/ostream.h>

#include <map>
#include <string>

#define ZMT_STATIC  //  Required as we link to the static version of zmq.

#include <zmq.hpp>

#include "colors.h"
#include "json_decoding.h"

// Assign colors to verbosity levels.
Colors::Color GetVerbosityColor(const std::string& Verbosity) {
  if (Verbosity == "Warning") {
    return Colors::Gold3;
  } else if (Verbosity == "Error" || Verbosity == "Fatal") {
    return Colors::Red3;
  }
  return Colors::None;
}

/**
 * Print messages from unreal engine to the console.
 */
class MessagePrinter {
 public:
  // Handle JSON-encoded message.
  void HandleMessage(const std::string& message) {
    DecodedMessage Decoded;
#if 0
    // when debugging, print the raw JSON message
    fmt::print("{}\n", message);
#endif
    try {
      Decoded = DecodeJSON(message);
    } catch (const DecodingError& Error) {
      fmt::print("Dropping a malformed message. Reason: {}\n", Error.what());
      return;
    }
    Print(Decoded);
  }

  // Print message with color.
  void Print(const DecodedMessage& message) {
    const bool bShouldSkip = message.verbosity == "VeryVerbose" || message.verbosity == "Log" ||
                             message.verbosity == "UnknownVerbosity" ||
                             message.verbosity == "NoLogging";
    if (bShouldSkip) {
      return;
    }

    const auto GuidColor = message.is_server ? ServerColor : GetColorForGUID(message.guid);
    const auto VerbosityColor = GetVerbosityColor(message.verbosity);
    const constexpr int MaxGuidLength = 5;
    const std::string GuidTruncated =
        message.guid.size() > MaxGuidLength
            ? message.guid.substr(message.guid.size() - MaxGuidLength - 1, MaxGuidLength)
            : message.guid;

    // Check if we need to truncate the category to keep things aligned:
    constexpr int kMaxAdaptiveCategorySize = 15;
    const int CategoryLength = static_cast<int>(message.category.size());
    if (CategoryLength > MaxCategoryLength) {
      MaxCategoryLength = std::min(CategoryLength, kMaxAdaptiveCategorySize);
    }
    const std::string CategoryTruncated =
        (CategoryLength > kMaxAdaptiveCategorySize)
            ? message.category.substr(0, kMaxAdaptiveCategorySize - 3) + "..."
            : message.category;
    fmt::print(
        "{IdColor:<5}[{GUID}, {Type}] {VerbosityColor}{LogCategory:>{MaxCategoryLength}}: "
        "{LogMessage}{EndColor}\n",
        fmt::arg("IdColor", GuidColor), fmt::arg("GUID", GuidTruncated),
        fmt::arg("Type", message.is_server ? "Server" : "Client"),
        fmt::arg("VerbosityColor", VerbosityColor), fmt::arg("LogCategory", CategoryTruncated),
        fmt::arg("MaxCategoryLength", MaxCategoryLength),
        fmt::arg("LogMessage", message.message_body), fmt::arg("EndColor", Colors::None));
  }

 private:
  // Some predefined colors.
  // TODO(gareth): Load these params from a file...
  const Colors::Color ServerColor{Colors::DodgerBlue1};
  const std::array<Colors::Color, 4> ClientColors = {{
      Colors::BlueViolet,
      Colors::Chartreuse4,
      Colors::CadetBlue,
      Colors::DarkMagenta,
  }};

  // Map of GUIDs to an index in `ClientColors`.
  std::map<std::string, int> GuidToColorIndex;
  int LastIndexAssigned{0};

  // The longest category string we've seen so far (for text alignment).
  int MaxCategoryLength{0};

  // Assign colors in a rotation to new GUIDs as we see them.
  Colors::Color GetColorForGUID(const std::string& guid) {
    if (GuidToColorIndex.count(guid) > 0) {
      return ClientColors[GuidToColorIndex.at(guid)];
    }
    LastIndexAssigned = (LastIndexAssigned + 1) % static_cast<int>(ClientColors.size());
    GuidToColorIndex[guid] = LastIndexAssigned;
    return ClientColors[LastIndexAssigned];
  }
};

template <typename CallbackType>
void RunMainLoop(zmq::socket_t& Socket, CallbackType Callback) {
  for (;;) {
    zmq::message_t Message{};
    zmq::recv_result_t Result{};
    try {
      Result = Socket.recv(Message);
    } catch (const zmq::error_t& Error) {
      fmt::print("{}zeromq experienced Error during receive: %s{}\n", Colors::Red, Error.what(),
                 Colors::None);
      continue;
    }
    if (!Result.has_value()) {
      fmt::print("{}Receive returned empty result.{}\n", Colors::Yellow, Colors::None);
      continue;  // The operation would have blocked, so just skip it for now.
    }
    // We have some data to write out.
    const std::string MessageString{static_cast<const char*>(Message.data()), Message.size()};
    Callback(MessageString);
  };
}

template <typename CallbackType>
void RunMainLoop(CallbackType Callback) {
  try {
    zmq::context_t Context{1};
    zmq::socket_t Socket{Context, zmq::socket_type::sub};
    Socket.set(zmq::sockopt::subscribe, "");
    Socket.bind("tcp://127.0.0.1:8990");
    RunMainLoop(Socket, Callback);
  } catch (const zmq::error_t& err) {
    fmt::print("Encountered error setting up zmq socket: {}\n", err.what());
    throw;
  }
}

// TODO(gareth): Accept some command line args.
int main() {
  MessagePrinter Printer{};
  RunMainLoop([&](const std::string& message) { Printer.HandleMessage(message); });
  return 0;
}
