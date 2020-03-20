/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "nighthaunt/LadyOlynder.h"

namespace Nighthaunt {
    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    bool LadyOlynder::s_registered = false;

    Unit *LadyOlynder::Create(const ParameterList &parameters) {
        auto unit = new LadyOlynder();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LadyOlynder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LadyOlynder::Create,
                    nullptr,
                    nullptr,
                    LadyOlynder::ComputePoints,
                    {
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Lady Olynder", factoryMethod);
        }
    }

    LadyOlynder::LadyOlynder() :
            Nighthaunt("Lady Olynder", 6, WOUNDS, 10, 4, true),
            m_staff(Weapon::Type::Melee, "Staff of Midnight", 2, 3, 3, 3, -2, RAND_D3),
            m_claws(Weapon::Type::Melee, "Banshee Handmaidens' Spectral Claws", 1, 6, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MORTARCH, LADY_OLYNDER};
        m_weapons = {&m_staff, &m_claws};

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool LadyOlynder::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    Wounds LadyOlynder::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_claws.name())) {
            return {0, 1};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void LadyOlynder::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 10.0f);
        for (auto unit : units) {
            auto roll = Dice::roll2D6();
            if (roll > unit->bravery()) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    void LadyOlynder::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Lifting the Veil
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0f);
        if (!units.empty()) {
            const auto roll = Dice::rollD6();
            if (roll >= 2) {
                const auto numSlain = units[0]->applyDamage({0, roll});
                if (numSlain) {
                    heal(Dice::rollD3());
                }
            }
        }

        if (!m_graveSandsOfTimeUsed) {
            auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 6.0f);
            if (unit) {
                unit->applyDamage({0, Dice::rollD6()});
                m_graveSandsOfTimeUsed = true;
            } else if (remainingWounds() < wounds()) {
                heal(Dice::rollD6());
                m_graveSandsOfTimeUsed = true;
            }
        }
    }

    void LadyOlynder::onRestore() {
        Unit::onRestore();

        m_graveSandsOfTimeUsed = false;
    }

    int LadyOlynder::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
