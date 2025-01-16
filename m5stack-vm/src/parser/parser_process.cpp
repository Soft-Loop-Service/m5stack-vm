#include "./parser.hpp"
#include "./../debug.hpp"
#include "M5Core2.h"

namespace Parser
{
    void ParserSystem::recursionProcess(int scope_index)
    {
        std::vector<ByteCodeLine> current_bytecode = local_scope[scope_index].getByteCode();

        for (int i = 0; i < current_bytecode.size(); i++)
        {
            Bytecode::opcr opcode = current_bytecode[i].getOpecode();

            switch (opcode)
            {
            case Bytecode::Opecode::push:
            {

                Bytecode::opcr type = current_bytecode[i].getOperandType(1);
                String opd = current_bytecode[i].getOperand(2);

                LocalVariable new_local_variable(type);
                new_local_variable.setValueAnalysis(opd);

                stack_system->push(new_local_variable);

                break;
            }

            case Bytecode::Opecode::push_function:
            {
                break;
            }

            case Bytecode::Opecode::pop:
            {
                stack_system->pop();
                break;
            }

            case Bytecode::Opecode::s_invokevirtual:
            {
                LocalVariable local_variable = stack_system->pop();

                if (local_variable.getType() != Bytecode::Opecode::d_function)
                {
                    output_debug("Error: Type is not matched.");
                    break;
                }

                int function_index = local_variable.getValueInt();
                recursionProcess(function_index);

                break;
            }

            case Bytecode::Opecode::s_invokevirtual_constructor:
            {
                break;
            }

            case Bytecode::Opecode::s_instance:
            {
                break;
            }

            case Bytecode::Opecode::s_inside:
            {
                break;
            }

            case Bytecode::Opecode::s_store:
            {

                int s_index = current_bytecode[i].getOperandInt(2);

                if (s_index == -1)
                {
                    output_debug("Error: Store index is not found.");
                    break;
                }

                LocalVariable local_variable = stack_system->pop();

                local_scope[scope_index].setLocalVariable(s_index, local_variable);

                // LocalVariable local_variable = local_scope[scope_index].getLocalVariable(current_bytecode[i].getOperandType(0));
                // local_variable.setValueAnalysis(current_bytecode[i].getOperand(2));
                // local_scope[scope_index].setLocalVariable(local_variable);
                break;
            }

            case Bytecode::Opecode::s_load:
            {

                int s_index = current_bytecode[i].getOperandInt(2);

                if (s_index == -1)
                {
                    output_debug("Error: Load index is not found.");
                    break;
                }

                LocalVariable local_variable = local_scope[scope_index].getLocalVariable(s_index);

                stack_system->push(local_variable);

                break;
            }

            case Bytecode::Opecode::s_jump:
            {
                break;
            }

            case Bytecode::Opecode::s_label_point:
            {
                break;
            }

            case Bytecode::Opecode::c_add:
            {
                LocalVariable local_variable_1 = stack_system->pop();
                LocalVariable local_variable_2 = stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(a + b);
                    break;
                }
                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(a + b);
                    break;
                }

                case Bytecode::Opecode::d_str:
                {
                    String a = local_variable_1.getValueString();
                    String b = local_variable_2.getValueString();
                    new_local_variable.setValue(a + b);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(a || b);
                    break;
                }

                case Bytecode::Opecode::d_pointer:
                {
                    vint a = local_variable_1.getValueVInt();
                    vint b = local_variable_2.getValueVInt();
                    vint c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    break;
                }

                case Bytecode::Opecode::d_reference:
                {
                    vstring a = local_variable_1.getValueVString();
                    vstring b = local_variable_2.getValueVString();
                    vstring c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    break;
                }

                case Bytecode::Opecode::d_html_dom:
                {
                    String a = local_variable_1.getValueString();
                    String b = local_variable_2.getValueString();
                    new_local_variable.setValue(a + b);
                    break;
                }

                default:
                    output_debug("Error: Type is not matched.");
                    break;
                }
            }

            case Bytecode::Opecode::c_subtraction:
            {

                LocalVariable local_variable_1 = stack_system->pop();
                LocalVariable local_variable_2 = stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(a - b);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(a - b);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(a && b);
                    break;
                }

                case Bytecode::Opecode::d_pointer:
                {
                    vint a = local_variable_1.getValueVInt();
                    vint b = local_variable_2.getValueVInt();
                    vint c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    break;
                }

                case Bytecode::Opecode::d_reference:
                {
                    vstring a = local_variable_1.getValueVString();
                    vstring b = local_variable_2.getValueVString();
                    vstring c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    break;
                }

                default:
                    output_debug("Error: Type is not matched.");
                    break;
                }

                break;
            }

            case Bytecode::Opecode::c_multiplication:
            {

                LocalVariable local_variable_1 = stack_system->pop();

                LocalVariable local_variable_2 = stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(a * b);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(a * b);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(a && b);
                    break;
                }

                default:
                    output_debug("Error: Type is not matched.");
                    break;
                }

                break;
            }
            case Bytecode::Opecode::c_division:
            {

                LocalVariable local_variable_1 = stack_system->pop();
                LocalVariable local_variable_2 = stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(a / b);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(a / b);
                    break;
                }

                default:
                    output_debug("Error: Type is not matched.");
                    break;
                }

                break;
            }

            case Bytecode::Opecode::s_if_acmpeq:
            {

                break;
            }

            case Bytecode::Opecode::s_if_acmpne:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmpeq:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmpne:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmpge:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmpgt:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmple:
            {
                break;
            }

            case Bytecode::Opecode::s_if_icmplt:
            {
                break;
            }

            case Bytecode::Opecode::head_start_function:
            {
                break;
            }

            case Bytecode::Opecode::head_end_function:
            {
                break;
            }

            case Bytecode::Opecode::head_start_class:
            {
                break;
            }

            case Bytecode::Opecode::head_end_class:
            {
                break;
            }

            case Bytecode::Opecode::head_value_definition:
            {
                break;
            }

            case Bytecode::Opecode::head_start_global_scope:
            {
                break;
            }

            case Bytecode::Opecode::head_end_global_scope:
            {
                break;
            }

            case Bytecode::Opecode::program_start:
            {
                break;
            }

            case Bytecode::Opecode::program_end:
            {
                break;
            }

            default:
                break;
            }
        }
    }

    void ParserSystem::process()
    {
        output_debug("Process is started.");

        recursionProcess(0);

        output_debug("Process is ended.");
    }
};