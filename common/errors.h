#pragma once

enum class errorcode {
  OK = 0,           // Success
  NotFound,         // Resource not found
  InvalidArgument,  // Invalid input or argument
  PermissionDenied, // Access denied
  UpdateError,      // Can't update
  OutOfRange,       // Value out of range
  Unavailable,      // Resource unavailable
  AlreadyExists,    // Resource already exists
  InternalError,    // Generic internal error
  IntitializationError,
  ReadError,
  WriteError,
  Cancelled,  // Operation cancelled
  FailToOpen, // Failed to Open
  Unknown     // Unknown error
};
