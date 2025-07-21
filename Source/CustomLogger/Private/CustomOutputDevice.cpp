// Copyright 2021 Gareth Cross
#include "CustomOutputDevice.h"
#include "CustomLogger.h"

#include "JsonEncoding.h"

#if ENABLE_CUSTOM_OUTPUT_DEVICE
#define ZMQ_STATIC
#include <zmq.hpp>
#endif

FCustomOutputDevice::FCustomOutputDevice() : FOutputDevice() {
#if ENABLE_CUSTOM_OUTPUT_DEVICE
  UniqueID = FGuid::NewGuid();
  // Setup ZMQ context and socket so we can publish
  try {
    Context = MakeUnique<zmq::context_t>(1);
    check(Context);
    Socket = MakeUnique<zmq::socket_t>(*Context, zmq::socket_type::pub);
    check(Socket);
    Socket->connect("tcp://127.0.0.1:8990");
  } catch (const zmq::error_t &Error) {
    const char *ErrPtr = Error.what();
    UE_LOG(LogCustomLogger, Error, TEXT("Failed to setup ZMQ socket: %hs"),
           ErrPtr ? ErrPtr : "<unknown>");
  }
#endif
}

#if ENABLE_CUSTOM_OUTPUT_DEVICE
// Create ZMQ message (by copy) from a string.
static zmq::message_t CreateZmqMessage(const char *String) {
  return zmq::message_t{static_cast<const void *>(String),
                        static_cast<size_t>(TCString<char>::Strlen(String))};
}
#endif

void FCustomOutputDevice::Serialize(const TCHAR *V,
                                    const ELogVerbosity::Type Verbosity,
                                    const FName &Category) {
#if ENABLE_CUSTOM_OUTPUT_DEVICE
  if (!V || !Socket) {
    return;
  }
  if (Category == LogCustomLogger.GetCategoryName()) {
    // Don't send our own messages. Don't want this method to trigger an
    // infinite loop by mistake.
    return;
  }

  const FString OutputJSON =
      JsonEncoding::EncodeToJSON(V, Verbosity, Category, UniqueID.ToString());
  if (OutputJSON.IsEmpty()) {
    return;
  }

  try {
    // Convert to UTF-8 before publishing.
    const FTCHARToUTF8 OutputJsonAsUtf8{*OutputJSON};
    zmq::message_t Message = CreateZmqMessage(OutputJsonAsUtf8.Get());
    const zmq::send_result_t Result =
        Socket->send(std::move(Message), zmq::send_flags::dontwait);
    if (!Result.has_value()) {
      // This indicates that we got EAGAIN, which means sending would have
      // blocked.
      UE_LOG(
          LogCustomLogger, Warning,
          TEXT("Failed to send over zmq as the operation would have blocked."));
    }
  } catch (const zmq::error_t &Error) {
    const char *ErrPtr = Error.what();
    UE_LOG(LogCustomLogger, Error,
           TEXT("Failed to send message over zmq socket: %hs"),
           ErrPtr ? ErrPtr : "<unknown>");
  }
#endif
}

void FCustomOutputDevice::TearDown() {
  FOutputDevice::TearDown();
#if ENABLE_CUSTOM_OUTPUT_DEVICE
  UE_LOG(LogCustomLogger, Display, TEXT("Tearing down zmq socket."));
  Socket.Reset();
  Context.Reset();
#endif
}
