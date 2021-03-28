#pragma once

namespace ndifixos {
namespace status {

class Status {
 public:
  enum Code {
    Success,
    Failed,
    Full,
  };

  Status(Code code) : code_{code} {}
  bool isSuccess() { return this->code_ == Code::Success; }

 private:
  Code code_;
};

}  // namespace status
}  // namespace ndifixos
