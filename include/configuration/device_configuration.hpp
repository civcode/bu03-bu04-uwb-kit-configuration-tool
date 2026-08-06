#ifndef DEVICE_CONFIG_HPP_
#define DEVICE_CONFIG_HPP_

#include <string>

struct DeviceParameters {
    int id;
    int role;
    int channel;
    int rate;
};

struct SensorData {
    float accX;
    float accY;
    float accZ;
    float angle;
};

struct TwrParameters {
    int tagCapacity;
    int antennaDelay;
    bool isKalmanFilterEnabled;
    float kalmanQ;
    float kalmanR;
    float correctionParameterA;
    float correctionParameterB;
    bool isPositioningEnabled;
    int positioningDimension;
};

struct PdoaParameters {
    int dlist;
    int klist;
    int net;
    int anchId;
    int rate;
    bool isFilterEnabled;
    int userCmd;
    int pdoaOffset;
    int rngOffset;
};

struct TagParameters {
    std::string a64;
    int a16;
    int F;
    int S;
    int M;
};

struct DeviceConfiguration {

    static constexpr int kCurrentVersion = 1;

    int version{kCurrentVersion};
    DeviceParameters deviceParam;
    TwrParameters twrParam;
    PdoaParameters pdoaParam;
};

#endif // DEVICE_CONFIG_HPP_