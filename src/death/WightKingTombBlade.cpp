/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/WightKingTombBlade.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int BASESIZE = 32;
    static const int BASESIZE_MOUNT = 60; // x35 oval
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool WightKingWithBalefulTombBlade::s_registered = false;

    WightKingWithBalefulTombBlade::WightKingWithBalefulTombBlade() :
            LegionOfNagashBase("Wight King with Baleful Tomb Blade", 4, WOUNDS, 10, 3, false),
            m_balefulTombBlade(Weapon::Type::Melee, "Baleful Tomb Blade", 1, 4, 3, 3, -1, 1),
            m_steedsHoovesAndTeeth(Weapon::Type::Melee, "Skeletal Steed's Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_balefulTombBlade, &m_steedsHoovesAndTeeth};
    }

    Unit *WightKingWithBalefulTombBlade::Create(const ParameterList &parameters) {
        auto unit = new WightKingWithBalefulTombBlade();
        bool steed = GetBoolParam("Steed", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(steed);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WightKingWithBalefulTombBlade::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
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

    bool WightKingWithBalefulTombBlade::configure(bool hasSteed) {
        auto model = new Model(hasSteed ? BASESIZE_MOUNT : BASESIZE, wounds());

        m_hasSteed = hasSteed;
        m_hasMount = hasSteed;

        model->addMeleeWeapon(&m_balefulTombBlade);
        if (m_hasSteed) {
            model->addMeleeWeapon(&m_steedsHoovesAndTeeth);
            m_move = 12;
        }
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds WightKingWithBalefulTombBlade::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                       int woundRoll) const {
        // Beheading Strike
        if ((hitRoll >= 6) && (weapon->name() == m_balefulTombBlade.name())) {
            return {Dice::rollD3(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WightKingWithBalefulTombBlade::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void WightKingWithBalefulTombBlade::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Death
