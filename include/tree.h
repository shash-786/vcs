#pragma once

#include "GitObject.h"

class tree : public GitObject {
public:
  std::string getContent();
  std::string getHash();
};
