// Copyright 2021 Gareth Cross
// ReSharper disable CppUE4CodingStandardNamingViolationWarning
#pragma once
#include "Misc/OutputDevice.h"

// Forward declarations.
namespace zmq {
class context_t;
class socket_t;
} // namespace zmq

#if !UE_BUILD_SHIPPING || USE_LOGGING_IN_SHIPPING
#define ENABLE_CUSTOM_OUTPUT_DEVICE 1
#else
#define ENABLE_CUSTOM_OUTPUT_DEVICE 0
#endif

/**
 * Custom output device for intercepting log messages and republishing them.
 */
class FCustomOutputDevice : public FOutputDevice {
public:
  // Setup zmq
  FCustomOutputDevice();

  // Pack up and send message.
  virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;

  // Close socket and discard context.
  virtual void TearDown() override;

  virtual bool IsMemoryOnly() const override {
    return false;
  }

  virtual bool CanBeUsedOnAnyThread() const override {
    return false;
  }

  virtual bool CanBeUsedOnMultipleThreads() const override {
    return false;
  }

private:
#if ENABLE_CUSTOM_OUTPUT_DEVICE
  FGuid UniqueID;
  TUniquePtr<zmq::context_t> Context;
  TUniquePtr<zmq::socket_t> Socket;
#endif
};
