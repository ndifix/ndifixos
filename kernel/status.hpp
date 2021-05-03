#pragma once

namespace ndifixos {
namespace status {

class Status {
 public:
  enum Code {
    Success,
    Failed,
    Full,
    OutOfRange,
    NoEnoughMemory,
    InvalidDeviceId,
  };

  Status(Code code) : code_{code} {}
  bool isSuccess() const { return this->code_ == Code::Success; }

 private:
  Code code_;
};

template <class T>
struct ValWithStatus {
  T val;
  Status status;
};

}  // namespace status
}  // namespace ndifixos
