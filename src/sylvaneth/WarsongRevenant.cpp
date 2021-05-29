/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/WarsongRevenant.h>
#include <UnitFactory.h>
#include <sylvaneth/SylvanethSpells.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {

    class UnleashSwarmOfSpites : public AreaOfEffectSpell {
    public:
        explicit UnleashSwarmOfSpites(Unit *caster) :
                AreaOfEffectSpell(caster, "Unleash Swarm of Spites", 7, 0, 9, 1, 0) {}

    protected:

        int getDamage(int castingRoll) const override {
            Dice::RollResult result;
            Dice::RollD6(castingRoll, result);
            return result.rollsGE(5);
        }
    };

    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 275;

    bool WarsongRevenant::s_registered = false;

    WarsongRevenant::WarsongRevenant(Glade glade, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SylvanethBase("Warsong Revenant", 8, g_wounds, 8, 5, true, g_pointsPerUnit) {
        m_keywords = {ORDER, SYLVANETH, HERO, WIZARD, WARSONG_REVENANT};
        m_weapons = {&m_vines};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 2;

        s_globalBraveryMod.connect(this, &WarsongRevenant::alariellesSong, &m_songSlot);

        setGlade(glade);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_vines);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<UnleashSwarmOfSpites>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Throne_Of_Vines, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Regrowth, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::The_Dwellers_Below, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Deadly_Harvest, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Verdurous_Harmony, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Treesong, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    WarsongRevenant::~WarsongRevenant() {
        m_songSlot.disconnect();
    }

    Unit *WarsongRevenant::Create(const ParameterList &parameters) {
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_aspectsOfWar[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new WarsongRevenant(glade, trait, artefact, general);
    }

    void WarsongRevenant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WarsongRevenant::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    WarsongRevenant::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_aspectsOfWar[0], g_aspectsOfWar),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Warsong Revenant", factoryMethod);
        }
    }

    int WarsongRevenant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int WarsongRevenant::alariellesSong(const Unit *unit) {
        if (distanceTo(unit) <= 12.0) {
            if (isFriendly(unit)) return 1;
            else return -1;
        }
        return 0;
    }

    Wounds WarsongRevenant::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SylvanethBase::applyWoundSave(wounds, attackingUnit);
        // Arboreal Cloak
        int numSixes = 0;
        return ignoreWounds(totalWounds, 4, numSixes);
    }

    int WarsongRevenant::castingModifier() const {
        auto mod = SylvanethBase::castingModifier();
        // Wyldwood Revenants
        auto woods = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 9.0);
        if (woods != nullptr) {
            mod++;
        }
        return mod;
    }

    int WarsongRevenant::unbindingModifier() const {
        auto mod = SylvanethBase::unbindingModifier();
        // Wyldwood Revenants
        auto woods = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 9.0);
        if (woods != nullptr) {
            mod++;
        }
        return mod;
    }

} // namespace Sylvaneth
