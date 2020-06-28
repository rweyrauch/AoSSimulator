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

enum Phase {
    Initiative,
    Hero,
    Movement,
    Shooting,
    Charge,
    Combat,
    Battleshock
};

enum RandomNumber {
    RAND_D3 = -1,
    RAND_D6 = -2,
    RAND_2D6 = -3,
    RAND_3D6 = -4,
    RAND_4D6 = -5,
    RAND_2D3 = -6,
};

enum PlayerId {
    None = -1,
    Red = 0,
    Blue = 1
};

enum Rerolls {
    NoRerolls = 0,
    RerollOnes,
    RerollOnesAndTwos,
    RerollSixes,
    RerollFailed,
    RerollSuccessful
};

const int NoSave = 7;

enum Role {
    Battleline,
    Leader,
    Other,
    Behemoth,
    LeaderBehemoth,
};

enum BuffableAttribute {
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
    AttacksMelee,
    AttacksMissile,

    NUM_BUFFABLE_ATTRIBUTES
};

enum MovementRules {
    Fly = 0,
    RunAndShoot,
    RunAndCharge,
    RetreatAndShoot,
    RetreatAndCharge,

    NUM_MOVEMENT_RULES
};

enum Realm {
    Aqshy,
    Azyr,
    Chamon,
    Ghur,
    Ghyran,
    Hysh,
    Shyish,
    Ulgu
};

enum Keyword {
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
    CITIES_OF_SIGMAR,
    COLLEGIATE_ARCANE,
    IRONWELD_ARSENAL,
    OGOR_MAWTRIBES,
    GUTBUSTERS,
    OSSIARCH_BONEREAPERS,
    LUMINETH_REALM_LORDS,

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
    SORCERER,

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
    AETHERWINGS,
    STEELHEARTS_CHAMPIONS,
    THE_FARSTRIDERS,
    AVERON_STORMSIRE,
    STORMSIRES_CURSEBREAKERS,

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
    SKARR_BLOODWRATH,
    RIPTOOTH,
    MAGORES_FIENDS,
    GARREKS_REAVERS,

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
    OUTCASTS,

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
    RIPPAS_SNARLFANGS,
    ZARBAG,
    ZARBAGS_GITZ,
    FANATIC,
    LOONSMASHA,
    SPORESPLATTA,
    GOBBAPALOOZA,
    SCAREMONGER,
    BREWGIT,
    SPIKER,
    BOGGLEYE,
    SHROOMANCER,
    MOLLOG,
    TROGGOTH_HAG,

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
    BLOODWRACK_SHRINE,
    CAULDRON_OF_BLOOD,
    MORATHI,
    HIGH_ORACLE_OF_KHAINE,
    THE_SHADOW_QUEEN,

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
    ISHARANN,
    AKHELIAN_GUARD,
    MORRSARR_GUARD,
    ISHLAEN_GUARD,
    LEVIADON,
    AKHELIAN_KING,
    VOLTURNOS,
    ALLOPEX,
    EIDOLON,
    ASPECT_OF_THE_STORM,
    ASPECT_OF_THE_SEA,
    TIDECASTER,
    SOULRENDER,
    SOULSCRYER,
    SOUL_WARDEN,
    LOTANN,

    // Idoneth Deepkin Enclave
    IONRACH,
    DHOMHAIN,
    FUETHAN,
    MORPHANN,
    NAUTILAR,
    BRIOMDAR,

    // Nighthaunt
    GRIMGHAST_REAPERS,
    CHAINRASP_HORDE,
    BLADEGHEIST_REVENANTS,
    GLAIVEWRAITH_STALKERS,
    DREADSCYTHE_HARRIDANS,
    SPIRIT_TORMENT,
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
    MYRMOURN_BANSHEES,
    MOURNGUL,

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
    GREAT_UNCLEAN_ONE,
    ROTIGUS,
    POXBRINGER,
    HERALD_OF_NURGLE,
    EPIDEMIUS,
    TALLYMAN_OF_NURGLE,
    SPOILPOX_SCRIVENER,
    SLOPPITY_BILEPIPER,
    HORTICULOUS_SLIMUX,
    NURGLINGS,
    ORGHOTTS_DAEMONSPEW,
    BLOAB_ROTSPAWNED,
    MORBIDEX_TWICEBORN,
    LORD_OF_AFFLICTIONS,
    FESTUS_THE_LEECHLORD,
    HARBINGER_OF_DECAY,
    LORD_OF_BLIGHTS,
    GUTROT_SPUME,
    FECULA_FLYBLOWN,
    THE_WURMSPAT,
    BLESSED_SONS,

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
    MORGHAST,
    MORGHAST_ARCHAI,
    MORGHAST_HARBINGERS,
    ARKHAN,
    NAGASH,
    BAT_SWARMS,
    FELL_BATS,
    MANNFRED,
    NEFERATA,
    PRINCE_VHORDRAI,
    VAMPIRE_LORD,
    BLOODSEEKER_PALANQUIN,
    COVEN_THRONE,
    MORTIS_ENGINE,
    CORPSE_CART,

    // Flesh-eater Court
    MORDANT,
    COURTIER,
    ABHORRANT,
    CRYPT_GHAST_COURTIER,
    CRYPT_GHOULS,
    ABHORRANT_GHOUL_KING,
    ZOMBIE_DRAGON,
    CRYPT_FLAYERS,
    CRYPT_HAUNTER_COURTIER,
    CRYPT_HORRORS,
    CRYPT_INFERNAL_COURTIER,
    VARGHULF_COURTIER,
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
    MASTERCLAN,
    STORMVERMIN,
    NIGHT_RUNNERS,
    GUTTER_RUNNERS,
    CLANRATS,
    STORMFIENDS,
    PLAGUE_MONKS,
    HELL_PIT_ABOMINATION,
    FIGHTING_BEAST,
    PACK,
    RATLING_GUN,
    WARPFIRE_THROWER,
    WARP_LIGHTNING_CANNON,
    DOOMWHEEL,
    DOOM_FLAYER,
    WARLOCK_ENGINEER,
    WARLOCK_BOMBARDIER,
    ARCH_WARLOCK,
    GREY_SEER,
    SCREAMING_BELL,
    VERMINLORD,
    VERMINLORD_WARPSEER,
    VERMINLORD_WARBRINGER,
    VERMINLORD_CORRUPTOR,
    VERMINLORD_DECEIVER,
    LORD_SKREECH_VERMINKING,
    THANQUOL,
    SKRYRE_ACOLYTES,
    WARPLOCK_JEZZAILS,
    WARP_GRINDER,
    PLAGUE_PRIEST,
    PLAGUECLAW,
    PLAGUE_FURNACE,
    PLAGUE_CENSER_BEARERS,
    CLAWLORD,
    PACKMASTERS,
    MASTER_MOULDER,
    RAT_OGORS,
    RAT_SWARMS,
    GIANT_RATS,
    DEATHMASTER,
    SKRITCH_SPITECLAW,
    SPITECLAWS_SWARM,

    // Tzeentch
    ARCANITE,
    KAIRIC_ACOLYTES,
    TZAANGORS,
    TZAANGOR_ENLIGHTENED,
    FLAMERS_OF_TZEENTCH,
    HORROR,
    HORROR_OF_TZEENTCH,
    FLAMER,
    EXALTED_FLAMERS,
    TZAANGOR_SKYFIRES,
    SCREAMERS,
    LORD_OF_CHANGE,
    KAIROS_FATEWEAVER,
    MAGISTER,
    BURNING_CHARIOTS,
    GAUNT_SUMMONER,
    THE_CHANGELING,
    THE_BLUE_SCRIBES,
    TZAANGOR_SHAMAN,
    CURSELING,
    FATESKIMMER,
    VORTEMIS_THE_ALL_SEEING,
    OGROID_THAUMATURGE,
    FATEMASTER,
    CHANGECASTER,
    FLUXMASTER,
    EYES_OF_THE_NINE,

    // Tzeentch Change Covens
    ETERNAL_CONFLAGRATION,
    HOSTS_DUPLICITOUS,
    HOSTS_ARCANUM,
    CULT_OF_THE_TRANSIENT_FORM,
    PYROFANE_CULT,
    GUILD_OF_SUMMONERS,

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
    COLD_ONE,
    OLDBLOOD,
    SAURUS_WARRIORS,
    SAURUS_GUARD,
    SKINKS,
    SKINK,
    SUNBLOOD,
    CARNOSAUR,
    ETERNITY_WARDEN,
    SCAR_VETERAN,
    ASTROLITH_BEARER,
    STARSEER,
    STARPRIEST,
    TROGLODON,
    CHAMELEON_SKINKS,
    SALAMANDER,
    RAZORDON,
    HUNTING_PACK,
    KROXIGOR,
    BASTILADON,
    TERRADON_RIDERS,
    TERRADON_CHIEF,
    TERRADON,
    RIPPERDACTYL_RIDERS,
    RIPPERDACTYL_CHIEF,
    RIPPERDACTYL,
    STEGADON,
    ENGINE_OF_THE_GODS,
    STARMASTER,
    SLANN,
    LORD_KROAK,
    ORACLE,

    // Ways of the Seraphon
    STARBORNE,
    COALESCED,

    // Seraphon Constellations
    KOATLS_CLAW,
    THUNDER_LIZARD,
    DRACOTHIONS_TAIL,
    FANGS_OF_SOTEK,

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
    UNDIVIDED,

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
    THEDDRA_SKULL_SCRYER,
    DARKOATH_WARQUEEN,
    DARKOATH_CHIEFTAIN,
    SLAMBO,
    MARK_OF_CHAOS,
    EYE_OF_THE_GODS,
    MANTICORE,
    VARANGUARD,
    MINDSTEALER_SPHIRANX,
    FOMOROID_CRUSHER,
    SPIRE_TYRANTS,
    OGROID_MYRMIDON,
    DAEMON_PRINCE,
    BELAKOR,
    SLAUGHTERBRUTE,
    MUTALITH_VORTEX_BEAST,
    SOUL_GRINDER,

    // Slaves to Darkness Damned Legion
    RAVAGERS,
    CABALISTS,
    DESPOILERS,
    HOST_OF_THE_EVERCHOSEN,

    // Free Agents (no faction)
    GOTREK_GURNISSON,

    // Cities of Sigmar
    HAMMERHAL,
    LIVING_CITY,
    GREYWATER_FASTNESS,
    PHOENICIUM,
    ANVILGARD,
    HALLOWHEART,
    TEMPESTS_EYE,

    ASSASSIN,
    ANOINTED,
    BATTLEMAGE,
    BLACK_ARK_CORSAIRS,
    BLACK_ARK_FLEETMASTER,
    FREEGUILD_GENERAL,
    FREEGUILD,
    DEMIGRYPH_KNIGHTS,
    FREEGUILD_GREATSWORDS,
    FREEGUILD_GUARD,
    FREEGUILD_OUTRIDERS,
    FREEGUILD_PISTOLIERS,
    FREEGUILD_HANDGUNNERS,
    FREEGUILD_CROSSBOWMEN,
    FLAGELLANTS,
    LUMINARK_OF_HYSH,
    CELESTIAL_HURRICANUM,
    BLACK_GUARD,
    EXECUTIONERS,
    BLEAKSWORDS,
    DREADSPEARS,
    DARKSHARDS,
    COGSMITH,
    GYROCOPTERS,
    GYROBOMBERS,
    STEAM_TANK,
    HELBLASTER_VOLLEY_GUN,
    HELSTORM_ROCKET_BATTERY,
    PHOENIX_GUARD,
    FROSTHEART_PHOENIX,
    FLAMESPYRE_PHOENIX,
    DREADLORD,
    WAR_HYDRA,
    DRAKESPAWN_CHARIOTS,
    DRAKESPAWN_KNIGHTS,
    SHADOW_WARRIORS,
    DARK_RIDERS,
    KHARIBDYSS,
    SCOURGERUNNER_CHARIOTS,
    NOMAD_PRINCE,
    WILD_RIDERS,
    SORCERESS,

    // Ogor Mawtribes
    OGOR,
    TYRANT,
    OGOR_GLUTTONS,
    LEADBELCHERS,
    IRONBLASTER,
    RHINOX,
    STONEHORN,
    FROSTLORD,
    THUNDERTUSK,
    HUSKARD,
    STONEHORN_BEASTRIDERS,
    THUNDERTUSK_BEASTRIDERS,
    ICEBROW_HUNTER,
    FROST_SABRES,
    ICEFALL_YHETESS,
    MOURNFANG_PACK,
    BUTCHER,
    IRONGUTS,
    GORGERS,
    GNOBLAR_SCRAPLAUNCHER,
    GNOBLARS,
    SLAUGHTERMASTER,
    FIREBELLY,
    MANEATERS,
    HROTHGORN,
    HROTHGORNS_MANTRAPPERS,

    // Mawtribes
    MEATFIST,
    BLOODGULLET,
    UNDERGUTS,
    BOULDERHEAD,
    THUNDERBELLIES,
    WINTERBITE,

    // Ossiarch Bonereapers
    HEKATOS,
    LIEGE,
    VOKMORTIAN,
    MORTEK_GUARD,
    NECROPOLIS_STALKERS,
    KAVALOS_DEATHRIDERS,
    IMMORTIS_GUARD,
    KATAKROS,
    ARCH_KAVALOS_ZANDTOS,
    LIEGE_KAVALOS,
    MORTISAN,
    MORTISAN_SOULREAPER,
    MORTISAN_SOULMASON,
    MORTISAN_BONESHAPER,
    MORTEK_CRAWLER,
    GOTHIZZAR_HARVESTER,

    // Ossiarch BoneReaper Legions
    MORTIS_PRAETORIANS,
    PETRIFEX_ELITE,
    STALLIARCH_LORDS,
    IVORY_HOST,
    NULL_MYRIAD,
    CREMATORIANS,

    // Kharadron Overlords
    SKYFARERS,
    SKYFARER,
    SKYWARDENS,
    MARINE,
    GRUNDSTOK_THUNDERERS,
    SKYVESSEL,
    GRUNDSTOK_GUNHAULER,
    ENDRINRIGGERS,
    ENDRINMASTER,
    BROKK_GRUNGSSON,
    AETHERIC_NAVIGATOR,
    AETHER_KHEMIST,
    ARKANAUT_ADMIRAL,
    ARKANAUT_COMPANY,
    ARKANAUT_FRIGATE,
    ARKANAUT_IRONCLAD,
    BJORGEN_THUNDRIK,
    THUNDRIKS_PROFITEERS,

    // KO Skyports
    BARAK_NAR,
    BARAK_ZILFIN,
    BARAK_ZON,
    BARAK_URBAZ,
    BARAK_MHORNAR,
    BARAK_THRYNG,

    // Lumineth Realm-Lords
    VANARI,
    AELEMENTIRI,
    ALARITH,
    SCINARI,
    AURALAN_WARDENS,
    AURALAN_SENTINELS,
    DAWNRIDERS,
    LIGHT_OF_ELTHARION,
    STONEGUARD,
    STONEMAGE,
    CATHALLAR,
    SPIRIT_OF_THE_MOUNTAIN,
};

Keyword GrandAllianceStringToKeyword(const std::string &allianceName);

Keyword FactionStringToKeyword(const std::string &factionName);

const std::string &GrandAllianceKeywordToString(Keyword ga);

const std::string &FactionKeywordToString(Keyword faction);

struct Duration {
    Phase phase;
    int round;
    PlayerId player;
};

bool Expired(const Duration &duration, const Duration &current);

struct ModifierBuff {
    int modifier;
    Duration duration;
};

struct RerollBuff {
    Rerolls rerolls;
    Duration duration;
};

struct MovementRuleBuff {
    bool allowed;
    Duration duration;
};

enum Verbosity : int {
    Silence = 0,
    Normal,
    Narrative,
    Debug
};

struct Wounds {

    enum class Source {
        WeaponMelee,
        WeaponMissile,
        Ability,
        Spell,
        Prayer
    };

    int normal = 0;
    int mortal = 0;
    Source source = Source::WeaponMelee;

    Wounds &operator+=(const Wounds &w) {
        normal += w.normal;
        mortal += w.mortal;
        return *this;
    }

    Wounds &operator-=(const Wounds &w) {
        normal -= w.normal;
        mortal -= w.mortal;
        return *this;
    }

    Wounds &clamp() {
        normal = std::max(0, normal);
        mortal = std::max(0, mortal);
        return *this;
    }
};

std::string PhaseToString(Phase phase);

std::string PlayerIdToString(PlayerId id);

PlayerId GetEnemyId(PlayerId friendlyId);

void Initialize(Verbosity verbosity = Verbosity::Normal);

void SetVerbosity(Verbosity verbosity);

Verbosity GetVerbosity();

void SimLog(Verbosity verbosity, const char *format, ...);


double AverageRandomValue(int value);


#endif //AGEOFSIGMARSIM_H
