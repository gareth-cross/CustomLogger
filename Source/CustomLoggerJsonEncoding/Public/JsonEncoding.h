// Copyright 2021 Gareth Cross
#pragma once
#include "CoreMinimal.h"

class FCustomLoggerJsonEncodingModule : public IModuleInterface {
};

namespace JsonEncoding {

// Encode log message to JSON.
FString CUSTOMLOGGERJSONENCODING_API EncodeToJSON(const TCHAR* V, const ELogVerbosity::Type Verbosity,
                                                  const FName& Category,
                                                  const FString& UniqueID);

} // namespace JsonEncoding
