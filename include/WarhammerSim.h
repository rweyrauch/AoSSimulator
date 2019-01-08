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

enum RandomNumber
{
    RAND_D3 = -1,
    RAND_D6 = -2,
    RAND_2D6 = -3,
    RAND_3D6 = -4,
    RAND_4D6 = -5
};

enum class PlayerId
{
    One = 0,
    Two = 1
};

enum Rerolls
{
    NoRerolls = 0,
    RerollOnes,
    RerollFailed,
};

enum HitModifier
{
    NoExtraHits = 0,
    ExtraHitOn6,
};

enum AttackModifier
{
    ExtraAttackOnCharge,
};

enum DamageModifier
{
    PlusOneDamage,
};

enum Keyword
{
    ORDER,
    CHAOS,
    DEATH,
    DESTRUCTION,

    MORTAL,
    DAEMON,
    HUMAN,
    MONSTER,
    HERO,
    WIZARD,
    CELESTIAL,

    STORMCAST_ETERNAL,
    SACROSANCT,
    ORDINATOS,
    WAR_MACHINE,
    CELESTAR_BALLISTA,
    JUSTICAR,
    JUDICATORS,
    REDEEMER,
    LIBERATORS,
    LORD_ORDINATOR,
    SEQUITORS,

    // Stormcast Stormhosts
    HAMMERS_OF_SIGMAR,
    HALLOWED_KNIGHTS,
    CELESTIAL_VINDICATORS,
    ANVILS_OF_THE_HELDENHAMMER,
    KNIGHTS_EXCELSIOR,
    CELESTIAL_WARBRINGERS,
    TEMPEST_LORDS,
    ASTRAL_TEMPLARS,

    KHORNE,
    BLOODBOUND,
    BLOODREAVERS,

    SYLVANETH,
    ALARIELLE_THE_EVERQUEEN,

    SQUIG,
    GLOOMSPITE_GITZ,
    MOONCLAN,
    SQUIG_HERD,
    SQUIG_HOPPERS,
    BOINGROT_BOUNDERZ,

};

#endif //WARHAMMERSIM_H
