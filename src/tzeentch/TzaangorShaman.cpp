/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/TzaangorShaman.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 150;

    bool TzaangorShaman::s_registered = false;

    Unit *TzaangorShaman::Create(const ParameterList &parameters) {
        auto unit = new TzaangorShaman();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TzaangorShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TzaangorShaman::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TzaangorShaman::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_arcaniteCommandTraits[0], g_arcaniteCommandTraits),
                            EnumParameter("Artefact", g_arcaniteArtefacts[0], g_arcaniteArtefacts),
                            EnumParameter("Lore", g_loreOfFate[0], g_loreOfFate)
                    },
                    CHAOS,
                    {TZEENTCH, BEASTS_OF_CHAOS}
            };
            s_registered = UnitFactory::Register("Tzaangor Shaman", factoryMethod);
        }
    }

    TzaangorShaman::TzaangorShaman() :
            TzeentchBase("Tzaangor Shaman", 16, WOUNDS, 6, 5, true),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, HERO, WIZARD, TZAANGOR_SHAMAN};
        m_weapons = {&m_staff, &m_dagger, &m_teethAndHorns};

        s_globalToHitMod.connect(this, &TzaangorShaman::visionsToHitMod, &m_visionsSlot);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    TzaangorShaman::~TzaangorShaman() {
        m_visionsSlot.disconnect();
    }

    bool TzaangorShaman::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int TzaangorShaman::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    int TzaangorShaman::visionsToHitMod(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (attacker->hasKeyword(TZAANGOR_SKYFIRES) && (distanceTo(attacker) <= 12.0) && weapon->isMissile()) return 1;
        if (attacker->hasKeyword(TZAANGOR_ENLIGHTENED) && (distanceTo(attacker) <= 12.0)) return 1;
        return 0;
    }

} // Tzeentch