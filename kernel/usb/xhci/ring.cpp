#include "ring.hpp"

#include "../memory.hpp"

namespace ndifixos::usb::xhci {
Ring::~Ring() {
  if (buf != nullptr) {
    FreeMem(buf);
  }
}

status::Status Ring::Initialize(size_t buf_size) {
  if (buf != nullptr) {
    FreeMem(buf);
  }

  cycle_bit = true;
  write_index = 0;
  this->buf_size = buf_size;

  buf = AllocArray<TRB>(buf_size, 64, 64 * 1024);
  if (buf == nullptr) {
    return status::Status::NoEnoughMemory;
  }
  memset(buf, 0, buf_size * sizeof(TRB));

  return status::Status::Success;
}

void Ring::CopyToLast(const std::array<uint32_t, 4>& data) {
  for (int i = 0; i < 3; ++i) {
    // data[0..2] must be written prior to data[3].
    buf[write_index].data[i] = data[i];
  }

  buf[write_index].data[3] =
      (data[3] & 0xfffffffeu) | static_cast<uint32_t>(cycle_bit);
}

void Ring::Push(const std::array<uint32_t, 4>& data) {
  CopyToLast(data);

  ++write_index;
  if (write_index == buf_size - 1) {
    LinkTRB link{buf};
    link.bits.toggle_cycle = true;
    CopyToLast(link.data);

    write_index = 0;
    cycle_bit = !cycle_bit;
  }
}

status::Status EventRing::Initialize(size_t buf_size,
                                     InterrupterRegisterSet* interrupter) {
  if (buf != nullptr) {
    FreeMem(buf);
  }

  cycle_bit = true;
  this->buf_size = buf_size;
  this->interrupter = interrupter;

  buf = AllocArray<TRB>(buf_size, 64, 64 * 1024);
  if (buf == nullptr) {
    return status::Status::NoEnoughMemory;
  }
  memset(buf, 0, buf_size * sizeof(TRB));

  erst = AllocArray<EventRingSegmentTableEntry>(1, 64, 64 * 1024);
  if (erst == nullptr) {
    FreeMem(buf);
    return status::Status::NoEnoughMemory;
  }

  erst[0].bits.ring_segment_base_address = reinterpret_cast<uint64_t>(buf);
  erst[0].bits.ring_segment_size = buf_size;

  ERSTSZ_Bitmap erstsz{};
  erstsz.SetSize(1);
  interrupter->ERSTSZ.Write(erstsz);

  WriteDequeuePointer(&buf[0]);

  ERSTBA_Bitmap erstba{};
  erstba.SetPointer(reinterpret_cast<uint64_t>(erst));
  interrupter->ERSTBA.Write(erstba);

  return status::Status::Success;
}

void EventRing::WriteDequeuePointer(TRB* p) {
  auto erdp = interrupter->ERDP.Read();
  erdp.SetPointer(reinterpret_cast<uint64_t>(p));
  interrupter->ERDP.Write(erdp);
}

void EventRing::Pop() {
  auto p = ReadDequeuePointer() + 1;

  TRB* segment_begin =
      reinterpret_cast<TRB*>(erst[0].bits.ring_segment_base_address);
  TRB* segment_end = segment_begin + erst[0].bits.ring_segment_size;

  if (p == segment_end) {
    p = segment_begin;
  }

  WriteDequeuePointer(p);
}
}  // namespace ndifixos::usb::xhci
