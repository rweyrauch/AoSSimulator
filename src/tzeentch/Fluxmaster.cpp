/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fluxmaster.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    class BlueFireOfTzeentch : public Spell {
    public:
        explicit BlueFireOfTzeentch(Unit* caster);

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override;
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    BlueFireOfTzeentch::BlueFireOfTzeentch(Unit *caster) :
            Spell(caster, "Blue Fire of Tzeentch", 5, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result BlueFireOfTzeentch::apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(9, roll);
        if (roll.rollsGE(6) > 0) {
            target->applyDamage({0, roll.rollsGE(6), Wounds::Source::Spell}, m_caster);
        }

        return Spell::Result::Success;
    }

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool FluxmasterHeraldOfTzeentchOnDisc::s_registered = false;

    Unit *FluxmasterHeraldOfTzeentchOnDisc::Create(const ParameterList &parameters) {
        auto unit = new FluxmasterHeraldOfTzeentchOnDisc();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int FluxmasterHeraldOfTzeentchOnDisc::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void FluxmasterHeraldOfTzeentchOnDisc::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FluxmasterHeraldOfTzeentchOnDisc::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    FluxmasterHeraldOfTzeentchOnDisc::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fluxmaster Herald of Tzeentch on Disc", factoryMethod);
        }
    }

    FluxmasterHeraldOfTzeentchOnDisc::FluxmasterHeraldOfTzeentchOnDisc() :
            TzeentchBase(),
            m_flames(Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, FLUXMASTER};
        m_weapons = {&m_flames, &m_staff, &m_dagger, &m_teethAndHorns};
        m_hasMount = true;
        m_teethAndHorns.setMount(true);
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool FluxmasterHeraldOfTzeentchOnDisc::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_flames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BlueFireOfTzeentch>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int FluxmasterHeraldOfTzeentchOnDisc::rollCasting(int &unmodifiedRoll) const {
        auto roll = TzeentchBase::rollCasting(unmodifiedRoll);
        if (!m_usedArcaneTome) {
            int roll0 = Dice::RollD6();
            int roll1 = Dice::RollD6();
            int roll2 = Dice::RollD6();

            unmodifiedRoll = std::max(roll0, std::max(roll1, roll2));
            roll = unmodifiedRoll + castingModifier();

            m_usedArcaneTome = true;
        }
        return roll;
    }

} // namespace Tzeentch


