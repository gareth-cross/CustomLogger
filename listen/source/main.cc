#include <iostream>
#include <string>
#include <unordered_map>

#include <fmt/ostream.h>
#include <zmq.hpp>

#include "colors.h"
#include "json_decoding.h"

// Assign colors to verbosity levels.
colors::Color GetVerbosityColor(const std::string& verbosity) {
  if (verbosity == "Warning") {
    return colors::Gold3;
  } else if (verbosity == "Error" || verbosity == "Fatal") {
    return colors::Red3;
  }
  return colors::None;
}

/**
 * Print messages from unreal engine to the console.
 */
class MessagePrinter {
 public:
  // Handle JSON-encoded message.
  void HandleMessage(const std::string& message) {
    if (auto decoded = DecodeJSON(message); decoded.has_value()) {
      Print(*decoded);
    }
  }

  // Print message with color.
  void Print(const DecodedMessage& message) {
    const bool should_skip = message.verbosity == "VeryVerbose" || message.verbosity == "Log" ||
                             message.verbosity == "UnknownVerbosity" || message.verbosity == "NoLogging";
    if (should_skip) {
      return;
    }

    const auto guid_color = message.is_server ? server_color_ : GetColorForGUID(message.guid);
    const auto verbosity_color = GetVerbosityColor(message.verbosity);
    constexpr std::size_t max_guid_length = 5;
    const std::string guid_truncated =
        message.guid.size() > max_guid_length
            ? message.guid.substr(message.guid.size() - max_guid_length - 1, max_guid_length)
            : message.guid;

    // Check if we need to truncate the category to keep things aligned:
    constexpr std::size_t max_adaptive_category_size = 15;
    const std::size_t category_length = message.category.size();
    if (category_length > max_category_length_) {
      max_category_length_ = std::min(category_length, max_adaptive_category_size);
    }
    const std::string category_truncated = (category_length > max_adaptive_category_size)
                                               ? message.category.substr(0, max_adaptive_category_size - 3) + "..."
                                               : message.category;
    fmt::vprint(
        "{}[{}, {}] {}{:>{}}: {}{}\n",
        fmt::make_format_args(guid_color, guid_truncated, message.is_server ? "Server" : "Client", verbosity_color,
                              category_truncated, max_category_length_, message.message_body, colors::None));
  }

 private:
  // Some predefined colors.
  // TODO(gareth): Load these params from a file...
  const colors::Color server_color_{colors::DodgerBlue1};
  const std::array<colors::Color, 4> client_colors_ = {{
      colors::BlueViolet,
      colors::Chartreuse4,
      colors::CadetBlue,
      colors::DarkMagenta,
  }};

  // Map of GUIDs to an index in `ClientColors`.
  std::unordered_map<std::string, std::size_t> guid_to_color_index_;
  std::size_t last_index_assigned_{0};

  // The longest category string we've seen so far (for text alignment).
  std::size_t max_category_length_{0};

  // Assign colors in a rotation to new GUIDs as we see them.
  colors::Color GetColorForGUID(const std::string& guid) {
    if (guid_to_color_index_.contains(guid)) {
      return client_colors_[guid_to_color_index_.at(guid)];
    }
    last_index_assigned_ = (last_index_assigned_ + 1) % client_colors_.size();
    guid_to_color_index_[guid] = last_index_assigned_;
    return client_colors_[last_index_assigned_];
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
      fmt::print("{}zeromq experienced Error during receive: %s{}\n", colors::Red, Error.what(), colors::None);
      continue;
    }
    if (!Result.has_value()) {
      fmt::print("{}Receive returned empty result.{}\n", colors::Yellow, colors::None);
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
  std::set_terminate([]() {
    try {
      std::exception_ptr eptr{std::current_exception()};
      if (eptr) {
        std::rethrow_exception(eptr);
      } else {
        std::fprintf(stderr, "Exiting without exception.\n");
      }
    } catch (const std::exception& ex) {
      std::fprintf(stderr, "Unhandled exception:\n%s\n", ex.what());
    } catch (...) {
      std::fprintf(stderr, "Unknown exception caught.\n");
    }
    std::flush(std::cerr);
    std::exit(EXIT_FAILURE);
  });

  MessagePrinter Printer{};
  RunMainLoop([&](const std::string& message) { Printer.HandleMessage(message); });
  return 0;
}
