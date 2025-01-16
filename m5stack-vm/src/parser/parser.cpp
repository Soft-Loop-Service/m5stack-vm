#include "./parser.hpp"
#include "./../debug.hpp"
#include "M5Core2.h"
namespace Parser
{

    Bytecode::opcr getDec(String hexStr)
    {
        unsigned int decimalValue;
        decimalValue = hexStr.toInt();
        return decimalValue;

        std::stringstream ss;
        ss << std::hex << hexStr; // 16進数モードで文字列を設定
        ss >> decimalValue;       // unsigned intに変換
        return decimalValue;      // unsigned intに変換
    };

    LocalVariable::LocalVariable(Bytecode::opcr type, int variable_unique_id)
    {
        this->type = type;
        this->variable_unique_id = variable_unique_id;
        this->store_type = store_type_map[type];
    }

    LocalScope::LocalScope()
    {
        children = {};
        local_variable = {};
    }

    LocalScope::LocalScope(int index, int scope_type, int directly_index, int parent_index)
    {
        children = {};
        local_variable = {};
        this->index = index;
        this->scope_type = scope_type;
        this->directly_index = directly_index;
        this->parent_index = parent_index;
    }

    void LocalScope::addChildren(int index)
    {
        children.push_back(index);
    }

    void LocalScope::addLocalVariable(Bytecode::opcr type, int variable_unique_id)
    {
        local_variable.push_back(LocalVariable(type, variable_unique_id));
    }

    void LocalScope::addByteCode(ByteCodeLine byte_code_line)
    {
        byte_code.push_back(byte_code_line);
    }
    std::vector<ByteCodeLine> LocalScope::getByteCode()
    {
        return byte_code;
    }
    vint LocalScope::getChildren()
    {
        return children;
    }
    std::vector<LocalVariable> LocalScope::getLocalVariable()
    {
        return local_variable;
    }
    int LocalScope::getIndex()
    {
        return index;
    }
    int LocalScope::getScopeType()
    {
        return scope_type;
    }
    int LocalScope::getDirectlyIndex()
    {
        return directly_index;
    }
    int LocalScope::getParentIndex()
    {
        return parent_index;
    }

    void StackSystem::push(LocalVariable local_variable)
    {
        stack_system.push_back(local_variable);
    }

    LocalVariable StackSystem::pop()
    {
        LocalVariable local_variable = stack_system.back();
        stack_system.pop_back();
        return local_variable;
    }

    LocalVariable StackSystem::getTop()
    {
        return stack_system.back();
    }

    void StackSystem::clear()
    {
        stack_system.clear();
    }

    int StackSystem::size()
    {
        return stack_system.size();
    }

    std::vector<LocalVariable> StackSystem::getStack()
    {
        return stack_system;
    }

    ByteCodeLine::ByteCodeLine()
    {
        this->opecode = 0;
        this->operand_list = {};
    }
    ByteCodeLine::ByteCodeLine(Bytecode::opcr opecode, vstring operand_list)
    {
        this->opecode = opecode;
        this->operand_list = operand_list;
    }

    ByteCodeLine::ByteCodeLine(vstring operand_list)
    {
        if (operand_list.size() > 0)
        {
            this->opecode = getDec(operand_list[0]);

            if (operand_list.size() > 1)
            {
                this->operand_list = {};
                for (int i = 1; i < operand_list.size(); i++)
                {
                    this->operand_list.push_back(operand_list[i]);
                }
            }
        }
    }

    Bytecode::opcr ByteCodeLine::getOpecode()
    {
        return opecode;
    }

    int ByteCodeLine::getOperandListSize()
    {
        return operand_list.size();
    }

    vstring ByteCodeLine::getOperandList()
    {
        return operand_list;
    }

    String ByteCodeLine::getOperand(int index)
    {
        if (index >= operand_list.size())
        {
            return String("");
        }
        return operand_list[index];
    }

    ParserSystem::ParserSystem()
    {
        received_data = {};
        local_scope = {};
        stack_system = new StackSystem();
    }
    ParserSystem::ParserSystem(SourceCode rd)
    {
        received_data = rd;
        local_scope = {};
        stack_system = new StackSystem();
    }
    ParserSystem::~ParserSystem()
    {
    }

    void ParserSystem::refresh(SourceCode rd)
    {
        received_data = rd;
        local_scope = {};
        stack_system = {};
    }

    bool ParserSystem::hasProgram(int line, int column)
    {
        if (line >= received_data.size())
        {
            // output_debug("Error: Line is out of range.");
            return false;
        }
        if (column >= received_data[line].size())
        {
            // output_debug("Error: Column is out of range.");
            return false;
        }

        return true;
    }

    Bytecode::opcr ParserSystem::getProgramOpecode(int line, int column)
    {
        if (line >= received_data.size())
        {
            // output_debug("Error: Line is out of range.");
            return -1;
        }
        if (column >= received_data[line].size())
        {
            // output_debug("Error: Column is out of range.");
            return -1;
        }

        String program = received_data[line][column];
        return (unsigned int)program.toInt();
    }
    int ParserSystem::getProgramInt(int line, int column)
    {
        if (line >= received_data.size())
        {
            // output_debug("Error: Line is out of range.");
            return -1;
        }
        if (column >= received_data[line].size())
        {
            // output_debug("Error: Column is out of range.");
            return -1;
        }

        String program = received_data[line][column];
        return program.toInt();
    }
    String ParserSystem::getProgram(int line, int column)
    {
        if (line >= received_data.size())
        {
            // output_debug("Error: Line is out of range.");
            return String("");
        }
        if (column >= received_data[line].size())
        {
            // output_debug("Error: Column is out of range.");
            return String("");
        }

        String program = received_data[line][column];
        return program;
    }

    void ParserSystem::parser()
    {
        // シリアル通信により、すべて送られているか不安要素がある
        // ループ回数がある程度多くてもいいので、都度確認していく

        int start_flag_latest_line = -1;
        int end_flag_first_line = -1;

        // 不完全ではないか確認
        for (int i = 0; i < received_data.size(); i++)
        {
            if (!hasProgram(i, 0))
            {
                continue;
            }

            if (getProgram(i, 0) == String(Bytecode::Opecode::program_start))
            {
                start_flag_latest_line = i;
            }
            if (start_flag_latest_line >= 0 && getProgram(i, 0) == String(Bytecode::Opecode::program_end))
            {
                end_flag_first_line = i;
                break;
            }
        }

        if (!(start_flag_latest_line >= 0) || !(end_flag_first_line >= 0))
        {
            output_debug("Error: Program is not complete.");
            return;
        }
        else
        {
            output_debug("Program is complete.", {start_flag_latest_line, end_flag_first_line});
        }

        // ここから
        SourceCode received_data_copy = {};

        for (int i = 0; i < received_data.size(); i++)
        {
            if (start_flag_latest_line < i && i < end_flag_first_line)
            {
                if (!hasProgram(i, 0))
                {
                    continue;
                }

                String f = getProgram(i, 0);
                if (f[0] == '#' || f == "#")
                { // コメントアウト
                    continue;
                }

                received_data_copy.push_back(received_data[i]);
            }
        }

        received_data = received_data_copy;

        for (int i = 0; i < received_data.size(); i++)
        {

            if (getProgramOpecode(i, 0) == Bytecode::Opecode::head_value_definition)
            {
                int local_stack_index = getProgramInt(i, 1);
                int scope_type = getProgramInt(i, 2);
                int directly_index = getProgramInt(i, 3);
                int parent_index = getProgramInt(i, 4);
                local_scope[local_stack_index] = LocalScope(local_stack_index, scope_type, directly_index, parent_index);

                if (parent_index >= 0)
                {
                    local_scope[parent_index].addChildren(local_stack_index);
                }

                for (int j = 5; j < received_data[i].size(); j += 2)
                {
                    Bytecode::opcr type = getDec(getProgram(i, j));
                    int variable_unique_id = getProgramInt(i, j + 1);
                    local_scope[local_stack_index].addLocalVariable(type, variable_unique_id);
                }

                output_debug("Local scope", {i, local_stack_index, scope_type, directly_index, parent_index});
            }
        }

        output_debug("Local scope is created.", local_scope.size());

        int local_stack_index = 0;
        bool inside_flag = 0; // 0:外部, 1:関数内/クラス内, 2:グローバルスコープ
        // バイトコードの全移設
        for (int i = 0; i < received_data.size(); i++)
        {
            if (getProgramOpecode(i, 0) == Bytecode::Opecode::head_start_function ||
                getProgramOpecode(i, 0) == Bytecode::Opecode::head_start_class)
            {
                local_stack_index = getProgramInt(i, 1);
                inside_flag = 1;
                continue;
            }
            if (getProgramOpecode(i, 0) == Bytecode::Opecode::head_start_global_scope)
            {
                local_stack_index = 0;
                inside_flag = 2;
                continue;
            }

            if (getProgramOpecode(i, 0) == Bytecode::Opecode::head_end_function ||
                getProgramOpecode(i, 0) == Bytecode::Opecode::head_end_class ||
                getProgramOpecode(i, 0) == Bytecode::Opecode::head_end_global_scope)
            {
                local_stack_index = 0;
                inside_flag = 0;
                continue;
            }

            if (inside_flag > 0)
            {
                local_scope[local_stack_index].addByteCode(ByteCodeLine(received_data[i]));
            }
        }
        output_debug("Bytecode import is created.");

        // バイトコードの実行
        process();
    }

    void ParserSystem::all_output_local_scope()
    {
        output_debug("* * * * * * all output local scope * * * * * * ");
        for (auto itr = local_scope.begin(); itr != local_scope.end(); ++itr)
        {
            output_debug("-    Local Scope", {itr->first, itr->second.getIndex(), itr->second.getScopeType(), itr->second.getDirectlyIndex(), itr->second.getParentIndex()});
            for (int i = 0; i < itr->second.getLocalVariable().size(); i++)
            {
                output_debug("-        Local Variable VALUE : ", {itr->second.getLocalVariable()[i].type, itr->second.getLocalVariable()[i].getCastString()});
                output_debug("-        Local Variable ID___ : ", {itr->second.getLocalVariable()[i].type, itr->second.getLocalVariable()[i].variable_unique_id});
            }
            for (int i = 0; i < itr->second.getByteCode().size(); i++)
            {
                output_debug("-        ByteCode", {itr->second.getByteCode()[i].getOpecode()});
                for (int j = 0; j < itr->second.getByteCode()[i].getOperandList().size(); j++)
                {
                    output_debug({"-            Operand", itr->second.getByteCode()[i].getOperand(j)});
                }
            }
        }
    }

    void ParserSystem::all_output_stack_system()
    {
        output_debug("* * * * * * all output stack system * * * * * * ");

        for (int i = 0; i < stack_system->size(); i++)
        {
            output_debug("-    Stack System", {String(i), String(stack_system->getStack()[i].type), stack_system->getStack()[i].getCastString()});
        }
    }
};
