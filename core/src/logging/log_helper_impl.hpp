#pragma once

#include <optional>
#include <ostream>

#include <logging/level.hpp>
#include <logging/log.hpp>
#include <logging/log_extra.hpp>
#include <logging/spdlog.hpp>

namespace logging {

class LogHelper::Impl final {
 public:
  using char_type = std::streambuf::char_type;
  using int_type = std::streambuf::int_type;

  explicit Impl(LoggerPtr logger, Level level) noexcept;

  void SetEncoding(Encode encode_mode) noexcept { encode_mode_ = encode_mode; }

  spdlog::details::log_msg& Message() noexcept { return msg_; }
  std::size_t Capacity() const noexcept { return msg_.raw.capacity(); }

  bool IsStreamInitialized() const noexcept { return !!lazy_stream_; }
  std::ostream& Stream() { return GetLazyInitedStream().ostr; }
  std::streambuf* StreamBuf() { return &GetLazyInitedStream().sbuf; }

  LogExtra& GetLogExtra() { return extra_; }

  std::streamsize xsputn(const char_type* s, std::streamsize n);
  int_type overflow(int_type c);

  const LoggerPtr& GetLogger() const { return logger_; }

 private:
  class BufferStd final : public std::streambuf {
   public:
    explicit BufferStd(Impl& impl) : impl_{impl} {}

   private:
    int_type overflow(int_type c) override;
    std::streamsize xsputn(const char_type* s, std::streamsize n) override;

    Impl& impl_;
  };

  struct LazyInitedStream {
    BufferStd sbuf;
    std::ostream ostr;

    explicit LazyInitedStream(Impl& impl) : sbuf{impl}, ostr(&sbuf) {}
  };

  LazyInitedStream& GetLazyInitedStream();

 private:
  LoggerPtr logger_;
  spdlog::details::log_msg msg_;
  Encode encode_mode_;
  std::optional<LazyInitedStream> lazy_stream_;
  LogExtra extra_;
};

}  // namespace logging
