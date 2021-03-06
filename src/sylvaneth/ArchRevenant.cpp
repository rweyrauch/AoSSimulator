/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/ArchRevenant.h>
#include <UnitFactory.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool ArchRevenant::s_registered = false;

    ArchRevenant::ArchRevenant(Glade glade, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SylvanethBase("Arch-Revenant", 12, g_wounds, 8, 4, true, g_pointsPerUnit),
            m_glaive(Weapon::Type::Melee, "Revenant's Glaive", 2, 3, 3, 3, -2, 2),
            m_tailPincers(Weapon::Type::Melee, "Zephyrspite's Tail Pincers", 1, 1, 4, 3, 0, RAND_D3) {
        m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, HERO, ARCH_REVENANT};
        m_weapons = {&m_glaive, &m_tailPincers};
        m_battleFieldRole = Role::Leader;

        setGlade(glade);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalToHitReroll.connect(this, &ArchRevenant::championOfKurnothToHitRerolls, &m_championsSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_tailPincers);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Call to Battle", 12, 9, GamePhase::Combat,
                                                             Attribute::Attacks_Melee, 1,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>{SYLVANETH}));

        m_points = g_pointsPerUnit;
    }

    ArchRevenant::~ArchRevenant() {
        m_championsSlot.disconnect();
    }

    Unit *ArchRevenant::Create(const ParameterList &parameters) {
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_aspectsOfWar[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new ArchRevenant(glade, trait, artefact, general);
    }

    void ArchRevenant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArchRevenant::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    ArchRevenant::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_aspectsOfWar[0], g_aspectsOfWar),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Arch-Revenant", factoryMethod);
        }
    }

    Rerolls ArchRevenant::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Crescent Shield
        if (!m_crescentShieldProtection) {
            return Rerolls::Ones;
        }
        return SylvanethBase::toHitRerolls(weapon, unit);
    }

    Rerolls ArchRevenant::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Crescent Shield
        if (m_crescentShieldProtection) {
            return Rerolls::Ones;
        }

        return SylvanethBase::toSaveRerolls(weapon, attacker);
    }

    int ArchRevenant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Rerolls
    ArchRevenant::championOfKurnothToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(KURNOTH_HUNTERS) && (distanceTo(attacker) <= 12.0))
            return Rerolls::Ones;
        return Rerolls::None;
    }

} // namespace Sylvaneth
