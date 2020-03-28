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

namespace OgorMawtribes {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    bool Slaughtermaster::s_registered = false;

    Unit *Slaughtermaster::Create(const ParameterList &parameters) {
        auto unit = new Slaughtermaster();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, None);
        unit->setMawtribe(tribe);

        bool ok = unit->configure();
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
                            {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None,
                             MawtribesBase::Winterbite, 1}
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Slaughtermaster", factoryMethod);
        }
    }

    Slaughtermaster::Slaughtermaster() :
            MawtribesBase("Slaughtermaster", 6, WOUNDS, 8, 5, false),
            m_stumpBlades(Weapon::Type::Melee, "Stump Blades", 1, RAND_2D6, 3, 3, 0, 1),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
            m_assortedWeapons(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 3, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER, SLAUGHTERMASTER};
        m_weapons = {&m_stumpBlades, &m_bite, &m_assortedWeapons};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Slaughtermaster::configure() {
        auto model = new Model(BASESIZE, wounds());

        model->addMeleeWeapon(&m_stumpBlades);
        model->addMeleeWeapon(&m_assortedWeapons);
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<Rockchomper>(this));

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
        const auto roll = Dice::rollD6();
        if (roll == 1) applyDamage({0, 1});
        else heal(1);
    }

    void Slaughtermaster::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Bloodgruel
        const auto roll = Dice::rollD6();
        if (roll == 1) applyDamage({0, 1});
        else heal(1);
    }

    void Slaughtermaster::onStartHero(PlayerId player) {
        MawtribesBase::onStartHero(player);

        // Great Cauldron
        if (owningPlayer() == player) {
            const auto roll = Dice::rollD6();
            switch (roll) {
                case 1:
                    // Bad Meat
                    applyDamage({0, Dice::rollD3()});
                    break;
                case 2:
                    // Troggoth Guts
                    heal(Dice::rollD3());
                    {
                        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
                        for (auto unit : units) {
                            if (unit->hasKeyword(OGOR)) unit->heal(1);
                        }
                    }
                    break;
                case 3:
                case 4:
                    // Spinemarrow
                {
                    auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), OGOR, 12.0f);
                    if (unit) {
                        unit->buffModifier(BuffableAttribute::ToHitMelee, 1, {Hero, m_battleRound + 1, owningPlayer()});
                    }
                }
                    break;
                case 5:
                case 6:
                    // Bonecrusher
                {
                    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0f);
                    for (auto unit : units) {
                        if (Dice::rollD6() >= 4) unit->applyDamage({0, Dice::rollD3()});
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }

    int Slaughtermaster::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace OgorMawtribes
