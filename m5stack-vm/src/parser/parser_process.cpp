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
                break;
            }

            case Bytecode::Opecode::push_function:
            {
                break;
            }

            case Bytecode::Opecode::pop:
            {
                break;
            }

            case Bytecode::Opecode::s_invokevirtual:
            {
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
                break;
            }

            case Bytecode::Opecode::s_load:
            {
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
                break;
            }

            case Bytecode::Opecode::c_subtraction:
            {
                break;
            }

            case Bytecode::Opecode::c_multiplication:
            {
                break;
            }
            case Bytecode::Opecode::c_division:
            {
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