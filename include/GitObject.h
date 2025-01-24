#pragma once
#include <iostream>

class GitObject {
protected:
  std::string hash;
  std::string type;

public:
  ~GitObject() = default;
  virtual std::string getContent() = 0;
  virtual std::string getHash() = 0;
};
