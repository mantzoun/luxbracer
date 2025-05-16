/*
 * Copyright 2025 Stavros Mantzouneas
 */
#ifndef ENGINE_TYPES__H
#define ENGINE_TYPES__H

#include "json.hpp"
#include "dpp/dpp.h"

typedef std::string str;
typedef dpp::snowflake disc_id;

namespace luxbracer {
    enum engineError {
        ENGINE_OK,
        ENGINE_ERROR,
        ENGINE_ITEM_EXISTS,
    };

    enum planetGovernment {
        PLANET_GOV_STATES,
        PLANET_GOV_CORPORTATE,
        PLANET_GOV_SOCIALIST,
        PLANET_GOV_ANARCHY,
        PLANET_GOV_DICTATORSHIP,
        PLANET_GOV_MAX
    };

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
}

#endif /* ENGINE_TYPES__H */
