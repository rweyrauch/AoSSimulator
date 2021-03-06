/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/OrrukArrowBoys.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool SavageOrrukArrowboys::s_registered = false;

    Unit *SavageOrrukArrowboys::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool thumper = GetBoolParam("Skull Thumper", parameters, true);
        bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

        return new SavageOrrukArrowboys(warclan, numModels, thumper, totem, ComputePoints(parameters));
    }

    void SavageOrrukArrowboys::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Skull Thumper"),
                            BoolParameter("Bone Totem Bearer"),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };
            s_registered = UnitFactory::Register("Savage Orruk Arrowboys", factoryMethod);
        }
    }

    SavageOrrukArrowboys::SavageOrrukArrowboys(Warclan warclan, int numModels, bool skullThumper, bool totemBearer, int points) :
            Bonesplitterz(warclan, "Savage Orruk Arrowboys", 5, g_wounds, 5, 6, false, points),
            m_stingaBow(Weapon::Type::Missile, "Stinga Bow", 18, 2, 5, 4, 0, 1),
            m_boneShiv(Weapon::Type::Melee, "Bone Shiv", 1, 1, 4, 4, 0, 1),
            m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_ARROWBOYS};
        m_weapons = {&m_stingaBow, &m_boneShiv, &m_chompa};

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_stingaBow);
        bossModel->addMeleeWeapon(&m_chompa);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_stingaBow);
            model->addMeleeWeapon(&m_boneShiv);
            if (totemBearer) {
                model->setName(Model::TotemBearer);
                totemBearer = false;
            } else if (skullThumper) {
                model->setName("Skull Thumper");
                skullThumper = false;
            }
            addModel(model);
        }
    }

    int SavageOrrukArrowboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Bonesplitterz::extraAttacks(attackingModel, weapon, target);
        // Loadsa Arrows
        if (weapon->name() == m_stingaBow.name() && remainingModels() >= 15) {
            extra++;
        }
        return extra;
    }

    int SavageOrrukArrowboys::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Aim Fer Its Eyes
        if (weapon->name() == m_stingaBow.name() && target->hasKeyword(MONSTER)) {
            return -1;
        }
        return Bonesplitterz::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SavageOrrukArrowboys::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Bonesplitterz
