// Copyright 2021 Gareth Cross
#include "JsonEncoding.h"

#include "Json.h"

namespace JsonEncoding {
// Format JSON object to a human-readable string.
template <class CharType, class PrintPolicy>
bool WriteJsonObjectToString(const TSharedRef<FJsonObject> &JsonObject,
                             FString &OutJsonString, int32 Indent) {
  TSharedRef<TJsonWriter<CharType, PrintPolicy>> JsonWriter =
      TJsonWriterFactory<CharType, PrintPolicy>::Create(&OutJsonString, Indent);
  const bool bSuccess = FJsonSerializer::Serialize(JsonObject, JsonWriter);
  JsonWriter->Close();
  return bSuccess;
}

FString EncodeToJSON(const TCHAR *V, const ELogVerbosity::Type Verbosity,
                     const FName &Category, const FString &UniqueID) {
  const FDateTime DateTimeUtc = FDateTime::UtcNow();

  // Convert to JSON
  const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
  JsonObject->SetStringField(TEXT("Verbosity"), ToString(Verbosity));
  JsonObject->SetStringField(TEXT("Category"), Category.ToString());
  JsonObject->SetStringField(TEXT("MessageBody"), V);
  JsonObject->SetStringField(TEXT("GUID"), UniqueID);
  JsonObject->SetStringField(TEXT("DateTimeUTC"), DateTimeUtc.ToIso8601());
  if (GEngine) {
    if (const UWorld *const World = GEngine->GetCurrentPlayWorld();
        World != nullptr) {
      JsonObject->SetBoolField(TEXT("IsServer"),
                               World->GetNetMode() == NM_ListenServer ||
                                   World->GetNetMode() == NM_DedicatedServer);
    }
  }

  FString OutputJSON;
  const bool bSuccess =
      WriteJsonObjectToString<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>(
          JsonObject, OutputJSON, 2);
  if (!bSuccess) {
    return {};
  }
  return OutputJSON;
}
} // namespace JsonEncoding

IMPLEMENT_MODULE(FCustomLoggerJsonEncodingModule, CustomLoggerJsonEncoding)
