#ifndef LOCAL_VARIABLE_HPP
#define LOCAL_VARIABLE_HPP

#include "./../bytecode_definition.hpp"
#include "./../debug.hpp"

namespace Parser
{

    class LocalVariable
    {

    public:
        Bytecode::opcr opcr_type;

        enum ValueType
        {
            INT,
            FLOAT,
            BOOLEAN,
            STRING,
            VINT,
            VSTRING,
            VFLOAT,
            MAP,
            PAIR
        } store_type;
        union
        {
            int intValue;
            float floatValue;
            bool booleanValue;
            String *stringValue;
            vint *vectorIntValue;
            vstring *vectorStringValue;
            vfloat *vectorFloatValue;
            std::map<String, LocalVariable> *mapValue;
            std::pair<String, LocalVariable> *pairValue;
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
            {Bytecode::Opecode::d_boolean, BOOLEAN},
            {Bytecode::Opecode::d_json, MAP},
            {Bytecode::Opecode::d_pair, PAIR}};

        LocalVariable();
        LocalVariable(Bytecode::opcr);

        LocalVariable(const LocalVariable &other)
            : store_type(other.store_type), opcr_type(other.opcr_type)
        {
            switch (store_type)
            {
            case INT:
                intValue = other.intValue;
                break;
            case FLOAT:
                floatValue = other.floatValue;
                break;
            case BOOLEAN:
                booleanValue = other.booleanValue;
                break;
            case STRING:
                stringValue = other.stringValue ? new String(*other.stringValue) : nullptr;
                break;
            case VINT:
                vectorIntValue = other.vectorIntValue ? new vint(*other.vectorIntValue) : nullptr;
                break;
            case VSTRING:
                vectorStringValue = other.vectorStringValue ? new vstring(*other.vectorStringValue) : nullptr;
                break;
            case VFLOAT:
                vectorFloatValue = other.vectorFloatValue ? new vfloat(*other.vectorFloatValue) : nullptr;
                break;
            case MAP:
                mapValue = other.mapValue ? new std::map<String, LocalVariable>(*other.mapValue) : nullptr;
                break;
            case PAIR:
                pairValue = other.pairValue ? new std::pair<String, LocalVariable>(*other.pairValue) : nullptr;
                break;

            default:
                break;
            }
        }

        void clear()
        {

            // stringValueがnullptrでない場合にのみ解放
            if (store_type == STRING && stringValue != nullptr)
            {
                delete stringValue;
                stringValue = nullptr;
            }
            // vectorIntValueがnullptrでない場合にのみ解放
            else if (store_type == VINT && vectorIntValue != nullptr)
            {
                delete vectorIntValue;
                vectorIntValue = nullptr;
            }
            // vectorStringValueがnullptrでない場合にのみ解放
            else if (store_type == VSTRING && vectorStringValue != nullptr)
            {
                delete vectorStringValue;
                vectorStringValue = nullptr;
            }
            else if (store_type == VFLOAT && vectorFloatValue != nullptr)
            {
                delete vectorFloatValue;
                vectorFloatValue = nullptr;
            }
            else if (store_type == MAP && mapValue != nullptr)
            {
                delete mapValue;
                mapValue = nullptr;
            }

            else if (store_type == PAIR && pairValue != nullptr)
            {
                delete pairValue;
                pairValue = nullptr;
            }

            // store_typeをデフォルトのINTにリセット
            // store_type = INT;

            // 各値をデフォルトにリセット
            intValue = 0;
            floatValue = 0.0;
            booleanValue = false;
        }

        bool hasStoreTypeMap(Bytecode::opcr type)
        {
            return store_type_map.find(type) != store_type_map.end();
        }

        ~LocalVariable()
        {
            // output_debug("LocalVariable is deleted.", {getCastString()});
            clear();
        }

        void setValueAnalysis(vstring args)
        {
            clear();

            if (!hasStoreTypeMap(opcr_type))
            {
                output_debug(String("ERROR:"), opcr_type);

                throw std::runtime_error("Error: Type is not matched.");
            }

            switch (store_type_map[opcr_type])
            {
            case INT:
            {
                vectorIntValue = new vint();

                for (int i = 0; i < args.size(); i++)
                {
                    canConvertToIntTry(args[i]);
                    vectorIntValue->push_back(args[i].toInt());
                }

                break;
            }

            case FLOAT:
            {
                vectorFloatValue = new vfloat();

                for (int i = 0; i < args.size(); i++)
                {
                    canConvertToFloatTry(args[i]);
                    vectorFloatValue->push_back(args[i].toFloat());
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
            if (!hasStoreTypeMap(opcr_type))
            {
                output_debug(String("ERROR:"), opcr_type);

                throw std::runtime_error("Error: Type is not matched.");
            }
            switch (store_type_map[opcr_type])
            {
            case INT:
            {
                canConvertToIntTry(arg);
                intValue = arg.toInt();
                break;
            }
            case FLOAT:
            {
                canConvertToFloatTry(arg);
                floatValue = arg.toFloat();
                break;
            }
            case BOOLEAN:
            {
                booleanValue = arg == "true";
                break;
            }
            case STRING:
            {
                stringValue = new String(arg);
                break;
            }
            }
        }

        void setValue(int value)
        {
            clear();
            store_type = INT;
            intValue = value;
        }

        void setValue(float value)
        {
            clear();
            store_type = FLOAT;
            floatValue = value;
        }

        void setValue(bool value)
        {
            clear();
            store_type = BOOLEAN;
            booleanValue = value;
        }

        void setValue(const String value)
        {
            clear();
            store_type = STRING;
            stringValue = new String(value);
        }

        void setValue(const vint value)
        {
            clear();
            store_type = VINT;
            vectorIntValue = new vint(value);
        }

        void setValue(const vstring value)
        {
            clear();
            store_type = VSTRING;
            vectorStringValue = new vstring(value);
        }
        void setValue(const vfloat value)
        {
            clear();
            store_type = VFLOAT;
            vectorFloatValue = new vfloat(value);
        }
        void setValue(const std::map<String, LocalVariable> value)
        {
            clear();
            store_type = MAP;
            mapValue = new std::map<String, LocalVariable>(value);
        }
        void setValue(const std::pair<String, LocalVariable> value)
        {
            clear();
            store_type = PAIR;
            pairValue = new std::pair<String, LocalVariable>(value);
        }
        void setMapByPair(const std::pair<String, LocalVariable> value)
        {
            clear();
            store_type = MAP;
            mapValue = new std::map<String, LocalVariable>();
            mapValue->insert(value);
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
            return opcr_type;
        }

        Bytecode::opcr getStoreType() const
        {
            return store_type;
        }

        std::map<String, LocalVariable> getMapValue() const
        {
            return *mapValue;
        }

        std::pair<String, LocalVariable> getPairValue() const
        {
            return *pairValue;
        }

        void setMapValue(String name, LocalVariable lv)
        {
            // mapか確認
            if (store_type != MAP)
            {
                throw std::runtime_error("Error: Type is not matched.");
            }
            mapValue->insert(std::make_pair(name, lv));
        }

        void addMapValue(std::pair<String, LocalVariable> p)
        {
            // mapか確認
            if (store_type != MAP)
            {
                throw std::runtime_error("Error: Type is not matched.");
            }
            mapValue->insert(p);
        }

        bool isMap()
        {
            return store_type == MAP;
        }

        bool isPair()
        {
            return store_type == PAIR;
        }

        bool hasMapValue(String name)
        {
            // mapか確認
            if (store_type != MAP)
            {
                throw std::runtime_error("Error: Type is not matched.");
            }
            return mapValue->find(name) != mapValue->end();
        }

        LocalVariable getMapValue(String name)
        {

            // mapか確認
            if (store_type != MAP)
            {
                throw std::runtime_error("Error: Type is not matched.");
            }
            if (mapValue->find(name) == mapValue->end())
            {
                throw std::runtime_error("Error: Map key is not found.");
            }

            return mapValue->at(name);
        }

        String getCastString()
        {
            if (!hasStoreTypeMap(opcr_type))
            {
                output_debug(String("ERROR:"), opcr_type);
                throw std::runtime_error("Error: Type is not matched.");
            }

            ValueType store_type = store_type_map.at(opcr_type);
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
            case VFLOAT:
            {
                String rv;
                for (int i = 0; i < vectorFloatValue->size(); i++)
                {
                    rv += String((*vectorFloatValue)[i]);
                    if (i != vectorFloatValue->size() - 1)
                        rv += ",";
                }
                return rv;
            }
            case MAP:
            {
                String rv;
                for (auto it = mapValue->begin(); it != mapValue->end(); it++)
                {
                    rv += String(it->first) + ":" + it->second.getCastString();
                    if (it != --mapValue->end())
                        rv += ",";
                }
                return rv;
            }
            case PAIR:
            {
                return pairValue->first + ":" + pairValue->second.getCastString();
            }
            default:
                std::runtime_error("Error: Type is not matched. (unknown)");
                return "unknown";
            }
        }
    };
} // namespace Parser

class LocalVariable
{

public:
    Bytecode::opcr opcr_type;

    enum ValueType
    {
        INT,
        FLOAT,
        BOOLEAN,
        STRING,
        VINT,
        VSTRING,
        VFLOAT,
        MAP,
        PAIR
    } store_type;
    union
    {
        int intValue;
        float floatValue;
        bool booleanValue;
        String *stringValue;
        vint *vectorIntValue;
        vstring *vectorStringValue;
        vfloat *vectorFloatValue;
        std::map<String, LocalVariable> *mapValue;
        std::pair<String, LocalVariable> *pairValue;
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
        {Bytecode::Opecode::d_boolean, BOOLEAN},
        {Bytecode::Opecode::d_json, MAP},
        {Bytecode::Opecode::d_pair, PAIR}};

    LocalVariable();
    LocalVariable(Bytecode::opcr);

    LocalVariable(const LocalVariable &other)
        : store_type(other.store_type), opcr_type(other.opcr_type)
    {
        switch (store_type)
        {
        case INT:
            intValue = other.intValue;
            break;
        case FLOAT:
            floatValue = other.floatValue;
            break;
        case BOOLEAN:
            booleanValue = other.booleanValue;
            break;
        case STRING:
            stringValue = other.stringValue ? new String(*other.stringValue) : nullptr;
            break;
        case VINT:
            vectorIntValue = other.vectorIntValue ? new vint(*other.vectorIntValue) : nullptr;
            break;
        case VSTRING:
            vectorStringValue = other.vectorStringValue ? new vstring(*other.vectorStringValue) : nullptr;
            break;
        case VFLOAT:
            vectorFloatValue = other.vectorFloatValue ? new vfloat(*other.vectorFloatValue) : nullptr;
            break;
        case MAP:
            mapValue = other.mapValue ? new std::map<String, LocalVariable>(*other.mapValue) : nullptr;
            break;
        case PAIR:
            pairValue = other.pairValue ? new std::pair<String, LocalVariable>(*other.pairValue) : nullptr;
            break;

        default:
            break;
        }
    }

    void
    clear()
    {
        // stringValueがnullptrでない場合にのみ解放
        if (store_type == STRING && stringValue != nullptr)
        {
            delete stringValue;
            stringValue = nullptr;
        }
        // vectorIntValueがnullptrでない場合にのみ解放
        else if (store_type == VINT && vectorIntValue != nullptr)
        {
            delete vectorIntValue;
            vectorIntValue = nullptr;
        }
        // vectorStringValueがnullptrでない場合にのみ解放
        else if (store_type == VSTRING && vectorStringValue != nullptr)
        {
            delete vectorStringValue;
            vectorStringValue = nullptr;
        }
        else if (store_type == VFLOAT && vectorFloatValue != nullptr)
        {
            delete vectorFloatValue;
            vectorFloatValue = nullptr;
        }
        else if (store_type == MAP && mapValue != nullptr)
        {
            delete mapValue;
            mapValue = nullptr;
        }

        else if (store_type == PAIR && pairValue != nullptr)
        {
            delete pairValue;
            pairValue = nullptr;
        }

        // store_typeをデフォルトのINTにリセット
        store_type = INT;

        // 各値をデフォルトにリセット
        intValue = 0;
        floatValue = 0.0;
        booleanValue = false;
    }

    void semi_clear()
    {
        // stringValueがnullptrでない場合にのみ解放
        if (store_type == STRING && stringValue != nullptr)
        {
            delete stringValue;
            stringValue = nullptr;
        }
        // vectorIntValueがnullptrでない場合にのみ解放
        else if (store_type == VINT && vectorIntValue != nullptr)
        {
            delete vectorIntValue;
            vectorIntValue = nullptr;
        }
        // vectorStringValueがnullptrでない場合にのみ解放
        else if (store_type == VSTRING && vectorStringValue != nullptr)
        {
            delete vectorStringValue;
            vectorStringValue = nullptr;
        }
        else if (store_type == VFLOAT && vectorFloatValue != nullptr)
        {
            delete vectorFloatValue;
            vectorFloatValue = nullptr;
        }
        else if (store_type == MAP && mapValue != nullptr)
        {
            delete mapValue;
            mapValue = nullptr;
        }

        else if (store_type == PAIR && pairValue != nullptr)
        {
            delete pairValue;
            pairValue = nullptr;
        }

        // store_typeをデフォルトのINTにリセット
        // store_type = INT; //これだけしない

        // 各値をデフォルトにリセット
        intValue = 0;
        floatValue = 0.0;
        booleanValue = false;
    }

    bool hasStoreTypeMap(Bytecode::opcr type)
    {
        return store_type_map.find(type) != store_type_map.end();
    }

    ~LocalVariable()
    {
        // output_debug("LocalVariable is deleted.", {getCastString()});
        clear();
    }

    void setValueAnalysis(vstring args)
    {
        semi_clear();

        if (!hasStoreTypeMap(opcr_type))
        {
            output_debug(String("ERROR:"), opcr_type);

            throw std::runtime_error("Error: Type is not matched.");
        }

        switch (store_type_map[opcr_type])
        {
        case INT:
        {
            vectorIntValue = new vint();

            for (int i = 0; i < args.size(); i++)
            {
                canConvertToIntTry(args[i]);
                vectorIntValue->push_back(args[i].toInt());
            }

            break;
        }

        case FLOAT:
        {
            vectorFloatValue = new vfloat();

            for (int i = 0; i < args.size(); i++)
            {
                canConvertToFloatTry(args[i]);
                vectorFloatValue->push_back(args[i].toFloat());
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
        semi_clear();
        if (!hasStoreTypeMap(opcr_type))
        {
            output_debug(String("ERROR:"), opcr_type);

            throw std::runtime_error("Error: Type is not matched.");
        }
        switch (store_type_map[opcr_type])
        {
        case INT:
        {
            canConvertToIntTry(arg);
            intValue = arg.toInt();
            break;
        }
        case FLOAT:
        {
            canConvertToFloatTry(arg);
            floatValue = arg.toFloat();
            break;
        }
        case BOOLEAN:
        {
            booleanValue = arg == "true";
            break;
        }
        case STRING:
        {
            stringValue = new String(arg);
            break;
        }
        }
    }

    void setValue(int value)
    {
        clear();
        store_type = INT;
        intValue = value;
    }

    void setValue(float value)
    {
        clear();
        store_type = FLOAT;
        floatValue = value;
    }

    void setValue(bool value)
    {
        clear();
        store_type = BOOLEAN;
        booleanValue = value;
    }

    void setValue(const String value)
    {
        clear();
        store_type = STRING;
        stringValue = new String(value);
    }

    void setValue(const vint value)
    {
        clear();
        store_type = VINT;
        vectorIntValue = new vint(value);
    }

    void setValue(const vstring value)
    {
        clear();
        store_type = VSTRING;
        vectorStringValue = new vstring(value);
    }
    void setValue(const vfloat value)
    {
        clear();
        store_type = VFLOAT;
        vectorFloatValue = new vfloat(value);
    }
    void setValue(const std::map<String, LocalVariable> value)
    {
        clear();
        store_type = MAP;
        mapValue = new std::map<String, LocalVariable>(value);
    }
    void setValue(const std::pair<String, LocalVariable> value)
    {
        clear();
        store_type = PAIR;
        pairValue = new std::pair<String, LocalVariable>(value);
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
        return opcr_type;
    }

    Bytecode::opcr getStoreType() const
    {
        return store_type;
    }

    std::map<String, LocalVariable> getMapValue() const
    {
        return *mapValue;
    }

    std::pair<String, LocalVariable> getPairValue() const
    {
        return *pairValue;
    }

    void setMapValue(String name, LocalVariable lv)
    {
        // mapか確認
        if (store_type != MAP)
        {
            throw std::runtime_error("Error: Type is not matched.");
        }
        mapValue->insert(std::make_pair(name, lv));
    }

    void addMapValue(std::pair<String, LocalVariable> p)
    {
        // mapか確認
        if (store_type != MAP)
        {
            throw std::runtime_error("Error: Type is not matched.");
        }
        mapValue->insert(p);
    }

    bool isMap()
    {
        return store_type == MAP;
    }

    bool isPair()
    {
        return store_type == PAIR;
    }

    bool hasMapValue(String name)
    {
        // mapか確認
        if (store_type != MAP)
        {
            throw std::runtime_error("Error: Type is not matched.");
        }
        return mapValue->find(name) != mapValue->end();
    }

    LocalVariable getMapValue(String name)
    {

        // mapか確認
        if (store_type != MAP)
        {
            throw std::runtime_error("Error: Type is not matched.");
        }
        if (mapValue->find(name) == mapValue->end())
        {
            throw std::runtime_error("Error: Map key is not found.");
        }

        return mapValue->at(name);
    }

    String getCastString()
    {
        if (!hasStoreTypeMap(opcr_type))
        {
            output_debug(String("ERROR:"), opcr_type);
            throw std::runtime_error("Error: Type is not matched.");
        }

        ValueType store_type = store_type_map.at(opcr_type);
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
        case VFLOAT:
        {
            String rv;
            for (int i = 0; i < vectorFloatValue->size(); i++)
            {
                rv += String((*vectorFloatValue)[i]);
                if (i != vectorFloatValue->size() - 1)
                    rv += ",";
            }
            return rv;
        }
        case MAP:
        {
            String rv;
            for (auto it = mapValue->begin(); it != mapValue->end(); it++)
            {
                rv += String(it->first) + ":" + it->second.getCastString();
                if (it != --mapValue->end())
                    rv += ",";
            }
            return rv;
        }
        case PAIR:
        {
            return pairValue->first + ":" + pairValue->second.getCastString();
        }
        default:
            std::runtime_error("Error: Type is not matched. (unknown)");
            return "unknown";
        }
    }
};

#endif