/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Hellstriders.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    bool Hellstriders::s_registered = false;

    Hellstriders::Hellstriders() :
            SlaaneshBase("Hellstriders", 14, WOUNDS, 6, 4, false),
            m_clawSpear(Weapon::Type::Melee, "Claw-spear", 1, 1, 3, 4, -1, 1),
            m_clawSpearReaver(Weapon::Type::Melee, "Claw-spear", 1, 2, 3, 4, -1, 1),
            m_hellscourge(Weapon::Type::Melee, "Hellscourge", 3, 1, 3, 4, 0, 1),
            m_hellscourgeReaver(Weapon::Type::Melee, "Hellscourge", 3, 2, 3, 4, 0, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
        m_weapons = {&m_clawSpear, &m_clawSpearReaver, &m_hellscourge, &m_hellscourgeReaver, &m_poisonedTongue};

        s_globalBattleshockReroll.connect(this, &Hellstriders::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Hellstriders::~Hellstriders() {
        m_hornblowerSlot.disconnect();
    }

    bool
    Hellstriders::configure(int numModels, WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_bannerBearer = bannerBearer;
        m_hornblower = hornblower;

        auto reaver = new Model(BASESIZE, wounds());
        if (weapons == ClawSpear)
            reaver->addMeleeWeapon(&m_clawSpearReaver);
        else if (weapons == Hellscourge)
            reaver->addMeleeWeapon(&m_hellscourgeReaver);
        reaver->addMeleeWeapon(&m_poisonedTongue);
        addModel(reaver);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == ClawSpear)
                model->addMeleeWeapon(&m_clawSpear);
            else if (weapons == Hellscourge)
                model->addMeleeWeapon(&m_hellscourge);
            model->addMeleeWeapon(&m_poisonedTongue);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Hellstriders::Create(const ParameterList &parameters) {
        auto unit = new Hellstriders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, ClawSpear);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels, weapons, iconBearer, bannerBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hellstriders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {ClawSpear, Hellscourge};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter( "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter( "Weapons", ClawSpear, weapons),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Banner Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Hellstriders", factoryMethod);
        }
    }

    std::string Hellstriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == ClawSpear) return "Claw-spear";
            else if (parameter.intValue == Hellscourge) return "Hellscourge";
        }
        return SlaaneshBase::ValueToString(parameter);
    }

    int Hellstriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Claw-spear") { return ClawSpear; }
        else if (enumString == "Hellscourge") { return Hellscourge; }
        return SlaaneshBase::EnumStringToInt(enumString);
    }

    Rerolls Hellstriders::chargeRerolls() const {
        if (m_bannerBearer) {
            return RerollFailed;
        }
        return Unit::chargeRerolls();
    }

    int Hellstriders::braveryModifier() const {
        int modifier = Unit::braveryModifier();
        if (m_iconBearer) {
            modifier += 2;
        }
        return modifier;
    }

    int Hellstriders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    Rerolls Hellstriders::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && m_hornblower && (distanceTo(unit) <= 6.0)) return RerollOnes;

        return NoRerolls;
    }

} // namespace Slaanesh
