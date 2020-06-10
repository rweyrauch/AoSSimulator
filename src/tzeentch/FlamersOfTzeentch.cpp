/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/FlamersOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    bool FlamersOfTzeentch::s_registered = false;

    FlamersOfTzeentch::FlamersOfTzeentch() :
            TzeentchBase("Flamers of Tzeentch", 9, WOUNDS, 10, 5, true),
            m_warpflame(Weapon::Type::Missile, "Warpflame", 18, 3, 4, 3, 0, RAND_D3),
            m_warpflamePyrocaster(Weapon::Type::Missile, "Warpflame", 18, 4, 4, 3, 0, RAND_D3),
            m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 1, 2, 5, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, FLAMER, FLAMERS_OF_TZEENTCH};
        m_weapons = {&m_warpflame, &m_warpflamePyrocaster, &m_flamingMaw};
    }

    bool FlamersOfTzeentch::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto pyrocaster = new Model(BASESIZE, wounds());
        pyrocaster->addMissileWeapon(&m_warpflamePyrocaster);
        pyrocaster->addMeleeWeapon(&m_flamingMaw);
        addModel(pyrocaster);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_warpflame);
            model->addMeleeWeapon(&m_flamingMaw);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *FlamersOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new FlamersOfTzeentch();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void FlamersOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FlamersOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    FlamersOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Flamers of Tzeentch", factoryMethod);
        }
    }

    Wounds FlamersOfTzeentch::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = TzeentchBase::computeReturnedDamage(weapon, saveRoll);

        // Touched by Fire
        Dice::RollResult mortalSaves;
        Dice::rollD6(wounds.mortal, mortalSaves);
        wounds.mortal += mortalSaves.rollsGE(5);

        return wounds;
    }

    int FlamersOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = TzeentchBase::toHitModifier(weapon, target);

        // Capricious Warpflame
        if (target->remainingModels() >= 20) mod += 2;
        else if (target->remainingModels() >= 10) mod++;

        // Guided by Billowing Flames
        if (weapon->name() == m_warpflame.name()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
            for (auto ip : units) {
                if (ip->hasKeyword(EXALTED_FLAMERS)) {
                    mod++;
                    break;
                }
            }
        }
        return mod;
    }

    int FlamersOfTzeentch::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace Tzeentch