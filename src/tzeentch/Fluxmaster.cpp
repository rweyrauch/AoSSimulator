/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fluxmaster.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 0;

    bool FluxmasterHeraldOfTzeentchOnDisc::s_registered = false;

    Unit *FluxmasterHeraldOfTzeentchOnDisc::Create(const ParameterList &parameters) {
        auto unit = new FluxmasterHeraldOfTzeentchOnDisc();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int FluxmasterHeraldOfTzeentchOnDisc::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void FluxmasterHeraldOfTzeentchOnDisc::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FluxmasterHeraldOfTzeentchOnDisc::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    FluxmasterHeraldOfTzeentchOnDisc::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange)
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fluxmaster Herald of Tzeentch on Disc", factoryMethod);
        }
    }

    FluxmasterHeraldOfTzeentchOnDisc::FluxmasterHeraldOfTzeentchOnDisc() :
            TzeentchBase(),
            m_flames(Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, FLUXMASTER};
        m_weapons = {&m_flames, &m_staff, &m_dagger, &m_teethAndHorns};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool FluxmasterHeraldOfTzeentchOnDisc::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_flames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Tzeentch


