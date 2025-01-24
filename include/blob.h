#pragma once

#include "GitObject.h"

class blob : public GitObject {
public:
  std::string GetContent();
  std::string GetHash();
};
