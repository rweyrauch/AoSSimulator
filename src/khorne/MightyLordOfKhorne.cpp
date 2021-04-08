/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MightyLordOfKhorne.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    class Gorelord : public CommandAbility {
    public:
        explicit Gorelord(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    Gorelord::Gorelord(Unit *source) :
            CommandAbility(source, "Gorelord", 16, 16, Phase::Charge) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {KHORNE, MORTAL};
    }

    bool Gorelord::apply(Unit *target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(KHORNE) && unit->hasKeyword(MORTAL)) {
                unit->buffReroll(Attribute::Charge_Distance, Rerolls::Failed, defaultDuration());
            }
        }
        return true;
    }

    static const int g_basesize = 60;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool MightyLordOfKhorne::s_registered = false;

    MightyLordOfKhorne::MightyLordOfKhorne(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Mighty Lord of Khorne", 5, g_wounds, 9, 3, false),
            m_axeOfKhorne(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
            m_bloodDarkClaws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, MIGHTY_LORD_OF_KHORNE};
        m_weapons = {&m_axeOfKhorne, &m_bloodDarkClaws};
        m_battleFieldRole = Role::Leader;

        // Collar of Khorne
        m_totalUnbinds = 1;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axeOfKhorne);
        model->addMeleeWeapon(&m_bloodDarkClaws);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<Gorelord>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *MightyLordOfKhorne::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new MightyLordOfKhorne(host, trait, artefact, general);
    }

    void MightyLordOfKhorne::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MightyLordOfKhorne::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    MightyLordOfKhorne::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Mighty Lord of Khorne", factoryMethod);
        }
    }

    int MightyLordOfKhorne::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MightyLordOfKhorne::onEnemyModelWoundedWithWeapon(Unit *enemyUnit, const Weapon *weapon,
                                                           const Wounds &weaponDamage) {
        KhorneBase::onEnemyModelWoundedWithWeapon(enemyUnit, weapon, weaponDamage);

        if (weapon->name() == m_axeOfKhorne.name()) {
            // Reality-splitting Axe
            if (Dice::RollD6() >= 5) {
                PLOG_INFO << name() << " slays wounded model from enemy unit " << enemyUnit->name() << " with " <<
                    weapon->name() << " Reality-splitting Axe ability.";
                enemyUnit->slay(1);
            }
        }
    }

} // namespace Khorne