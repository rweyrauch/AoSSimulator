/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/KurdossValentian.h"
#include "Roster.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 180;

    bool KurdossValentian::s_registered = false;

    Unit *KurdossValentian::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KurdossValentian(procession, general);
    }

    void KurdossValentian::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KurdossValentian::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KurdossValentian::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Kurdoss Valentian", factoryMethod);
        }
    }

    KurdossValentian::KurdossValentian(Procession procession, bool isGeneral) :
            Nighthaunt(procession, "Kurdoss Valentian", 6, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KURDOSS_VALENTIAN};
        m_weapons = {&m_sceptre, &m_claws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_claws.setMount(true);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sceptre);
        model->addMeleeWeapon(&m_claws);
        addModel(model);
    }

    Wounds KurdossValentian::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_claws.name())) {
            return {0, 1};
        }

        // Soul-crushing Smite
        if ((woundRoll == 6) && (weapon->name() == m_sceptre.name())) {
            return {Dice::RollD6(), 0};
        }
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls KurdossValentian::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (target->isGeneral() && (weapon->name() == m_sceptre.name())) {
            return Rerolls::Failed;
        }
        return Nighthaunt::toHitRerolls(weapon, target);
    }

    int KurdossValentian::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void KurdossValentian::onStartHero(PlayerId player) {
        Nighthaunt::onStartHero(player);

        // If I Cannot Rule, None Shall Rule!
        if (GetEnemyId(owningPlayer()) == player) {
            auto roster = Board::Instance()->getPlayerRoster(player);
            if (roster) {
                if (roster->getCommandPoints() > 0) {
                    if (Dice::RollD6() >= 5) {
                        roster->addCommandPoints(-1);
                    }
                }
            }
        }
    }

} // namespace Nighthaunt
