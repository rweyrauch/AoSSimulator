/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/LoonbossOnGiantCaveSquig.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include <iostream>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    bool LoonbossOnGiantCaveSquig::s_registered = false;

    LoonbossOnGiantCaveSquig::LoonbossOnGiantCaveSquig() :
            GloomspiteGitzBase("Loonboss on Giant Cave Squig", RAND_2D6, WOUNDS, 6, 4, true),
            m_massiveFangFilledGob(Weapon::Type::Melee, "Massive Fang-filled Gob", 1, 4, 4, 3, -1, RAND_D3),
            m_moonCutta(Weapon::Type::Melee, "Moon-cutta", 1, 5, 3, 4, 0, 1),
            m_moonclanStabba(Weapon::Type::Melee, "Moonclan Stabba", 2, 5, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
        m_weapons = {&m_massiveFangFilledGob, &m_moonCutta, &m_moonclanStabba};
        m_battleFieldRole = Leader;
        m_hasMount = true;
    }

    bool LoonbossOnGiantCaveSquig::configure(WeaponOptions weapon) {
        auto model = new Model(BASESIZE, wounds());

        model->addMeleeWeapon(&m_massiveFangFilledGob);
        if (weapon == Mooncutta) {
            model->addMeleeWeapon(&m_moonCutta);
        } else if (weapon == MoonclanStabba) {
            model->addMeleeWeapon(&m_moonclanStabba);
        }
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LoonbossOnGiantCaveSquig::Create(const ParameterList &parameters) {
        auto unit = new LoonbossOnGiantCaveSquig();
        WeaponOptions weapon = (WeaponOptions) GetEnumParam("weapons", parameters, Mooncutta);

        auto trait = (CommandTrait)GetEnumParam("Command Trait", parameters, g_blessingsOfTheBadMoon[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact)GetEnumParam("Artefact", parameters, g_troglodyticTreasures[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string LoonbossOnGiantCaveSquig::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "weapons") {
            if (parameter.intValue == Mooncutta) { return "Mooncutta"; }
            else if (parameter.intValue == MoonclanStabba) { return "MoonclanStabba"; }
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int LoonbossOnGiantCaveSquig::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Mooncutta") {
            return Mooncutta;
        } else if (enumString == "MoonclanStabba") {
            return MoonclanStabba;
        }
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    void LoonbossOnGiantCaveSquig::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LoonbossOnGiantCaveSquig::Create,
                    LoonbossOnGiantCaveSquig::ValueToString,
                    LoonbossOnGiantCaveSquig::EnumStringToInt,
                    LoonbossOnGiantCaveSquig::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_blessingsOfTheBadMoon[0], g_blessingsOfTheBadMoon),
                            EnumParameter("Artefact", g_troglodyticTreasures[0], g_troglodyticTreasures),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonboss on Giant Cave Squig", factoryMethod);
        }
    }

    void LoonbossOnGiantCaveSquig::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Redcap Mushrooms
            m_toHitRerolls = NoRerolls;
            m_toWoundRerolls = NoRerolls;

            if (!m_eatenRedcapMushroom) {
                if (m_meleeTarget) {
                    std::cout << "Eating the Redcap Mushroom!" << std::endl;
                    m_eatenRedcapMushroom = true;
                    m_toHitRerolls = RerollFailed;
                    m_toWoundRerolls = RerollFailed;
                }
            }
        }
    }

    int LoonbossOnGiantCaveSquig::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        return GloomspiteGitzBase::toHitModifier(weapon, unit);
    }

    Rerolls LoonbossOnGiantCaveSquig::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_moonCutta.name() || weapon->name() == m_moonclanStabba.name())
            return m_toHitRerolls;
        return GloomspiteGitzBase::toHitRerolls(weapon, target);
    }

    Rerolls LoonbossOnGiantCaveSquig::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_moonCutta.name() || weapon->name() == m_moonclanStabba.name())
            return m_toWoundRerolls;
        return GloomspiteGitzBase::toWoundRerolls(weapon, target);
    }

    int LoonbossOnGiantCaveSquig::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz

