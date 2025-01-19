
#ifndef __PARSER
#define __PARSER

#include "./../bytecode_definition.hpp"
#include <sstream>
#include "./../debug.hpp"

#include "./renderer.hpp"
#include "./dom_tree.hpp"
#include <stdexcept>

// フラットパーサー

namespace Parser
{
    class LocalVariable;
    class LocalScope;
    class ScopeSystem;

    template <typename T>
    class Store
    {
        std::map<int, T> store;
        int unique_id = 0;

        int getUniqueId()
        {
            return ++unique_id;
        }

    public:
        Store()
        {
            store = {};
            unique_id = 0;
        }

        int setValue(int id, const T value)
        {
            store[id] = value;
            return id;
        }

        int setValue(const T value)
        {
            int id = getUniqueId();
            store[id] = value;
            return id;
        }

        T getValue(int id) const
        {
            return store.at(id);
        }

        void clear()
        {
            store.clear();
        }

        bool hasValue(int id) const
        {
            return store.find(id) != store.end();
        }
    };

    Bytecode::opcr getDec(std::string hexStr);

    template <typename T>
    class StackSystem // スタックシステム
    {
        std::vector<T> stack;

    public:
        StackSystem()
        {
            stack = {};
            output_debug("STACK | ", {"constructor", String(stack.size())});
        }

        void push(T local_variable)
        {
            output_debug("SS-PA | ", {"PUSH VALUE", String(stack.size())});
            stack.push_back(local_variable);
        }

        T pop()
        {
            if (stack.size() == 0)
            {
                throw std::runtime_error("Error: Stack is empty.");
            }
            else
            {
                output_debug("SS-PO | ", {"POP VALUE", String(stack.size())});
            }

            T local_variable = stack.back();
            stack.pop_back();
            return local_variable;
        }

        T getTop()
        {
            return stack.back();
        }

        void clear()
        {
            stack.clear();
        }

        int size()
        {
            return stack.size();
        }

        int has()
        {
            return stack.size() > 0;
        }

        std::vector<T> getStack()
        {
            return stack;
        }

        T getVariable(int v)
        {
            return stack[v];
        }

        T *getStackTopReferece()
        {
            return &stack.back();
        }
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
        int getOperandListSize();

        vstring getOperandList();

        Bytecode::opcr getOperandType();

        // index
        Bytecode::opcr getOperandType(int);
        int getOperandInt(int);

        // index
        String getOperand(int);
    };

    class ScopeSystem
    {
    protected:
        vint children;

        // key: directly_index
        std::map<int, LocalVariable> local_variable;

        int index;
        int scope_type;
        int directly_index;
        int parent_index;

    public:
        ScopeSystem();

        // int index, int scope_type, int directly_index, int parent_index
        ScopeSystem(int, int, int, int);
        ScopeSystem(LocalScope);

        // int index
        void addChildren(int);

        // opcr, int variable_unique_id
        void addLocalVariable(Bytecode::opcr, int);

        int getIndex();
        int getScopeType();
        int getDirectlyIndex();
        int getParentIndex();

        vint getChildren();
        std::map<int, LocalVariable> getLocalVariableList();
        LocalVariable getLocalVariable(int);

        void setLocalVariableList(std::map<int, LocalVariable>);
        void setLocalVariable(int, LocalVariable);
    };

    // ローカルスコープ 木構造
    class LocalScope : public ScopeSystem
    {
        std::vector<ByteCodeLine> byte_code;

    public:
        LocalScope();
        // int index, int scope_type, int directly_index, int parent_index
        LocalScope(int, int, int, int);

        // ByteCodeLine byte_code_line
        void addByteCode(ByteCodeLine);

        std::vector<ByteCodeLine> getByteCode();
    };

    class CallStackScope : public ScopeSystem
    {
        // CallStackがpopされ、戻ってきたときに、処理を開始する地点
        int return_point;

    public:
        CallStackScope();
        // int local_scope_index, int scope_type, int directly_index, int parent_index
        CallStackScope(int, int, int, int);

        CallStackScope(LocalScope, int);

        int getReturnPoint();
        void setReturnPoint(int);

        // 参照を取得
        CallStackScope *getReference()
        {
            return this;
        }
    };

    class ParserSystem
    {
    private:
        // token分割したフラットなバイトコード
        SourceCode received_data;

        // クラスや変数の上下関係・スコープを管理する 設計図
        std::map<int, LocalScope> local_scope;

        // クラス等をインスタンス化したものを管理。keyはinstance ID
        // CallStackScope内変数にはこのkeyを挿入する
        std::map<int, LocalScope> class_scope;

        // スタックマシン 演算機能を持つ
        StackSystem<LocalVariable> *opecode_stack_system;

        // コーススタックシステム ローカル変数はここで管理する
        StackSystem<CallStackScope> *call_stack_system;

        DOM_Tree::DomSystem *dom_system;

        DOM_Renderer::Renderer *dom_renderer;

    public:
        ParserSystem();
        ParserSystem(SourceCode rd);
        ~ParserSystem();

        void refresh(SourceCode rd);

        // int line, int column
        int getProgramInt(int, int);

        // int line, int column
        String getProgram(int, int);

        // int line, int column
        bool hasProgram(int, int);

        // int line, int column
        Bytecode::opcr getProgramOpecode(int, int);

        // callstack_systemの、現在地とそれより深い領域の変数を探す
        // int stack_index
        // int directly_index
        LocalVariable searchLocalVariableInCallStack(int, int);

        // current_scope_index directly_indexと、その上位を探す
        LocalVariable searchLocalVariableInLocalScope(int, int);

        void parser();

        void recursionProcess();
        void process();

        void all_output_local_scope();
        void all_output_opecode_stack_system();
        void all_output_call_stack_system();
        void all_output_dom_system();
    };
};

#endif