/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Zombies.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Death {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 60;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool Zombies::s_registered = false;

    Zombies::Zombies() :
            LegionOfNagashBase("Zombies", 4, WOUNDS, 10, NoSave, false),
            m_zombieBite(Weapon::Type::Melee, "Zombie Bite", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE};
        m_weapons = {&m_zombieBite};

        s_globalBraveryMod.connect(this, &Zombies::standardBearerBraveryMod, &m_standardSlot);
    }

    Zombies::~Zombies() {
        m_standardSlot.disconnect();
    }

    bool Zombies::configure(int numModels, bool standardBearer, bool noiseMaker) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearer = standardBearer;
        m_noiseMaker = noiseMaker;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_zombieBite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Zombies::Create(const ParameterList &parameters) {
        auto unit = new Zombies();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool noisemaker = GetBoolParam("Noisemaker", parameters, false);

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, standardBearers, noisemaker);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Zombies::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Standard Bearers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Noisemaker", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {DEADWALKERS}
            };
            s_registered = UnitFactory::Register("Zombies", factoryMethod);
        }
    }

    int Zombies::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toHitModifier(weapon, target);

        // Vigour Mortis
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
        for (auto ip : units) {
            if (ip->hasKeyword(CORPSE_CARTS)) {
                modifier += 1;
                break;
            }
        }

        // Dragged Down and Torn Apart
        if (remainingModels() >= 40) {
            modifier += 2;
        } else if (remainingModels() >= 20) {
            modifier += 1;
        }

        return modifier;
    }

    int Zombies::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toWoundModifier(weapon, target);

        // Dragged Down and Torn Apart
        if (remainingModels() >= 40) {
            modifier += 2;
        } else if (remainingModels() >= 20) {
            modifier += 1;
        }

        return modifier;
    }

    int Zombies::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int Zombies::standardBearerBraveryMod(const Unit *unit) {
        if (m_standardBearer && !isFriendly(unit) && (distanceTo(unit) <= 6.0f)) return -1;
        return 0;
    }

    int Zombies::rollChargeDistance() const {
        // Noise Maker
        auto dist = Unit::rollChargeDistance();
        if (m_noiseMaker) {
            return std::max(6, dist);
        }
        return dist;
    }

} //namespace Death
