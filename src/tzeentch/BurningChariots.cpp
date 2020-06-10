/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/BurningChariots.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch {
    static const int BASESIZE = 120;  //x92
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    bool BurningChariotsOfTzeentch::s_registered = false;

    BurningChariotsOfTzeentch::BurningChariotsOfTzeentch() :
            TzeentchBase("Burning Chariots of Tzeentch", 14, WOUNDS, 10, 5, true),
            m_warpflame(Weapon::Type::Missile, "Billowing Warpflame", 18, 6, 4, 3, -1, RAND_D3),
            m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 6, 4, 3, 0, 1),
            m_jabs(Weapon::Type::Melee, "Blue Horrors' Jabs", 1, 3, 5, 5, 0, 1),
            m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 2, 4, 5, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, FLAMER, EXALTED_FLAMERS, HORROR, TZEENTCH, BURNING_CHARIOTS};
        m_weapons = {&m_warpflame, &m_bite, &m_jabs, &m_flamingMaw};
    }

    bool BurningChariotsOfTzeentch::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_warpflame);
            model->addMeleeWeapon(&m_flamingMaw);
            model->addMeleeWeapon(&m_jabs);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BurningChariotsOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new BurningChariotsOfTzeentch();
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

    void BurningChariotsOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BurningChariotsOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    BurningChariotsOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Burning Chariots of Tzeentch", factoryMethod);
        }
    }

    Wounds BurningChariotsOfTzeentch::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = TzeentchBase::computeReturnedDamage(weapon, saveRoll);

        // Touched by Fire
        Dice::RollResult mortalSaves;
        Dice::rollD6(wounds.mortal, mortalSaves);
        wounds.mortal += mortalSaves.rollsGE(5);

        return wounds;
    }

    int BurningChariotsOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = TzeentchBase::toHitModifier(weapon, target);

        // Capricious Warpflame
        if (target->remainingModels() >= 20) mod += 2;
        else if (target->remainingModels() >= 10) mod++;

        return mod;
    }

    Wounds BurningChariotsOfTzeentch::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER)) {
            return {Dice::rollD3(), 0};
        }
        return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BurningChariotsOfTzeentch::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }


} //namespace Tzeentch