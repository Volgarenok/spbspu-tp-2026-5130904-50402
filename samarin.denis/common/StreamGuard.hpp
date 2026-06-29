#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace samarin {
  class StreamGuard {
  public:
    StreamGuard(const StreamGuard &) = delete;
    StreamGuard(StreamGuard &&) = delete;
    explicit StreamGuard(std::basic_ios< char > &stream);
    ~StreamGuard();

    StreamGuard &operator=(const StreamGuard &) = delete;
    StreamGuard &operator=(StreamGuard &&) = delete;

  private:
    std::basic_ios< char > &stream_;
    std::ios_base::fmtflags flags_;
    std::streamsize precision_;
    char fill_;
  };
}

#endif
