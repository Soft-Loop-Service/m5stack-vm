#include "./renderer.hpp"
#include "./../debug.hpp"
#include "./../bytecode_definition.hpp"
#include <M5Core2.h>

namespace Parser
{
    namespace DOM_Renderer
    {
        Renderer::Renderer()
        {
            dom_tree = {};
        }

        Renderer::Renderer(std::vector<DOM_Tree::DomNode> dom_tree)
        {
            this->dom_tree = dom_tree;
        }

        void Renderer::rendering()
        {
            Serial.println("Rendering * * * * ");

            if (get_debug_mode_level() == 0)
            {
                M5.Lcd.fillScreen(TFT_BLACK);
                M5.Lcd.setCursor(0, 0);
            }

            Serial.println("Rendering is started.");
            RenderingDomNode rdn(0, 0, 0, 0);
            rendering(0, rdn);

            Serial.println("Rendering is completed.");
        }

        RenderingDomNode Renderer::rendering(int index, RenderingDomNode rdn)
        {
            Serial.printf("REND |  %d* * * * \n", index);

            for (int i = 0; i < dom_tree[index].getChildren().size(); i++)
            {
                RenderingDomNode rv_rdn = rendering(dom_tree[index].getChildren()[i], rdn);
                rdn.x += rv_rdn.width;
                rdn.y += rv_rdn.height;
                rdn.width = rv_rdn.width;
                rdn.height = rv_rdn.height;
            }

            Serial.printf("REND | RDN %d %d %d %d %d \n", index, rdn.x, rdn.y, rdn.width, rdn.height);

            if (dom_tree[index].hasAttribute("style"))
            {

                Serial.printf("REND | STTYLE-1 %d %d \n", index, dom_tree.size());

                LocalVariable style = dom_tree[index].getAttribute("style");

                Serial.printf("REND | STTYLE-2 %d %d %d\n", index, dom_tree.size(), style.getType());

                // style map valueの中身をSerialで出力

                std::map<String, LocalVariable> style_map;

                if (style.getType() == Bytecode::Opecode::d_json)
                {
                    style_map = style.getMapValue();
                }
                else if (style.getType() == Bytecode::Opecode::d_pair)
                {
                    style_map = {style.getPairValue()};
                }
                else
                {
                    Serial.printf("REND | STTYLE-2-1 %d %d %d\n", index, dom_tree.size(), style.getType());
                }

                Serial.printf("REND | STTYLE-2-2 %d %d %d\n", index, dom_tree.size(), style_map.size());

                for (auto itr = style_map.begin(); itr != style_map.end(); ++itr)
                {
                    Serial.printf("REND | STTYLE-3 %d %d %s %d\n", index, dom_tree.size(), itr->first.c_str(), itr->second.getType());
                }

                Serial.printf("REND | STTYLE-3-1 %d %d %d\n", index, dom_tree.size(), style_map.size());

                if (style_map.find("width") != style_map.end())
                {
                    rdn.width = style_map["width"].getValueCastInt();
                }
                if (style_map.find("height") != style_map.end())
                {
                    rdn.height = style_map["height"].getValueCastInt();
                }
                if (style_map.find("x") != style_map.end())
                {
                    rdn.x = style_map["x"].getValueCastInt();
                }
                if (style_map.find("y") != style_map.end())
                {
                    rdn.y = style_map["y"].getValueCastInt();
                }
                if (style_map.find("backgroundColor") != style_map.end())
                {
                    if (get_debug_mode_level() == 0)
                    {
                        M5.Lcd.fillRect(rdn.x, rdn.y, rdn.width, rdn.height, color_map[style_map["backgroundColor"].getValueString()]);
                    }
                }
                if (style_map.find("color") != style_map.end())
                {
                    if (get_debug_mode_level() == 0)
                    {
                        M5.Lcd.setTextColor(color_map[style_map["color"].getValueString()]);
                    }
                }
                if (style_map.find("font") != style_map.end())
                {
                    if (get_debug_mode_level() == 0)
                    {
                        M5.Lcd.setTextFont(style_map["font"].getValueCastInt());
                    }
                }
                if (style_map.find("fontSize") != style_map.end())
                {
                    int size = style_map["fontSize"].getValueCastInt();

                    if (get_debug_mode_level() == 0)
                    {
                        M5.Lcd.setTextSize(size);
                    }

                    std::pair<int, int> size_x_y = font_size_map[size];
                    rdn.width = size_x_y.first;
                    rdn.height = size_x_y.second;
                }

                Serial.printf("REND | STTYLE-4 %d %d %d %d %d\n", index, dom_tree.size(), rdn.x, rdn.y, rdn.width);
            }

            String text_content = dom_tree[index].getTextContent();

            if (text_content != "")
            {
                Serial.printf("REND | TEXT %d %d %d %d %d %s\n", index, rdn.x, rdn.y, rdn.width, rdn.height, text_content.c_str());
                if (get_debug_mode_level() == 0)
                {
                    M5.Lcd.drawString(text_content.c_str(), rdn.x, rdn.y);
                }
            }

            return rdn;
        }

    };
}
