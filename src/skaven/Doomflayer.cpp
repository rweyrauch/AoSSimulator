/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Doomflayer.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 60;

    bool Doomflayer::s_registered = false;

    Doomflayer::Doomflayer() :
            Skaventide("Doom-flayer", RAND_2D6, g_wounds, 4, 5, false, g_pointsPerUnit),
            m_whirlingBlades(Weapon::Type::Melee, "Whirling Blades", 1, 0, 3, 3, -1, 1),
            m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, DOOM_FLAYER};
        m_weapons = {&m_whirlingBlades, &m_rustyKnives};
    }

    void Doomflayer::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_whirlingBlades);
        model->addMeleeWeapon(&m_rustyKnives);
        addModel(model);
    }

    Unit *Doomflayer::Create(const ParameterList &parameters) {
        auto unit = new Doomflayer();

        unit->configure();
        return unit;
    }

    void Doomflayer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Doom-flayer", factoryMethod);
        }
    }

    int Doomflayer::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Skaventide::toHitModifier(weapon, target);

        // Whirling Death
        if (m_charged && (weapon->name() == m_whirlingBlades.name())) {
            modifier += 1;
        }

        return modifier;
    }

    int Doomflayer::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // More-more Whirling Death
        if (weapon->name() == m_whirlingBlades.name()) {
            // Decide to overcharge
            if (moreMore()) {
                auto roll1 = Dice::RollD6();
                auto roll2 = Dice::RollD6();
                if ((roll1 == roll2) || (roll1 + roll2 == 7)) {
                    m_moreMoreFailed = true;
                }
                return (roll1 + roll2);
            }

            m_moreMoreFailed = false;
            return RAND_D6;
        }

        return Unit::extraAttacks(attackingModel, weapon, target);
    }

    void Doomflayer::onRestore() {
        Unit::onRestore();

        m_moreMoreFailed = false;
    }

    void Doomflayer::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        if (m_moreMoreFailed) {
            slay(1);
            m_moreMoreFailed = false;
        }
    }

    int Doomflayer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Skaven
