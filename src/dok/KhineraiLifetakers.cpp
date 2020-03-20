/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/KhineraiLifetakers.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 40;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool KhineraiLifetakers::s_registered = false;

    KhineraiLifetakers::KhineraiLifetakers() :
            DaughterOfKhaine("Khinerai Lifetakers", 14, WOUNDS, 7, 6, true),
            m_barbedSickle(Weapon::Type::Melee, "Barbed Sickle", 1, 2, 3, 4, 0, 1),
            m_barbedSickleHarridynn(Weapon::Type::Melee, "Barbed Sickle", 1, 2, 2, 4, 0, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, KHINERAI_HARPIES, KHINERAI_LIFETAKERS};
        m_weapons = {&m_barbedSickle, &m_barbedSickleHarridynn};
    }

    bool KhineraiLifetakers::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto harridynn = new Model(BASESIZE, wounds());
        harridynn->addMeleeWeapon(&m_barbedSickleHarridynn);
        addModel(harridynn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_barbedSickle);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *KhineraiLifetakers::Create(const ParameterList &parameters) {
        auto unit = new KhineraiLifetakers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KhineraiLifetakers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Khinerai Lifetakers", factoryMethod);
        }
    }

    void KhineraiLifetakers::onBeginTurn(int battleRound) {
        DaughterOfKhaine::onBeginTurn(battleRound);

        // reset weapon damage (if previously charged)
        m_barbedSickle.setDamage(m_barbedSickle.damage());
        m_barbedSickleHarridynn.setDamage(m_barbedSickleHarridynn.damage());
    }

    void KhineraiLifetakers::onCharged() {
        DaughterOfKhaine::onCharged();

        // Death on the Wind
        m_barbedSickle.setDamage(m_barbedSickle.damage() + 1);
        m_barbedSickleHarridynn.setDamage(m_barbedSickleHarridynn.damage() + 1);
    }

    Wounds KhineraiLifetakers::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
        // Heartpiercer Shield
        if (!weapon->isMissile()) {
            // 1 mortal wound for each save of a 6
            wounds += {0, 1};
        }
        return wounds;
    }

    int KhineraiLifetakers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int KhineraiLifetakers::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);

        // Heartpiercer Shield
        if (!weapon->isMissile()) mod++;

        return mod;
    }

} // namespace DaughtersOfKhaine