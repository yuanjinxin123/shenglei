/**
 * @file defer.h
 * @author shenqd
 * @brief
 * @version 0.1
 * @date 2022-04-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <functional>
#include <utility>
class DeferHelper {
 public:
  DeferHelper(std::function<void()> &&func) : m_func(std::move(func)){};

  virtual ~DeferHelper() {
    if (m_func) m_func();
  };

 private:
  std::function<void()> m_func;
};
#define CONNECTION(text1, text2) text1##text2
#define CONNECT(text1, text2) CONNECTION(text1, text2)
#define defer(code) DeferHelper CONNECT(L, __LINE__)([&]() { code; })
