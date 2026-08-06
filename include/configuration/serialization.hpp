#ifndef SERIALIZATION_HPP_
#define SERIALIZATION_HPP_

#include "nlohmann/json.hpp"

#include "configuration/device_configuration.hpp"

using json = nlohmann::json;

inline void to_json(json& j, const DeviceParameters& p) {
    j = json{
        {"id", p.id},
        {"role", p.role},
        {"channel", p.channel},
        {"rate", p.rate}
    };
}

inline void from_json(const json& j, DeviceParameters& p) {
    j.at("id").get_to(p.id);
    j.at("role").get_to(p.role);
    j.at("channel").get_to(p.channel);
    j.at("rate").get_to(p.rate);
}

inline void to_json(json& j, const SensorData& s) {
    j = json{
        {"accX", s.accX},
        {"accY", s.accY},
        {"accZ", s.accZ},
        {"angle", s.angle}
    };
}

inline void from_json(const json& j, SensorData& s) {
    j.at("accX").get_to(s.accX);
    j.at("accY").get_to(s.accY);
    j.at("accZ").get_to(s.accZ);
    j.at("angle").get_to(s.angle);
}

inline void to_json(json& j, const TwrParameters& t) {
    j = json{
        {"tagCapacity", t.tagCapacity},
        {"antennaDelay", t.antennaDelay},
        {"isKalmanFilterEnabled", t.isKalmanFilterEnabled},
        {"kalmanQ", t.kalmanQ},
        {"kalmanR", t.kalmanR},
        {"correctionParameterA", t.correctionParameterA},
        {"correctionParameterB", t.correctionParameterB},
        {"isPositioningEnabled", t.isPositioningEnabled},
        {"positioningDimension", t.positioningDimension}
    };
}

inline void from_json(const json& j, TwrParameters& t) {
    j.at("tagCapacity").get_to(t.tagCapacity);
    j.at("antennaDelay").get_to(t.antennaDelay);
    j.at("isKalmanFilterEnabled").get_to(t.isKalmanFilterEnabled);
    j.at("kalmanQ").get_to(t.kalmanQ);
    j.at("kalmanR").get_to(t.kalmanR);
    j.at("correctionParameterA").get_to(t.correctionParameterA);
    j.at("correctionParameterB").get_to(t.correctionParameterB);
    j.at("isPositioningEnabled").get_to(t.isPositioningEnabled);
    j.at("positioningDimension").get_to(t.positioningDimension);
}

inline void to_json(json& j, const PdoaParameters& p) {
    j = json{
        {"dlist", p.dlist},
        {"klist", p.klist},
        {"net", p.net},
        {"anchId", p.anchId},
        {"rate", p.rate},
        {"isFilterEnabled", p.isFilterEnabled},
        {"userCmd", p.userCmd},
        {"pdoaOffset", p.pdoaOffset},
        {"rngOffset", p.rngOffset}
    };
}

inline void from_json(const json& j, PdoaParameters& p) {
    j.at("dlist").get_to(p.dlist);
    j.at("klist").get_to(p.klist);
    j.at("net").get_to(p.net);
    j.at("anchId").get_to(p.anchId);
    j.at("rate").get_to(p.rate);
    j.at("isFilterEnabled").get_to(p.isFilterEnabled);
    j.at("userCmd").get_to(p.userCmd);
    j.at("pdoaOffset").get_to(p.pdoaOffset);
    j.at("rngOffset").get_to(p.rngOffset);
}

inline void to_json(json& j, const TagParameters& t) {
    j = json{
        {"a64", t.a64},
        {"a16", t.a16},
        {"F", t.F},
        {"S", t.S},
        {"M", t.M}
    };
}

inline void from_json(const json& j, TagParameters& t) {
    j.at("a64").get_to(t.a64);
    j.at("a16").get_to(t.a16);
    j.at("F").get_to(t.F);
    j.at("S").get_to(t.S);
    j.at("M").get_to(t.M);
}

inline void to_json(json& j, const DeviceConfiguration& d) {
    j = json{
        {"version", d.version},
        {"deviceParam", d.deviceParam},
        {"twrParam", d.twrParam},
        {"pdoaParam", d.pdoaParam}
    };
}

inline void from_json(const json& j, DeviceConfiguration& d) {
    j.at("version").get_to(d.version);
    j.at("deviceParam").get_to(d.deviceParam);
    j.at("twrParam").get_to(d.twrParam);
    j.at("pdoaParam").get_to(d.pdoaParam);
}

#endif // SERIALIZATION_HPP_