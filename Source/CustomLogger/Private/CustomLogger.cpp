// Copyright 2021 Gareth Cross
#include "CustomLogger.h"
#include "Misc/OutputDeviceRedirector.h"

#include "CustomOutputDevice.h"

#define LOCTEXT_NAMESPACE "FCustomLoggerModule"

void FCustomLoggerModule::StartupModule() {
#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
  FOutputDeviceRedirector* const Redirector = FOutputDeviceRedirector::Get();
  if (!Redirector) {
    UE_LOG(LogCustomLogger, Warning, TEXT("Failed to access output re-director. Nothing will be logged."));
    return;
  }
  // Create output device and attach it
  OutputDevice = MakeShared<FCustomOutputDevice>();
  if (ensure(OutputDevice)) {
    Redirector->AddOutputDevice(OutputDevice.Get());
  }
#endif
}

void FCustomLoggerModule::ShutdownModule() {
#if !UE_BUILD_SHIPPING && BUILD_CUSTOMER_LOGGER
  if (!OutputDevice) {
    return;
  }
  FOutputDeviceRedirector* const Redirector = FOutputDeviceRedirector::Get();
  if (Redirector) {
    Redirector->RemoveOutputDevice(OutputDevice.Get());
  }
  OutputDevice->TearDown();
  OutputDevice.Reset();
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomLoggerModule, CustomLogger)
DEFINE_LOG_CATEGORY(LogCustomLogger)
