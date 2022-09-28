#ifndef _MATPLOTLIB_H
#define _MATPLOTLIB_H

#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#define __XXX_YYY_ZZZ_POPEN___ _popen
#define __XXX_YYY_ZZZ_PCLOSE___ _pclose
#else
#include <memory>
#define __XXX_YYY_ZZZ_POPEN___ popen
#define __XXX_YYY_ZZZ_PCLOSE___ pclose
#endif

#define PY_MAJOR_VERSION 3
#include "matplotlibcpp.h"

std::string
__XXX_YYY_ZZZ_STRIP__(const std::string &str,
                      const std::string &whitespace = " \n\r\t\f\v") {
  size_t from = str.find_first_not_of(whitespace);

  if (from == std::string::npos) {
    return "";
  }
  size_t to = str.find_last_not_of(whitespace);
  assert(to != std::string::npos);

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
  const std::string CONDA_PREFIX = getenv("CONDA_PREFIX");
  if (CONDA_PREFIX.empty()) {
    // do default python config
  } else {
    const std::string PYTHON_VERSION = __XXX_YYY_ZZZ_COMMAND__(
        "python3 --version | cut -d ' ' -f2 | cut -d '.' -f-2");
    const std::string PYTHONHOME =
        std::string(getenv("CONDA_PREFIX")) + "/lib/python" + PYTHON_VERSION;
    const std::string PYTHONPATH = PYTHONHOME + ":" + PYTHONHOME +
                                   "/site-packages:" + PYTHONHOME +
                                   "/lib-dynload";
    setenv("PYTHONHOME", PYTHONHOME.c_str(), 1);
    setenv("PYTHONPATH", PYTHONPATH.c_str(), 1);
    std::cout << "PYTHON_VERSION: " << PYTHON_VERSION << std::endl;
    std::cout << "PYTHONHOME    : " << getenv("PYTHONHOME") << std::endl;
    std::cout << "PYTHONPATH    : " << getenv("PYTHONPATH") << std::endl;
  }

  return true;
}

static bool __UNUSED_XXX_YYY_ZZZ_INIT__ = __XXX_YYY_ZZZ_INIT__();

#endif
