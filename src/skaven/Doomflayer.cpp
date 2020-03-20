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
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 60;

    bool Doomflayer::s_registered = false;

    Doomflayer::Doomflayer() :
            Skaventide("Doom-flayer", RAND_2D6, WOUNDS, 4, 5, false),
            m_whirlingBlades(Weapon::Type::Melee, "Whirling Blades", 1, 0, 3, 3, -1, 1),
            m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, DOOM_FLAYER};
        m_weapons = {&m_whirlingBlades, &m_rustyKnives};
    }

    bool Doomflayer::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_whirlingBlades);
        model->addMeleeWeapon(&m_rustyKnives);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Doomflayer::Create(const ParameterList &parameters) {
        auto unit = new Doomflayer();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
                auto roll1 = Dice::rollD6();
                auto roll2 = Dice::rollD6();
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

    Wounds Doomflayer::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        if (m_moreMoreFailed) {
            slay(1);
            m_moreMoreFailed = false;
        }
        return wounds;
    }

    int Doomflayer::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }


} // namespace Skaven
