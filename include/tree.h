#pragma once

#include "GitObject.h"

class tree : public GitObject {
public:
  std::string GetContent() { return ""; }
  std::string GetHash() { return ""; }
};
