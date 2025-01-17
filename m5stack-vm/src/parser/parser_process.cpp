#include "./parser.hpp"
#include "./../debug.hpp"
#include "M5Core2.h"
#include <stdexcept>

namespace Parser
{
    void ParserSystem::recursionProcess()
    {

        CallStackScope *p_current_call_stack = call_stack_system->getStackTopReferece();

        output_debug("Recursion Process", {
                                              p_current_call_stack->getIndex(),
                                              p_current_call_stack->getReturnPoint(),
                                              (int)local_scope[p_current_call_stack->getIndex()].getByteCode().size(),
                                          });

        std::vector<ByteCodeLine> current_bytecode = local_scope[p_current_call_stack->getIndex()].getByteCode();

        // label ID -> bytecode index
        std::map<int, int> label_map = {};

        for (int i = 0; i < current_bytecode.size(); i++)
        {
            if (current_bytecode[i].getOpecode() == Bytecode::Opecode::s_label_point)
            {
                label_map[current_bytecode[i].getOperandInt(1)] = i;
            }
        }

        for (int i = p_current_call_stack->getReturnPoint(); i < current_bytecode.size(); i++)
        {
            if (permission_proceed == false)
            {
                return;
            }

            Bytecode::opcr opcode = current_bytecode[i].getOpecode();

            output_debug("Opcode", {opcode});

            switch (opcode)
            {
            case Bytecode::Opecode::push:
            {

                Bytecode::opcr type = current_bytecode[i].getOperandType(1);
                output_debug("PUSH | ", {type});

                String opd = current_bytecode[i].getOperand(2);
                output_debug("PUSH | ", {opd});

                LocalVariable new_local_variable(type);

                output_debug("PUSH | ", {"NEW VALUE"});

                new_local_variable.setValueAnalysis(opd);

                output_debug("PUSH | ", {"SET VALUE"});

                opecode_stack_system->push(new_local_variable);
                output_debug("PUSH | ", {"PUSH VALUE"});
                break;
            }

            case Bytecode::Opecode::push_function:
            {
                break;
            }

            case Bytecode::Opecode::pop:
            {
                opecode_stack_system->pop();
                break;
            }

            case Bytecode::Opecode::s_invokevirtual:
            {
                LocalVariable local_variable = opecode_stack_system->pop();

                if (local_variable.getType() != Bytecode::Opecode::d_function)
                {
                    output_debug({"ERROR:", String(local_variable.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                int function_index = local_variable.getValueInt();

                // recursionProcess();

                p_current_call_stack->setReturnPoint(i + 1);
                call_stack_system->push(CallStackScope(local_scope[function_index], 0));

                return;
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
                    output_debug({"ERROR:", "2", String(current_bytecode[i].getOperandListSize())});
                    throw std::runtime_error("Error: Store index is not found.");
                    break;
                }

                LocalVariable local_variable = opecode_stack_system->pop();

                p_current_call_stack->setLocalVariable(s_index, local_variable);

                // LocalVariable local_variable = local_scope[scope_index].getLocalVariable(current_bytecode[i].getOperandType(0));
                // local_variable.setValueAnalysis(current_bytecode[i].getOperand(2));
                // local_scope[scope_index].setLocalVariable(local_variable);
                break;
            }

            case Bytecode::Opecode::s_load:
            {

                int d_index = current_bytecode[i].getOperandInt(2);

                if (d_index == -1)
                {
                    output_debug({"ERROR:", "2", String(current_bytecode[i].getOperandListSize())});
                    throw std::runtime_error("Error: Load index is not found.");
                    break;
                }

                LocalVariable local_variable = searchLocalVariableInCallStack(call_stack_system->size() - 1, d_index);

                opecode_stack_system->push(local_variable);

                break;
            }

            case Bytecode::Opecode::s_jump:
            {
                int label_id = current_bytecode[i].getOperandInt(1);
                i = label_map[label_id];
                break;
            }

            case Bytecode::Opecode::s_label_point:
            {
                break;
            }

            case Bytecode::Opecode::c_add:
            {
                output_debug("ADD | ", {"ADD VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(b + a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }
                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(b + a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_str:
                {
                    String a = local_variable_1.getValueString();
                    String b = local_variable_2.getValueString();
                    new_local_variable.setValue(b + a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(b || a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_pointer:
                {
                    vint a = local_variable_1.getValueVInt();
                    vint b = local_variable_2.getValueVInt();
                    vint c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_reference:
                {
                    vstring a = local_variable_1.getValueVString();
                    vstring b = local_variable_2.getValueVString();
                    vstring c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_html_dom:
                {
                    String a = local_variable_1.getValueString();
                    String b = local_variable_2.getValueString();
                    new_local_variable.setValue(b + a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                default:

                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }
                break;
            }

            case Bytecode::Opecode::c_subtraction:
            {
                output_debug("SUB | ", {"SUB VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});

                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {

                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(b - a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(b - a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(b && a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_pointer:
                {
                    vint a = local_variable_1.getValueVInt();
                    vint b = local_variable_2.getValueVInt();
                    vint c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_reference:
                {
                    vstring a = local_variable_1.getValueVString();
                    vstring b = local_variable_2.getValueVString();
                    vstring c = a;
                    c.insert(c.end(), b.begin(), b.end());
                    new_local_variable.setValue(c);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                break;
            }

            case Bytecode::Opecode::c_multiplication:
            {
                output_debug("MUL | ", {"MUL VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(b * a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(b * a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    new_local_variable.setValue(b && a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                break;
            }
            case Bytecode::Opecode::c_division:
            {
                output_debug("DIV | ", {"DIV VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                LocalVariable new_local_variable(local_variable_1.getType());

                switch (local_variable_1.getType())
                {
                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    new_local_variable.setValue(b / a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    new_local_variable.setValue(b / a);
                    opecode_stack_system->push(new_local_variable);
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
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
                output_debug("IF ICMPEQ | ", {"ICMPEQ VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b == a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b == a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b == a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

                break;
            }

            case Bytecode::Opecode::s_if_icmpne:
            {
                output_debug("IF ICMPNE | ", {"ICMPNE VALUE"});

                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b != a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b != a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b != a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

                break;
            }

            case Bytecode::Opecode::s_if_icmpge:
            {
                output_debug("IF ICMPGE | ", {"ICMPGE VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b >= a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b >= a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b >= a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

                break;
            }

            case Bytecode::Opecode::s_if_icmpgt:
            {
                output_debug("IF ICMPGT | ", {"ICMPGT VALUE"});

                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b > a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b > a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b > a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

                break;
            }

            case Bytecode::Opecode::s_if_icmple:
            {
                output_debug("IF ICMPLE | ", {"ICMPLE VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b <= a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b <= a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b <= a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

                break;
            }

            case Bytecode::Opecode::s_if_icmplt:
            {
                output_debug("IF ICMPLT | ", {"ICMPLT VALUE"});

                LocalVariable local_variable_1 = opecode_stack_system->pop();
                LocalVariable local_variable_2 = opecode_stack_system->pop();

                if (local_variable_1.getType() != local_variable_2.getType())
                {
                    output_debug({"ERROR:", String(local_variable_1.getType()), String(local_variable_2.getType())});
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                bool result = false;

                switch (local_variable_1.getType())
                {

                case Bytecode::Opecode::d_int:
                {
                    int a = local_variable_1.getValueInt();
                    int b = local_variable_2.getValueInt();
                    result = b < a;
                    break;
                }

                case Bytecode::Opecode::d_float:
                {
                    float a = local_variable_1.getValueFloat();
                    float b = local_variable_2.getValueFloat();
                    result = b < a;
                    break;
                }

                case Bytecode::Opecode::d_boolean:
                {
                    bool a = local_variable_1.getValueBoolean();
                    bool b = local_variable_2.getValueBoolean();
                    result = b < a;
                    break;
                }

                default:
                    throw std::runtime_error("Error: Type is not matched.");
                    break;
                }

                if (!result)
                {
                    int label_id = current_bytecode[i].getOperandInt(1);
                    i = label_map[label_id];
                }

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

        call_stack_system->pop();

        // all_output_local_scope();
        all_output_opecode_stack_system();
        all_output_call_stack_system();
    }

    void ParserSystem::process()
    {
        output_debug("Process is started.");

        call_stack_system->push(CallStackScope(local_scope[0], 0));
        while (call_stack_system->has())
        {
            if (permission_proceed == false)
            {
                stop(true);
                return;
            }

            recursionProcess();
        }

        output_debug("Process is ended.");
    }
};