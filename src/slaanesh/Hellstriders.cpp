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
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Hellstriders::s_registered = false;

    Hellstriders::Hellstriders() :
            SlaaneshBase("Hellstriders", 14, g_wounds, 6, 4, false),
            m_clawSpear(Weapon::Type::Melee, "Claw-spear", 1, 1, 3, 4, -1, 1),
            m_clawSpearReaver(Weapon::Type::Melee, "Claw-spear", 1, 2, 3, 4, -1, 1),
            m_hellscourge(Weapon::Type::Melee, "Hellscourge", 3, 1, 3, 4, 0, 1),
            m_hellscourgeReaver(Weapon::Type::Melee, "Hellscourge", 3, 2, 3, 4, 0, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
        m_weapons = {&m_clawSpear, &m_clawSpearReaver, &m_hellscourge, &m_hellscourgeReaver, &m_poisonedTongue};
        m_hasMount = true;

        s_globalBattleshockReroll.connect(this, &Hellstriders::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Hellstriders::~Hellstriders() {
        m_hornblowerSlot.disconnect();
    }

    bool
    Hellstriders::configure(int numModels, WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_bannerBearer = bannerBearer;
        m_hornblower = hornblower;

        auto reaver = new Model(g_basesize, wounds());
        if (weapons == Claw_Spear)
            reaver->addMeleeWeapon(&m_clawSpearReaver);
        else if (weapons == Hellscourge)
            reaver->addMeleeWeapon(&m_hellscourgeReaver);
        reaver->addMeleeWeapon(&m_poisonedTongue);
        addModel(reaver);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Claw_Spear)
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
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Claw_Spear);
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
            static const std::array<int, 2> weapons = {Claw_Spear, Hellscourge};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter( "Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Claw_Spear, weapons),
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
            if (parameter.intValue == Claw_Spear) return "Claw-spear";
            else if (parameter.intValue == Hellscourge) return "Hellscourge";
        }
        return SlaaneshBase::ValueToString(parameter);
    }

    int Hellstriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Claw-spear") { return Claw_Spear; }
        else if (enumString == "Hellscourge") { return Hellscourge; }
        return SlaaneshBase::EnumStringToInt(enumString);
    }

    Rerolls Hellstriders::chargeRerolls() const {
        if (m_bannerBearer) {
            return Reroll_Failed;
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Hellstriders::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && m_hornblower && (distanceTo(unit) <= 6.0)) return Reroll_Ones;

        return No_Rerolls;
    }

} // namespace Slaanesh
