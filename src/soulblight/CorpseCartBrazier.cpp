/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/CorpseCartBrazier.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 80;

    bool CorpseCartWithBalefireBrazier::s_registered = false;

    Unit *CorpseCartWithBalefireBrazier::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        return new CorpseCartWithBalefireBrazier(legion);
    }

    int CorpseCartWithBalefireBrazier::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void CorpseCartWithBalefireBrazier::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEADWALKERS}
            };
            s_registered = UnitFactory::Register("Corpse Cart with Balefire Brazier", factoryMethod);
        }
    }

    CorpseCartWithBalefireBrazier::CorpseCartWithBalefireBrazier(Legion legion) :
            SoulblightBase(legion, "Corpse Cart with Balefire Brazier", 4, g_wounds, 10, 6, false, g_pointsPerUnit),
            m_goad(Weapon::Type::Melee, "Corpse Goad", 2, 2, 4, 4, 0, 1),
            m_lash(Weapon::Type::Melee, "Corpse Lash", 1, 3, 4, 4, 0, 1),
            m_blades(Weapon::Type::Melee, "Rusty Blades", 1, RAND_2D6, 5, 5, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, CORPSE_CART};
        m_weapons = {&m_goad, &m_lash, &m_blades};
        s_globalCastMod.connect(this, &CorpseCartWithBalefireBrazier::brazierCastingMod, &m_brazierSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_goad);
        model->addMeleeWeapon(&m_lash);
        model->addMeleeWeapon(&m_blades);
        addModel(model);
    }

    CorpseCartWithBalefireBrazier::~CorpseCartWithBalefireBrazier() {
        m_brazierSlot.disconnect();
    }

    int CorpseCartWithBalefireBrazier::brazierCastingMod(const Unit *caster) {
        if (!isFriendly(caster) && hasKeyword(WIZARD) && (distanceTo(caster) <= 18.0)) return -1;
        return 0;
    }

    void CorpseCartWithBalefireBrazier::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) {
            // Malefic Fumes
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
            for (auto unit : units) {
                if (unit->hasKeyword(WIZARD)) {
                    if (Dice::RollD6() >= 4) {
                        unit->applyDamage({0, 1}, this);
                    }
                }
            }
        }
    }

} // namespace Soulblight
