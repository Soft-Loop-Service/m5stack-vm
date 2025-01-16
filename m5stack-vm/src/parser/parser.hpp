
#ifndef __PARSER
#define __PARSER

#include "./../bytecode_definition.hpp"
#include <sstream>
// フラットパーサー

namespace Parser
{
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
        Store() = default;

        int setValue(int id, const T &value)
        {
            store[id] = value;
            return id;
        }

        int setValue(const T &value)
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
    class StackSystem // スタックシステム
    {
        std::vector<LocalVariable> stack;

    public:
        StackSystem() = default;
        void push(LocalVariable local_variable);
        LocalVariable pop();
        LocalVariable getTop();
        void clear();
        int size();
        std::vector<LocalVariable> getStack();
    };

    class LocalVariable
    {
    public:
        enum ValueType
        {
            INT,
            FLOAT,
            BOOLEAN,
            STRING,
            VINT,
            VSTRING
        } store_type;
        union
        {
            int intValue;
            float floatValue;
            bool booleanValue;
            String *stringValue;
            vint *vectorIntValue;
            vstring *vectorStringValue;
        };

        std::map<Bytecode::opcr, ValueType> store_type_map = {
            {Bytecode::Opecode::d_int, INT},
            {Bytecode::Opecode::d_float, FLOAT},
            {Bytecode::Opecode::d_str, STRING},
            {Bytecode::Opecode::d_pointer, VINT},
            {Bytecode::Opecode::d_reference, VINT},
            {Bytecode::Opecode::d_html_dom, STRING},
            {Bytecode::Opecode::d_function, INT},
            {Bytecode::Opecode::d_class, INT},
            {Bytecode::Opecode::d_boolean, BOOLEAN}};

        Bytecode::opcr type;

        LocalVariable() : store_type(INT), intValue(0), type(0) {};
        LocalVariable(Bytecode::opcr);
        ~LocalVariable() { clear(); }

        void setValueAnalysis(vstring args)
        {
            clear();
            switch (store_type_map[type])
            {
            case INT:
            {
                vectorIntValue = new vint();

                for (int i = 0; i < args.size(); i++)
                {
                    vectorIntValue->push_back(args[i].toInt());
                }

                break;
            }

            case FLOAT:
            {
                vectorIntValue = new vint();

                for (int i = 0; i < args.size(); i++)
                {
                    vectorIntValue->push_back(args[i].toFloat());
                }

                break;
            }

            case STRING:
            {
                vectorStringValue = new vstring();

                for (int i = 0; i < args.size(); i++)
                {
                    vectorStringValue->push_back(args[i]);
                }

                break;
            }

            default:
                break;
            }
        }
        void setValueAnalysis(String arg)
        {
            clear();
            switch (store_type_map[type])
            {
            case INT:
                intValue = arg.toInt();
                break;
            case FLOAT:
                floatValue = arg.toFloat();
                break;
            case BOOLEAN:
                booleanValue = arg == "true";
                break;
            case STRING:
                stringValue = new String(arg);
                break;
            }
        }

        void setValue(int value)
        {
            clear();
            type = INT;
            intValue = value;
        }

        void setValue(float value)
        {
            clear();
            type = FLOAT;
            floatValue = value;
        }

        void setValue(bool value)
        {
            clear();
            type = BOOLEAN;
            booleanValue = value;
        }

        void setValue(const String value)
        {
            clear();
            type = STRING;
            stringValue = new String(value);
        }

        void setValue(const vint value)
        {
            clear();
            type = VINT;
            vectorIntValue = new vint(value);
        }

        void setValue(const vstring value)
        {
            clear();
            type = VSTRING;
            vectorStringValue = new vstring(value);
        }

        int getValueInt() const
        {
            return intValue;
        }

        float getValueFloat() const
        {
            return floatValue;
        }

        bool getValueBoolean() const
        {
            return booleanValue;
        }

        String getValueString() const
        {
            return *stringValue;
        }

        vint getValueVInt() const
        {
            return *vectorIntValue;
        }

        vstring getValueVString() const
        {
            return *vectorStringValue;
        }

        Bytecode::opcr getType() const
        {
            return type;
        }

        String getCastString() const
        {
            switch (store_type)
            {
            case INT:
                return String(intValue);
            case FLOAT:
                return String(floatValue);
            case BOOLEAN:
                return booleanValue ? "true" : "false";
            case STRING:
                return *stringValue;
            case VINT:
            {
                String rv;
                for (int i = 0; i < vectorIntValue->size(); i++)
                {
                    rv += String((*vectorIntValue)[i]);
                    if (i != vectorIntValue->size() - 1)
                        rv += ",";
                }
                return rv;
            }
            case VSTRING:
            {
                String rv;
                for (int i = 0; i < vectorStringValue->size(); i++)
                {
                    rv += (*vectorStringValue)[i];
                    if (i != vectorStringValue->size() - 1)
                        rv += ",";
                }
                return rv;
            }
            default:
                return "unknown";
            }
        }

        void clear()
        {
            if (type == STRING)
                delete stringValue;
            if (type == VINT)
                delete vectorIntValue;
            if (type == VSTRING)
                delete vectorStringValue;
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

    class LocalScope // ローカルスコープ 木構造
    {
        std::vector<ByteCodeLine> byte_code;
        vint children;
        std::map<int, LocalVariable> local_variable;
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

        std::vector<ByteCodeLine> getByteCode();
        vint getChildren();
        std::map<int, LocalVariable> getLocalVariableList();
        LocalVariable LocalScope::getLocalVariable(int);

        void setLocalVariableList(std::map<int, LocalVariable>);
        void setLocalVariable(int, LocalVariable);

        int getIndex();
        int getScopeType();
        int getDirectlyIndex();
        int getParentIndex();
    };

    class ParserSystem
    {
    private:
        SourceCode received_data;

        std::map<int, LocalScope> local_scope;
        StackSystem *stack_system;

    public:
        ParserSystem();
        ParserSystem(SourceCode rd);
        ~ParserSystem();

        void refresh(SourceCode rd);

        int getProgramInt(int line, int column);
        String getProgram(int line, int column);

        bool hasProgram(int line, int column);
        Bytecode::opcr getProgramOpecode(int line, int column);

        void parser();

        void recursionProcess(int);
        void process();

        void all_output_local_scope();
        void all_output_stack_system();
    };
};

#endif