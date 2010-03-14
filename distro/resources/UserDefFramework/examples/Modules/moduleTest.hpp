#ifndef MODULETEST_HPP
#define MODULETEST_HPP

#include <Common/Item/module.hpp>

namespace iqrcommon {

    class ClsModuleTest : public ClsModule
    {
    public:
	ClsModuleTest();

	void init();
	void update();
	void cleanup();

    private:
	ClsModuleTest(const ClsModuleTest&);

	/* input from group */
	StateVariablePtr* inputStateVariablePtr;

	/* output to group */
	ClsStateVariable* outputStateVariable;

    };
}

#endif
