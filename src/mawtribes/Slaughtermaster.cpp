/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "mawtribes/Slaughtermaster.h"
#include "MawtribesPrivate.h"
#include "MawtribesLore.h"

namespace OgorMawtribes {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool Slaughtermaster::s_registered = false;

    Unit *Slaughtermaster::Create(const ParameterList &parameters) {
        auto unit = new Slaughtermaster();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_butcherTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_butcherArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_butcherLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }


    void Slaughtermaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Slaughtermaster::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Slaughtermaster::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_butcherTraits[0], g_butcherTraits),
                            EnumParameter("Artefact", g_butcherArtefacts[0], g_butcherArtefacts),
                            EnumParameter("Lore", g_butcherLore[0], g_butcherLore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Slaughtermaster", factoryMethod);
        }
    }

    Slaughtermaster::Slaughtermaster() :
            MawtribesBase("Slaughtermaster", 6, g_wounds, 8, 5, false),
            m_stumpBlades(Weapon::Type::Melee, "Stump Blades", 1, RAND_2D6, 3, 3, 0, 1),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
            m_assortedWeapons(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 3, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER, SLAUGHTERMASTER};
        m_weapons = {&m_stumpBlades, &m_bite, &m_assortedWeapons};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Slaughtermaster::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_stumpBlades);
        model->addMeleeWeapon(&m_assortedWeapons);
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        //m_knownSpells.push_back(std::make_unique<Rockchomper>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        if (hasKeyword(BLOODGULLET)) {
            m_totalSpells++;
            m_totalUnbinds++;
        }

        m_points = Slaughtermaster::ComputePoints(1);

        return true;
    }

    void Slaughtermaster::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    void Slaughtermaster::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    void Slaughtermaster::onStartHero(PlayerId player) {
        MawtribesBase::onStartHero(player);

        // Great Cauldron
        if (owningPlayer() == player) {
            const auto roll = Dice::RollD6();
            switch (roll) {
                case 1:
                    // Bad Meat
                    applyDamage({0, Dice::RollD3()}, this);
                    break;
                case 2:
                    // Troggoth Guts
                    heal(Dice::RollD3());
                    {
                        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
                        for (auto unit : units) {
                            if (unit->hasKeyword(OGOR)) unit->heal(1);
                        }
                    }
                    break;
                case 3:
                case 4:
                    // Spinemarrow
                {
                    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), OGOR, 12.0);
                    if (unit) {
                        unit->buffModifier(BuffableAttribute::To_Hit_Melee, 1, {Hero, m_battleRound + 1, owningPlayer()});
                    }
                }
                    break;
                case 5:
                case 6:
                    // Bonecrusher
                {
                    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
                    for (auto unit : units) {
                        if (Dice::RollD6() >= 4) unit->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }

    int Slaughtermaster::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
