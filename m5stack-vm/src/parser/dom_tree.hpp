#ifndef DOM_TREE
#define DOM_TREE

#include <vector>
#include "./../definition.hpp"
#include "./../debug.hpp"
#include "./../bytecode_definition.hpp"
#include "./local_variable.hpp"

namespace Parser
{

    namespace DOM_Tree
    {

        class DomNode
        {
        private:
            int dom_index;
            int parent_index;
            std::vector<int> children;
            std::map<String, LocalVariable> attributes;
            String tag;
            String text_content;

            int call_stack_len; // コールスタックの差分で、text_contentの量を決定する

        public:
            DomNode();

            // tag_name
            DomNode(String);

            // tag_name ,attributes
            DomNode(String, LocalVariable);

            // コピーコンストラクタ
            DomNode(const DomNode &other)
                : dom_index(other.dom_index), parent_index(other.parent_index), children(other.children), attributes(other.attributes), tag(other.tag), text_content(other.text_content), call_stack_len(other.call_stack_len)
            {
            }

            ~DomNode();

            int getDomIndex();
            String getTag();
            String getTextContent();
            std::vector<int> getChildren();
            std::map<String, LocalVariable> getAttributes();
            LocalVariable getAttribute(String);

            bool hasAttribute(String key);

            int getParentIndex();
            void setDomIndex(int);
            void setParentIndex(int);
            void setCentent(String);
            void addChild(int);
            void setAttribute(String, LocalVariable);

            void setOpecodeStackLen(int);
            int getOpecodeStackLen();
        };
        class DomSystem
        {
            std::vector<DomNode> dom_tree;

            // 操作中のDOM このindexのDOM要素の子要素として追加していく
            int select_index;

        public:
            DomSystem();
            ~DomSystem();

            void addDomNode(DomNode);
            void setDomNode(int, DomNode);

            int addDomNodeTree(DomNode);

            void setTopDomNodeTextContent(String);

            void climbNode();

            std::vector<DomNode> getDomTree();
            DomNode getDomNode(int);

            DomNode getTopSelectNode();

            int domSize();
        };
    };

};

#endif