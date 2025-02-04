#include "./parser.hpp"
#include "./../debug.hpp"
#include "./../built_in/built_function.hpp"
#include <M5Core2.h>
#include <stdexcept>
#include "./local_variable.hpp"

namespace Parser
{
    int ParserSystem::resolveDirectlyIndexToLocalScope(int directly_index)
    {
        // local_scope iterator

        for (auto itr = local_scope.begin(); itr != local_scope.end(); ++itr)
        {
            if (itr->second.getDirectlyIndex() == directly_index)
            {
                output_debug("Resolve Directly Index to Local Scope", {directly_index, itr->first});

                return itr->first;
            }
        }

        throw std::runtime_error("Error: Directly index is not found.");

        // for (int i = 0; i < local_scope.size(); i++)
        // {
        //     if (local_scope[i].getDirectlyIndex() == local_stack_index)
        //     {

        //     }
        // }
    }

    void ParserSystem::recursionProcess()
    {

        CallStackScope *p_current_call_stack = call_stack_system->getStackTopReferece();

        output_debug("Recursion Process", {
                                              p_current_call_stack->getIndex(),
                                              p_current_call_stack->getReturnPoint(),
                                              (int)(local_scope[p_current_call_stack->getIndex()].getByteCode().size()),
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
            output_debug_memory();

            Bytecode::opcr opcode = current_bytecode[i].getOpecode();

            output_debug("Opcode", {i, opcode});

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
                int function_scope_index = resolveDirectlyIndexToLocalScope(function_index);

                // recursionProcess();

                p_current_call_stack->setReturnPoint(i + 1);
                call_stack_system->push(CallStackScope(local_scope[function_scope_index], 0));

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

            case Bytecode::Opecode::s_special_invokevirtual:
            {
                int function_id = current_bytecode[i].getOperand(1).toInt();

                if (function_id == 1)
                {
                    LocalVariable local_variable = opecode_stack_system->pop();
                    BuiltIn::BuiltInFunction::built_print(local_variable.getCastString());
                }

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

            case Bytecode::Opecode::h_dom_start:
            {
                output_debug("H_DOM_START | ", {"H_DOM_START VALUE"});

                String tag = current_bytecode[i].getOperand(1);
                int attribute_count = current_bytecode[i].getOperandInt(2);

                DOM_Tree::DomNode new_node(tag);

                for (int ac = 0; ac < attribute_count; ac++)
                {
                    LocalVariable name = opecode_stack_system->pop();
                    LocalVariable json = opecode_stack_system->pop();

                    new_node.setAttribute(name.getValueString(), json);
                }

                new_node.setOpecodeStackLen(opecode_stack_system->size());

                int dom_index = dom_system->addDomNodeTree(new_node);
                break;
            }
            case Bytecode::Opecode::h_dom_end:
            {
                output_message("H_DOM_END | ", {"H_DOM_END VALUE"});

                int opecode_stack_current_size = opecode_stack_system->size();
                int opecode_stack_dom_size = dom_system->getTopSelectNode().getOpecodeStackLen(); // こっちのほうが少ないはず

                std::vector<LocalVariable> local_variable_list = {};

                output_message("H_DOM_END", {opecode_stack_current_size, opecode_stack_dom_size, opecode_stack_current_size - opecode_stack_dom_size});

                // コールスタックをoutput_message

                for (int osc_i = 0; osc_i < opecode_stack_current_size; osc_i++)
                {
                    LocalVariable lvp = opecode_stack_system->getVariable(osc_i);
                    output_message("CALL STACK : H_DOM_END | ", {lvp.getCastString()});
                }

                output_message("H_DOM_END-2", {"H_DOM_END VALUE"});

                for (int osc_i = 0; osc_i < opecode_stack_current_size - opecode_stack_dom_size; osc_i++)
                {
                    local_variable_list.push_back(opecode_stack_system->pop());
                }

                output_message("H_DOM_END-3", {"H_DOM_END VALUE"});

                for (int osc_i = local_variable_list.size() - 1; osc_i >= 0; osc_i--)
                {
                    int dom_index = dom_system->addDomNodeTree(DOM_Tree::DomNode(local_variable_list[osc_i]));

                    output_message("H_DOM_END-3-1", {local_variable_list[osc_i].getCastString()});
                    dom_system->getTopSelectNode().addChild(dom_index);
                    output_message("H_DOM_END-3-2", {local_variable_list[osc_i].getCastString()});
                }

                output_message("H_DOM_END-4", {"H_DOM_END VALUE"});

                dom_system->climbNode();
                break;
            }

            case Bytecode::Opecode::h_to_html:
            {
                output_debug("H_TO_HTML | ", {"H_TO_HTML VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop(); // topから 1 pair

                dom_system->setTopDomNodeTextContent(local_variable_1.getValueString());
                break;
            }

            case Bytecode::Opecode::j_construction:
            {
                output_debug("J_CONST | ", {"J_CONST VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop(); // key
                LocalVariable local_variable_2 = opecode_stack_system->pop(); // value

                LocalVariable new_local_variable(Bytecode::Opecode::d_pair);

                new_local_variable.setValue(std::make_pair(local_variable_1.getValueString(), local_variable_2));

                opecode_stack_system->push(new_local_variable);
                break;
            }
            case Bytecode::Opecode::j_series:
            {
                output_debug("J_SERIES | ", {"J_SERIES VALUE"});
                LocalVariable local_variable_1 = opecode_stack_system->pop(); // topから 1 pair
                LocalVariable local_variable_2 = opecode_stack_system->pop(); // topから 2 pair | map

                output_debug("J_SERIES | ", {local_variable_1.getType(), local_variable_2.getType(), local_variable_2.isMap()});

                if (local_variable_2.isMap())
                {
                    local_variable_2.addMapValue(local_variable_1.getPairValue());
                    opecode_stack_system->push(local_variable_2);
                    break;
                }

                LocalVariable new_local_variable(Bytecode::Opecode::d_json);
                new_local_variable.setMapByPair(local_variable_1.getPairValue());
                new_local_variable.addMapValue(local_variable_2.getPairValue());
                opecode_stack_system->push(new_local_variable);

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
            {
                break;
            }
            }
            all_output_opecode_stack_system();
        }

        call_stack_system->pop();

        // all_output_local_scope();
        all_output_opecode_stack_system();
        all_output_call_stack_system();
        all_output_dom_system();
    }

    void ParserSystem::process()
    {
        output_message("Process is started.");

        call_stack_system->push(CallStackScope(local_scope[0], 0));
        dom_system = new DOM_Tree::DomSystem();

        while (call_stack_system->has())
        {
            recursionProcess();
        }

        dom_renderer = new DOM_Renderer::Renderer(dom_system->getDomTree());
        dom_renderer->rendering();

        output_message("Process is ended.");
    }
};