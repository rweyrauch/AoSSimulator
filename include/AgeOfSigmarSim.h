/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include <plog/Log.h>
#include "Dice.h"

const float ENGAGEMENT_RANGE = 0.5f;

enum class GamePhase : int {
    Deployment,
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

enum class PlayerId : int {
    None = -1,
    Red = 0,
    Blue = 1
};

enum class Rerolls {
    None = 0,
    Ones,
    Ones_And_Twos,
    Sixes,
    Failed,
    Successful
};

const int NoSave = 7;

enum class Role : int {
    Battleline = 0,
    Leader,
    Other,
    Behemoth,
    Leader_Behemoth,
    Artillery,
    Artillery_Behemoth
};

const int Automatically_Cast_No_Unbind = 42;
const int Automatically_Unbound = 21;
const int Autopass_Battleshock = 99;

enum class Attribute : int {
    Move_Distance = 0,
    Run_Distance,
    Charge_Distance,
    Pile_In_Distance,

    To_Hit_Missile,
    To_Hit_Melee,
    To_Wound_Missile,
    To_Wound_Melee,
    To_Save_Melee,
    To_Save_Missile,
    Bravery,
    Casting_Roll,
    Unbinding_Roll,
    Target_To_Hit_Melee,
    Target_To_Hit_Missile,
    Target_To_Wound_Melee,
    Target_To_Wound_Missile,
    Target_To_Save_Melee,
    Target_To_Save_Missile,

    Attacks_Melee,
    Attacks_Missile,
    Weapon_Rend_Melee,
    Weapon_Rend_Missile,
    Weapon_Damage_Melee,
    Weapon_Damage_Missile,

    Num_Attributes
};

enum class Ability : int {
    Ignore_Battleshock = 0,
    Auto_Max_Run,
    Extra_Hit_On_Value,
    Ignore_All_Wounds_On_Value,
    Extra_Mortal_Wound_On_Hit_Roll,

    Fights_First,
    Fights_Last,
    Fights_On_Death,

    Cannot_Use_Command_Abilities,

    Cast_Spells,
    Unbind_Spells,
    Can_Attack,

    Num_Abilities
};

enum class MovementRule : int {
    Can_Fly = 0,
    Can_Move,
    Can_Run,
    Can_Retreat,
    Can_Charge,
    Can_PileIn,

    Run_And_Shoot,
    Run_And_Charge,
    Retreat_And_Shoot,
    Retreat_And_Charge,

    Halve_Movement,
    Halve_Charge_Roll,
    Halve_Run_Roll,

    Double_Movement,
    Triple_Movement,

    Num_Movement_Rules
};

enum class Realm : int {
    None = 0,
    Aqshy,
    Azyr,
    Chamon,
    Ghur,
    Ghyran,
    Hysh,
    Shyish,
    Ulgu
};

const int DurationRestOfGame = 9999;

struct Duration {
    GamePhase phase;
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

struct AbilityBuff {
    int value;
    Duration duration;
};

struct Wounds {

    enum class Source {
        Weapon_Melee,
        Weapon_Missile,
        Ability,
        Spell,
        Prayer
    };

    int normal = 0;
    int mortal = 0;
    Source source = Source::Weapon_Melee;

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

    [[nodiscard]] bool zero() const { return (mortal + normal == 0); }

    friend std::ostream &operator<<(std::ostream &os, const Wounds &wounds);
};

class UnmodifiedCastingRoll {
public:
    int d1, d2;

    operator int() const { return d1 + d2; }

    [[nodiscard]] bool isDouble() const { return d1 == d2; }
};

std::string PhaseToString(GamePhase phase);

std::string PlayerIdToString(PlayerId id);

PlayerId GetEnemyId(PlayerId friendlyId);

void Initialize(plog::Severity level);

double AverageRandomValue(int value);

