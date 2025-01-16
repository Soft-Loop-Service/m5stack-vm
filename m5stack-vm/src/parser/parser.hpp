
#ifndef __PARSER
#define __PARSER

#include "./../bytecode_definition.hpp"
#include <sstream>

// フラットパーサー

namespace Parser
{
    Bytecode::opcr getDec(std::string hexStr);
    class StackSystem // スタックシステム
    {
    public:
        Bytecode::opcr opecode;
        vstring operand_list;
        StackSystem();
        StackSystem::StackSystem(Bytecode::opcr opecode, vstring operand_list);
        StackSystem::StackSystem(vstring operand_list);
    };

    class LocalVariable // ローカル変数
    {
    public:
        Bytecode::opcr type;
        int variable_unique_id;
        LocalVariable();
        LocalVariable(Bytecode::opcr type, int namevariable_unique_id);
    };

    class ByteCodeLine
    {
        Bytecode::opcr opecode;
        vstring operand_list;

    public:
        ByteCodeLine();
        ByteCodeLine(Bytecode::opcr opecode, vstring operand_list);
        ByteCodeLine(vstring operand_list);

        Bytecode::opcr getOpecode();
        vstring getOperandList();
        String getOperand(int index);
    };

    class LocalScope // ローカルスコープ 木構造
    {
        std::vector<ByteCodeLine> byte_code;
        vint children;
        std::vector<LocalVariable> local_variable;
        int index;
        int scope_type;
        int directly_index;
        int parent_index;

    public:
        LocalScope();
        LocalScope(int index, int scope_type, int directly_index, int parent_index);
        void addChildren(int index);
        void addLocalVariable(Bytecode::opcr type, int variable_unique_id);
        void addByteCode(ByteCodeLine byte_code_line);
    };

    class ParserSystem
    {
    private:
        SourceCode received_data;

        std::map<int, LocalScope> local_scope;
        std::vector<StackSystem> stack_system;

    public:
        ParserSystem();
        ParserSystem(SourceCode rd);
        ~ParserSystem();
        int getProgramInt(int line, int column);
        String getProgram(int line, int column);
        Bytecode::opcr getProgramOpecode(int line, int column);
        void parser();
    };
};

#endif