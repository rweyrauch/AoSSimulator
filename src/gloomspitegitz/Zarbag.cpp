/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Zarbag.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Zarbag::s_registered = false;

    Zarbag::Zarbag() :
            GloomspiteGitzBase("Zarbag", 5, g_wounds, 4, 6, false, g_pointsPerUnit),
            m_sickle(Weapon::Type::Melee, "Cursed Sickle", 2, 3, 3, 3, -1, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN, ZARBAG};
        m_weapons = {&m_sickle};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    void Zarbag::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sickle);

        //m_knownSpells.push_back(std::make_unique<FaceOfDaBadMoon>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Zarbag::Create(const ParameterList &parameters) {
        auto unit = new Zarbag();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheMoonclans[0]);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure(lore);
        return unit;
    }

    void Zarbag::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Zarbag::Create,
                    Zarbag::ValueToString,
                    Zarbag::EnumStringToInt,
                    Zarbag::ComputePoints,
                    {
                            EnumParameter("Lore of the Moonclans", g_loreOfTheMoonclans[0], g_loreOfTheMoonclans),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Zarbag", factoryMethod);
        }
    }

    std::string Zarbag::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Moonclans") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int Zarbag::EnumStringToInt(const std::string &enumString) {
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int Zarbag::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int Zarbag::castingModifier() const {
        auto mod = GloomspiteGitzBase::castingModifier();

        // Sniffer Spite
        if (!m_usedSnifferSpite) {
            mod += 2;
            m_usedSnifferSpite = true;
        }
        return mod;
    }

} // namespace GloomspiteGitz
