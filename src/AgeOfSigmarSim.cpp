/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <AgeOfSigmarSim.h>
#include <UnitFactory.h>
#include <map>
#include <algorithm>
#include <sstream>
#include <cstdarg>
#include <magic_enum.hpp>

std::string PhaseToString(GamePhase phase) {
    return std::string(magic_enum::enum_name(phase));
}

std::string PlayerIdToString(PlayerId id) {
    return std::string(magic_enum::enum_name(id));
}

double AverageRandomValue(int value) {
    constexpr double avgD3 = (1 + 2 + 3) / 3.0;
    constexpr double avgD6 = (1 + 2 + 3 + 4 + 5 + 6) / 6.0;

    auto avgValue = (double) value;
    switch (value) {
        case RAND_D3:
            avgValue = avgD3;
            break;
        case RAND_2D3:
            avgValue = 2.0 * avgD3;
            break;
        case RAND_D6:
            avgValue = avgD6;
            break;
        case RAND_2D6:
            avgValue = 2.0 * avgD6;
            break;
        case RAND_3D6:
            avgValue = 3.0 * avgD6;
            break;
        case RAND_4D6:
            avgValue = 4.0 * avgD6;
            break;
        default:
            break;
    }
    return avgValue;
}

PlayerId GetEnemyId(PlayerId friendlyId) {
    if (friendlyId == PlayerId::Red) {
        return PlayerId::Blue;
    } else if (friendlyId == PlayerId::Blue) {
        return PlayerId::Red;
    }
    return PlayerId::None;
}


static const std::map<std::string, Keyword> g_allianceNameLookup = {
        {"Order",       ORDER},
        {"Chaos",       CHAOS},
        {"Death",       DEATH},
        {"Destruction", DESTRUCTION}
};

static const std::map<std::string, Keyword> g_factionNameLookup = {
        {"Stormcast Eternal",    STORMCAST_ETERNAL},
        {"Khorne",               KHORNE},
        {"Sylvaneth",            SYLVANETH},
        {"Gloomspite Gitz",      GLOOMSPITE_GITZ},
        {"Moonclan",             MOONCLAN},
        {"Nighthaunt",           NIGHTHAUNT},
        {"Daughters of Khaine",  DAUGHTERS_OF_KHAINE},
        {"Idoneth Deepkin",      IDONETH_DEEPKIN},
        {"Beasts of Chaos",      BEASTS_OF_CHAOS},
        {"Slaanesh",             SLAANESH},
        {"Tzeentch",             TZEENTCH},
        {"Nurgle",               NURGLE},
        {"Slaves to Darkness",   SLAVES_TO_DARKNESS},
        {"Flesh Eater Courts",   FLESH_EATER_COURTS},
        {"Grand Host of Nagash", GRAND_HOST_OF_NAGASH},
        {"Legion of Blood",      LEGION_OF_BLOOD},
        {"Legion of Night",      LEGION_OF_NIGHT},
        {"Legion of Sacrament",  LEGION_OF_SACRAMENT},
        {"Soulblight",           SOULBLIGHT},
        {"Beastclaw Raiders",    BEASTCLAW_RAIDERS},
        {"Bonesplitterz",        BONESPLITTERZ},
        {"Greenskinz",           GREENSKINZ},
        {"Ironjawz",             IRONJAWZ},
        {"Darkling Covens",      DARKLING_COVENS},
        {"Devoted of Sigmar",    DEVOTED_OF_SIGMAR},
        {"Dispossessed",         DISPOSSESSED},
        {"Eldritch Council",     ELDRITCH_COUNCIL},
        {"Free Peoples",         FREE_PEOPLES},
        {"Fyreslayers",          FYRESLAYERS},
        {"Kharadron Overlords",  KHARADRON_OVERLORDS},
        {"Order Draconis",       ORDER_DRACONIS},
        {"Order Serpentis",      ORDER_SERPENTIS},
        {"Phoenix Temple",       PHOENIX_TEMPLE},
        {"Scourge Privateers",   SCOURGE_PRIVATEERS},
        {"Seraphon",             SERAPHON},
        {"Shadowblades",         SHADOWBLADES},
        {"Swifthawk Agents",     SWIFTHAWK_AGENTS},
        {"Wanderers",            WANDERER},
        {"Skaven",               SKAVEN},
        {"Deathrattle",          DEATHRATTLE},
        {"Deadwalkers",          DEADWALKERS},
        {"Deathmages",           DEATHMAGES},
        {"Deathlords",           DEATHLORDS},
        {"Everchosen",           EVERCHOSEN},
        {"Free Agent",           FREE_AGENT},
        {"Bonesplitterz",        BONESPLITTERZ},
        {"Cities of Sigmar",     CITIES_OF_SIGMAR},
        {"Gutbusters",           GUTBUSTERS},
        {"Ogor Mawtribes",       OGOR_MAWTRIBES},
        {"Ossiarch Bonereapers", OSSIARCH_BONEREAPERS},
        {"Lumineth Realm-Lords", LUMINETH_REALM_LORDS},
        {"Sons of Behemat",      SONS_OF_BEHEMAT}
};

Keyword GrandAllianceStringToKeyword(const std::string &allianceName) {
    auto ki = g_allianceNameLookup.find(allianceName);
    if (ki != g_factionNameLookup.end())
        return ki->second;
    return UNKNOWN;
}

Keyword FactionStringToKeyword(const std::string &factionName) {
    auto ki = g_factionNameLookup.find(factionName);
    if (ki != g_factionNameLookup.end())
        return ki->second;
    return UNKNOWN;
}

static const std::string g_notFound("Not Found");

const std::string &GrandAllianceKeywordToString(Keyword ga) {
    for (const auto &ip : g_allianceNameLookup) {
        if (ip.second == ga)
            return ip.first;
    }
    return g_notFound;
}

const std::string &FactionKeywordToString(Keyword faction) {
    for (const auto &ip : g_factionNameLookup) {
        if (ip.second == faction)
            return ip.first;
    }
    return g_notFound;
}

bool Expired(const Duration &duration, const Duration &current) {
    return ((duration.player == current.player) && (duration.round < current.round) &&
            (duration.phase == current.phase));
}

std::ostream &operator<<(std::ostream &os, const Wounds &wounds) {
    os << "{" << wounds.normal << ", " << wounds.mortal << "}: " << magic_enum::enum_name(wounds.source);
    return os;
}
