/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Kroxigor.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    bool Kroxigor::s_registered = false;

    Kroxigor::Kroxigor() :
            SeraphonBase("Kroxigor", 8, WOUNDS, 7, 4, false),
            m_maul(Weapon::Type::Melee, "Drakebite Maul", 2, 4, 4, 3, -1, 2),
            m_hammer(Weapon::Type::Melee, "Moon Hammer", 2, 0, 4, 3, -1, 2),
            m_jaws(Weapon::Type::Melee, "Vice-like Jaws", 1, 1, 4, 3, -1, 1) {
        m_keywords = {ORDER, SERAPHON, KROXIGOR};
        m_weapons = {&m_maul, &m_hammer, &m_jaws};
    }

    bool Kroxigor::configure(int numModels, int numMoonhammers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }
        const int maxHammers = numModels / 3;
        if (numMoonhammers > maxHammers) {
            return false;
        }

        for (auto i = 0; i < numMoonhammers; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_hammer);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }
        for (auto i = numMoonhammers; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_maul);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Kroxigor::Create(const ParameterList &parameters) {
        auto unit = new Kroxigor();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numHammers = GetIntParam("Moon Hammers", parameters, MIN_UNIT_SIZE / 3);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, SeraphonBase::None);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels, numHammers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Kroxigor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Integer, "Moon Hammers", 1, 0, MAX_UNIT_SIZE / 3, 1},
                            {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne,
                             SeraphonBase::Coalesced, 1},
                            {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None,
                             SeraphonBase::FangsOfSotek, 1}
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Kroxigor", factoryMethod);
        }
    }


    int Kroxigor::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int Kroxigor::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SeraphonBase::toHitModifier(weapon, target);

        // Battle Synergy
        auto skinks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), SKINK, 6.0);
        if (skinks) mod++;

        return mod;
    }

    int Kroxigor::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);
        if ((weapon->name() == m_hammer.name())) {
            extra += getModelsWithin(attackingModel, target, 2.0);
        }
        return extra;
    }

    Wounds Kroxigor::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Jaws Like a Steel Trap
        if ((hitRoll == 6) && (weapon->name() == m_jaws.name())) {
            return {weapon->damage(), 1};
        }
        return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
