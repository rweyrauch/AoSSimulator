/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Karanak.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool Karanak::s_registered = false;

    Karanak::Karanak(SlaughterHost host) :
            KhorneBase("Karanak", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, FLESH_HOUND, KHORNE, HERO, KARANAK};
        m_weapons = {&m_goreSlickClaws, &m_savageMaws};

        // Brass Collar of Bloody Vengeance
        m_totalUnbinds = 1;

        setSlaughterHost(host);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_goreSlickClaws);
        model->addMeleeWeapon(&m_savageMaws);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Karanak::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);

        return new Karanak(host);
    }

    void Karanak::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Karanak::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Karanak::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Karanak", factoryMethod);
        }
    }

    Rerolls Karanak::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Prey of the Blood God
        if (m_pQuarry && (m_pQuarry->name() == target->name())) {
            return Rerolls::Failed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    Rerolls Karanak::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Prey of the Blood God
        if (m_pQuarry && (m_pQuarry->name() == target->name())) {
            return Rerolls::Failed;
        }
        return KhorneBase::toWoundRerolls(weapon, target);
    }

    void Karanak::onBeginTurn(int battleRound, PlayerId player) {
        if (battleRound == 1) {
            // Select a quarry for Karanak
            // TODO: Select a _good_ unit to be Karanak's quarry
            m_pQuarry = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        }

        KhorneBase::onBeginTurn(battleRound, player);
    }

    int Karanak::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Karanak::onUnboundSpell(Unit *caster, int castRoll) {
        KhorneBase::onUnboundSpell(caster, castRoll);

        // Brass Collar of Bloody Vengeance
        caster->applyDamage({0, Dice::RollD3()}, this);
    }

} // namespace Khorne