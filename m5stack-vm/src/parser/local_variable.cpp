
#include "./local_variable.hpp"

namespace Parser
{
    LocalVariable::LocalVariable()
    {
        this->opcr_type = Bytecode::Opecode::d_int;
        this->store_type = store_type_map[Bytecode::Opecode::d_int];

        this->intValue = 0;
        this->floatValue = 0.0;
        this->booleanValue = false;
        this->stringValue = nullptr;
        this->vectorIntValue = nullptr;
        this->vectorStringValue = nullptr;
        this->vectorFloatValue = nullptr;
        this->mapValue = nullptr;
        this->pairValue = nullptr;
    }

    LocalVariable::LocalVariable(Bytecode::opcr type)
    {
        this->opcr_type = type;

        if (!hasStoreTypeMap(type))
        {
            output_debug(String(type));
            throw std::runtime_error("Error: Type is not matched. (constructor opcr)");
        }

        this->store_type = store_type_map[type];

        this->intValue = 0;
        this->floatValue = 0.0;
        this->booleanValue = false;
        this->stringValue = nullptr;
        this->vectorIntValue = nullptr;
        this->vectorStringValue = nullptr;
        this->vectorFloatValue = nullptr;
        this->mapValue = nullptr;
        this->pairValue = nullptr;
    }
}