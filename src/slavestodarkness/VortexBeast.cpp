/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include <climits>
#include <Roster.h>
#include "slavestodarkness/VortexBeast.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    bool MutalithVortexBeast::s_registered = false;

    Unit *MutalithVortexBeast::Create(const ParameterList &parameters) {
        auto unit = new MutalithVortexBeast();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, Undivided);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MutalithVortexBeast::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void MutalithVortexBeast::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MutalithVortexBeast::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    MutalithVortexBeast::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch, 1},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, TZEENTCH}
            };
            s_registered = UnitFactory::Register("Mutalith Vortex Beast", factoryMethod);
        }
    }

    MutalithVortexBeast::MutalithVortexBeast() :
            SlavesToDarknessBase("Mutalith Vortex Beast", 10, WOUNDS, 7, 4, false),
            m_claws(Weapon::Type::Melee, "Crushing Claws", 2, 4, 4, 1, -1, RAND_D3),
            m_maw(Weapon::Type::Melee, "Betentacled Maw", 2, RAND_3D6, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SLAVES_TO_DARKNESS, MONSTER, MUTALITH_VORTEX_BEAST};
        m_weapons = {&m_claws, &m_maw};
    }

    bool MutalithVortexBeast::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_maw);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void MutalithVortexBeast::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Mutant Regeneration
            heal(Dice::rollD3());

            // Aura of Mutilation
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 18.0f);
            for (auto unit : units) {
                const auto roll = Dice::rollD6();
                switch (roll) {
                    case 1:
                        // Hideous Disfigurement
                        unit->buffModifier(BuffableAttribute::Bravery, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 2:
                        // Troggbrains
                        unit->buffModifier(BuffableAttribute::RunDistance, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 3:
                        // Gift of Mutations
                        unit->buffModifier(BuffableAttribute::MoveDistance, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 4:
                        // Tide of Transmogrification
                        unit->applyDamage({0, Dice::rollD3()});
                        break;
                    case 5:
                        // Maelstrom of Change
                        unit->applyDamage({0, Dice::rollD6()});
                        break;
                    case 6: {
                        // Spawnchange
                        auto numSlain = unit->applyDamage({0, Dice::rollD6()});
                        if (numSlain) {
                            if (remainingWounds() < initialWounds()) {
                                heal(Dice::rollD3());
                            }
                            else {
                                // Summon a Chaos Spawn
                                auto factory = UnitFactory::LookupUnit("Chaos Spawn");
                                if (factory) {
                                    if (m_roster) {
                                        auto unit = UnitFactory::Create("Chaos Spawn", factory->m_parameters);
                                        unit->setPosition(position(), m_orientation);
                                        m_roster->addUnit(unit);
                                    }
                                }
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                // Only apply to first enemy unit found
                break;
            }
        }
    }

} // namespace SlavesToDarkness


