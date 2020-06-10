/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/SistersOfSlaughter.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool SistersOfSlaughter::s_registered = false;

    SistersOfSlaughter::SistersOfSlaughter() :
            DaughterOfKhaine("Sisters of Slaughter", 6, WOUNDS, 7, 6, false),
            m_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1),
            m_sacrificialKnifeHandmaiden(Weapon::Type::Melee, "Sacrificial Knife (Handmaiden)", 1, 2, 2, 4, 0, 1),
            m_barbedWhip(Weapon::Type::Melee, "Barbed Whip", 2, 2, 3, 4, 0, 1),
            m_barbedWhipHandmaiden(Weapon::Type::Melee, "Barbed Whip (Handmaiden)", 2, 2, 2, 4, 0, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, SISTERS_OF_SLAUGHTER};
        m_weapons = {&m_sacrificialKnife, &m_sacrificialKnifeHandmaiden, &m_barbedWhip, &m_barbedWhipHandmaiden};

        // Dance of Death
        m_pileInMove = 6;
    }

    bool SistersOfSlaughter::configure(int numModels, bool sacrificialKnife, bool hornblowers, bool standardBearers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_hasSacrificialKnife = sacrificialKnife;
        m_hornblowers = hornblowers;
        m_standardBearers = standardBearers;

        if (m_hornblowers) {
            m_runAndCharge = true;
        }

        auto handmaiden = new Model(BASESIZE, wounds());
        if (m_hasSacrificialKnife) {
            handmaiden->addMeleeWeapon(&m_sacrificialKnifeHandmaiden);
        }
        handmaiden->addMeleeWeapon(&m_barbedWhipHandmaiden);
        addModel(handmaiden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_hasSacrificialKnife) {
                model->addMeleeWeapon(&m_sacrificialKnife);
            }
            model->addMeleeWeapon(&m_barbedWhip);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SistersOfSlaughter::Create(const ParameterList &parameters) {
        auto unit = new SistersOfSlaughter();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool sacrificialKnife = GetBoolParam("Sacrificial Knife", parameters, true);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels, sacrificialKnife, hornblowers, standardBearers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SistersOfSlaughter::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Sacrificial Knife"),
                            BoolParameter("Hornblowers"),
                            BoolParameter("Standard Bearers"),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Sisters of Slaughter", factoryMethod);
        }
    }

    int SistersOfSlaughter::rollBattleshock() const {
        if (m_standardBearers) {
            int r1 = Dice::rollD6();
            int r2 = Dice::rollD6();
            return std::min(r1, r2);
        }
        return DaughterOfKhaine::rollBattleshock();
    }

    int SistersOfSlaughter::toSaveModifier(const Weapon *weapon) const {
        int modifier = DaughterOfKhaine::toSaveModifier(weapon);
        // Bladed Bucklers
        if (!m_hasSacrificialKnife) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds SistersOfSlaughter::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
        // Bladed Bucklers
        if (!weapon->isMissile()) {
            // 1 mortal wound for each save of a 6
            wounds += {0, 1};
        }
        return wounds;
    }

    int SistersOfSlaughter::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace DaughtersOfKhaine