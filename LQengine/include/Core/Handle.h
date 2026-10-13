#pragma once
#include <stdint.h>

#define LQ_HANDLE(_name)                                                             \
	struct _name { uint16_t idx; };                                                  \
	inline bool isValid(_name _handle) { return LQ::kInvalidHandle != _handle.idx; }

#define LQ_INVALID_HANDLE { LQ::kInvalidHandle }

namespace LQ {
  constexpr uint16_t kInvalidHandle = UINT16_MAX;
  template <typename T>
  struct Handle {
    struct Impl;

  protected:
    Impl* impl = nullptr;

  public:
    Handle() = default;
    explicit Handle(std::nullptr_t) : impl(nullptr) {};
    Handle(Impl* impl_) : impl(impl_) {};

    operator T() const noexcept {
      OX_CHECK_NULL(impl);
      return T(impl);
    }

    T unwrap() const noexcept {
      OX_CHECK_NULL(impl);
      return T(impl);
    }

    operator bool() const noexcept { return impl; }
    auto operator->() const noexcept {
      OX_CHECK_NULL(impl);
      return impl;
    }

    bool operator==(Handle other) const noexcept { return impl == other.impl; }
  };
}