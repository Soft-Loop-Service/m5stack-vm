

#include "./definition.hpp"
#include "./bytecode_definition.hpp"
namespace Bytecode
{
    namespace Opecode
    {
        const std::map<String, opcr> type_map = {
            {"int", d_int},
            {"str", d_str},
            {"float", d_float},
            {"pointer", d_pointer},
            {"reference", d_reference},
            {"html_dom", d_html_dom},
            {"function", d_function},
            {"class", d_class},
            {"boolean", d_boolean}};

        opcr resolvOpecrType(String type, vstring token_class_type)
        {
            std::map<String, opcr> cp_type_map = type_map;

            printf("resolvOpecrType %s |", type.c_str());

            for (int i = 0; i < token_class_type.size(); i++)
            {
                cp_type_map[token_class_type[i]] = i + d_class + 1;
                cp_type_map["r_" + token_class_type[i]] = i + d_reference + 1;
                cp_type_map["p_" + token_class_type[i]] = i + d_pointer + 1;
            }

            for (auto itr = cp_type_map.begin(); itr != cp_type_map.end(); ++itr)
            {
                printf(" %s:%d |", itr->first.c_str(), itr->second);
            }

            printf("\n");

            if (cp_type_map.find(type) == cp_type_map.end())
            {
                return 0;
            }

            return cp_type_map.at(type);
        }

        opcr resolvOpecrType(String type)
        {
            if (type_map.find(type) == type_map.end())
            {
                return 0;
            }

            return type_map.at(type);
        }
    }
}
