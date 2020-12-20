/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageOrruks.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 300;

    bool SavageOrruks::s_registered = false;

    Unit *SavageOrruks::Create(const ParameterList &parameters) {
        auto unit = new SavageOrruks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Chompa);
        bool thumper = GetBoolParam("Skull Thumper", parameters, true);
        bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, weapons, thumper, totem);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SavageOrruks::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Chompa, Savage_Stikka};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter<2>("Weapons", weapons[0], weapons),
                            BoolParameter("Skull Thumper"),
                            BoolParameter("Bone Totem Bearer"),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Savage Orruks", factoryMethod);
        }
    }

    SavageOrruks::SavageOrruks() :
            Bonesplitterz("Savage Orruks", 5, g_wounds, 5, 6, false),
            m_chompa(Weapon::Type::Melee, "Chompa", 1, 2, 4, 3, 0, 1),
            m_stikka(Weapon::Type::Melee, "Savage Stikka", 2, 2, 4, 4, 0, 1),
            m_chompaBoss(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1),
            m_stikkaBoss(Weapon::Type::Melee, "Savage Stikka", 2, 3, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUKS};
        m_weapons = {&m_chompa, &m_stikka, &m_chompaBoss, &m_stikkaBoss};
        m_battleFieldRole = Battleline;
    }

    bool SavageOrruks::configure(int numModels, WeaponOption weapons, bool skullThumper, bool totemBearer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        if (weapons == Chompa) {
            bossModel->addMeleeWeapon(&m_chompaBoss);
        } else if (weapons == Savage_Stikka) {
            bossModel->addMeleeWeapon(&m_stikkaBoss);
        }
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Chompa) {
                model->addMeleeWeapon(&m_chompa);
            } else if (weapons == Savage_Stikka) {
                model->addMeleeWeapon(&m_stikka);
            }
            if (totemBearer) {
                model->setName(Model::TotemBearer);
                totemBearer = false;
            }
            else if (skullThumper) {
                model->setName("Skull Thumper");
                skullThumper = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string SavageOrruks::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Chompa) { return "Chompa"; }
            else if (parameter.intValue == Savage_Stikka) { return "Savage Stikka"; }
        }
        return Bonesplitterz::ValueToString(parameter);
    }

    int SavageOrruks::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chompa") { return Chompa; }
        else if (enumString == "Savage Stikka") { return Savage_Stikka; }
        return Bonesplitterz::EnumStringToInt(enumString);
    }

    int SavageOrruks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Spirit of Gorkamorka
        if (remainingModels() >= 15) {
            extra++;
        }
        return extra;
    }

    int SavageOrruks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Bonesplitterz
