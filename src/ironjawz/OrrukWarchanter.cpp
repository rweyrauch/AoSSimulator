/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukWarchanter.h>
#include <UnitFactory.h>
#include <Board.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool OrrukWarchanter::s_registered = false;

    OrrukWarchanter::OrrukWarchanter(Warclan warclan, Warbeat warbeat, bool isGeneral) :
            Ironjawz(warclan, "Orruk Warchanter", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_stikks(Weapon::Type::Melee, "Gorkstikk and Morkstikk", 1, 6, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, TOTEM, WARCHANTER};
        m_weapons = {&m_stikks};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_stikks);
        addModel(model);

        m_warbeat = warbeat;
    }

    Unit *OrrukWarchanter::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        auto beat = (Warbeat) GetEnumParam("Warbeat", parameters, g_warbeats[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new OrrukWarchanter(warclan, beat, general);
    }

    void OrrukWarchanter::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWarchanter::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukWarchanter::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Warbeat", g_warbeats[0], g_warbeats),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Warchanter", factoryMethod);
        }
    }

    int OrrukWarchanter::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void OrrukWarchanter::onStartHero(PlayerId player) {
        Ironjawz::onStartHero(player);

        // Violent Fury
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 15.0);
            for (auto unit : units) {
                if (unit->hasKeyword(IRONJAWZ) && (unit->remainingModels() > 0)) {
                    unit->buffModifier(Attribute::Weapon_Damage_Melee, 1,
                                       {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
            }
        }
    }

    int OrrukWarchanter::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto hits = Ironjawz::generateHits(unmodifiedHitRoll, weapon, unit);
        // Rhythm of Destruction
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_stikks.name())) {
            return 2;
        }
        return hits;
    }

} // namespace Ironjawz
