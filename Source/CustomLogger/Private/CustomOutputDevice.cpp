// Copyright 2021 Gareth Cross
#include "CustomOutputDevice.h"
#include "CustomLogger.h"

#include "JsonEncoding.h"

#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
#define ZMQ_STATIC
#include <zmq.hpp>
#endif

FCustomOutputDevice::FCustomOutputDevice()
  : FOutputDevice() {
#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
  UniqueID = FGuid::NewGuid();
  // Setup ZMQ context and socket so we can publish
  try {
    Context = MakeUnique<zmq::context_t>(1);
    check(Context);
    Socket = MakeUnique<zmq::socket_t>(*Context, zmq::socket_type::pub);
    check(Socket);
    Socket->connect("tcp://127.0.0.1:8990");
  } catch (const zmq::error_t& Error) {
    const char* ErrPtr = Error.what();
    UE_LOG(LogCustomLogger, Error, TEXT("Failed to setup ZMQ socket: %s"), ErrPtr ? ErrPtr : "<unknown>");
  }
#endif
}

#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
// Create ZMQ message (by copy) from a string.
template <typename CharType = ANSICHAR>
static zmq::message_t CreateZmqMessage(const CharType* String) {
  return zmq::message_t{static_cast<const void*>(String), TCString<CharType>::Strlen(String) * sizeof(CharType)};
}
#endif

void FCustomOutputDevice::Serialize(const TCHAR* V, const ELogVerbosity::Type Verbosity, const FName& Category) {
#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
  if (!V || !Socket) {
    return;
  }
  if (Category == LogCustomLogger.GetCategoryName()) {
    // Don't send our own messages. Don't want this method to trigger an infinite loop by mistake.
    return;
  }

  const FString OutputJSON = JsonEncoding::EncodeToJSON(V, Verbosity, Category, UniqueID.ToString());
  if (OutputJSON.IsEmpty()) {
    return;
  }

  try {
    // Lifetime of TCHAR_TO_ANSI() will not exceed this call to create the message:
    zmq::message_t Message = CreateZmqMessage(TCHAR_TO_ANSI(*OutputJSON));
    const zmq::send_result_t Result = Socket->send(std::move(Message), zmq::send_flags::dontwait);
    if (!Result.has_value()) {
      // This indicates that we got EAGAIN, which means sending would have blocked.
      UE_LOG(LogCustomLogger, Warning, TEXT("Failed to send over zmq as the operation would have blocked."));
    }
  } catch (const zmq::error_t& Error) {
    const char* ErrPtr = Error.what();
    UE_LOG(LogCustomLogger, Error, TEXT("Failed to send message over zmq socket: %s"), ErrPtr ? ErrPtr : "<unknown>");
  }
#endif
}

void FCustomOutputDevice::TearDown() {
  FOutputDevice::TearDown();
#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
  UE_LOG(LogCustomLogger, Display, TEXT("Tearing down zmq socket."));
  Socket.Reset();
  Context.Reset();
#endif
}
