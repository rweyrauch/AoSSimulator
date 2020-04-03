/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "bonesplitterz/OrrukMorboys.h"

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool SavageOrrukMorboys::s_registered = false;

    Unit *SavageOrrukMorboys::Create(const ParameterList &parameters) {
        auto unit = new SavageOrrukMorboys();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool thumper = GetBoolParam("Skull Thumper", parameters, true);
        bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, thumper, totem);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SavageOrrukMorboys::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz,
                             Bonesplitterz::Icebone, 1},
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };
            s_registered = UnitFactory::Register("Savage Orruk Morboys", factoryMethod);
        }
    }

    SavageOrrukMorboys::SavageOrrukMorboys() :
            Bonesplitterz("Savage Orruk Morboys", 5, WOUNDS, 6, 6, false),
            m_chompaAndShiv(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 3, 4, 3, 0, 1),
            m_chompaAndShivBoss(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 4, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_MORBOYS};
        m_weapons = {&m_chompaAndShiv, &m_chompaAndShivBoss};
    }

    bool SavageOrrukMorboys::configure(int numModels, bool skullThumper, bool totemBearer) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_thumper = skullThumper;
        m_totemBearer = totemBearer;

        // Add the Boss
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_chompaAndShivBoss);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_chompaAndShiv);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int SavageOrrukMorboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Spirit of Gorkamorka
        if (remainingModels() >= 15) {
            extra++;
        }
        return extra;
    }

    int SavageOrrukMorboys::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int SavageOrrukMorboys::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Power of the Beast Spirit
        if (!weapon->isMissile()) {
            // Search for slain enemy monsters
            auto units = Board::Instance()->getAllUnits(GetEnemyId(owningPlayer()));
            for (auto unit : units) {
                if (unit->hasKeyword(MONSTER) && (unit->remainingModels() == 0)) {
                    mod++;
                    break;
                }
            }
        }
        return mod;
    }

} // namespace Bonesplitterz
