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
#include "Dice.h"

enum Phase {
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

enum PlayerId {
    None = -1,
    Red = 0,
    Blue = 1
};

enum Rerolls {
    No_Rerolls = 0,
    Reroll_Ones,
    Reroll_Ones_And_Twos,
    Reroll_Sixes,
    Reroll_Failed,
    Reroll_Successful
};

const int NoSave = 7;

enum Role {
    Battleline,
    Leader,
    Other,
    Behemoth,
    Leader_Behemoth,
    Artillery,
    Artillery_Behemoth
};

const int Automatically_Cast_No_Unbind = 42;

enum BuffableAttribute {
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

    Num_Buffable_Attributes
};

enum BuffableAbility {
    Ignore_Battleshock = 0,
    Auto_Max_Run,
    Extra_Hit_On_Value,
    Ignore_All_Wounds_On_Value,
    Extra_Mortal_Wound_On_Hit_Roll,

    Fights_First,
    Fights_Last,

    Cannot_Use_Command_Abilities,

    Num_Buffable_Abilities
};

enum MovementRules {
    Can_Fly = 0,
    Can_Move,
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

    Num_Movement_Rules
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

struct AbilityBuff {
    int value;
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
};

std::string PhaseToString(Phase phase);

std::string PlayerIdToString(PlayerId id);

PlayerId GetEnemyId(PlayerId friendlyId);

void Initialize(Verbosity verbosity = Verbosity::Normal);

void SetVerbosity(Verbosity verbosity);

Verbosity GetVerbosity();

void SimLog(Verbosity verbosity, const char *format, ...);

double AverageRandomValue(int value);

