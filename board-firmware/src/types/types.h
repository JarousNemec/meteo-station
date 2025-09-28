#ifndef TYPES_H
#define TYPES_H

struct SimpleTime {
    int hour, minute, second;

    int getTimestamp() const {
        return hour * 3600 + minute * 60 + second;
    }
};
#endif //TYPES_H
