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
  std::unique_ptr<GitObject> CreateObject() override;
};

class CommitFactory : public GitObjectFactory {
private:
  std::unique_ptr<GitObject> CreateObject() override;
};

class TreeFactory : public GitObjectFactory {
private:
  std::unique_ptr<GitObject> CreateObject() override;
};
