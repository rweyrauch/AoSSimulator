/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/DragonOgors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool DragonOgors::s_registered = false;

    DragonOgors::DragonOgors() :
            BeastsOfChaosBase("Dragon Ogors", 8, WOUNDS, 6, 4, false),
            m_pairedAncientWeapons(Weapon::Type::Melee, "Paired Ancient Weapons", 1, 6, 3, 3, 0, 1),
            m_draconicWarglaive(Weapon::Type::Melee, "Draconic War-glaive", 2, 4, 3, 3, -1, 1),
            m_draconicCrusher(Weapon::Type::Melee, "Draconic Crusher", 1, 3, 3, 3, 0, 2),
            m_rakingForeclaws(Weapon::Type::Melee, "Raking Foreclaws", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, THUNDERSCORN, DRAGON_OGORS};
        m_weapons = {&m_pairedAncientWeapons, &m_draconicWarglaive, &m_draconicCrusher, &m_rakingForeclaws};
    }

    bool DragonOgors::configure(int numModels, int numPairedWeapons, int numGlaives, int numCrushers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }
        if (numCrushers + numPairedWeapons + numGlaives != numModels) {
            return false;
        }

        for (auto i = 0; i < numPairedWeapons; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_pairedAncientWeapons);
            addModel(model);
        }
        for (auto i = 0; i < numGlaives; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_draconicWarglaive);
            addModel(model);
        }
        for (auto i = 0; i < numCrushers; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_draconicCrusher);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *DragonOgors::Create(const ParameterList &parameters) {
        auto unit = new DragonOgors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numPairedWeapons = GetIntParam("Paired Ancient Weapons", parameters, numModels);
        int numGlaives = GetIntParam("Draconic War-glaive", parameters, 0);
        int numCrushers = GetIntParam("Draconic Crusher", parameters, 0);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, numPairedWeapons, numGlaives, numCrushers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DragonOgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Integer, "Paired Ancient Weapons", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE,
                             MIN_UNIT_SIZE},
                            {ParamType::Integer, "Draconic War-glaive", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE,
                             MIN_UNIT_SIZE},
                            {ParamType::Integer, "Draconic Crusher", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE,
                             MIN_UNIT_SIZE},
                            {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None,
                             BeastsOfChaosBase::Gavespawn, 1},
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Dragon Ogors", factoryMethod);
        }
    }

    Rerolls DragonOgors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Storm Rage
        auto shaggoth = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), DRAGON_OGOR, 12.0f);
        if (shaggoth) {
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int DragonOgors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos