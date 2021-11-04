// Copyright 2021 Gareth Cross
#pragma once
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustomLogger, Log, All);

class FCustomOutputDevice; //  Fwd declare.

class FCustomLoggerModule : public IModuleInterface {
public:
  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

private:
  TSharedPtr<FCustomOutputDevice> OutputDevice;
};
