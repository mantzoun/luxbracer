#ifndef PLANET__H
#define PLANET__H

#include <cstdint>
#include <string>

//#include "stateEntity,h"
//#include "accessPoint"

namespace luxbracer {
    enum planetSize{
        PLANET_SIZE_TINY,
        PLANET_SIZE_SMALL,
        PLANET_SIZE_STANDARD,
        PLANET_SIZE_LARGE,
        PLANET_SIZE_HUGE,
    };

    enum planetGravity{
        PLANET_G_NEGLIGABLE,
        PLANET_G_LOW,
        PLANET_G_STANDARD,
        PLANET_G_HIGH,
        PLANET_G_EXTREME,
    };

    enum planetTemp{
        PLANET_TEMP_EXTREMECOLD,
        PLANET_TEMP_FREEZING,
        PLANET_TEMP_COLD,
        PLANET_TEMP_TEMPERATE,
        PLANET_TEMP_HOT,
        PLANET_TEMP_SCORCHING,
    };

    enum planetAtmoPress{
        PLANET_ATMO_NONE,
        PLANET_ATMO_THIN,
        PLANET_ATMO_STANDARD,
        PLANET_ATMO_THICK,
    };

    enum biosphereType {
        PLANET_BIO_NONE,
        PLANET_BIO_PROTOZOIC,
        PLANET_BIO_MATURE,
        PLANET_BIO_TERRAFORM,
    };


    class Planet {
        private:
            std::string     _name;
            std::string     _system;
            uint64_t        _id;
 //           List<stateEntity> _states
 //           List<accessPoint> _accessPoints
 //           planetType _type;
//            planetSize _size;
//            planetGravity _gravity;
//            planetTemp _temperature;
//            planetAtmoPress _atmospherePressure;
//            bool _atmoBreathable;
//            biosphereType _biosphere;
        public:
            Planet(const std::string& name, const std::string& system, uint64_t id);

            std::string name(void) const;
    };
}

#endif /* PLANET__H */
