/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <string>
#include <utility>
#include "AgeOfSigmarSim.h"
#include "AoSKeywords.h"
#include "Abilities.h"

class Unit;

class Prayer {
public:

    explicit Prayer(Unit *priest, std::string name, int prayingValue, int range, int damageOn1 = 0) :
            m_priest(priest),
            m_name(std::move(name)),
            m_prayingValue(prayingValue),
            m_range(range),
            m_damageOn1(damageOn1) {}

    virtual ~Prayer() = default;

    bool pray(Unit *target, int round);

    bool pray(double x, double y, int round);

    int prayingValue() const { return m_prayingValue; }

    int range() const { return m_range; }

    const std::string &name() const { return m_name; }

    Abilities::Target allowedTargets() const { return m_allowedTargets; }

    const std::vector<Keyword> &allowedTargetKeywords() const { return m_targetKeywords; }

protected:

    virtual bool apply(int prayingRoll, Unit *target) = 0;

    virtual bool apply(int prayingRoll, double x, double y) = 0;

    Unit *m_priest = nullptr;
    std::string m_name;
    int m_prayingValue = 0;
    Duration m_duration{};
    int m_range = 0;
    int m_round = 0;
    int m_damageOn1 = 0;

    bool m_lineOfSiteRequired = true;
    Abilities::Target m_allowedTargets = Abilities::Target::None;
    Abilities::EffectType m_effect = Abilities::EffectType::Utility;
    std::vector<Keyword> m_targetKeywords;

};


class DamagePrayer : public Prayer {
public:
    DamagePrayer(Unit *priest, const std::string &name, int prayingValue, int range, int damage, int damageOn1 = 0);

protected:

    bool apply(int prayingRoll, Unit *target) override;

    bool apply(int prayingRoll, double x, double y) override { return false; }

    virtual int getDamage(Unit *target, int prayingRoll) const;

    virtual void secondaryEffect(Unit *target, int round) const {}

    int m_damage = 0;
};

class HealPrayer : public Prayer {
public:
    HealPrayer(Unit *priest, const std::string &name, int prayingValue, int range, int healing, int damageOn1 = 0);

protected:

    bool apply(int prayingValue, Unit *target) override;

    bool apply(int prayingValue, double x, double y) override { return false; }

    virtual int getHealing(int prayingRoll) const {
        return m_healing;
    }

    int m_healing = 0;
};

class BuffModifierPrayer : public Prayer {
public:
    BuffModifierPrayer(Unit *priest, const std::string &name, int prayingValue, int range,
                       Attribute which, int modifier, Abilities::Target allowedTargets, int damageOn1 = 0);

protected:

    bool apply(int prayingRoll, Unit *target) override;

    bool apply(int prayingRoll, double x, double y) override { return false; }

    virtual int getModifier(int prayingRoll) const;

    Attribute m_attribute = Attribute::To_Hit_Melee;
    int m_modifier = 0;
};

class BuffRerollPrayer : public Prayer {
public:
    BuffRerollPrayer(Unit *priest, const std::string &name, int prayingValue, int range,
                     Attribute which, Rerolls reroll, Abilities::Target allowedTargets, int damageOn1 = 0);

protected:

    bool apply(int prayingRoll, Unit *target) override;

    bool apply(int prayingRoll, double x, double y) override { return false; }

    Attribute m_attribute = Attribute::To_Hit_Melee;
    Rerolls m_reroll = Rerolls::None;
};
