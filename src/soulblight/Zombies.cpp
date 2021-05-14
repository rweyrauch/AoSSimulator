/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Zombies.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 115;
    static const int g_pointsMaxUnitSize = 2 * g_pointsPerBlock;

    bool Zombies::s_registered = false;

    Zombies::Zombies(CursedBloodline legion, int numModels, bool standardBearer, bool noiseMaker, int points) :
            SoulblightBase(legion, "Zombies", 4, g_wounds, 10, NoSave, false, points),
            m_zombieBite(Weapon::Type::Melee, "Zombie Bite", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE};
        m_weapons = {&m_zombieBite};
        m_battleFieldRole = Role::Battleline;

        s_globalBraveryMod.connect(this, &Zombies::standardBearerBraveryMod, &m_standardSlot);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_zombieBite);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (noiseMaker) {
                model->setName("Noisemaker");
                noiseMaker = false;
            }
            addModel(model);
        }
    }

    Zombies::~Zombies() {
        m_standardSlot.disconnect();
    }

    Unit *Zombies::Create(const ParameterList &parameters) {
        auto legion = (CursedBloodline) GetEnumParam("Legion", parameters, g_legions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool noisemaker = GetBoolParam("Noisemaker", parameters, false);
        return new Zombies(legion, numModels, standardBearers, noisemaker, ComputePoints(parameters));
    }

    void Zombies::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Noisemaker"),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Zombies", factoryMethod);
        }
    }

    int Zombies::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = SoulblightBase::toHitModifier(weapon, target);

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
        int modifier = SoulblightBase::toWoundModifier(weapon, target);

        // Dragged Down and Torn Apart
        if (remainingModels() >= 40) {
            modifier += 2;
        } else if (remainingModels() >= 20) {
            modifier += 1;
        }

        return modifier;
    }

    int Zombies::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Zombies::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive(Model::StandardBearer) && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

    int Zombies::rollChargeDistance() {
        // Noise Maker
        auto dist = SoulblightBase::rollChargeDistance();
        if (isNamedModelAlive("Noisemaker")) {
            return std::max(6, dist);
        }
        return dist;
    }

} //namespace Soulblight
