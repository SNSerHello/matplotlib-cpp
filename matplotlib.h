#ifndef _XXX_YYY_ZZZ_MATPLOTLIB_H
#define _XXX_YYY_ZZZ_MATPLOTLIB_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#ifdef _WIN32
#define __XXX_YYY_ZZZ_POPEN___ _popen
#define __XXX_YYY_ZZZ_PCLOSE___ _pclose
#else
#define __XXX_YYY_ZZZ_POPEN___ popen
#define __XXX_YYY_ZZZ_PCLOSE___ pclose
#endif

std::string
__XXX_YYY_ZZZ_STRIP__(const std::string &str,
                      const std::string &whitespace = " \n\r\t\f\v") {
  size_t from = str.find_first_not_of(whitespace);

  if (from == std::string::npos) {
    return "";
  }
  size_t to = str.find_last_not_of(whitespace);

  return str.substr(from, (to - from) + 1);
}

std::string __XXX_YYY_ZZZ_COMMAND__(const std::string &cmd) {
  using pipe_ptr = std::unique_ptr<FILE, decltype(__XXX_YYY_ZZZ_PCLOSE___) *>;
  pipe_ptr pipe(__XXX_YYY_ZZZ_POPEN___(cmd.c_str(), "r"),
                __XXX_YYY_ZZZ_PCLOSE___);
  if (pipe == nullptr) {
    std::cout << "error: failed to execute: " << cmd << std::endl;
    return "";
  }

  const int BUF_SIZE = 1023;
  char buf[BUF_SIZE + 1];
  buf[BUF_SIZE] = '\0';

  std::stringstream ss;
  while (fgets(buf, BUF_SIZE, pipe.get()) != NULL) {
    ss << buf;
  }

  if (__XXX_YYY_ZZZ_PCLOSE___(pipe.release()) != 0) {
    return "";
  }

  return __XXX_YYY_ZZZ_STRIP__(ss.str());
}

static bool __XXX_YYY_ZZZ_INIT__() {
  auto myenv = [](const std::string &name) {
    const char *value = getenv(name.c_str());
    return std::string(value == NULL ? "" : value);
  };
  const std::string CONDA_PREFIX = myenv("CONDA_PREFIX");
  if (CONDA_PREFIX.empty()) {
    // do default python config
  } else {
    const std::string PYTHON_VERSION = __XXX_YYY_ZZZ_COMMAND__(
        "python3 --version | cut -d ' ' -f2 | cut -d '.' -f-2");
#ifdef _WIN32
    const std::string PYTHONHOME = CONDA_PREFIX + R"(\lib)";
    const std::string PYTHONPATH = PYTHONHOME + ";" + PYTHONHOME +
                                   R"(\site-packages)";
    _putenv_s("PYTHONHOME", PYTHONHOME.c_str());
    _putenv_s("PYTHONPATH", PYTHONPATH.c_str());
#else
    const std::string PYTHONHOME =
        CONDA_PREFIX + "/lib/python" + PYTHON_VERSION;
    const std::string PYTHONPATH = PYTHONHOME + ":" + PYTHONHOME +
                                   "/site-packages:" + PYTHONHOME +
                                   "/lib-dynload";
    setenv("PYTHONHOME", PYTHONHOME.c_str(), 1);
    setenv("PYTHONPATH", PYTHONPATH.c_str(), 1);
#endif
    std::cout << "PYTHON_VERSION: " << PYTHON_VERSION << std::endl;
    std::cout << "PYTHONHOME    : " << myenv("PYTHONHOME") << std::endl;
    std::cout << "PYTHONPATH    : " << myenv("PYTHONPATH") << std::endl;
  }

  return true;
}

static bool __UNUSED_XXX_YYY_ZZZ_INIT__ = __XXX_YYY_ZZZ_INIT__();

#define PY_MAJOR_VERSION 3
#include "matplotlibcpp.h"

#endif
