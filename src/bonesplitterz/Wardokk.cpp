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

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool Wardokk::s_registered = false;

    Unit *Wardokk::Create(const ParameterList &parameters) {
        auto unit = new Wardokk();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
        unit->setWarclan(warclan);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Wardokk::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz,
                             Bonesplitterz::Icebone, 1},
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Wardokk", factoryMethod);
        }
    }

    Wardokk::Wardokk() :
            Bonesplitterz("Wardokk", 5, WOUNDS, 7, 6, false),
            m_bonebeastStikk(Weapon::Type::Melee, "Bonebeast Stikk", 1, 1, 4, 3, 0, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, PRIEST, WIZARD, WARDOKK};
        m_weapons = {&m_bonebeastStikk};
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Wardokk::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bonebeastStikk);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Wardokk::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void Wardokk::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Ritual Dance
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
            for (auto unit : units) {
                if (unit->hasKeyword(BONESPLITTERZ) && (unit->remainingWounds() < unit->initialWounds())) {
                    // Grimdokk Dance
                    if (Dice::rollD6() >= 3)
                        unit->heal(Dice::rollD3());
                    break;
                }
                else if (unit->hasKeyword(WIZARD) && unit->hasKeyword(BONESPLITTERZ)) {
                    // Weirddokk Dance
                    if (Dice::rollD6() >= 3)
                        unit->buffModifier(CastingRoll, 1, {Hero, m_battleRound+1, owningPlayer()});
                    break;
                }
                else if (unit->meleeTarget() != nullptr) {
                    // Glyphdokk Dance
                    if (Dice::rollD6() >= 3)
                        unit->buffModifier(ToSave, 1, {Hero, m_battleRound+1, owningPlayer()});
                    break;
                }
            }
        }
    }

} // namespace Bonesplitterz
