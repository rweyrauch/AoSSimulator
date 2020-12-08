/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MYARISPURIFIERS_H
#define MYARISPURIFIERS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class MyarisPurifiers : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MyarisPurifiers();

        ~MyarisPurifiers() override = default;

        bool configure();

    protected:

    private:

        Weapon m_bow,
            m_mallet,
            m_greatsword,
            m_dagger;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crushing Blow                    TODO
// Guardians                        TODO
// Sunmetal Weapons                 TODO
//

} // namespace LuminethRealmLords

#endif // MYARISPURIFIERS_H
