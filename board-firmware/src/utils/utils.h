#pragma once
#include <Arduino.h>
#include <types/types.h>

#ifndef UTILS_H
#define UTILS_H
bool tryParseInt(const char *str, int &out);
bool tryParseHttpTime(const char *dtStr, SimpleTime &dt);
char *processIncomingData(const char *topic, const byte *payload, unsigned int length);
#endif //UTILS_H
