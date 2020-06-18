/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/BoarboyManiaks.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool SavageBoarboyManiaks::s_registered = false;

    Unit *SavageBoarboyManiaks::Create(const ParameterList &parameters) {
        auto unit = new SavageBoarboyManiaks();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool thumper = GetBoolParam("Boar Thumper", parameters, true);
        bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, thumper, totem);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SavageBoarboyManiaks::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Boar Thumper"),
                            BoolParameter("Bone Totem Bearer"),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Savage Boarboy Maniaks", factoryMethod);
        }
    }

    SavageBoarboyManiaks::SavageBoarboyManiaks() :
            Bonesplitterz("Savage Boarboy Maniaks", 12, WOUNDS, 6, 6, false),
            m_chompas(Weapon::Type::Melee, "Pair of Chompas", 1, 4, 4, 3, 0, 1),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
            m_chompasBoss(Weapon::Type::Melee, "Pair of Chompas", 1, 5, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOY_MANIAKS};
        m_weapons = {&m_chompas, &m_tusksAndHooves, &m_chompasBoss};
    }

    bool SavageBoarboyManiaks::configure(int numModels, bool boarThumper, bool totemBearer) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_thumper = boarThumper;
        m_totemBearer = totemBearer;

        // Add the Boss
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_chompasBoss);
        bossModel->addMeleeWeapon(&m_tusksAndHooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_chompas);
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int
    SavageBoarboyManiaks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Maniak Fury
        if (weapon->name() == m_chompas.name() && remainingModels() >= 5) {
            extra++;
        }
        return extra;
    }

    int SavageBoarboyManiaks::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Unit::toHitModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    int SavageBoarboyManiaks::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Unit::toWoundModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    int SavageBoarboyManiaks::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Bonesplitterz
