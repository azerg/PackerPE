// SimplePackPE.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <boost/program_options.hpp>
#include "PeLib.h"

std::vector<uint8_t> readFile(const _TCHAR* filename)
{
  std::streampos fileSize;
  std::ifstream file(filename, std::ios::binary);

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> fileData(static_cast<unsigned int>(fileSize));
  file.read((char*)&fileData[0], fileSize);
  return fileData;
}

namespace po = boost::program_options;

struct CmdCtx
{
  std::string infile;
  std::string outfile;
};

typedef decltype(CmdCtx::infile) InifileType;
typedef decltype(CmdCtx::outfile) OutfileType;

#define IN_FILE_CMD "infile"
#define OUT_FILE_CMD "outfile"

CmdCtx CmdToContext(po::variables_map&& vm)
{
  CmdCtx ctx;

  if (vm.count(IN_FILE_CMD)) {
    ctx.infile = vm[IN_FILE_CMD].as<InifileType>();
  }

  if (vm.count(OUT_FILE_CMD)) {
    ctx.infile = vm[OUT_FILE_CMD].as<OutfileType>();
  }

  return ctx;
}



int _tmain(int argc, _TCHAR* argv[])
{
  po::options_description desc("Allowed options");

  desc.add_options()
    ("help,h", "produce help message")
    ("compression", po::value<int>(), "set compression level")
    (IN_FILE_CMD",i", po::value<InifileType>(), "source file")
    (OUT_FILE_CMD",o", po::value<OutfileType>(), "output file.If not defined, \"source_name.BAK\" will be used.")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  auto ctx = CmdToContext(std::move(vm));

  auto pef = PeLib::openPeFile(ctx.infile);
  if (!pef)
  {
    std::cout << "Invalid PE File" << std::endl;
    return 2;
  }

  return 0;
}

