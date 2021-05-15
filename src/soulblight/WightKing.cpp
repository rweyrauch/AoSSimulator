/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WightKing.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool WightKing::s_registered = false;

    WightKing::WightKing(CursedBloodline bloodline, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(bloodline, "Wight King", 4, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_balefulTombBlade(Weapon::Type::Melee, "Baleful Tomb Blade", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_balefulTombBlade};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_balefulTombBlade);
        addModel(model);
    }

    Unit *WightKing::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WightKing(bloodline, trait, artefact, general);
    }

    void WightKing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Wight King", factoryMethod);
        }
    }

    int WightKing::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Soulblight
