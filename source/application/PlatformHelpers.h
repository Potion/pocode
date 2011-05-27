#pragma once

class poRect;
class poTexture;

double getTime();
unsigned int getNumCpus();

// generates a texture wrapped to bounds with font mapping applied to the document
// tex is allocated, stripped contains text with no markup
void renderTextToTexture(poRect rect, TiXmlDocument *doc, const poFontMap &fonts, poTexture **tex, std::string *stripped, poRect *actual);

// http://canonical.org/~kragen/strlen-utf8.html
int utf8strlen(const std::string &str);
