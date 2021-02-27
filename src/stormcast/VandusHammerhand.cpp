/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/VandusHammerhand.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 260;

    bool VandusHammerhand::s_registered = false;

    VandusHammerhand::VandusHammerhand() :
            MountedStormcastEternal("Vandus Hammerhand", 10, g_wounds, 9, 3, false),
            m_heldensen(Weapon::Type::Melee, "Heldensen", 2, 3, 3, 2, -1, 3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, HAMMERS_OF_SIGMAR, LORD_CELESTANT,
                      VANDUS_HAMMERHAND};
        m_weapons = {&m_heldensen, &m_clawsAndFangs};
        m_battleFieldRole = Leader;
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);

        s_globalBraveryMod.connect(this, &VandusHammerhand::lordOfTheHammerhandsBraveryMod, &m_lordSlot);
    }

    VandusHammerhand::~VandusHammerhand() {
        m_lordSlot.disconnect();
    }

    bool VandusHammerhand::configure(MountTrait trait) {

        m_mountTrait = trait;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_heldensen);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Vengeful Determination", 12, 12, Phase::Combat,
                                                                Attacks_Melee, 1, Abilities::Target::SelfAndFriendly,
                                                                std::vector<Keyword>(HAMMERS_OF_SIGMAR)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *VandusHammerhand::Create(const ParameterList &parameters) {
        auto unit = new VandusHammerhand();
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, (int)MountTrait::None);

        unit->setStormhost(Stormhost::Hammers_Of_Sigmar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(trait);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VandusHammerhand::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Mount Trait", g_dracothMountTrait[0], g_dracothMountTrait),
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Vandus Hammerhand", factoryMethod);
        }
    }

    int VandusHammerhand::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = MountedStormcastEternal::extraAttacks(attackingModel, weapon, target);

        // Heldensen
        if (m_charged && weapon->name() == m_heldensen.name()) {
            attacks += Dice::RollD3();
        }
        return attacks;
    }

    Wounds VandusHammerhand::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            // D6 instead of 1
            return {Dice::RollD6(), 0};
        }
        return MountedStormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int VandusHammerhand::lordOfTheHammerhandsBraveryMod(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(HAMMERS_OF_SIGMAR) && (distanceTo(unit) <= 24.0)) return 42;
        return 0;
    }

    int VandusHammerhand::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void VandusHammerhand::onStartShooting(PlayerId player) {
        MountedStormcastEternal::onStartShooting(player);

        // Storm Breath
        if (owningPlayer() == player) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 12.0)) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
                auto units = Board::Instance()->getUnitsWithin(unit, GetEnemyId(owningPlayer()), 2.0);
                for (auto target : units) {
                    if (Dice::RollD6() >= 4) {
                        target->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
            }
        }
    }

} // namespace StormcastEternals