#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <iostream>
#include <cerrno>
#include <map>
#include <stack>

typedef std::string str;
using namespace std;

namespace global_vars
{
    inline str entryname;
    inline str platform;
    inline str inputfilename;
    inline str outputfilename;
    inline unsigned long long memory_size;
    inline bool start;
}

#include "bfcclib\file.hpp"

#include "bfstart.hpp"
#include "output\outcoff.hpp"


