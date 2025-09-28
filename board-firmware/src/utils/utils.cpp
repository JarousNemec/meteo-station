#include <utils/utils.h>

bool tryParseInt(const char *str, int &out) {
    errno = 0;
    char *end;
    const long v = strtol(str, &end, 10);
    if (errno == ERANGE || end == str) return false;
    while (*end) {
        if (!isspace(*end)) return false;
        end++;
    }
    if (v < -32768L || v > 32767L) return false;
    out = static_cast<int>(v);
    return true;
}

bool tryParseHttpTime(const char *dtStr, SimpleTime &dt) {
    char weekday[4], dayBuf[3], monBuf[4], yearBuf[5], hourBuf[3], minBuf[3], secBuf[3];

    //Wed, 09 Jul 2025 09:11:57 GMT
    const int ret = sscanf(dtStr, "%3s, %2s %3s %4s %2s:%2s:%2s GMT",
                           weekday, dayBuf, monBuf, yearBuf, hourBuf, minBuf, secBuf);
    if (ret != 7) return false;

    if (!tryParseInt(hourBuf, dt.hour)) {
        return false;
    }
    if (!tryParseInt(minBuf, dt.minute)) {
        return false;
    }
    if (!tryParseInt(secBuf, dt.second)) {
        return false;
    }
    dt.hour += 2;
    if (dt.hour == 24) dt.hour = 0;
    else if (dt.hour == 25) dt.hour = 1;

    Serial.print("Current parsed time is: ");
    Serial.print(dt.hour);
    Serial.print(":");
    Serial.print(dt.minute);
    Serial.print(":");
    Serial.println(dt.second);
    return true;
}

char *processIncomingData(const char *topic, const byte *payload, unsigned int length) {
    static char *data = nullptr;
    static unsigned int capacity = 0;

    if (length + 1 > capacity) {
        free(data);
        capacity = length + 1;
        data = static_cast<char *>(malloc(capacity));
        if (!data) {
            Serial.println("Memory alloc failed");
            capacity = 0;
            return nullptr;
        }
    }
    memcpy(data, payload, length);
    data[length] = '\0';

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    Serial.println(data);
    Serial.println("-----------------------");

    return data;
}
