#pragma once

#include "GitObject.h"

class blob : public GitObject {
public:
  std::string getContent();
  std::string getHash();
};
