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
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool thumper = GetBoolParam("Boar Thumper", parameters, true);
        bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);
        return new SavageBoarboyManiaks(warclan, numModels, thumper, totem, ComputePoints(parameters));
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

    SavageBoarboyManiaks::SavageBoarboyManiaks(Warclan warclan, int numModels, bool boarThumper, bool totemBearer, int points) :
            Bonesplitterz(warclan, "Savage Boarboy Maniaks", 12, g_wounds, 6, 6, false, points) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOY_MANIAKS};
        m_weapons = {&m_chompas, &m_tusksAndHooves, &m_chompasBoss};
        m_hasMount = true;
        m_tusksAndHooves.setMount(true);

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
    }

    int SavageBoarboyManiaks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
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
