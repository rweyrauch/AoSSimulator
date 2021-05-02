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
    static const int g_basesize = 32;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool SavageBoarboyManiaks::s_registered = false;

    Unit *SavageBoarboyManiaks::Create(const ParameterList &parameters) {
        auto unit = new SavageBoarboyManiaks(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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

    SavageBoarboyManiaks::SavageBoarboyManiaks(int points) :
            Bonesplitterz("Savage Boarboy Maniaks", 12, g_wounds, 6, 6, false, points),
            m_chompas(Weapon::Type::Melee, "Pair of Chompas", 1, 4, 4, 3, 0, 1),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
            m_chompasBoss(Weapon::Type::Melee, "Pair of Chompas", 1, 5, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOY_MANIAKS};
        m_weapons = {&m_chompas, &m_tusksAndHooves, &m_chompasBoss};
        m_hasMount = true;
        m_tusksAndHooves.setMount(true);
    }

    bool SavageBoarboyManiaks::configure(int numModels, bool boarThumper, bool totemBearer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_chompasBoss);
        bossModel->addMeleeWeapon(&m_tusksAndHooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_chompas);
            model->addMeleeWeapon(&m_tusksAndHooves);
            if (totemBearer) {
                model->setName(Model::TotemBearer);
                totemBearer = false;
            } else if (boarThumper) {
                model->setName("Boar Thumper");
                boarThumper = false;
            }

            addModel(model);
        }

        return true;
    }

    int
    SavageBoarboyManiaks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Bonesplitterz::extraAttacks(attackingModel, weapon, target);
        // Maniak Fury
        if (weapon->name() == m_chompas.name() && remainingModels() >= 5) {
            extra++;
        }
        return extra;
    }

    int SavageBoarboyManiaks::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Bonesplitterz::toHitModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    int SavageBoarboyManiaks::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Bonesplitterz::toWoundModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    int SavageBoarboyManiaks::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Bonesplitterz
