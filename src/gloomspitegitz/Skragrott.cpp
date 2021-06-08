/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Skragrott.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <gloomspitegitz/NikkitNikkit.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 220;

    bool Skragrott::s_registered = false;

    Skragrott::Skragrott(Allegiance allegiance, Lore lore, bool isGeneral) :
            GloomspiteGitzBase(allegiance, "Skragrott", 4, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_daMoonOnnaStikkMissile(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
            m_daMoonOnnaStikk(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, LOONBOSS, SKRAGROTT};
        m_weapons = {&m_daMoonOnnaStikk, &m_daMoonOnnaStikkMissile};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 2;
        m_totalSpells = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_daMoonOnnaStikkMissile);
        model->addMeleeWeapon(&m_daMoonOnnaStikk);

        m_knownSpells.push_back(std::make_unique<NikkitNikkit>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        addModel(model);
    }

    Unit *Skragrott::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheMoonclans[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Skragrott(allegiance, lore, general);
    }

    void Skragrott::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore of the Moonclans", g_loreOfTheMoonclans[0], g_loreOfTheMoonclans),
                            BoolParameter("General"),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Skragrott", factoryMethod);
        }
    }

    std::string Skragrott::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Moonclans") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int Skragrott::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) {
            return (int) lore;
        }
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int Skragrott::castingModifier() const {
        // Loonking's Crown
        return GloomspiteGitzBase::castingModifier() + 1;
    }

    int Skragrott::unbindingModifier() const {
        // Loonking's Crown
        return GloomspiteGitzBase::unbindingModifier() + 1;
    }

    Wounds Skragrott::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Loonking's Crown
        int numSixes = 0;
        return ignoreWounds(wounds, 4, numSixes);
    }

    void Skragrott::onStartHero(PlayerId playerId) {
        GloomspiteGitzBase::onStartHero(playerId);

        // Babbling Wand
        if (isGeneral() && (owningPlayer() == playerId) && m_roster) {
            if (Dice::RollD6() >= 4) {
                m_roster->addCommandPoints(Dice::RollD3());
            }
        }
    }

    int Skragrott::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
