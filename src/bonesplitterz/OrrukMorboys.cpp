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
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool SavageOrrukMorboys::s_registered = false;

    Unit *SavageOrrukMorboys::Create(const ParameterList &parameters) {
        auto unit = new SavageOrrukMorboys(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool thumper = GetBoolParam("Skull Thumper", parameters, true);
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

    void SavageOrrukMorboys::Init() {
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
            s_registered = UnitFactory::Register("Savage Orruk Morboys", factoryMethod);
        }
    }

    SavageOrrukMorboys::SavageOrrukMorboys(int points) :
            Bonesplitterz("Savage Orruk Morboys", 5, g_wounds, 6, 6, false, points),
            m_chompaAndShiv(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 3, 4, 3, 0, 1),
            m_chompaAndShivBoss(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 4, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_MORBOYS};
        m_weapons = {&m_chompaAndShiv, &m_chompaAndShivBoss};
    }

    bool SavageOrrukMorboys::configure(int numModels, bool skullThumper, bool totemBearer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_chompaAndShivBoss);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_chompaAndShiv);
            if (totemBearer) {
                model->setName(Model::TotemBearer);
                totemBearer = false;
            } else if (skullThumper) {
                model->setName("Skull Thumper");
                skullThumper = false;
            }
            addModel(model);
        }

        return true;
    }

    int SavageOrrukMorboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Bonesplitterz::extraAttacks(attackingModel, weapon, target);
        // Spirit of Gorkamorka
        if (remainingModels() >= 15) {
            extra++;
        }
        return extra;
    }

    int SavageOrrukMorboys::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int SavageOrrukMorboys::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Bonesplitterz::toHitModifier(weapon, target);

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
