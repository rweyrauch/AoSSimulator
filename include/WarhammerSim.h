/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_H
#define WARHAMMERSIM_H

#include <cinttypes>
#include <string>
#include <vector>
#include "Dice.h"

enum class Phase
{
    Initiative,
    Hero,
    Movement,
    Shooting,
    Charge,
    Combat,
    Battleshock
};

enum RandomNumber
{
    RAND_D3 = -1,
    RAND_D6 = -2,
    RAND_2D6 = -3,
    RAND_3D6 = -4,
    RAND_4D6 = -5,
    RAND_2D3 = -6,
};

enum class PlayerId
{
    None = -1,
    Red = 0,
    Blue = 1
};

enum Rerolls
{
    NoRerolls = 0,
    RerollOnes,
    RerollOnesAndTwos,
    RerollSixes,
    RerollFailed,
};

const int NoSave = 7;

enum class Role
{
    Battleline,
    Leader,
    Other,
    Behemoth,
    LeaderBehemoth,
};

enum Keyword
{
    // Grand alliances
    ORDER = 0,
    CHAOS,
    DEATH,
    DESTRUCTION,

    // Factions
    STORMCAST_ETERNAL,
    KHORNE,
    SYLVANETH,
    GLOOMSPITE_GITZ,
    MOONCLAN,
    NIGHTHAUNT,
    DAUGHTERS_OF_KHAINE,
    IDONETH_DEEPKIN,
    BEASTS_OF_CHAOS,
    SLAANESH,
    TZEENTCH,
    NURGLE,
    SLAVES_TO_DARKNESS,
    FLESH_EATERS_COURT,
    GRAND_HOST_OF_NAGASH,
    LEGION_OF_BLOOD,
    LEGION_OF_NIGHT,
    LEGION_OF_SACRAMENT,
    SOULBLIGHT,
    BEASTCLAW_RAIDERS,
    BONESPLITTERZ,
    GREENSKINZ,
    IRONJAWZ,
    DARKLING_COVENS,
    DEVOTED_OF_SIGMAR,
    DISPOSSESSED,
    ELDRITCH_COUNCIL,
    FREE_PEOPLES,
    FYRESLAYERS,
    KHARADRON_OVERLORDS,
    ORDER_DRACONIS,
    ORDER_SERPENTIS,
    PHOENIX_TEMPLE,
    SCOURGE_PRIVATEERS,
    SERAPHON,
    SHADOWBLADES,
    SWIFTHAWK_AGENTS,
    WANDERER,
    SKAVEN,
    DEATHRATTLE,
    DEADWALKERS,

    // Common
    MORTAL,
    DAEMON,
    HUMAN,
    MONSTER,
    HERO,
    WIZARD,
    PRIEST,
    CELESTIAL,
    WAR_MACHINE,
    GROT,
    AELF,
    MALIGNANT,
    SUMMONABLE,
    TOTEM,
    SCENERY,
    GARGANT,
    DUARDIN,
    KNIGHTS,
    SERFS,

    // Stormcast
    SACROSANCT,
    ORDINATOS,
    CELESTAR_BALLISTA,
    JUSTICAR,
    JUDICATORS,
    REDEEMER,
    LIBERATORS,
    LORD_ORDINATOR,
    SEQUITORS,
    PALADIN,
    RETRIBUTORS,
    LORD_CELESTANT,
    KNIGHT_INCANTOR,
    KNIGHT_QUESTOR,
    KNIGHT_ZEPHYROS,
    VANGUARD_HUNTERS,
    ANGELOS,
    DRACOTH,
    STARDRAKE,
    CASTIGATORS,
    VANGUARD_RAPTORS,
    VANDUS_HAMMERHAND,

    // Stormcast Stormhosts
    HAMMERS_OF_SIGMAR,
    HALLOWED_KNIGHTS,
    CELESTIAL_VINDICATORS,
    ANVILS_OF_THE_HELDENHAMMER,
    KNIGHTS_EXCELSIOR,
    CELESTIAL_WARBRINGERS,
    TEMPEST_LORDS,
    ASTRAL_TEMPLARS,

    // Khorne
    BLOODBOUND,
    BLOODREAVERS,
    BLOOD_WARRIORS,
    BLOODLETTERS,
    SKULLREAPERS,
    WRATHMONGERS,
    KHORGORATHS,
    BLOODTHIRSTER,
    BLOODTHIRSTER_OF_UNFETTERED_FURY,
    BLOODTHIRSTER_OF_INSENSATE_RAGE,
    WRATH_OF_KHORNE_BLOODTHIRSTER,
    SKARBRAND,
    FLESH_HOUND,
    KARANAK,
    FLESH_HOUNDS,

    // Sylvaneth
    ALARIELLE_THE_EVERQUEEN,
    SPIRIT_OF_DURTHU,
    KURNOTH_HUNTERS,
    DRYADS,
    TREE_REVENANTS,
    SPITE_REVENANTS,
    SYLVANETH_WYLDWOOD,

    // Gloomspite Gitz
    SQUIG,
    SQUIG_HERD,
    SQUIG_HOPPERS,
    BOINGROT_BOUNDERZ,
    LOONBOSS,
    MANGLER_SQUIG,
    COLOSSAL_SQUIG,
    SHOOTAS,
    STABBAS,
    ROCKGUT,
    TROGGOTH,
    DANKHOLD,
    TROGGBOSS,
    SKRAGROTT,
    ALEGUZZLER,
    SQUIG_GOBBA,

    // Daughters of Khaine
    MELUSAI,
    KHINERAI_HARPIES,
    WITCH_AELVES,
    BLOOD_SISTERS,
    SISTERS_OF_SLAUGHTER,
    KHINERAI_LIFETAKERS,
    HAG_QUEEN,

    // Daughters of Khaine Temples
    HAGG_NAR,
    DRAICHI_GANETH,
    THE_KRAITH,
    KHAILEBRON,
    KHELT_NAR,

    // Idoneth Deepkin
    NAMARTI,
    THRALLS,
    REAVERS,
    AKHELIAN,
    AKHELIAN_GUARD,
    MORRSARR_GUARD,
    ISHLAEN_GUARD,

    // Nighthaunt
    GRIMGHAST_REAPERS,
    CHAINRASP_HORDE,
    BLADEGHEIST_REVENANTS,
    GLAIVEWRAITH_STALKERS,
    DREADSCYTHE_HARRIDANS,
    SPIRIT_TORMENTS,
    CHAINGHASTS,

    // Beasts of Chaos
    BULLGOR,
    BULLGORS,
    WARHERD,
    GHORGON,
    BRAYHERD,
    GOR,
    GORS,
    BESTIGORS,
    UNGORS,

    // Slaanesh
    DAEMONETTES,
    SEEKERS,
    FIENDS,

    // Nurgle
    PLAGUEBEARER,
    PLAGUEBEARERS,
    ROTBRINGER,
    PUTRID_BLIGHTKINGS,
    LORD_OF_PLAGUES,

    // Death
    BLACK_KNIGHTS,
    SKELETON,
    GRAVE_GUARD,
    SKELETON_WARRIORS,
    BLOOD_KNIGHTS,
    VAMPIRE,
    VARGHEISTS,
    ZOMBIE,
    DIRE_WOLVES,
    CORPSE_CARTS,

    // Flesh-eater Court
    MORDANT,
    COURTIER,
    ABHORRANT,
    CRYPT_GHAST_COURTIER,
    CRYPT_GHOULS,
    TERRORGHEIST,
    ABHORRANT_GHOUL_KING,
    ZOMBIE_DRAGON,
    CRYPT_FLAYERS,
    CRYPT_HAUNTER_COURTIER,
    CRYPT_HORRORS,
    CRYPT_INFERNAL_COURTIER,
    VARGHULF_COURTIER,
    PRINCE_VHORDRAI,

    // Wanderers,
    ETERNAL_GUARD,
    GLADE_GUARD,
    SISTERS_OF_THE_THORN,
    SISTERS_OF_THE_WATCH,
    WILDWOOD_RANGERS,

    // Dispossessed
    WARRIORS,
    HAMMERERS,
    QUARRELLERS,
    LONGBEARDS,
    THUNDERERS,
    IRONBREAKERS,
    IRONDRAKES,

    // Skaven
    SKAVENTIDE,
    CLANS_VERMINUS,
    CLANS_ESHIN,
    CLANS_MOULDER,
    CLANS_SKRYRE,
    CLANS_PESTILENS,
    STORMVERMIN,
    NIGHT_RUNNERS,
    GUTTER_RUNNERS,
    CLANRATS,
    STORMFIENDS,
    PLAGUE_MONKS,
    HELL_PIT_ABOMINATION,
    FIGHTING_BEAST,
};

enum Duration
{
    CurrentPhase,       // Spell, prayer, ability lasts unit the end of the current phase.
    StartOfNextHero,    // .. lasts unit the start of player's next hero phase.
};

enum class Verbosity
{
    Silence,
    Normal,
    Narrative,
    Debug
};

struct Wounds
{
    int normal;
    int mortal;

    Wounds& operator += (const Wounds& w)
    {
        normal += w.normal;
        mortal += w.mortal;
        return *this;
    }
};


enum class ParamType
{
    Boolean,
    Integer,
};

struct Parameter
{
    ParamType m_paramType;
    std::string m_name;
    union
    {
        bool m_boolValue;
        int m_intValue;
    };
    int m_minValue = 0;
    int m_maxValue = 0;
    int m_increment = 1;
};

typedef std::vector<Parameter> ParameterList;

std::string ParameterValueToString(const Parameter& param);
ParameterList::const_iterator FindParam(const std::string& name, const ParameterList& parameters);
int GetIntParam(const std::string& name, const ParameterList& parameters, int defaultValue);
bool GetBoolParam(const std::string& name, const ParameterList& parameters, bool defaultValue);

std::string PhaseToString(Phase phase);
std::string PlayerIdToString(PlayerId id);
PlayerId GetEnemyId(PlayerId friendlyId);

void Initialize(Verbosity verbosity = Verbosity::Normal);

Verbosity GetVerbosity();

float AverageRandomValue(int value);

#endif //WARHAMMERSIM_H
