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
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 140;

    bool Fateskimmer::s_registered = false;

    Unit *Fateskimmer::Create(const ParameterList &parameters) {
        auto unit = new Fateskimmer();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        bool ok = unit->configure(lore);
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
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fateskimmer", factoryMethod);
        }
    }

    Fateskimmer::Fateskimmer() :
            TzeentchBase("Fateskimmer", 16, g_wounds, 10, 5, true),
            m_magicalFlames(Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
            m_bite(Weapon::Type::Melee, "Lamprey Bite", 1, 6, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, FATESKIMMER};
        m_weapons = {&m_magicalFlames, &m_staff, &m_dagger, &m_bite};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_bite.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Fateskimmer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_magicalFlames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_bite);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<AreaOfEffectSpell>(this, "Tzeentch's Firestorm", 8, 0, 9, RAND_D3, 3));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds Fateskimmer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER) && (weapon->name() == m_bite.name())) {
            return {Dice::RollD3(), 0};
        }
        return TzeentchBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Fateskimmer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch