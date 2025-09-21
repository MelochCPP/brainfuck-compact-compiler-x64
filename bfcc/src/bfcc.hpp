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
#include <map>

typedef std::string str;
using namespace std;

#include "bfcclib\file.hpp"

#include "bfstart.hpp"
#include "output\outcoff.hpp"


namespace global_vars
{
    inline str platform;
    inline str inputfilename;
    inline str outputfilename;
    inline unsigned long long memory_size;
    inline bool start;
}