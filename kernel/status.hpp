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
    HostControllerNotHalted,
  };

  Status(Code code) : code_{code} {}
  bool isSuccess() const { return this->code_ == Code::Success; }

  const char* Name() {
    switch (code_) {
      case Success:
        return "Success";
      case Failed:
        return "Failed";
      case Full:
        return "Full";
      case OutOfRange:
        return "OutOfRange";
      case NoEnoughMemory:
        return "NoEnoughMemory";
      case InvalidDeviceId:
        return "InvalidDeviceId";
      case HostControllerNotHalted:
        return "HostControllerNotHalted";
    }
  }

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
