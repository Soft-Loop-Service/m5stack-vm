
#ifndef __RENDERER_HPP
#define __RENDERER_HPP
#include "./dom_tree.hpp"

namespace Parser
{
    struct RenderingDomNode
    {
        int x;
        int y;
        int width;
        int height;

        RenderingDomNode()
        {
            this->x = 0;
            this->y = 0;
            this->width = 0;
            this->height = 0;
        }

        RenderingDomNode(int x, int y, int width, int height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }
    };

    namespace DOM_Renderer
    {
        class Renderer
        {
        private:
            std::vector<DOM_Tree::DomNode> dom_tree;

            std::map<String, int16_t> color_map = {
                {"black", M5.Lcd.color565(0, 0, 0)},
                {"white", M5.Lcd.color565(255, 255, 255)},
                {"red", M5.Lcd.color565(255, 0, 0)},
                {"green", M5.Lcd.color565(0, 255, 0)},
                {"blue", M5.Lcd.color565(0, 0, 255)},
                {"yellow", M5.Lcd.color565(255, 255, 0)},
                {"cyan", M5.Lcd.color565(0, 255, 255)},
                {"magenta", M5.Lcd.color565(255, 0, 255)},
                {"gray", M5.Lcd.color565(128, 128, 128)},
                {"darkgray", M5.Lcd.color565(169, 169, 169)},
                {"lightgray", M5.Lcd.color565(211, 211, 211)},
                {"purple", M5.Lcd.color565(128, 0, 128)},
                {"orange", M5.Lcd.color565(255, 165, 0)},
                {"pink", M5.Lcd.color565(255, 192, 203)},
                {"brown", M5.Lcd.color565(165, 42, 42)},
                {"transparent", M5.Lcd.color565(0, 0, 0)}};

            /*
            文字サイズ	幅	高さ
            1	6	8
            2	12	16
            3	18	24
            4	24	32
            5	30	40
            6	36	48
            7	42	56
            */
            std::map<int, std::pair<int, int>> font_size_map = {
                {1, {6, 8}},
                {2, {12, 16}},
                {3, {18, 24}},
                {4, {24, 32}},
                {5, {30, 40}},
                {6, {36, 48}},
                {7, {42, 56}}};

        public:
            Renderer();
            Renderer(std::vector<DOM_Tree::DomNode>);
            void rendering();
            RenderingDomNode rendering(int, RenderingDomNode);
        };
    };
} // namespace Parser

#endif