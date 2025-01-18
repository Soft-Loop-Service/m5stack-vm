#include "./parser.hpp"
#include "./../debug.hpp"
#include "./../built_in/built_function.hpp"
#include "M5Core2.h"
#include <stdexcept>

namespace Parser
{

    Bytecode::opcr getDec(String hexStr)
    {
        unsigned int decimalValue;
        canConvertToIntTry(hexStr);
        decimalValue = hexStr.toInt();
        return decimalValue;

        std::stringstream ss;
        ss << std::hex << hexStr; // 16進数モードで文字列を設定
        ss >> decimalValue;       // unsigned intに変換
        return decimalValue;      // unsigned intに変換
    };

    ScopeSystem::ScopeSystem()
    {
        children = {};
        local_variable = {};
        // byte_code = {};
        this->index = -1;
        this->scope_type = -1;
        this->directly_index = -1;
        this->parent_index = -1;
    }

    ScopeSystem::ScopeSystem(int index, int scope_type, int directly_index, int parent_index)
    {
        children = {};
        local_variable = {};
        // byte_code = {};
        this->index = index;
        this->scope_type = scope_type;
        this->directly_index = directly_index;
        this->parent_index = parent_index;
    }

    ScopeSystem::ScopeSystem(LocalScope ls)
    {
        children = ls.children;
        local_variable = ls.local_variable;
        // byte_code = ls.byte_code;
        this->index = ls.index;
        this->scope_type = ls.scope_type;
        this->directly_index = ls.directly_index;
        this->parent_index = ls.parent_index;
    }

    LocalScope::LocalScope()
    {

        children = {};
        local_variable = {};
        byte_code = {};
        this->index = -1;
        this->scope_type = -1;
        this->directly_index = -1;
        this->parent_index = -1;
    }

    LocalScope::LocalScope(int index, int scope_type, int directly_index, int parent_index)
    {
        children = {};
        local_variable = {};
        byte_code = {};
        this->index = index;
        this->scope_type = scope_type;
        this->directly_index = directly_index;
        this->parent_index = parent_index;
    }

    CallStackScope::CallStackScope()
    {
        children = {};
        local_variable = {};
        this->index = -1;
        this->scope_type = -1;
        this->directly_index = -1;
        this->parent_index = -1;
        this->return_point = 0;
    }

    CallStackScope::CallStackScope(int index, int scope_type, int directly_index, int parent_index)
    {
        children = {};
        local_variable = {};
        this->index = index;
        this->scope_type = scope_type;
        this->directly_index = directly_index;
        this->parent_index = parent_index;
        this->return_point = 0;
    }

    CallStackScope::CallStackScope(LocalScope ls, int return_point)
    {
        children = ls.getChildren();
        local_variable = ls.getLocalVariableList();
        this->index = ls.getIndex();
        this->scope_type = ls.getScopeType();
        this->directly_index = ls.getDirectlyIndex();
        this->parent_index = ls.getParentIndex();
        this->return_point = return_point;
    }

    void CallStackScope::setReturnPoint(int return_point)
    {
        this->return_point = return_point;
    }

    int CallStackScope::getReturnPoint()
    {
        return return_point;
    }

    void ScopeSystem::addChildren(int index)
    {
        children.push_back(index);
    }

    void ScopeSystem::addLocalVariable(Bytecode::opcr type, int directly_index)
    {
        local_variable[directly_index] = (LocalVariable(type));
    }

    void LocalScope::addByteCode(ByteCodeLine byte_code_line)
    {
        byte_code.push_back(byte_code_line);
    }
    std::vector<ByteCodeLine> LocalScope::getByteCode()
    {
        return byte_code;
    }
    vint ScopeSystem::getChildren()
    {
        return children;
    }
    std::map<int, LocalVariable> ScopeSystem::getLocalVariableList()
    {
        return local_variable;
    }
    LocalVariable ScopeSystem::getLocalVariable(int directly_index)
    {
        return local_variable[directly_index];
    }

    void ScopeSystem::setLocalVariableList(std::map<int, LocalVariable> lv)
    {
        local_variable = lv;
    }
    void ScopeSystem::setLocalVariable(int directly_index, LocalVariable lv)
    {
        local_variable[directly_index] = lv;
    }

    int ScopeSystem::getIndex()
    {
        return index;
    }
    int ScopeSystem::getScopeType()
    {
        return scope_type;
    }
    int ScopeSystem::getDirectlyIndex()
    {
        return directly_index;
    }
    int ScopeSystem::getParentIndex()
    {
        return parent_index;
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
            this->operand_list = operand_list;
        }
        else
        {
            throw std::runtime_error("Error: Operand list is empty.");
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
    Bytecode::opcr ByteCodeLine::getOperandType()
    {
        int index = 1;
        if (index >= operand_list.size())
        {
            throw std::runtime_error("Error: Operand index is out of range.");
        }
        canConvertToIntTry(operand_list[index]);
        return operand_list[index].toInt();
    }

    Bytecode::opcr ByteCodeLine::getOperandType(int index)
    {
        if (index >= operand_list.size())
        {
            throw std::runtime_error("Error: Operand index is out of range.");
        }
        canConvertToIntTry(operand_list[index]);
        return operand_list[index].toInt();
    }

    int ByteCodeLine::getOperandInt(int index)
    {
        if (index >= operand_list.size())
        {
            throw std::runtime_error("Error: Operand index is out of range.");
        }

        canConvertToIntTry(operand_list[index]);

        return operand_list[index].toInt();
    }
    String ByteCodeLine::getOperand(int index)
    {
        if (index >= operand_list.size())
        {
            throw std::runtime_error("Error: Operand index is out of range.");
        }
        return operand_list[index];
    }

    ParserSystem::ParserSystem()
    {
        received_data = {};
        local_scope = {};

        call_stack_system = new StackSystem<CallStackScope>();
        opecode_stack_system = new StackSystem<LocalVariable>();
        class_scope = {};

        dom_system = new DOM_Tree::DomSystem();

        permission_proceed = true;
    }
    ParserSystem::ParserSystem(SourceCode rd)
    {
        received_data = rd;
        local_scope = {};
        call_stack_system = new StackSystem<CallStackScope>();
        opecode_stack_system = new StackSystem<LocalVariable>();

        dom_system = new DOM_Tree::DomSystem();

        class_scope = {};
        permission_proceed = true;
    }
    ParserSystem::~ParserSystem()
    {
    }

    void ParserSystem::refresh(SourceCode rd)
    {
        received_data = rd;
        local_scope = {};
        opecode_stack_system = new StackSystem<LocalVariable>();
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

        canConvertToIntTry(program);

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
        canConvertToIntTry(program);
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
            std::runtime_error("Error: Program is not complete.");
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

    LocalVariable ParserSystem::searchLocalVariableInCallStack(int current_stack_index, int directly_index)
    {

        int search = current_stack_index;

        while (search >= 0)
        {
            std::map<int, LocalVariable> lvl = call_stack_system->getVariable(search).getLocalVariableList();

            if (lvl.find(directly_index) != lvl.end())
            {
                return lvl[directly_index];
            }

            search--;
        }

        throw std::runtime_error("Error: lv is not found. (call stack)");
    }

    LocalVariable ParserSystem::searchLocalVariableInLocalScope(int current_scope_index, int directly_index)
    {
        int search = current_scope_index;
        while (search >= 0)
        {
            std::map<int, Parser::LocalVariable> lvl = local_scope[search].getLocalVariableList();

            if (lvl.find(directly_index) != lvl.end())
            {
                return lvl[directly_index];
            }

            search = local_scope[search].getParentIndex();
        }

        throw std::runtime_error("Error: Local scope is not found.");
    }

    void ParserSystem::stop(bool p)
    {
        permission_proceed = p;
        // ここで強制停止処理を行う
    }

    void ParserSystem::all_output_local_scope()
    {
        output_debug("* * * * * * all output local scope * * * * * * ");
        for (auto itr = local_scope.begin(); itr != local_scope.end(); ++itr)
        {
            output_debug("-    Local Scope", {itr->first, itr->second.getIndex(), itr->second.getScopeType(), itr->second.getDirectlyIndex(), itr->second.getParentIndex()});
            // for (int i = 0; i < itr->second.getLocalVariable().size(); i++)
            // {
            //     output_debug("-        Local Variable VALUE : ", {itr->second.getLocalVariable()[i].type, itr->second.getLocalVariable()[i].getCastString()});
            //     output_debug("-        Local Variable ID___ : ", {itr->second.getLocalVariable()[i].type, itr->second.getLocalVariable()[i].directly_index});
            // }

            auto lv_list = itr->second.getLocalVariableList();

            for (auto itr2 = lv_list.begin(); itr2 != lv_list.end(); ++itr2)
            {
                output_debug("-        Local Variable VALUE : ", {String(itr2->first), String(itr2->second.getType()), itr2->second.getCastString()});
            }
            // output_debug("-        Local Variable VALUE : ", {String(c->first), String(itr2->second.type), itr2->second.getCastString()});

            output_debug("-        ByteCode(size)", {String(itr->second.getByteCode().size())});

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

    void ParserSystem::all_output_opecode_stack_system()
    {
        output_debug("* * * * * * all output stack system * * * * * * ");

        for (int i = 0; i < opecode_stack_system->size(); i++)
        {
            output_debug("-    Stack System", {String(i), String(opecode_stack_system->getStack()[i].opcr_type), opecode_stack_system->getStack()[i].getCastString()});
        }
    }

    void ParserSystem::all_output_call_stack_system()
    {
        output_debug("* * * * * * all output call stack system * * * * * * ");

        for (int i = 0; i < call_stack_system->size(); i++)
        {
            output_debug("-    Call Stack System", {String(i), String(call_stack_system->getStack()[i].getIndex()), String(call_stack_system->getStack()[i].getScopeType()), String(call_stack_system->getStack()[i].getDirectlyIndex()), String(call_stack_system->getStack()[i].getParentIndex())});

            auto lv_list = call_stack_system->getStack()[i].getLocalVariableList();

            for (auto itr2 = lv_list.begin(); itr2 != lv_list.end(); ++itr2)
            {
                output_debug("-        Local Variable VALUE : ", {String(itr2->first), String(itr2->second.getType()), itr2->second.getCastString()});
            }
        }
    }

    void ParserSystem::all_output_dom_system()
    {
        output_debug("* * * * * * all output dom system * * * * * * ");

        for (int i = 0; i < dom_system->domSize(); i++)
        {
            DOM_Tree::DomNode dn = dom_system->getDomNode(i);
            output_debug("-    Dom System", {String(i), dn.getTag(), dn.getTextContent(), String(dn.getOpecodeStackLen())});
            std::map<String, LocalVariable> attributes = dn.getAttributes();
            for (auto itr = attributes.begin(); itr != attributes.end(); ++itr)
            {
                output_debug("-        Attribute", {itr->first, itr->second.getCastString()});
            }
            vint children = dn.getChildren();
            for (int j = 0; j < children.size(); j++)
            {
                output_debug("-        Children", {String(children[j])});
            }
        }
    }

};
