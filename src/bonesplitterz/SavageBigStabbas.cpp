/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "bonesplitterz/SavageBigStabbas.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 8;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool SavageBigStabbas::s_registered = false;

    Unit *SavageBigStabbas::Create(const ParameterList &parameters) {
        auto unit = new SavageBigStabbas();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SavageBigStabbas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Savage Big Stabbas", factoryMethod);
        }
    }

    SavageBigStabbas::SavageBigStabbas() :
            Bonesplitterz("Savage Big Stabbas", 5, g_wounds, 6, 6, false),
            m_gorkToof(Weapon::Type::Melee, "Gorktoof", 3, 3, 3, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_BIG_STABBAS};
        m_weapons = {&m_gorkToof};
        // Savagely Enthusiastic
        m_runAndCharge = true;
    }

    void SavageBigStabbas::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        // Da Final Fling
        if (source == Wounds::Source::Weapon_Melee) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                auto roll = Dice::RollD6();
                if (unit->hasKeyword(MONSTER)) roll += 2;
                if (roll >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
        Bonesplitterz::onFriendlyModelSlain(numSlain, attacker, source);
    }

    bool SavageBigStabbas::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_gorkToof);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds SavageBigStabbas::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Bigger They Are
        if (target->hasKeyword(MONSTER)) {
            return {Dice::RollSpecial(RAND_D6), 0};
        }
        return Bonesplitterz::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SavageBigStabbas::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Bonesplitterz
