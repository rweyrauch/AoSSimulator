/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/LordOfPlagues.h>
#include <UnitFactory.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool LordOfPlagues::s_registered = false;

    LordOfPlagues::LordOfPlagues() :
            NurgleBase("Lord of Plagues", 4, g_wounds, 9, 4, false),
            m_plagueriddenGreatBlade(Weapon::Type::Melee, "Plague-ridden Great Blade", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, LORD_OF_PLAGUES};
        m_weapons = {&m_plagueriddenGreatBlade};
        m_battleFieldRole = Role::Leader;
    }

    bool LordOfPlagues::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_plagueriddenGreatBlade);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *LordOfPlagues::Create(const ParameterList &parameters) {
        auto unit = new LordOfPlagues();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalRotbringerCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalRotbringerArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordOfPlagues::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfPlagues::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    LordOfPlagues::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_mortalRotbringerCommandTraits[0],
                                          g_mortalRotbringerCommandTraits),
                            EnumParameter("Artefact", g_mortalRotbringerArtefacts[0], g_mortalRotbringerArtefacts),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Lord of Plagues", factoryMethod);
        }
    }

    int LordOfPlagues::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Plague-ridden Great Weapons
        if (unmodifiedHitRoll == 6) {
            // each 6 inflicts D6 hits
            return Dice::RollD6();
        }

        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int LordOfPlagues::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle

