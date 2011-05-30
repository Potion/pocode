#pragma once

#include "poRect.h"
#include "poFont.h"
#include "poTexture.h"

double getTime();
unsigned int getNumCpus();

// http://canonical.org/~kragen/strlen-utf8.html
int utf8strlen(const std::string &str);

