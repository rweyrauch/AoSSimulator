/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/TheChosenAxes.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 0;

    bool TheChosenAxes::s_registered = false;

    TheChosenAxes::TheChosenAxes() :
            Fyreslayer("The Chosen Axes", 4, g_wounds, 7, 5, false),
            m_handaxes(Weapon::Type::Melee, "Fyresteel Handaxes", 1, 2, 3, 3, 0, 1),
            m_handaxesTefk(Weapon::Type::Melee, "Fyresteel Handaxes", 1, 3, 3, 3, 0, 1),
            m_greatAxe(Weapon::Type::Missile, "Fyresteel Great Axe", 1, 2, 3, 4, -1, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, VULKITE_BERZERKERS, CHOSEN_AXES};
        m_weapons = {&m_handaxes, &m_handaxesTefk, &m_greatAxe};
    }

    bool TheChosenAxes::configure() {
        auto tefk = new Model(g_basesize, wounds());
        tefk->addMeleeWeapon(&m_handaxesTefk);
        tefk->setName("Tefk Flamebearer");
        addModel(tefk);

        auto maegrim = new Model(g_basesize, wounds());
        maegrim->addMeleeWeapon(&m_handaxes);
        maegrim->setName("Mad Maegrim");
        addModel(maegrim);

        auto vol = new Model(g_basesize, wounds());
        vol->addMeleeWeapon(&m_greatAxe);
        vol->setName("Vol Orrukbane");
        addModel(vol);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *TheChosenAxes::Create(const ParameterList &parameters) {
        auto unit = new TheChosenAxes();

        unit->setLodge(Lodge::Vostarg);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheChosenAxes::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheChosenAxes::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    TheChosenAxes::ComputePoints,
                    {
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("The Chosen Axes", factoryMethod);
        }
    }

    Rerolls TheChosenAxes::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_handaxes.name()) {
            return Rerolls::Failed;
        }
        return Fyreslayer::toHitRerolls(weapon, target);
    }

    int TheChosenAxes::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void TheChosenAxes::onRestore() {
        Fyreslayer::onRestore();
        m_usedBerserkFury = false;
        m_berserkFuryActive = false;
    }

    void TheChosenAxes::onStartCombat(PlayerId player) {
        Fyreslayer::onStartCombat(player);

        if (!m_usedBerserkFury) {
            if (meleeTarget() && (distanceTo(meleeTarget()) < 3.0)) {
                m_usedBerserkFury = true;
                m_berserkFuryActive = true;
            }
        }
    }

    void TheChosenAxes::onEndCombat(PlayerId player) {
        Fyreslayer::onEndCombat(player);

        m_berserkFuryActive = false;
    }

    void TheChosenAxes::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Fyreslayer::onFriendlyModelSlain(numSlain, attacker, source);

        // Berserk Fury
        if (m_berserkFuryActive && (numSlain > 0)) {
            doPileIn();
            int enemiesSlain = 0;
            fight(numSlain, attacker, enemiesSlain);
        }
    }

    int TheChosenAxes::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Fyreslayer::toWoundModifier(weapon, target);
        // Chosen Kin
        auto fjul = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), FJUL_GRIMNIR, 3.0);
        if (fjul != nullptr) {
            mod++;
        }
        return mod;
    }

} //namespace Fyreslayers