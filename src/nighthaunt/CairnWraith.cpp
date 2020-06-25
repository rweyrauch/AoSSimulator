/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/CairnWraith.h>
#include <UnitFactory.h>
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int BASESIZE = 25;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 60;

    bool CairnWraith::s_registered = false;

    Unit *CairnWraith::Create(const ParameterList &parameters) {
        auto unit = new CairnWraith();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int CairnWraith::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void CairnWraith::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Cairn Wraith", factoryMethod);
        }
    }

    CairnWraith::CairnWraith() :
            Nighthaunt("Cairn Wraith", 6, WOUNDS, 10, 4, true),
            m_scythe(Weapon::Type::Melee, "Reaper Scythe", 2, 3, 4, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, CAIRN_WRAITH};
        m_weapons = {&m_scythe};
    }

    bool CairnWraith::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_scythe);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds CairnWraith::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_scythe.name())) return {0, 2};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls CairnWraith::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaped Like Corn
        if (weapon->name() == m_scythe.name() && unit->remainingModels() >= 5) {
            return RerollFailed;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

} // namespace Nighthaunt
