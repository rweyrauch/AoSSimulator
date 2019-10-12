/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AGEOFSIGMARSIM_H
#define AGEOFSIGMARSIM_H

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

enum BuffableAttribute
{
    MoveDistance = 0,
    RunDistance,
    ChargeDistance,
    ToHitMissile,
    ToHitMelee,
    ToWoundMissile,
    ToWoundMelee,
    ToSave,
    Bravery,
    CastingRoll,
    UnbindingRoll,
    TargetToHitMelee,
    TargetToHitMissile,
    TargetToWoundMelee,
    TargetToWoundMissile,
    TargetToSave,

    NUM_BUFFABLE_ATTRIBUTES
};

enum MovementRules
{
    Fly = 0,
    RunAndShoot,
    RunAndCharge,
    RetreatAndShoot,
    RetreatAndCharge,

    NUM_MOVEMENT_RULES
};

enum Keyword
{
    UNKNOWN = -1,

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
    FLESH_EATER_COURTS,
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
    DEATHMAGES,
    DEATHLORDS,
    EVERCHOSEN,
    FREE_AGENT,

    // Common
    MORTAL,
    DAEMON,
    GREATER_DAEMON,
    HUMAN,
    MONSTER,
    HERO,
    WIZARD,
    PRIEST,
    CELESTIAL,
    WAR_MACHINE,
    WEAPON_TEAM,
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
    WARLOCK,
    ENGINEER,
    ORRUK,
    DRAGON,
    MONSTERS_OF_CHAOS,

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
    PROTECTORS,
    DECIMATORS,
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
    GRYPH_HOUNDS,
    CONCUSSORS,
    DESOLATORS,
    FULMINATORS,
    TEMPESTORS,
    DRACOTHIAN_GUARD,
    LORD_CASTELLANT,
    LORD_VERITANT,
    CORPUSCANT,
    EVOCATORS,
    DRACOLINE,
    PROSECUTORS,
    NEAVE_BLACKTALON,
    GAVRIEL_SUREHEART,
    LORD_EXORCIST,
    LORD_RELICTOR,
    LORD_ARCANUM,
    GRYPH_CHARGER,
    TAURALON,
    LORD_AQUILOR,
    KNIGHT_AZYROS,
    KNIGHT_HERALDOR,
    KNIGHT_VENATOR,
    KNIGHT_VEXILLOR,
    AVENTIS_FIRESTRIKE,
    ASTREIA_SOLBRIGHT,
    CELESTANT_PRIME,
    VANGUARD_PALLADORS,
    DRAKESWORN_TEMPLAR,

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
    BLOODLETTER,
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
    HERALD_OF_KHORNE,
    SKULLTAKER,
    BLOODMASTER,
    MIGHTY_SKULLCRUSHERS,
    BLOODCRUSHERS,
    SKULLMASTER,
    VORGAROTH_THE_SCARRED_AND_SKALOK_THE_SKULL_HOST_OF_KHORNE,
    MIGHTY_LORD_OF_KHORNE,
    SLAUGHTERPRIEST,
    KORGHOS_KHUL,
    EXALTED_DEATHBRINGER,
    HERALD_OF_KHORNE_ON_BLOOD_THRONE,
    SKULL_CANNONS,
    VALKIA_THE_BLOODY,
    BLOODSECRATOR,
    BLOODSTOKER,
    ASPIRING_DEATHBRINGER,
    SKULLGRINDER,
    SCYLA_ANFINGRIMM,
    LORD_OF_KHORNE_ON_JUGGERNAUT,

    // Khorne Slaughter Hosts
    REAPERS_OF_VENGEANCE,
    BLOODLORDS,
    GORETIDE,
    SKULLFIEND_TRIBE,

    // Sylvaneth
    ALARIELLE_THE_EVERQUEEN,
    SPIRIT_OF_DURTHU,
    KURNOTH_HUNTERS,
    DRYADS,
    TREE_REVENANTS,
    SPITE_REVENANTS,
    AWAKENED_WYLDWOOD,
    DRYCHA_HAMADRETH,
    TREELORD_ANCIENT,
    TREELORD,
    BRANCHWYCH,
    BRANCHWRAITH,
    SYLVANETH_WILDWOOD,
    FREE_SPIRITS,
    FOREST_FOLK,
    NOBLE_SPIRITS,
    ARCH_REVENANT,
    SKAETHS_WILD_HUNT,
    YLTHARI,
    YLTHARIS_GUARDIANS,
    THORNWYCH,

    // Sylvaneth Glades
    OAKENBROW,
    GNARLROOT,
    HEARTWOOD,
    IRONBARK,
    WINTERLEAF,
    DREADWOOD,
    HARVESTBOON,

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
    SPIDERFANG,
    SPIDER_RIDERS,
    FELLWATER,
    FUNGOID_CAVE_SHAMAN,
    MADCAP_SHAMAN,
    WEBSPINNER_SHAMAN,
    ARACHNAROK_SPIDER,
    SKITTERSTRAND,
    SNEAKY_SNUFFERS,
    SCUTTLEBOSS,

    // Daughters of Khaine
    MELUSAI,
    KHINERAI_HARPIES,
    WITCH_AELVES,
    BLOOD_SISTERS,
    SISTERS_OF_SLAUGHTER,
    KHINERAI_LIFETAKERS,
    HAG_QUEEN,
    KHINERAI_HEARTRENDERS,
    DOOMFIRE_WARLOCKS,
    BLOOD_STALKERS,
    SLAUGHTER_QUEEN,
    AVATAR_OF_KHAINE,
    BLOODWRACK_MEDUSA,

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
    LEVIADON,
    AKHELIAN_KING,
    VOLTURNOS,
    ALLOPEX,

    // Nighthaunt
    GRIMGHAST_REAPERS,
    CHAINRASP_HORDE,
    BLADEGHEIST_REVENANTS,
    GLAIVEWRAITH_STALKERS,
    DREADSCYTHE_HARRIDANS,
    SPIRIT_TORMENTS,
    CHAINGHASTS,
    SPIRIT_HOSTS,
    HEXWRAITHS,
    MORTARCH,
    LADY_OLYNDER,
    KURDOSS_VALENTIAN,
    REIKENOR_THE_GRIMHAILER,
    KNIGHT_OF_SHROUDS,
    GUARDIAN_OF_SOULS,
    DREADBLADE_HARROW,
    LORD_EXECUTIONER,
    TOMB_BANSHEE,
    CAIRN_WRAITH,
    BLACK_COACH,

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
    CYGOR,
    CENTIGORS,
    DOOMBULL,
    CHIMERA,
    GREAT_BRAY_SHAMAN,
    BEASTLORD,
    UNGOR,
    UNGOR_RAIDERS,
    TUSKGOR_CHARIOTS,
    COCKATRICE,
    CHAOS_SPAWN,
    CHAOS_WARHOUNDS,
    JABBERSLYTHE,
    CHAOS_GARGANT,
    RAZORGORS,
    THUNDERSCORN,
    DRAGON_OGORS,
    DRAGON_OGOR,
    SHAGGOTH,

    // Beasts of Chaos Greatfrays
    ALLHERD,
    DARKWALKERS,
    GAVESPAWN,

    // Slaanesh
    DAEMONETTES,
    DAEMONETTE,
    SEEKERS,
    FIENDS,
    KEEPER_OF_SECRETS,
    SHALAXI_HELBANE,
    SEEKER_CHARIOTS,
    SEEKER_CHARIOT,
    EXALTED_CHARIOT,
    HELLSTRIDERS,
    HERALD_OF_SLAANESH,
    INFERNAL_ENRAPTURESS,
    THE_MASQUE,
    VICELEADER,
    HEDONITE,
    SYLL_ESSKE,
    THE_VENGEFUL_ALLEGIANCE,
    THE_CONTORTED_EPITOME,
    BLADEBRINGER,
    HELLFLAYER,

    // Hosts of Slaanesh
    PRETENDERS,
    GODSEEKERS,
    INVADERS,

    // Nurgle
    PLAGUEBEARER,
    PLAGUEBEARERS,
    ROTBRINGER,
    PUTRID_BLIGHTKINGS,
    LORD_OF_PLAGUES,
    BEASTS_OF_NURGLE,
    THE_GLOTTKIN,
    PLAGUE_DRONES,
    PUSGOYLE_BLIGHTLORDS,

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
    WIGHT_KING,
    NECROMANCER,
    REANIMANT,
    MORGHAST,
    MORGHAST_ARCHAI,
    MORGHAST_HARBINGERS,

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
    ABHORRANT_ARCHREGENT,
    MENAGERIE,
    ROYAL_TERRORGHEIST,
    ROYAL_ZOMBIE_DRAGON,

    // Flesh-eater Court Grand Courts
    MORGAUNT,
    HOLLOWMOURNE,
    BLISTERSKIN,
    GRISTLEGORE,

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
    UNFORGED,
    WARDEN_KING,
    RUNELORD,

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
    RATLING_GUN,
    WARPFIRE_THROWER,
    WARP_LIGHTNING_CANNON,
    DOOMWHEEL,
    DOOM_FLAYER,

    // Tzeentch
    ARCANITE,
    KAIRIC_ACOLYTES,
    PINK_HORRORS,
    TZAANGORS,
    TZAANGOR_ENLIGHTENED,
    FLAMERS,

    // Ironjawz
    ARDBOYS,
    BRUTES,
    MAW_KRUSHA,
    MEGABOSS,
    GORDRAKK,
    GORE_GRUNTAS,
    WARCHANTER,
    IRONSKULLS_BOYZ,
    WEIRDNOB_SHAMAN,

    // Bonesplitterz
    SAVAGE_BOARBOY_MANIAKS,
    SAVAGE_ORRUK_ARROWBOYS,
    SAVAGE_BIG_STABBAS,
    SAVAGE_ORRUKS,
    SAVAGE_ORRUK_MORBOYS,
    SAVAGE_BOARBOYS,
    MANIAK_WEIRDNOB,
    WARDOKK,
    SAVAGE_BIG_BOSS,
    WURRGOG_PROPHET,
    BOARBOYS,

    // Ironjawz Warclans
    IRONSUNZ,
    BLOODTOOFS,
    CHOPPAS,

    // Bonesplitterz Warclans
    BONEGRINZ,
    DRAKKFOOT,
    ICEBONE,

    // Eldritch Council
    SWORDMASTERS,
    ARCHMAGE,
    ARCHMAGE_ON_DRAGON,
    DRAKESEER,
    LOREMASTER,

    // Greenskinz
    ROGUE_IDOL,
    WYVERN,
    ORRUK_WARBOSS,
    ORRUK_GREAT_SHAMAN,
    ORRUKS,
    ORRUK_BOARBOYS,
    ORRUK_BOAR_CHARIOTS,

    // Seraphon
    DREAD_SAURIAN,
    SAURUS,
    SAURUS_KNIGHTS,
    SAURUS_WARRIORS,
    SAURUS_GUARD,
    SKINKS,

    // Fyreslayers
    MAGMADROTH,
    VULKITE_BERZERKERS,
    HEARTHGUARD_BERZERKERS,
    AURIC_HEARTHGUARD,
    GRIMWRATH_BERZERKER,
    DOOMSEEKER,
    BATTLESMITH,
    AURIC_RUNEMASTER,
    AURIC_RUNEFATHER,
    AURIC_RUNESON,
    AURIC_RUNESMITER,

    // Fyreslayer Lodges
    VOSTARG,
    GREYFYRD,
    LOFNIR,
    HERMDAR,

    // Everchosen
    ARCHAON,

    // Slaves to Darkness
    CULTISTS,
    CYPHER_LORDS,
    FURIES,
    IRON_GOLEMS,
    UNTAMED_BEASTS,
    RAPTORYX,
    SPLINTERED_FANG,
    THE_UNMADE,
    CORVUS_CABAL,
    CHAOS_WARRIORS,
    CHAOS_CHARIOTS,
    CHAOS_CHOSEN,
    EXALTED_HERO_OF_CHAOS,
    CHAOS_KNIGHTS,
    CHAOS_LORD,
    CHAOS_SORCERER_LORD,
    CHAOS_MARAUDERS,
    CHAOS_MARAUDER_HORSEMEN,
    CHAOS_WARSHRINE,
    GODSWORN_HUNT,
    GOREBEAST_CHARIOTS,
    GOD_SPEAKER,
    THEDDRA_SKULL_CRAVER,
    DARKOATH_WARQUEEN,
    LORD_ON_DAEMONIC_MOUNT,
    SLAMBO,

    // Free Agents (no faction)
    GOTREK_GURNISSON,
};

Keyword grandAllianceStringToKeyword(const std::string& allianceName);
Keyword factionStringToKeyword(const std::string& factionName);
std::string factionKeywordToString(Keyword faction);

struct Duration
{
    Phase phase;
    int round;
    PlayerId player;
};

bool expired(const Duration& duration, const Duration& current);

struct ModifierBuff
{
    int modifier;
    Duration duration;
};

struct RerollBuff
{
    Rerolls rerolls;
    Duration duration;
};

struct MovementRuleBuff
{
    bool allowed;
    Duration duration;
};

enum class Verbosity : int
{
    Silence = 0,
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
    Enum,
};

enum
{
    SIM_FALSE = 0,
    SIM_TRUE = 1,
};

struct Parameter
{
    ParamType m_paramType;
    std::string m_name;
    int m_intValue = 0;
    int m_minValue = 0;
    int m_maxValue = 0;
    int m_increment = 1;
};

typedef std::vector<Parameter> ParameterList;

std::string ParameterValueToString(const Parameter& param);
ParameterList::const_iterator FindParam(const std::string& name, const ParameterList& parameters);
ParameterList::iterator FindParam(const std::string& name, ParameterList& parameters);
int GetIntParam(const std::string& name, const ParameterList& parameters, int defaultValue);
int GetEnumParam(const std::string& name, const ParameterList& parameters, int defaultValue);
bool GetBoolParam(const std::string& name, const ParameterList& parameters, bool defaultValue);

std::string PhaseToString(Phase phase);
std::string PlayerIdToString(PlayerId id);
PlayerId GetEnemyId(PlayerId friendlyId);

void Initialize(Verbosity verbosity = Verbosity::Normal);

void SetVerbosity(Verbosity verbosity);
Verbosity GetVerbosity();

void SimLog(Verbosity verbosity, const char* format, ...);

float AverageRandomValue(int value);

#endif //AGEOFSIGMARSIM_H