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
#include "bonesplitterz/Wardokk.h"
#include "BonesplitterzPrivate.h"
#include "BonesplitterzLore.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool Wardokk::s_registered = false;

    Unit *Wardokk::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_wizardCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Wardokk(warclan, lore, trait, artefact, general);
    }

    void Wardokk::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_wizardCommandTraits[0], g_wizardCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Wardokk", factoryMethod);
        }
    }

    Wardokk::Wardokk(Warclan warclan, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Bonesplitterz(warclan, "Wardokk", 5, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_bonebeastStikk(Weapon::Type::Melee, "Bonebeast Stikk", 1, 1, 4, 3, 0, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, PRIEST, WIZARD, WARDOKK};
        m_weapons = {&m_bonebeastStikk};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bonebeastStikk);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int Wardokk::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Wardokk::onStartHero(PlayerId player) {
        Bonesplitterz::onStartHero(player);

        // Ritual Dance
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->hasKeyword(BONESPLITTERZ) && (unit->remainingWounds() < unit->initialWounds())) {
                    // Grimdokk Dance
                    if (Dice::RollD6() >= 3)
                        unit->heal(Dice::RollD3());
                    break;
                } else if (unit->hasKeyword(WIZARD) && unit->hasKeyword(BONESPLITTERZ)) {
                    // Weirddokk Dance
                    if (Dice::RollD6() >= 3)
                        unit->buffModifier(Attribute::Casting_Roll, 1,
                                           {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    break;
                } else if (unit->meleeTarget() != nullptr) {
                    // Glyphdokk Dance
                    if (Dice::RollD6() >= 3) {
                        unit->buffModifier(Attribute::To_Save_Missile, 1,
                                           {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                        unit->buffModifier(Attribute::To_Save_Melee, 1,
                                           {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    }
                    break;
                }
            }
        }
    }

} // namespace Bonesplitterz
