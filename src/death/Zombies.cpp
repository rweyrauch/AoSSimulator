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
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 60;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 320;

    bool Zombies::s_registered = false;

    Zombies::Zombies() :
            LegionOfNagashBase("Zombies", 4, g_wounds, 10, NoSave, false),
            m_zombieBite(Weapon::Type::Melee, "Zombie Bite", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE};
        m_weapons = {&m_zombieBite};
        m_battleFieldRole = Battleline;

        s_globalBraveryMod.connect(this, &Zombies::standardBearerBraveryMod, &m_standardSlot);
    }

    Zombies::~Zombies() {
        m_standardSlot.disconnect();
    }

    bool Zombies::configure(int numModels, bool standardBearer, bool noiseMaker) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_zombieBite);
            if (standardBearer) {
                model->setName("Standard Bearer");
                standardBearer = false;
            }
            else if (noiseMaker) {
                model->setName("Noisemaker");
                noiseMaker = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Zombies::Create(const ParameterList &parameters) {
        auto unit = new Zombies();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool noisemaker = GetBoolParam("Noisemaker", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Noisemaker"),
                            EnumParameter("Legion", g_legions[0], g_legions)
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Zombies::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive("Standard Bearer") && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

    int Zombies::rollChargeDistance() const {
        // Noise Maker
        auto dist = Unit::rollChargeDistance();
        if (isNamedModelAlive("Noisemaker")) {
            return std::max(6, dist);
        }
        return dist;
    }

} //namespace Death
