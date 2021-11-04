// Copyright 2021 Gareth Cross
#include "CustomOutputDevice.h"
#include "CustomLogger.h"

#if !UE_BUILD_SHIPPING
#define ZMQ_STATIC
#include <zmq.hpp>
#endif

FCustomOutputDevice::FCustomOutputDevice()
  : FOutputDevice() {
#if !UE_BUILD_SHIPPING
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

// Create ZMQ message (by copy) from a string.
template <typename CharType = ANSICHAR>
static zmq::message_t CreateZmqMessage(const CharType* String) {
  return zmq::message_t{static_cast<const void*>(String), TCString<CharType>::Strlen(String) * sizeof(CharType)};
}

// Format JSON object to a human-readable string.
template <class CharType, class PrintPolicy>
bool WriteJsonObjectToString(const TSharedRef<FJsonObject>& JsonObject, FString& OutJsonString,
                             int32 Indent) {
  TSharedRef<TJsonWriter<CharType, PrintPolicy>> JsonWriter = TJsonWriterFactory<CharType, PrintPolicy>::Create(
      &OutJsonString, Indent);
  const bool bSuccess = FJsonSerializer::Serialize(JsonObject, JsonWriter);
  JsonWriter->Close();
  return bSuccess;
}

void FCustomOutputDevice::Serialize(const TCHAR* V, const ELogVerbosity::Type Verbosity, const FName& Category) {
#if !UE_BUILD_SHIPPING
  if (!V || !Socket) {
    return;
  }
  if (Category == LogCustomLogger.GetCategoryName()) {
    // Don't send our own messages. Don't want this method to trigger an infinite loop by mistake.
    return;
  }
  const FDateTime DateTimeUtc = FDateTime::UtcNow();

  // Convert to JSON
  // TODO(gareth): Use binary representation instead of this, and properly support utf-16.
  const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
  JsonObject->SetStringField(TEXT("Verbosity"), ToString(Verbosity));
  JsonObject->SetStringField(TEXT("Category"), Category.ToString());
  JsonObject->SetStringField(TEXT("MessageBody"), V);
  JsonObject->SetStringField(TEXT("GUID"), UniqueID.ToString());
  JsonObject->SetStringField(TEXT("DateTimeUTC"), DateTimeUtc.ToIso8601());
  if (GEngine) {
    UWorld* const World = GEngine->GetCurrentPlayWorld();
    if (World) {
      JsonObject->SetBoolField(TEXT("IsServer"), World->IsServer());
    }
  }

  FString OutputJSON;
  const bool bSuccess = WriteJsonObjectToString<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>(
      JsonObject, OutputJSON, 2);
  if (!bSuccess) {
    UE_LOG(LogCustomLogger, Warning, TEXT("Failed to write data to JSON."));
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
#if !UE_BUILD_SHIPPING
  Socket.Reset();
  Context.Reset();
#endif
}
