/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WightKingTombBlade.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_basesizeMount = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool WightKingWithBalefulTombBlade::s_registered = false;

    WightKingWithBalefulTombBlade::WightKingWithBalefulTombBlade(Legion legion, bool hasSteed, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(legion, "Wight King with Baleful Tomb Blade", 4, g_wounds, 10, 3, false, g_pointsPerUnit),
            m_balefulTombBlade(Weapon::Type::Melee, "Baleful Tomb Blade", 1, 4, 3, 3, -1, 1),
            m_steedsHoovesAndTeeth(Weapon::Type::Melee, "Skeletal Steed's Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_balefulTombBlade, &m_steedsHoovesAndTeeth};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(hasSteed ? g_basesizeMount : g_basesize, wounds());

        m_hasSteed = hasSteed;
        m_hasMount = hasSteed;

        model->addMeleeWeapon(&m_balefulTombBlade);
        if (m_hasSteed) {
            model->addMeleeWeapon(&m_steedsHoovesAndTeeth);
            m_steedsHoovesAndTeeth.setMount(true);
            m_move = 12;
        }
        addModel(model);
    }

    Unit *WightKingWithBalefulTombBlade::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        bool steed = GetBoolParam("Steed", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WightKingWithBalefulTombBlade(legion, steed, trait, artefact, general);
    }

    void WightKingWithBalefulTombBlade::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("Steed"),
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {DEATHRATTLE}
            };
            s_registered = UnitFactory::Register("Wight King with Baleful Tomb Blade", factoryMethod);
        }
    }

    Wounds WightKingWithBalefulTombBlade::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                       int woundRoll) const {
        // Beheading Strike
        if ((hitRoll >= 6) && (weapon->name() == m_balefulTombBlade.name())) {
            return {Dice::RollD3(), 0};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int WightKingWithBalefulTombBlade::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void WightKingWithBalefulTombBlade::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Soulblight
