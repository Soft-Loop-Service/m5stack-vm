#include "dom_tree.hpp"

namespace Parser
{
    namespace DOM_Tree
    {
        DomNode::DomNode()
        {
            this->dom_index = 0;
            this->tag = "";
            this->text_content = "";
            this->children = {};
            this->attributes = {};
            this->parent_index = -1;
            this->call_stack_len = 0;
        }

        DomNode::DomNode(String tag)
        {
            this->dom_index = 0;
            this->tag = tag;
            this->text_content = "";
            this->children = {};
            this->attributes = {};
            this->parent_index = -1;
            this->call_stack_len = 0;
        }

        DomNode::DomNode(String tag, LocalVariable attributes)
        {
            this->dom_index = 0;
            this->tag = tag;
            this->text_content = "";
            this->children = {};
            this->attributes = attributes.getMapValue();
            this->parent_index = -1;
            this->call_stack_len = 0;
        }

        DomNode::~DomNode()
        {
            output_debug("DomNode is deleted. A");
            // 安全に各要素を削除
            this->children.clear();
            output_debug("DomNode is deleted. B");
            this->attributes.clear();
            output_debug("DomNode is deleted. C");
        }

        int DomNode::getDomIndex()
        {
            return this->dom_index;
        }

        String DomNode::getTag()
        {
            return this->tag;
        }

        String DomNode::getTextContent()
        {
            return this->text_content;
        }

        std::vector<int> DomNode::getChildren()
        {
            return this->children;
        }

        std::map<String, LocalVariable> DomNode::getAttributes()
        {
            return this->attributes;
        }

        LocalVariable DomNode::getAttribute(String key)
        {
            return this->attributes[key];
        }

        int DomNode::getParentIndex()
        {
            return this->parent_index;
        }

        void DomNode::setAttribute(String key, LocalVariable value)
        {
            this->attributes[key] = value;
        }

        void DomNode::setParentIndex(int parent_index)
        {
            this->parent_index = parent_index;
        }

        void DomNode::addChild(int child_index)
        {
            this->children.push_back(child_index);
        }

        void DomNode::setDomIndex(int dom_index)
        {
            this->dom_index = dom_index;
        }

        void DomNode::setCentent(String text_content)
        {
            this->text_content = text_content;
        }

        void DomNode::setOpecodeStackLen(int call_stack_len)
        {
            this->call_stack_len = call_stack_len;
        }

        int DomNode::getOpecodeStackLen()
        {
            return this->call_stack_len;
        }

        DomSystem::DomSystem()
        {
            this->select_index = 0;
            this->dom_tree = {};
            this->dom_tree.emplace_back(DomNode("div")); // root
        }

        DomSystem::~DomSystem()
        {
        }

        void DomSystem::addDomNode(DomNode dom_node)
        {
            this->dom_tree.push_back(dom_node);
        }

        void DomSystem::setDomNode(int index, DomNode dom_node)
        {
            this->dom_tree[index] = dom_node;
        }

        int DomSystem::addDomNodeTree(DomNode dom_node)
        {
            int new_index = this->dom_tree.size();
            dom_node.setParentIndex(this->select_index);
            dom_node.setDomIndex(new_index);
            this->dom_tree.push_back(dom_node);
            this->dom_tree[this->select_index].addChild(new_index);

            this->select_index = new_index;
            return new_index;
        }

        void DomSystem::climbNode()
        {
            this->select_index = this->dom_tree[this->select_index].getParentIndex();
        }

        void DomSystem::setTopDomNodeTextContent(String text_content)
        {
            this->dom_tree[this->select_index].setCentent(text_content);
        }

        std::vector<DomNode> DomSystem::getDomTree()
        {
            return this->dom_tree;
        }

        DomNode DomSystem::getTopSelectNode()
        {
            return this->dom_tree[this->select_index];
        }

        DomNode DomSystem::getDomNode(int index)
        {
            return this->dom_tree[index];
        }

        int DomSystem::domSize()
        {
            return this->dom_tree.size();
        }
    };
};
