/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/LordOfBlights.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool LordOfBlights::s_registered = false;

    Unit *LordOfBlights::Create(const ParameterList &parameters) {
        auto unit = new LordOfBlights();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalRotbringerCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalRotbringerArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion)GetEnumParam("Plague Legion", parameters, (int)PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordOfBlights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfBlights::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    LordOfBlights::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_mortalRotbringerCommandTraits[0], g_mortalRotbringerCommandTraits),
                            EnumParameter("Artefact", g_mortalRotbringerArtefacts[0], g_mortalRotbringerArtefacts),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Lord of Blights", factoryMethod);
        }
    }

    LordOfBlights::LordOfBlights() :
            NurgleBase("Lord of Blights", 4, g_wounds, 8, 4, false),
            m_ripenedDeathsHead(Weapon::Type::Missile, "Thrice-ripened Death's Head", 14, 1, 3, 3, -3, RAND_D3),
            m_bountyDeathsHead(Weapon::Type::Missile, "Munificent Bounty Death's Head", 14, 1, 4, 3, 0, 1),
            m_hammer(Weapon::Type::Melee, "Bubotic Hammer", 1, 3, 3, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, LORD_OF_BLIGHTS};
        m_weapons = {&m_ripenedDeathsHead, &m_bountyDeathsHead, &m_hammer};
        m_battleFieldRole = Role::Leader;
    }

    bool LordOfBlights::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_ripenedDeathsHead);
        model->addMissileWeapon(&m_bountyDeathsHead);
        model->addMeleeWeapon(&m_hammer);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Rerolls LordOfBlights::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Vermid Shield
        if (!weapon->isMissile()) return Rerolls::Ones;
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int LordOfBlights::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
