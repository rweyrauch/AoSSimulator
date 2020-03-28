/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fateskimmer.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 140;

    bool Fateskimmer::s_registered = false;

    Unit *Fateskimmer::Create(const ParameterList &parameters) {
        auto unit = new Fateskimmer();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, TzeentchBase::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Fateskimmer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Fateskimmer::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    Fateskimmer::ComputePoints,
                    {
                            {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None,
                             TzeentchBase::GuildOfSummoners, 1},
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fateskimmer", factoryMethod);
        }
    }

    Fateskimmer::Fateskimmer() :
            TzeentchBase("Fateskimmer", 16, WOUNDS, 10, 5, true),
            m_magicalFlames(Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
            m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 6, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, FATESKIMMER};
        m_weapons = {&m_magicalFlames, &m_staff, &m_dagger, &m_bite};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Fateskimmer::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_magicalFlames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_bite);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds Fateskimmer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER) && (weapon->name() == m_bite.name())) {
            return {Dice::rollD3(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Fateskimmer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Tzeentch