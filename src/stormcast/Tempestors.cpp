/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/Tempestors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace StormcastEternals {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 6;

    bool Tempestors::s_registered = false;

    Tempestors::Tempestors() :
            StormcastEternal("Tempestors", 10, WOUNDS, 7, 3, false),
            m_stormBlast(Weapon::Type::Missile, "Storm Blast", 12, 1, 4, 0, 0, 0),
            m_volleystormCrossbow(Weapon::Type::Missile, "Volleystorm Crossbow", 12, 4, 3, 4, 0, 1),
            m_warblade(Weapon::Type::Melee, "Warblade", 1, 3, 3, 4, 0, 1),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, DRACOTHIAN_GUARD, TEMPESTORS};
        m_weapons = {&m_stormBlast, &m_volleystormCrossbow, &m_warblade, &m_clawsAndFangs};
    }

    bool Tempestors::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (int i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_stormBlast);
            model->addMissileWeapon(&m_volleystormCrossbow);
            model->addMeleeWeapon(&m_warblade);
            model->addMeleeWeapon(&m_clawsAndFangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Tempestors::Create(const ParameterList &parameters) {
        auto unit = new Tempestors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, NoStormhost);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Tempestors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Stormhost", NoStormhost, g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Tempestors", factoryMethod);
        }
    }

    Wounds Tempestors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((hitRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            return {Dice::rollD6(), 0};
        }

        // Storm Blast
        if (weapon->name() == m_stormBlast.name()) {
            return {0, Dice::rollD3()};
        }

        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls Tempestors::toSaveRerolls(const Weapon * /*weapon*/) const {
        // Sigmarite Shields
        return RerollOnes;
    }

    int Tempestors::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = StormcastEternal::targetHitModifier(weapon, attacker);

        // Disruptive Fire
        if (weapon->isMissile()) {
            auto unit = Board::Instance()->getUnitWithKeyword(attacker, owningPlayer(), TEMPESTORS, 12);
            if (unit != nullptr)
                modifier -= 1;
        }
        return modifier;
    }

    int Tempestors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }
} //namespace StormcastEternals