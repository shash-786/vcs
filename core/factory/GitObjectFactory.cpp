/*

Factory Design Pattern

*/

#include "../../include/core/GitObjectFactory.h"
#include <memory>

std::unique_ptr<GitObject> BlobFactory::CreateObject() {
  return std::make_unique<blob>();
}

std::unique_ptr<GitObject> CommitFactory::CreateObject() {
  return std::make_unique<commit>();
}

std::unique_ptr<GitObject> TreeFactory::CreateObject() {
  return std::make_unique<tree>();
}
