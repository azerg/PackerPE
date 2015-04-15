#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "PeLib.h"


template<typename T>
std::string toString(T x, char f = '0')
{
  std::stringstream ss;
  ss << std::setw(sizeof(T) * 2) << std::setfill(f) << std::hex << std::uppercase << x;
  return ss.str();
}

template<>
std::string toString<PeLib::byte>(PeLib::byte x, char f)
{
  std::stringstream ss;
  ss << std::setw(2) << std::setfill(f) << std::hex << std::uppercase << (int)x;
  return ss.str();
}

std::string formatOutput(const std::string& text, const std::string& val, const std::string& pad = "", unsigned int maxsize = 70)
{
  std::stringstream ss;

  ss << pad << text << std::setw(maxsize - text.length() - val.length() - pad.length()) << std::setfill(' ') << "";
  ss << val;
  return ss.str();
}

std::string centerOutput(const std::string& text, unsigned int maxsize = 70)
{
  std::stringstream ss;

  unsigned int left = (maxsize - text.length()) / 2;
  ss << std::setw(left) << std::setfill(' ') << "";
  ss << text;

  return ss.str();
}

void dump(const std::string& d)
{
  std::cout << d << std::endl;
}