#pragma once

#include "GitObject.h"
#include "blob.h"
#include "commit.h"
#include "tree.h"
#include <memory>

class GitObjectFactory {
public:
  virtual std::unique_ptr<GitObject> CreateObject() = 0;
  ~GitObjectFactory() = default;
};

class BlobFactory : public GitObjectFactory {
private:
  std::unique_ptr<GitObject> CreateObject() { return std::make_unique<blob>(); }
};

class CommitFactory : public GitObjectFactory {
private:
  std::unique_ptr<GitObject> CreateObject() {
    return std::make_unique<commit>();
  }
};

class TreeFactory : public GitObjectFactory {
private:
  std::unique_ptr<GitObject> CreateObject() { return std::make_unique<tree>(); }
};
