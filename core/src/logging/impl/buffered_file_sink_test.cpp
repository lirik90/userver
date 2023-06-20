#include "buffered_file_sink.hpp"

#include <gtest/gtest.h>

#include <userver/engine/async.hpp>
#include <userver/fs/blocking/read.hpp>
#include <userver/fs/blocking/temp_directory.hpp>
#include <userver/utest/utest.hpp>
#include <userver/utils/rand.hpp>
#include <userver/utils/text.hpp>

#include "sink_helper_test.hpp"

USERVER_NAMESPACE_BEGIN

namespace {
const auto msg_a = std::string(test::kEightMb, 'a');
const auto msg_b = std::string(test::kEightMb, 'b');
const auto msg_c = std::string(test::kEightMb, 'c');

const auto msg_d = std::string(test::kOneKb, 'd');
}  // namespace

UTEST(BufferedFileSink, StdoutSinkLog) {
  auto sink = logging::impl::BufferedStdoutFileSink();
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
}

UTEST(BufferedFileSink, StderrSinkLog) {
  auto sink = logging::impl::BufferedStderrFileSink();
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::critical, "message"}));
}

UTEST(BufferedFileSink, TestCreateFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
}

UTEST(BufferedFileSink, TestCreateFileMultiDir) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename = temp_root.GetPath() +
                               "/dir1/dir2/dir3/temp_file_" +
                               std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
}

UTEST(BufferedFileSink, TestWriteInFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::critical, "message"}));
}

UTEST(BufferedFileSink, CheckPermissionsFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  boost::filesystem::file_status stat = boost::filesystem::status(filename);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::owner_read);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::owner_write);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::owner_exe);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::group_read);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::group_write);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::group_exe);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::others_read);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::others_write);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::others_exe);
}

UTEST(BufferedFileSink, CheckPermissionsDir) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/dir/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  auto path = boost::filesystem::path(filename).parent_path();
  boost::filesystem::file_status stat = boost::filesystem::status(path);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::owner_read);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::owner_write);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::owner_exe);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::group_read);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::group_write);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::group_exe);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::others_read);
  ASSERT_FALSE(stat.permissions() & boost::filesystem::others_write);
  ASSERT_TRUE(stat.permissions() & boost::filesystem::others_exe);
}

UTEST(BufferedFileSink, TestValidWriteInFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");
}

UTEST(BufferedFileSink, TestReopenWithTruncate) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kTruncate));
  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());
}

UTEST(BufferedFileSink, TestReopenWithTruncateWrite) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kTruncate));
  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());

  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::info, "message 2"}));
  EXPECT_NO_THROW(sink.Flush());

  content = fs::blocking::ReadFileContents(filename);

  result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [info] message 2");
}

UTEST(BufferedFileSink, TestReopenWithoutTruncate) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kAppend));
  content = fs::blocking::ReadFileContents(filename);

  const auto result_reopen = test::NormalizeLogs(content);

  EXPECT_EQ(result_reopen.size(), 1);
  EXPECT_EQ(result_reopen.front(), "[datetime] [default] [warning] message");
}

UTEST(BufferedFileSink, TestReopenBeforeRemove) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  boost::filesystem::remove(filename);
  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kAppend));
  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());
}

UTEST(BufferedFileSink, TestReopenBeforeRemoveCreate) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");
  namespace b_fs = boost::filesystem;

  b_fs::remove(filename);

  // Create file manually
  auto fd = ::open(filename.c_str(), O_CLOEXEC | O_CREAT | O_WRONLY | O_APPEND,
                   b_fs::owner_write | b_fs::owner_read | b_fs::group_read |
                       b_fs::others_read);
  ASSERT_TRUE(fd != -1);
  ::close(fd);

  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kAppend));
  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());

  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  content = fs::blocking::ReadFileContents(filename);

  result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");
}

UTEST(BufferedFileSink, TestReopenBeforeRemoveAndWrite) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  boost::filesystem::remove(filename);
  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kAppend));
  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());

  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  content = fs::blocking::ReadFileContents(filename);

  result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");
}

UTEST(BufferedFileSink, TestReopenMoveFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());

  const std::string filename_2 =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());

  ASSERT_TRUE(filename != filename_2);

  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");

  ::rename(filename.c_str(), filename_2.c_str());

  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::info, "message 2"}));
  EXPECT_NO_THROW(sink.Flush());

  content = fs::blocking::ReadFileContents(filename_2);
  ASSERT_FALSE(content.empty());
  result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "[datetime] [default] [warning] message");
  EXPECT_EQ(result[1], "[datetime] [default] [info] message 2");

  EXPECT_NO_THROW(sink.Reopen(logging::impl::ReopenMode::kAppend));

  content = fs::blocking::ReadFileContents(filename);
  ASSERT_TRUE(content.empty());

  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Flush());

  content = fs::blocking::ReadFileContents(filename);

  result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result.front(), "[datetime] [default] [warning] message");
}

UTEST(BufferedFileSink, TestValidWriteMultiInFile) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
  EXPECT_NO_THROW(sink.Log({"basic", spdlog::level::info, "message 2"}));
  EXPECT_NO_THROW(sink.Log({"current", spdlog::level::critical, "message 3"}));
  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);

  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "[datetime] [default] [warning] message");
  EXPECT_EQ(result[1], "[datetime] [basic] [info] message 2");
  EXPECT_EQ(result[2], "[datetime] [current] [critical] message 3");
}

UTEST_MT(BufferedFileSink, WriteDataAsync, 4) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  auto task_1 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_2 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(sink.Log({"basic", spdlog::level::info, "message 2"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_3 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(
        sink.Log({"current", spdlog::level::critical, "message 3"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  task_1.Get();
  task_2.Get();
  task_3.Get();

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 3);
  auto expect_predicate = [](auto str) {
    return str == "[datetime] [default] [warning] message" ||
           str == "[datetime] [basic] [info] message 2" ||
           str == "[datetime] [current] [critical] message 3";
  };
  EXPECT_PRED1(expect_predicate, result[0]);
  EXPECT_PRED1(expect_predicate, result[1]);
  EXPECT_PRED1(expect_predicate, result[2]);
}

UTEST_MT(BufferedFileSink, WriteDataAsyncBigString, 4) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  auto task_1 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, msg_a}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_2 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(sink.Log({"basic", spdlog::level::info, msg_b}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_3 = engine::AsyncNoSpan([&sink] {
    EXPECT_NO_THROW(sink.Log({"current", spdlog::level::critical, msg_c}));
    EXPECT_NO_THROW(sink.Flush());
  });
  task_1.Get();
  task_2.Get();
  task_3.Get();

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 3);
  auto expect_predicate = [](auto str) {
    return str == "[datetime] [default] [warning] " + msg_a ||
           str == "[datetime] [basic] [info] " + msg_b ||
           str == "[datetime] [current] [critical] " + msg_c;
  };
  EXPECT_PRED1(expect_predicate, result[0]);
  EXPECT_PRED1(expect_predicate, result[1]);
  EXPECT_PRED1(expect_predicate, result[2]);
}

TEST(BufferedFileSink, WriteDataAsyncNoCore) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  auto task_1 = std::thread([&sink] {
    EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, "message"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_2 = std::thread([&sink] {
    EXPECT_NO_THROW(sink.Log({"basic", spdlog::level::info, "message 2"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_3 = std::thread([&sink] {
    EXPECT_NO_THROW(
        sink.Log({"current", spdlog::level::critical, "message 3"}));
    EXPECT_NO_THROW(sink.Flush());
  });
  task_1.join();
  task_2.join();
  task_3.join();

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 3);
  auto expect_predicate = [](auto str) {
    return str == "[datetime] [default] [warning] message" ||
           str == "[datetime] [basic] [info] message 2" ||
           str == "[datetime] [current] [critical] message 3";
  };
  EXPECT_PRED1(expect_predicate, result[0]);
  EXPECT_PRED1(expect_predicate, result[1]);
  EXPECT_PRED1(expect_predicate, result[2]);
}

TEST(BufferedFileSink, WriteDataAsyncNoCoreBigString) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));
  auto task_1 = std::thread([&sink] {
    EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, msg_a}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_2 = std::thread([&sink] {
    EXPECT_NO_THROW(sink.Log({"basic", spdlog::level::info, msg_b}));
    EXPECT_NO_THROW(sink.Flush());
  });
  auto task_3 = std::thread([&sink] {
    EXPECT_NO_THROW(sink.Log({"current", spdlog::level::critical, msg_c}));
    EXPECT_NO_THROW(sink.Flush());
  });
  task_1.join();
  task_2.join();
  task_3.join();

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), 3);
  auto expect_predicate = [](auto str) {
    return str == "[datetime] [default] [warning] " + msg_a ||
           str == "[datetime] [basic] [info] " + msg_b ||
           str == "[datetime] [current] [critical] " + msg_c;
  };
  EXPECT_PRED1(expect_predicate, result[0]);
  EXPECT_PRED1(expect_predicate, result[1]);
  EXPECT_PRED1(expect_predicate, result[2]);
}

UTEST_MT(BufferedFileSink, WriteDataAsyncWithReopen, 4) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));

  constexpr size_t count_tasks = 50;
  std::vector<engine::TaskWithResult<void>> tasks_logs{};
  std::vector<engine::TaskWithResult<void>> tasks_reopen{};

  tasks_logs.resize(count_tasks);
  tasks_logs.resize(count_tasks);

  for (auto& task : tasks_logs) {
    task = engine::AsyncNoSpan([&sink] {
      EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, msg_d}));
    });
  }
  for (auto& task : tasks_reopen) {
    task = engine::AsyncNoSpan(
        [&sink] { sink.Reopen(logging::impl::ReopenMode::kAppend); });
  }

  for (auto& task : tasks_logs) {
    task.Get();
  }
  for (auto& task : tasks_reopen) {
    task.Get();
  }

  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), count_tasks);
  for (const auto& log : result) {
    EXPECT_EQ(log, "[datetime] [default] [warning] " + msg_d);
  }
}

TEST(BufferedFileSink, WriteDataAsyncNoCoreWithReopen) {
  const auto temp_root = fs::blocking::TempDirectory::Create();
  const std::string filename =
      temp_root.GetPath() + "/temp_file_" + std::to_string(utils::Rand());
  auto sink = logging::impl::BufferedFileSink(filename);
  ASSERT_TRUE(boost::filesystem::exists(filename));

  constexpr size_t count_tasks = 50;
  std::vector<std::thread> tasks_logs{};
  std::vector<std::thread> tasks_reopen{};

  tasks_logs.resize(count_tasks);
  tasks_logs.resize(count_tasks);

  for (auto& task : tasks_logs) {
    task = std::thread([&sink] {
      EXPECT_NO_THROW(sink.Log({"default", spdlog::level::warn, msg_d}));
    });
  }
  for (auto& task : tasks_reopen) {
    task = std::thread(
        [&sink] { sink.Reopen(logging::impl::ReopenMode::kAppend); });
  }

  for (auto& task : tasks_logs) {
    task.join();
  }
  for (auto& task : tasks_reopen) {
    task.join();
  }

  EXPECT_NO_THROW(sink.Flush());

  auto content = fs::blocking::ReadFileContents(filename);
  const auto result = test::NormalizeLogs(content);

  EXPECT_EQ(result.size(), count_tasks);
  for (const auto& log : result) {
    EXPECT_EQ(log, "[datetime] [default] [warning] " + msg_d);
  }
}

USERVER_NAMESPACE_END
