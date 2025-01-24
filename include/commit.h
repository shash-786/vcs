#pragma once

#include "GitObject.h"
#include "tree.h"

class commit : public GitObject {
public:
  tree *t;
  commit *parent;

  std::string GetContent();
  std::string GetHash();
};
