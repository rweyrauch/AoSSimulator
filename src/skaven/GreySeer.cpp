/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
#include "SkavenPrivate.h"

namespace Skaven {

    class Wither : public Spell {
    public:
        explicit Wither(Unit *caster) :
                Spell(caster, "Witherl", 7, 13) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            if (Dice::RollD6() > target->wounds()) {
                target->applyDamage({0, RAND_D3, Wounds::Source::Spell}, m_caster);
                target->buffModifier(To_Hit_Melee, -1, defaultDuration());
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool GreySeer::s_registered = false;

    Unit *GreySeer::Create(const ParameterList &parameters) {
        auto unit = new GreySeer();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_masterClanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GreySeer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_masterClanCommandTraits[0], g_masterClanCommandTraits),
                            EnumParameter("Artefact", g_masterClanArtefacts[0], g_masterClanArtefacts),
                            EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Grey Seer", factoryMethod);
        }
    }

    GreySeer::GreySeer() :
            Skaventide("Grey Seer", 6, g_wounds, 6, 5, false),
            m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, HERO, WIZARD, GREY_SEER};
        m_weapons = {&m_staff};
        m_battleFieldRole = Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool GreySeer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<Wither>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int GreySeer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
